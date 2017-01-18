/* codegen.c
 *  by Andrew 'ChronalDragon' Chronister
 *
 * Code generator for producing the Twitter API functions.
 * Reads an API schema file in JSON, parses code snippets from twitter.c, and
 * then writes out to twitter_api.c all of the code necessary to collect
 * parameters and make the appropriate cURL queries to call the corresponding
 * twitter API endpoint.
 */

#define _XOPEN_SOURCE 700
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define TWC_CODEGEN_FIRST_PASS
#include "twitter.h"
#include "json.h"

#ifdef _WIN32
#define DEBUG_BREAK __debugbreak()
#else
#define DEBUG_BREAK __asm("int3")
#endif

// Twitter API endpoint descriptions
typedef struct
{
    twc_string Name;
    twc_string Type;
    twc_string Desc;
    twc_string$ Example;
    bool Required;

    twc_string FieldName;
} api_parameter;

typedef struct
{
    twc_string Path;
    twc_string$ PathParamName;

    twc_string Desc;

    twc_http_method Method;
    bool Unique; // Whether any other endpoint has the same path but different HTTP method

    int ParamCount;
    api_parameter* Params;
} api_endpoint;

// TODO
const char* twc_HttpMethodString[3] = {
    "GET",
    "POST",
    "DELETE",
};

// Templates for serializing API endpoint parameters for web request
typedef struct {
    bool ForURLSlug;
    twc_string TypeName;
    twc_string Text;
} serialization_template;

// TODO: Fine to keep these as globals?
static serialization_template Templates[30];
static int TemplateCount;

#include "tokenizer.c"

static twc_string TSTR(const char* String)
{
    twc_string Result = {0};
    Result.Ptr = String;
    Result.Size = strlen(Result.Ptr);
    return Result;
}

#define TSTRf(s) (int)s.Size, s.Ptr

char* ReadFileAndNullTerminate(const char* Filename)
{
    FILE* InFile = NULL;
    InFile = fopen(Filename, "r");
    if (InFile == NULL) {
        fprintf(stderr, "Could not open file %s\n", Filename);
        return NULL;
    }

    fseek(InFile, 0, SEEK_END);
    size_t InFileSize = ftell(InFile);
    fseek(InFile, 0, SEEK_SET);

    char* In = (char*)malloc(InFileSize + 1);
    fread(In, InFileSize, 1, InFile);
    In[InFileSize] = 0;
    fclose(InFile);
    return In;
}

json_value* Lookup(json_object* Object, char* Key)
{
    return json_Lookup(Object, json_String(Key));
}

json_array LookupArr(json_object* Object, char* Key)
{
    json_value* Val = Lookup(Object, Key);
    assert(Val != NULL);
    assert(JSON_TYPE(Val) == JSON_ARRAY);
    return *(json_array*)Val;   
}

twc_string LookupStr(json_object* Object, char* Key)
{
    json_value* Val = Lookup(Object, Key);
    assert(Val != NULL);
    assert(JSON_TYPE(Val) == JSON_STRING);
    json_string Str = *(json_string*)Val;
    twc_string Result = {0};
    Result.Ptr = Str.Start;
    Result.Size = Str.Length;
    return Result;
}

bool LookupBool(json_object* Object, char* Key)
{
    json_value* Val = Lookup(Object, Key);
    assert(Val != NULL);
    assert(JSON_TYPE(Val) == JSON_TRUE || JSON_TYPE(Val) == JSON_FALSE);
    bool Result = JSON_TYPE(Val) == JSON_TRUE;
    return Result;
}


serialization_template
ParseSerializationTemplate(tokenizer* Tokenizer)
{
    serialization_template Result = {0};

    token TemplateDecl = RequireToken(Tokenizer, Token_Identifier);
    if (twc_StringCompare(TemplateDecl.Text.Value, TSTR("twc_param_serialization")) == 0) {
        Result.ForURLSlug = false;
    }
    else if (twc_StringCompare(TemplateDecl.Text.Value, TSTR("twc_url_serialization")) == 0) {
        Result.ForURLSlug = true;
    }
    else {
        assert(!"Unexpected serialization template type!");
    }
    RequireToken(Tokenizer, Token_OpenParenthesis);
    token TypeToken = GetToken(Tokenizer);
    token CloseParen = GetToken(Tokenizer);
    while (CloseParen.Type != Token_CloseParenthesis &&
           CloseParen.Type != Token_EOF)
    {
        if (CloseParen.Text.Exists) {
            TypeToken.Text.Value.Size = 
                CloseParen.Text.Value.Size + 
                (CloseParen.Text.Value.Ptr - TypeToken.Text.Value.Ptr);
        }
        else {
            TypeToken.Text.Value.Size = 
                (CloseParen.Begin - TypeToken.Text.Value.Ptr) + 1;
        }
        CloseParen = GetToken(Tokenizer);
    }

    token Start = RequireToken(Tokenizer, Token_OpenCurlyBrace);
    int Level = 1;
    token End;
    do {
        End = GetToken(Tokenizer);
        if (End.Type == Token_OpenCurlyBrace) { ++Level; }
        else if (End.Type == Token_CloseCurlyBrace) { --Level; }
    } while (Level > 0 && End.Type != Token_EOF);

    twc_string TemplateStr;
    TemplateStr.Ptr = Start.Begin + 1;
    TemplateStr.Size = End.Begin - Start.Begin - 1;

    Result.Text = TemplateStr;
    Result.TypeName = TypeToken.Text.Value;

    return Result;
}

void Constantify(char* Dest, const char* Source, int Size)
{
    for (int i = 0; i < Size; ++i)
    {
        Dest[i] = toupper(Source[i]);
    }
}

twc_strbuf ConstantifyPath(twc_string Path)
{
    twc_strbuf Result = {0};
    Result.Ptr = malloc(Path.Size + 8);
    memcpy(Result.Ptr, "TWC_URL_", Result.Size += 8);
    for (uint SourceIndex = 0; SourceIndex < Path.Size; ++SourceIndex)
    {
        if (Path.Ptr[SourceIndex] == '/') { Result.Ptr[Result.Size++] = '_'; }
        else if (Path.Ptr[SourceIndex] == ':') { continue; }
        else if (Path.Ptr[SourceIndex] == '_') { continue; }
        else { Result.Ptr[Result.Size++] = toupper(Path.Ptr[SourceIndex]); }
    }
    return Result;
}

twc_strbuf ToPascalCase(twc_string Identifier)
{
    twc_strbuf Result = {0};
    Result.Ptr = malloc(Identifier.Size);
    bool Boundary = true;
    for (uint SourceIndex = 0; SourceIndex < Identifier.Size; ++SourceIndex)
    {
        if (Identifier.Ptr[SourceIndex] == '_' || Identifier.Ptr[SourceIndex] == ':'
            || Identifier.Ptr[SourceIndex] == '[' || Identifier.Ptr[SourceIndex] == ']') { 
            Boundary = true;
            continue;
        }
        else { 
            Result.Ptr[Result.Size++] = (Boundary ? toupper : tolower)(Identifier.Ptr[SourceIndex]); 
            Boundary = false;
        }
    }
    return Result;
}

twc_strbuf StructNameFromPath(twc_string Path, twc_http_method$ Method)
{
    twc_strbuf Result = {0};
    Result.Size = Path.Size + strlen("twc_") + strlen("_params"); // Upper bound
    if (Method.Exists) {
        Result.Size += strlen(twc_HttpMethodString[Method.Value]) + 1;
    }
    Result.Ptr = calloc(1, Result.Size);
    int DestIndex = 0;
    memcpy(Result.Ptr, "twc_", DestIndex += strlen("twc_"));
    if (Method.Exists) {
        const char* MethodStr = twc_HttpMethodString[Method.Value];
        for (uint i = 0; i < strlen(MethodStr); ++i) {
            Result.Ptr[DestIndex + i] = tolower(MethodStr[i]);
        }
        DestIndex += strlen(MethodStr);
        Result.Ptr[DestIndex++] = '_';
    }
    for (uint SourceIndex = 0; 
         SourceIndex < Result.Size; 
         ++SourceIndex)
    {
        if (Path.Ptr[SourceIndex] == '/') { Result.Ptr[DestIndex++] = '_'; }
        else if (CharIsAlpha(Path.Ptr[SourceIndex])) { 
            Result.Ptr[DestIndex++] = tolower(Path.Ptr[SourceIndex]);
        }
        else { continue; }
    }
    memcpy(Result.Ptr + DestIndex, "_params", strlen("_params"));
    Result.Size = (DestIndex += strlen("_params"));
    return Result;
}

twc_strbuf FunctionNameFromPath(twc_string Path, twc_http_method$ Method)
{
    twc_strbuf Result = {0};
    Result.Size = Path.Size + strlen("twc_"); // Upper bound
    if (Method.Exists) {
        Result.Size += strlen(twc_HttpMethodString[Method.Value]) + 1;
    }
    Result.Ptr = calloc(1, Result.Size);
    int DestIndex = 0;
    memcpy(Result.Ptr, "twc_", DestIndex += strlen("twc_"));
    if (Method.Exists) {
        const char* MethodStr = twc_HttpMethodString[Method.Value];
        for (uint i = 0; i < strlen(MethodStr); ++i) {
            Result.Ptr[DestIndex + i] = (i == 0 ? toupper : tolower)(MethodStr[i]);
        }
        DestIndex += strlen(MethodStr);
        Result.Ptr[DestIndex++] = '_';
    }
    bool NextCaps = true;
    for (uint SourceIndex = 0; 
         SourceIndex < Result.Size; 
         ++SourceIndex)
    {
        if (Path.Ptr[SourceIndex] == '/') 
        { 
            Result.Ptr[DestIndex++] = '_'; 
            NextCaps = true; 
        }
        else if (CharIsAlpha(Path.Ptr[SourceIndex])) { 
            Result.Ptr[DestIndex++] = (NextCaps ? toupper : tolower)(Path.Ptr[SourceIndex]);
            NextCaps = false;
        }
        else if (Path.Ptr[SourceIndex] == '_') {
            NextCaps = true;
        }
        else { continue; }
    }
    Result.Size = DestIndex;
    return Result;
}

twc_http_method GetHTTPMethod(twc_string String)
{
    if (twc_StringCompare(String, twc_ToString("GET")) == 0) return TWC_HTTP_GET;
    if (twc_StringCompare(String, twc_ToString("POST")) == 0) return TWC_HTTP_POST;
    if (twc_StringCompare(String, twc_ToString("DELETE")) == 0) return TWC_HTTP_DELETE;
    assert(!"Invalid HTTP method!");
    return (twc_http_method)-1;
}

const char* TranslateType(twc_string SchemaType, bool Required)
{
    if (twc_StringCompare(SchemaType, twc_ToString("bool")) == 0) {
        if (Required) { return "bool"; }
        else { return "bool$"; }
    }
    if (twc_StringCompare(SchemaType, twc_ToString("string")) == 0) {
        if (Required) { return "twc_string"; }
        else { return "twc_string$"; }
    }
    if (twc_StringCompare(SchemaType, twc_ToString("int")) == 0) {
        if (Required) { return "int"; }
        else { return "int$"; }
    }
    if (twc_StringCompare(SchemaType, twc_ToString("binary")) == 0) {
        if (Required) { return "twc_buffer"; }
        else { return "twc_buffer$"; }
    }
    if (twc_StringCompare(SchemaType, twc_ToString("status_id")) == 0) {
        if (Required) { return "twc_status_id"; }
        else { return "twc_status_id$"; }
    }
    if (twc_StringCompare(SchemaType, twc_ToString("user_id")) == 0) {
        if (Required) { return "twc_user_id"; }
        else { return "twc_user_id$"; }
    }
    if (twc_StringCompare(SchemaType, twc_ToString("cursor_id")) == 0) {
        if (Required) { return "twc_cursor_id"; }
        else { return "twc_cursor_id$"; }
    }
    if (twc_StringCompare(SchemaType, twc_ToString("place_id")) == 0) {
        if (Required) { return "twc_place_id"; }
        else { return "twc_place_id$"; }
    }
    // Unsupported, just let the user fill it out as they see fit
    return "const char*";
}

bool ParseAPISchema(char* APISchemaFileName, twc_out api_endpoint** Endpoints, twc_out int* EndpointCount)
{
    *Endpoints = NULL;
    *EndpointCount = 0;

    char* APIFile = ReadFileAndNullTerminate(APISchemaFileName);
    if (APIFile == NULL) { 
        fprintf(stderr, "Please provide a valid filename for the api schema JSON\n");
        return false;
    }
    json_result Parsed = json_Parse(APIFile, strlen(APIFile));

    if (Parsed.Error.Id == ERROR_NONE) 
    {
        if (JSON_TYPE(Parsed.Document) == JSON_ARRAY) 
        {
            json_array EndpointsArray = *(json_array*)Parsed.Document;
            *Endpoints = (api_endpoint*)calloc(EndpointsArray.Length, sizeof(api_endpoint));

            for (uint i = 0; i < EndpointsArray.Length; ++i) 
            {
                if (JSON_TYPE(EndpointsArray.Values[i]) != JSON_OBJECT) 
                {
                    // Unexpected json type in parsed json
                    return false;
                }

                api_endpoint Endpoint = {0};
                json_object* EndpointData = (json_object*)EndpointsArray.Values[i];
                
                Endpoint.Path = LookupStr(EndpointData, "path");

                char* PathParamStart = (memchr(Endpoint.Path.Ptr, ':', Endpoint.Path.Size));
                if (PathParamStart == NULL) 
                {
                    Endpoint.PathParamName = TWC_NONE(twc_string$);
                }
                else 
                {
                    twc_string ParamName = {0};
                    ParamName.Ptr = PathParamStart + 1;

                    char* PathParamEnd = (memchr(ParamName.Ptr, '/', Endpoint.Path.Size - (ParamName.Ptr - Endpoint.Path.Ptr)));
                    if (PathParamEnd == NULL) {
                        ParamName.Size = Endpoint.Path.Size - (PathParamStart - Endpoint.Path.Ptr) - 1;
                    }
                    else {
                        ParamName.Size = PathParamEnd - ParamName.Ptr;
                    }
                    Endpoint.PathParamName = TWC_SOME(ParamName, twc_string$);
                    //printf("Endpoint %.*s has path param %.*s\n", TSTRf(Endpoint.Path), TSTRf(ParamName));
                }

                Endpoint.Desc = LookupStr(EndpointData, "desc");

                Endpoint.Method = GetHTTPMethod(LookupStr(EndpointData, "method"));
                Endpoint.Unique = true;
                for (int j = 0; j < *EndpointCount; ++j) {
                    api_endpoint* Other = &(*Endpoints)[j];
                    if (twc_StringCompare(Other->Path, Endpoint.Path) == 0) {
                        Other->Unique = false;
                        Endpoint.Unique = false;
                    }
                }

                json_array ParamsArray = LookupArr(EndpointData, "params");
                Endpoint.ParamCount = ParamsArray.Length;
                Endpoint.Params = calloc(ParamsArray.Length, sizeof(api_parameter));

                for (uint p = 0; p < ParamsArray.Length; ++p) 
                {
                    json_object* ParamData = (json_object*)ParamsArray.Values[p];
                    twc_strbuf PascalName = ToPascalCase(LookupStr(ParamData, "name"));
                    Endpoint.Params[p].FieldName = (twc_string){ .Size = PascalName.Size, .Ptr = PascalName.Ptr };
                    Endpoint.Params[p].Name = LookupStr(ParamData, "name");
                    Endpoint.Params[p].Desc = LookupStr(ParamData, "desc");
                    Endpoint.Params[p].Type = LookupStr(ParamData, "type");
                    Endpoint.Params[p].Required = LookupBool(ParamData, "required");

                    if (Lookup(ParamData, "example") != NULL) 
                    {
                        Endpoint.Params[p].Example = TWC_SOME(LookupStr(ParamData, "example"), twc_string$);
                    }
                    else 
                    {
                        Endpoint.Params[p].Example = TWC_NONE(twc_string$);
                    }
                }
                (*Endpoints)[(*EndpointCount)++] = Endpoint;
            }
        }
        else 
        {
            // Unexpected top-level json type
            fprintf(stderr, "Unexpected type found at top-level of API json schema (should be array)\n");
            return false;
        }
    }
    else 
    {
        // Parse error
        fprintf(stderr, "Error encountered when parsing API json schema: %s\n", Parsed.Error.Message);
        return false;
    }
    return true;
}

void GenerateEndpointParams(FILE* OutFile, api_endpoint* Endpoint)
{
    twc_strbuf StructName = StructNameFromPath(Endpoint->Path, TWC_OPTION(!Endpoint->Unique, Endpoint->Method, twc_http_method$));

    fprintf(OutFile, "/* %.*s:\n%.*s */\n", TSTRf(Endpoint->Path), TSTRf(Endpoint->Desc));
    fprintf(OutFile, "typedef twc_param_struct {\n");
    int OptionalParams = 0;

    for (int p = 0; p < Endpoint->ParamCount; ++p) 
    {
        api_parameter* Param = Endpoint->Params + p; if (Param->Required) { continue; }

        const char* Type = TranslateType(Param->Type, false);
        // TODO block comment is bandaid, need actual comment printing
        fprintf(OutFile, "    /* %.*s */\n", TSTRf(Param->Desc));
        if (strcmp(Type, "UNSUPPORTED_TYPE") == 0) 
        {
            Type = "const char*"; 
            fprintf(OutFile, "    // Unsupported type\n");
        }
        fprintf(OutFile, "    %s %.*s;\n", Type, TSTRf(Param->FieldName));
        ++OptionalParams;
    }

    if (OptionalParams == 0) 
    {
        fprintf(OutFile, "    // No parameters documented as of generation\n");
        fprintf(OutFile, "    void* Reserved;\n");
    }

    fprintf(OutFile, "} %.*s;\n\n", TSTRf(StructName));

    free(StructName.Ptr);
}

void GenerateEndpointDeclaration(FILE* OutFile, api_endpoint* Endpoint)
{
    twc_strbuf StructName = StructNameFromPath(Endpoint->Path, 
                                               TWC_OPTION(!Endpoint->Unique, Endpoint->Method, twc_http_method$));
    twc_strbuf FuncName = FunctionNameFromPath(Endpoint->Path,
                                               TWC_OPTION(!Endpoint->Unique, Endpoint->Method, twc_http_method$));

    fprintf(OutFile, "extern twc_call_result\n");
    fprintf(OutFile, "%.*s(twc_state* Twitter,\n", TSTRf(FuncName));
    for (int p = 0; p < Endpoint->ParamCount; ++p) 
    {
        api_parameter* Param = Endpoint->Params + p;
        if (!Param->Required) { continue; }
        const char* Type = TranslateType(Param->Type, true);

        for (uint s = 0; s < FuncName.Size + 1; ++s) { fprintf(OutFile, " "); }
        fprintf(OutFile, "%s %.*s,\n", Type, TSTRf(Param->FieldName));
    }

    for (uint s = 0; s < FuncName.Size + 1; ++s) { fprintf(OutFile, " "); }
    fprintf(OutFile, "%.*s Params)", TSTRf(StructName));

    free(StructName.Ptr);
    free(FuncName.Ptr);
}

void GenerateEndpointSerialization(FILE* OutFile, api_endpoint* Endpoint)
{
    //TODO translate HTTP method into the name rather than just use the value
    twc_strbuf PathConstant = ConstantifyPath(Endpoint->Path);
    twc_string PathConstantStr = (twc_string){ .Ptr = PathConstant.Ptr, .Size = PathConstant.Size };

    fprintf(OutFile, "    twc_key_value_list ParamList = NULL;\n");

    for (int ParamIndex = 0; ParamIndex < Endpoint->ParamCount; ++ParamIndex)
    {
        api_parameter Param = Endpoint->Params[ParamIndex];

        const char* Type = TranslateType(Param.Type, Param.Required);
        {
            bool ForURLSlug = (Endpoint->PathParamName.Exists &&
                               twc_StringCompare(Endpoint->PathParamName.Value, Param.Name) == 0);
            bool FoundTemplate = false;
            twc_string TemplateText = {0};
            for (int TemplIndex = 0; TemplIndex < TemplateCount; ++TemplIndex)
            {
                serialization_template Template = Templates[TemplIndex];
                if ((Template.ForURLSlug == ForURLSlug) && 
                    twc_StringCompare(Template.TypeName, TSTR(Type)) == 0) {
                    FoundTemplate = true;
                    TemplateText = Template.Text;
                    break;
                }
            } 
            if (ForURLSlug && !FoundTemplate) {
                fprintf(OutFile, "    // (TODO: no serialization defined for URL slug type %s)\n", Type);
                fprintf(OutFile, "    twc_strbuf FilledURL;\n");
                fprintf(OutFile, "    assert(!\"Serialization needed for required URL parameter %.*s!\\n\");\n", TSTRf(Param.Name));
            }
            else if (!FoundTemplate) {
                fprintf(OutFile, "    // (TODO: no serialization defined for optional param type %s)\n", Type);
            }

            const char* C;
            const char* C2;
            for (C = C2 = TemplateText.Ptr; (C - TemplateText.Ptr) < (int)TemplateText.Size; ++C)
            {
                if (*C == '@') {
                    fprintf(OutFile, "%.*s", (int)(C - C2), C2);
                    twc_string ReplaceToken;
                    C2 = C + 2;
                    for (; (C2 - TemplateText.Ptr) < (int)TemplateText.Size && *(C2 - 1) != '@'; ++C2);
                    ReplaceToken.Ptr = C;
                    ReplaceToken.Size = C2 - C;

                    if (twc_StringCompare(ReplaceToken, TSTR("@FieldName@")) == 0) {
                        fprintf(OutFile, "%.*s", TSTRf(Param.FieldName));
                    }
                    else if (twc_StringCompare(ReplaceToken, TSTR("@ParamName@")) == 0) {
                        if (Param.Required) {
                            fprintf(OutFile, "%.*s", TSTRf(Param.FieldName));
                        }
                        else {
                            fprintf(OutFile, "Params.%.*s", TSTRf(Param.FieldName));
                        }
                    }
                    else if (twc_StringCompare(ReplaceToken, TSTR("@ParamString@")) == 0) {
                        fprintf(OutFile, "\"%.*s\"", TSTRf(Param.Name));
                    }
                    else if (twc_StringCompare(ReplaceToken, TSTR("@EndpointURL@")) == 0) {
                        fprintf(OutFile, "%.*s", TSTRf(PathConstant));
                    }
                    else {
                        fprintf(stderr, "ERROR: Unexpected template name %.*s", TSTRf(ReplaceToken));
                        exit(EXIT_FAILURE);
                    }

                    C = C2;
                }
            }
            fprintf(OutFile, "%.*s", (int)(C - C2), C2);
        }
    }

    twc_string CallURLVar = (Endpoint->PathParamName.Exists ? TSTR("FilledURL.Ptr") : PathConstantStr);
    fprintf(OutFile, "    return twc_MakeCall(Twitter, %d, %.*s, ParamList);\n", 
            Endpoint->Method, 
            TSTRf(CallURLVar));
    free(PathConstant.Ptr);
}

void 
DumpStreamToFile(FILE* Stream, FILE* File)
{
    char a = fgetc(Stream);
    do {
        fputc(a, File);
        a = fgetc(Stream);
    } while(a!=EOF);
}

int main(int ArgCount, char* ArgValues[])
{
    if (ArgCount <= 2) 
    { 
        puts("Usage: twc_codegen <file with templates> <api json schema> [<output header file> <output c file>]");
        puts("    e.g. twc_codegen twitter.c api.json");
        return EXIT_FAILURE;
    }

    char* DeclName = ArgCount >= 4 ? ArgValues[3] : "code/twitter_api.h";
    char* ImplName = ArgCount >= 5 ? ArgValues[4] : "code/twitter_api.c";

    FILE* DeclStream = fopen(DeclName, "w");
    if (DeclStream == NULL) {
        printf("Could not open %s for writing!\n", DeclName);
        return EXIT_FAILURE;
    }

    FILE* ImplStream = fopen(ImplName, "w");
    if (ImplStream == NULL) {
        printf("Could not open %s for writing!\n", ImplName);
        return EXIT_FAILURE;
    }

    tokenizer T = {0};
    tokenizer* Tokenizer = &T;

    char* TemplateFile = ReadFileAndNullTerminate(ArgValues[1]);

    T.At = T.Start = T.LineStart = TemplateFile;
    do {
        token Next = PeekToken(Tokenizer);
        while (!(Next.Type == Token_Identifier && 
                 (twc_StringCompare(Next.Text.Value, TSTR("twc_param_serialization")) == 0 ||
                 twc_StringCompare(Next.Text.Value, TSTR("twc_url_serialization")) == 0))) {
            GetToken(Tokenizer);
            Next = PeekToken(Tokenizer);
            if (Next.Type == Token_EOF) { break; }
        }
        if (Next.Type == Token_EOF) { break; }

        serialization_template Template = ParseSerializationTemplate(Tokenizer);
        Templates[TemplateCount++] = Template;
    } while(1);

    int EndpointCount = 0;
    api_endpoint* Endpoints = NULL;
    ParseAPISchema(ArgValues[2], &Endpoints, &EndpointCount);

    for (int EndpointIndex = 0; EndpointIndex < EndpointCount; ++EndpointIndex)
    {
        api_endpoint* Endpoint = Endpoints + EndpointIndex;
        if (twc_StringCompare(Endpoint->Path, TSTR("media/upload")) == 0) 
        {
            // Blacklist this one for now because it must be treated specially
            // (has POST form data)
            continue;
        }

        GenerateEndpointParams(DeclStream, Endpoint);
        GenerateEndpointDeclaration(DeclStream, Endpoint);
        fprintf(DeclStream, ";\n\n");

        GenerateEndpointDeclaration(ImplStream, Endpoint);
        fprintf(ImplStream, "\n{\n");
        GenerateEndpointSerialization(ImplStream, Endpoint);
        fprintf(ImplStream, "}\n\n");
    }

    fflush(DeclStream);
    fflush(ImplStream);

    fclose(DeclStream);
    fclose(ImplStream);
}
