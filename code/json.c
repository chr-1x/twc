#define _GNU_SOURCE
#include <stdio.h> // For printf
#include <string.h> // For memset 
#include <stdbool.h>
#include <assert.h>
#include <malloc.h> // for alloca // TODO is there another way to do the JSON array parsing?
#ifndef _MSC_VER
#include <alloca.h> // alloca on non-MSVC
#endif
#include "json.h"

#ifndef JSON_MALLOC
#include <stdlib.h>
#define JSON_MALLOC malloc
#endif

#ifdef __GNUC__
#define JSON_INLINE __attribute__((always_inline)) inline
#else
#define JSON_INLINE inline
#endif

#ifdef _MSC_VER
#define alloca _alloca
#include "asprintf.h"
#endif

typedef unsigned int uint;

typedef struct
{
    void* Base;
    size_t Available;
    size_t Used;
    size_t Requested;
} memory_arena;

#define PushStruct(Arena, T) (T*)_PushSize(Arena, sizeof(T))
#define PushArray(Arena, N, T) (T*)_PushSize(Arena, sizeof(T)*N)
#define PushSize(Arena, Size) _PushSize(Arena, Size)

JSON_INLINE void*
_PushSize(memory_arena* Arena, size_t Amount)
{
    Arena->Requested += Amount;
    if (Arena->Used + Amount > Arena->Available)
        return NULL;

    void* Result = (char*)Arena->Base + Arena->Used;
    Arena->Used += Amount;
    return Result;
}

typedef enum
{
    Token_Unknown,

    Token_OpenBracket,
    Token_CloseBracket,
    Token_OpenBrace,
    Token_CloseBrace,
    Token_Colon,
    Token_Comma,

    Token_String,
    Token_Number,
    Token_Literal,

    Token_EndOfStream,
} token_type;

typedef struct
{
    token_type Type;

    json_string Text;
    double Number;
} token;

typedef struct
{
    const char *At;
    const char* LineStart;
    const char* Start;
    int Line;
    int EOFIndex;

    json_parse_error Error;
    bool CheckingSize;
    bool CopyStrings;

    memory_arena TreeArena;
} tokenizer;

JSON_INLINE bool
IsEndOfLine(char C)
{
    bool Result = ((C == '\n') ||
                   (C == '\r'));

    return Result ;
}

JSON_INLINE bool
IsWhitespace(char C)
{
    bool Result = ((C == ' ') ||
                   (C == '\t') ||
                   (C == '\v') ||
                   (C == '\f') ||
                   IsEndOfLine(C));

    return Result;
}

JSON_INLINE bool
IsAlpha(char C)
{
    bool Result = (((C >= 'a') && (C <= 'z')) ||
                   ((C >= 'A') && (C <= 'Z')));

    return Result;
}

JSON_INLINE bool
IsNumber(char C)
{
    bool Result = (((C >= '0') && (C <= '9')) || C == '-' || C == '.');
    
    return Result;
}

extern json_string
json_StringEx(const char* String, int Length)
{
    json_string Result = { JSON_STRING };
    Result.Length = Length;
    Result.Start = String;
    return Result;
}

extern json_string
json_String(const char* CString)
{
    const char* C = CString;
    while (C && *(++C) != '\0');
    return json_StringEx(CString, C - CString);
}

JSON_INLINE bool
json_StringsEqual(json_string A, json_string B)
{
    if (A.Length != B.Length) return false;
    for (uint Index = 0;
        Index < A.Length;
        ++Index)
    {
        if (A.Start[Index] != B.Start[Index]) { return false; }
    }
    return true;
}

JSON_INLINE json_string
json_ReadString(memory_arena* Arena, json_string Src)
{
    json_string Dest = { JSON_STRING };
    char* Value = (char*)PushSize(Arena, Src.Length);
    if (Value == NULL) { return Dest; }

    int DestIndex = 0;
    for (uint SourceIndex = 0; 
        SourceIndex < Src.Length; 
        ++SourceIndex)
    {
        if (Src.Start[SourceIndex] == '\\') {
            switch(Src.Start[SourceIndex + 1]) {
                case '\"': continue;
                case '\\': continue;
                case '/': continue;

                case 'r':
                {
                    Value[DestIndex++] = '\r';
                    SourceIndex++;
                    continue;
                };
                case 'n': 
                {
                    Value[DestIndex++] = '\n';
                    SourceIndex++;
                    continue;
                };
            }
        }
        else { Value[DestIndex++] = Src.Start[SourceIndex]; }
    }
    Dest.Start = Value;
    Dest.Length = DestIndex;
    return Dest;
}

size_t
HashObjectKey(json_string Key)
{
    //TODO(chronister): BETTER HASH FUNCTION
    size_t Hash = 0;
    //TODO(chronister): Use SSE crc32 intrinsic if available
    for (const char *p = Key.Start; Key.Length > 0; ++p,--Key.Length) 
    {
        Hash = 31 * Hash + (int)*p;
    }
    return Hash;
}

json_value*
json_Lookup(json_object* Object, json_string Key)
{
    size_t KeyHash = HashObjectKey(Key);
    int Index = (int)(KeyHash % Object->PairTableSize);
    json_pair* Pair = Object->Pairs[Index];
    if (Pair == NULL) { return NULL; }
    while (!json_StringsEqual(Pair->Key, Key)) 
    {
        if (Pair->NextInHash == NULL) {
            return NULL;
        }
        Pair = Pair->NextInHash;
    }
    return Pair->Value;
}

JSON_INLINE bool
TokenEquals(token Token, const char *Match)
{
    const char *At = Match;
    for(uint Index = 0;
        Index < Token.Text.Length;
        ++Index, ++At)
    {
        if((*At == 0) || (Token.Text.Start[Index] != *At))
        {
            return(false);
        }
    }

    bool Result = (*At == 0);
    return Result;
}

JSON_INLINE void
ReportError(tokenizer* Tokenizer, const char* Message, int Col)
{
    Tokenizer->Error.Id = ERROR_SYNTAX;
    asprintf(&Tokenizer->Error.Message, "\nERROR: %s at line %d:%d\n", Message, Tokenizer->Line, Col);
}

#define DIE_IF_ERROR(Tokenizer, Value) if(Tokenizer->Error.Id != ERROR_NONE) return Value

static json_object*
ParseObject(tokenizer* Tokenizer);
static json_array*
ParseArray(tokenizer* Tokenizer);

static void
EatAllWhitespace(tokenizer *Tokenizer)
{
    for(;;)
    {
        if(IsEndOfLine(Tokenizer->At[0]))
        {
            ++Tokenizer->Line;
            Tokenizer->LineStart = Tokenizer->At;
            // Next statement will also match line
        }

        if(IsWhitespace(Tokenizer->At[0]))
        {
            ++Tokenizer->At;
        }
        // Also eats comments
        else if((Tokenizer->At[0] == '/') &&
                (Tokenizer->At[1] == '/'))
        {
            Tokenizer->At += 2;
            while(Tokenizer->At[0] && !IsEndOfLine(Tokenizer->At[0]))
            {
                ++Tokenizer->At;
            }
        }
        else if((Tokenizer->At[0] == '/') &&
                (Tokenizer->At[1] == '*'))
        {
            Tokenizer->At += 2;
            while(Tokenizer->At[0] &&
                  !((Tokenizer->At[0] == '*') &&
                    (Tokenizer->At[1] == '/')))
            {
                ++Tokenizer->At;
            }
            
            if(Tokenizer->At[0] == '*')
            {
                Tokenizer->At += 2;
            }
        }
        else
        {
            break;
        }
    }
}

static json_number
ParseNumber(tokenizer* Tokenizer)
{
    json_number Result = { JSON_NUMBER };
    json_string NumString;
    NumString.Start = Tokenizer->At;

    while(IsNumber(Tokenizer->At[0]) ||
          Tokenizer->At[0] == '.' ||
          Tokenizer->At[0] == '-' ||
          Tokenizer->At[0] == 'e' || Tokenizer->At[0] == 'E')
    {
        ++Tokenizer->At;
    }

    NumString.Length = (size_t)(Tokenizer->At - NumString.Start);

    char* NullTerminatedNumString = (char*)PushSize(&Tokenizer->TreeArena, NumString.Length + 1);
    if (NullTerminatedNumString == NULL) { return Result; }

    memset(NullTerminatedNumString, '\0', NumString.Length + 1);
    memcpy(NullTerminatedNumString, NumString.Start, NumString.Length);

    sscanf(NullTerminatedNumString, "%lf", &Result.Value);
    return Result;
}

static token
GetToken(tokenizer *Tokenizer)
{
    EatAllWhitespace(Tokenizer);

    token Token = { 0 };
    Token.Text.Type = JSON_STRING;
    Token.Text.Length = 1;
    Token.Text.Start = Tokenizer->At;
    char C = Tokenizer->At[0];

    if (Tokenizer->At - Tokenizer->Start == Tokenizer->EOFIndex) {
        Token.Type = Token_EndOfStream;
        return Token;
    }

    ++Tokenizer->At;
    switch(C)
    {
        case '\0': { Token.Type = Token_EndOfStream; } break;
        case '{': { Token.Type = Token_OpenBrace; } break;
        case '}': { Token.Type = Token_CloseBrace; } break;
        case '[': { Token.Type = Token_OpenBracket; } break;
        case ']': { Token.Type = Token_CloseBracket; } break;
        case ':': { Token.Type = Token_Colon; } break;
        case ',': { Token.Type = Token_Comma; } break;

        case '"':
        {
            Token.Type = Token_String;

            Token.Text.Start = Tokenizer->At;
            
            while(Tokenizer->At[0] &&
                  Tokenizer->At[0] != '"')
            {
                if((Tokenizer->At[0] == '\\') &&
                   Tokenizer->At[1])
                {
                    ++Tokenizer->At;
                }
                ++Tokenizer->At;
            }

            Token.Text.Length = Tokenizer->At - Token.Text.Start;
            if(Tokenizer->At[0] == '"')
            {
                ++Tokenizer->At;
            }

            if (Tokenizer->CopyStrings) {
                Token.Text = json_ReadString(&Tokenizer->TreeArena, Token.Text);
            }
        } break;
        
        default:
        {
            if (IsAlpha(C))
            {
                Token.Type = Token_Unknown;
                
                while(IsAlpha(Tokenizer->At[0]) ||
                      IsNumber(Tokenizer->At[0]) ||
                      (Tokenizer->At[0] == '_'))
                {
                    ++Tokenizer->At;
                }
                
                Token.Text.Length = Tokenizer->At - Token.Text.Start;

                if (TokenEquals(Token, "true") ||
                    TokenEquals(Token, "false") ||
                    TokenEquals(Token, "null"))
                {
                    Token.Type = Token_Literal;
                }
            }
            else if (IsNumber(C))
            {
                Token.Type = Token_Number;
                --Tokenizer->At;
                Token.Number = ParseNumber(Tokenizer).Value;
            }
            else
            {
                Token.Type = Token_Unknown;
            }
        } break;
    }

    return Token;
}

static token
PeekToken(tokenizer* Tokenizer)
{
    tokenizer Before = *Tokenizer;
    token Result = GetToken(Tokenizer);
    *Tokenizer = Before;
    return Result;
}

static bool
RequireToken(tokenizer* Tokenizer, token_type DesiredType)
{
    token Token = GetToken(Tokenizer);
    bool Result = (Token.Type == DesiredType);
    return Result;
}

static json_value*
ParseValue(tokenizer* Tokenizer)
{
    json_value* Result = NULL;
    token NextToken = PeekToken(Tokenizer);
    switch(NextToken.Type)
    {
        case Token_OpenBrace:
        {
            Result = (json_value*)ParseObject(Tokenizer);
            DIE_IF_ERROR(Tokenizer, Result);
        } break;

        case Token_OpenBracket:
        {
            Result = (json_value*)ParseArray(Tokenizer);
            DIE_IF_ERROR(Tokenizer, Result);
        } break;

        case Token_String:
        {
            token Token = GetToken(Tokenizer);
            json_string* ResultStr = PushStruct(&Tokenizer->TreeArena, json_string);
            if (ResultStr == NULL) { return Result; }

            *ResultStr = Token.Text;
            Result = (json_value*)ResultStr;
            DIE_IF_ERROR(Tokenizer, Result);
        } break;

        case Token_Number:
        {
            token Value = GetToken(Tokenizer);
            json_number* ResultNum = PushStruct(&Tokenizer->TreeArena, json_number);
            if (ResultNum == NULL) { return Result; }

            ResultNum->Type = JSON_NUMBER;
            ResultNum->Value = Value.Number;
            Result = (json_value*)ResultNum;
            DIE_IF_ERROR(Tokenizer, Result);
        } break;
        
        case Token_Literal:
        {
            token Value = GetToken(Tokenizer);
            Result = PushStruct(&Tokenizer->TreeArena, json_value);
            if (Result == NULL) { return Result; }

            if (TokenEquals(Value, "true")) {
                Result->Type = JSON_TRUE;
            } 
            else if (TokenEquals(Value, "false")) {
                Result->Type = JSON_FALSE;
            } 
            else if (TokenEquals(Value, "null")) {
                Result->Type = JSON_NULL;
            } 
            else {
                ReportError(Tokenizer, "Unrecognized literal", Tokenizer->At - Tokenizer->LineStart);
            }
            DIE_IF_ERROR(Tokenizer, Result);
        } break;

        default:
        {
            GetToken(Tokenizer);
            ReportError(Tokenizer, "Expected array, object, string, number, or true/false/NULL", Tokenizer->At - Tokenizer->LineStart);
            DIE_IF_ERROR(Tokenizer, Result);
        } break;
    }

    return Result;
}

static json_pair*
ParsePair(tokenizer* Tokenizer, bool First)
{
    json_pair* Result = PushStruct(&Tokenizer->TreeArena, json_pair);
    if (!First) {
        if (!RequireToken(Tokenizer, Token_Comma))
        {
            ReportError(Tokenizer, "Expected comma before next pair", 0);
            DIE_IF_ERROR(Tokenizer, Result);
        }
    }

    token KeyToken = GetToken(Tokenizer);
    if (KeyToken.Type != Token_String) {
        ReportError(Tokenizer, "Object keys must be strings", 0);
        DIE_IF_ERROR(Tokenizer, Result);
    }
    
    if (Result != NULL) Result->Key = KeyToken.Text;

    if (!RequireToken(Tokenizer, Token_Colon)) {
        ReportError(Tokenizer, "Expected colon after object key", 0);
        DIE_IF_ERROR(Tokenizer, Result);
    }
    
    json_value* Value = ParseValue(Tokenizer);
    if (Result != NULL) Result->Value = Value;
    DIE_IF_ERROR(Tokenizer, Result);

    return Result;
}

static json_object*
ParseObject(tokenizer* Tokenizer)
{
    json_object* Result = PushStruct(&Tokenizer->TreeArena, json_object);
    if (Result != NULL) {
        memset(Result, 0, sizeof(json_object));
        Result->Type = JSON_OBJECT;
    }

    if (RequireToken(Tokenizer, Token_OpenBrace))
    {
        json_pair* First = NULL;
        int PairTableSize = 64; // TODO(Chronister): stop hardcoding this
        json_pair** Pairs = PushArray(&Tokenizer->TreeArena, PairTableSize, json_pair*);
        if (Pairs != NULL) memset(Pairs, 0, PairTableSize * sizeof(json_pair*));
        
        json_pair* Last = NULL;
        json_pair* Current = NULL;

        for(;;)
        {
            token NextToken = PeekToken(Tokenizer);
            if (NextToken.Type == Token_CloseBrace)
            {
                GetToken(Tokenizer);
                break;
            }
            else
            {
                json_pair* Pair = ParsePair(Tokenizer, First == NULL);
                if (Pair == NULL) {
                    First = (json_pair*)0xDEADBEEF;
                    continue;
                }
                DIE_IF_ERROR(Tokenizer, Result);

                size_t Slot = HashObjectKey(Pair->Key) % PairTableSize;
                if (Pairs[Slot] == NULL ||
                    json_StringsEqual(Pairs[Slot]->Key, Pair->Key))
                {
                    Pairs[Slot] = Pair;
                    Current = Pairs[Slot];
                }
                else
                {
                    Current = Pairs[Slot];
                    while (Current->NextInHash != NULL) { Current = Current->NextInHash; }
                    Current->NextInHash = Pair;
                    Current = Pair;
                }
                if (Last != NULL) { Last->Next = Current; }
                Last = Current;

                if (First == NULL) { First = Last; }
            }
        }

        if (Result != NULL) {
            Result->First = First;
            Result->PairTableSize = PairTableSize;
            Result->Pairs = Pairs;
        }
    }

    return Result;
}

static json_array*
ParseArray(tokenizer* Tokenizer)
{
    struct json_array_temp_node
    {
        json_value* Value;

        struct json_array_temp_node* Next;
    };

    json_array* Result = NULL;
    if (RequireToken(Tokenizer, Token_OpenBracket))
    {
        int ItemsSoFar = 0;
        // Build up a queue of values that we've seen so that we only have to
        // parse them once.
        struct json_array_temp_node* First = NULL;
        struct json_array_temp_node* Last = NULL;
        for(;;)
        {
            token NextToken = PeekToken(Tokenizer);
            if (NextToken.Type == Token_CloseBracket)
            {
                GetToken(Tokenizer);
                break;
            }
            else {
                if (First != NULL) {
                    if (!RequireToken(Tokenizer, Token_Comma))
                    {
                        ReportError(Tokenizer, "Expected comma before next value in array", 0);
                        DIE_IF_ERROR(Tokenizer, Result);
                    }
                }

                json_value* Value = ParseValue(Tokenizer);
                DIE_IF_ERROR(Tokenizer, Result);
                ++ItemsSoFar;
                // TODO(Chronister): under certain circumstances this could cause stack
                // overflow. Is there another way to process the json array that
                // doesn't:
                //  a) Use memory arena space (it is very not-ideal to have
                //     2x the amount of space needed to actually store the
                //     arena just sitting around after processing is done)
                //  b) Use extraneous heap space (eventually, we'd prefer
                //     clients to be able to fully replace malloc/free in case they
                //     don't want to link with the CRT)
                //
                //  Idea: Copy the finalized struct / array of values to the
                //     start of the region once we're done splatting it out below.
                //     This will require some screwing around with the arena,
                //     but elegantly solves the problem without either of the
                //     above problems being invoked.
                struct json_array_temp_node* Node = (struct json_array_temp_node*)alloca(sizeof(struct json_array_temp_node));
                Node->Value = Value;
                if (Last) { Last->Next = Node; }
                Last = Node;

                if (First == NULL) { First = Node; }
            }
        }
        Result = PushStruct(&Tokenizer->TreeArena, json_array);
        json_value** Values = PushArray(&Tokenizer->TreeArena, ItemsSoFar, json_value*);
        if (Result == NULL || Values == NULL) { return NULL; }

        memset(Result, 0, sizeof(json_array));
        Result->Type = JSON_ARRAY;

        // Now flatten the linked list queue into an array
        Result->Values = Values;
        struct json_array_temp_node* Current = First;
        for (int Index = 0;
            Index < ItemsSoFar;
            ++Index)
        {
            Result->Values[Index] = Current->Value;
            ++Result->Length;
            if (Current->Next)
            {
                Current = Current->Next;
            }
            else { break; }
        }
    }
    return Result;
}

static json_value*
ParseStructure(tokenizer* Tokenizer)
{
    json_value* Result = NULL;
    token Token = PeekToken(Tokenizer);
    switch(Token.Type)
    {
        case Token_OpenBrace:
        {
            Result = (json_value*)ParseObject(Tokenizer);
            DIE_IF_ERROR(Tokenizer, Result);
        } break;

        case Token_OpenBracket:
        {
            Result = (json_value*)ParseArray(Tokenizer);
            DIE_IF_ERROR(Tokenizer, Result);
        } break;

        default:
        {
            GetToken(Tokenizer);
            ReportError(Tokenizer, "Array or object expected", 0);
            DIE_IF_ERROR(Tokenizer, Result);
        } break;
    }
    return Result;
}

extern json_result
json_ParseExtended(const char* Data, int DataSize,
                   void* ScratchMemory, size_t ScratchSize, 
                   size_t* OutScratchUsed, bool RetainSourceStrings)
{
    tokenizer Tokenizer = {0};
    Tokenizer.Start = Tokenizer.At = Tokenizer.LineStart = Data;
    Tokenizer.EOFIndex = DataSize;
    Tokenizer.Line = 1;
    Tokenizer.TreeArena.Base = ScratchMemory;
    Tokenizer.TreeArena.Available = ScratchSize;
    Tokenizer.CopyStrings = !RetainSourceStrings;

    json_result Result = {0};
    Result.Document = ParseStructure(&Tokenizer);
    Result.Error = Tokenizer.Error;

    // Pass out requested instead of used because if the parse succeeded, this
    // will be <= the amount available, but if the parse failed, we want the
    // client to know how much the parse actually needed.
    *OutScratchUsed = Tokenizer.TreeArena.Requested;
    return Result;
}

extern json_result 
json_Parse(const char* Data, int DataSize)
{
    tokenizer Tokenizer = {0};
    Tokenizer.Start = Tokenizer.At = Tokenizer.LineStart = Data;
    Tokenizer.EOFIndex = DataSize;
    Tokenizer.Line = 1;
    Tokenizer.TreeArena.Base = NULL;
    Tokenizer.TreeArena.Available = 0;
    Tokenizer.CopyStrings = true;

    ParseStructure(&Tokenizer);

    void* Mem = JSON_MALLOC(Tokenizer.TreeArena.Requested);
    size_t Used;
    json_result Result = json_ParseExtended(Data, DataSize, Mem, Tokenizer.TreeArena.Requested, &Used, false);
    return Result;
}
