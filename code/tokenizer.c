/* tokenizer.c
 *  by Andrew 'ChronalDragon' Chronister
 *
 * Basic, not-to-spec C tokenizing
 *
 * Part of the codegen translation unit (included there directly)
 * Separated out here for ease of navigation.
 */

typedef struct
{
    const char* At;
    const char* Start;

    const char* LineStart;
    int LineCount;
} tokenizer;

typedef enum
{
    Token_Unknown = 0,

    Token_Semicolon,
    Token_Colon,
    Token_Comma,
    Token_Period,
    Token_Equals,
    Token_Slash,
    Token_Plus,
    Token_Asterisk,
    Token_Ampersand,
    Token_Arobase,

    Token_OpenCurlyBrace,
    Token_CloseCurlyBrace,
    Token_OpenParenthesis,
    Token_CloseParenthesis,
    Token_OpenBracket,
    Token_CloseBracket,

    // For the following types, Text.exists = true
    Token_String,
    Token_Identifier,
    // For Token_Comment, Text does not include the comment boundary characters
    // (but for block comments, may include * and tab formatting present in the original comments)
    Token_Comment,
    // For Token_Preprocessor, Text includes the entire line (possibly broken over multiple source
    // lines) of the preprocessor command, excluding the final (\r)\n
    Token_Preprocessor,

    Token_EOF,
} token_type;

typedef struct
{
    token_type Type;
    const char* Begin;
    twc_string_o Text;
} token;

bool CharIsAlpha(char C) { return ('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z'); }
bool CharIsDigit(char C) { return ('0' <= C && C <= '9'); }

static void EatAllWhitespace(tokenizer* Tokenizer)
{
    while (*Tokenizer->At && 
           (*Tokenizer->At == ' ' || *Tokenizer->At == '\t' ||
            *Tokenizer->At == '\r' || *Tokenizer->At == '\n' ||
            *Tokenizer->At == '\f'))
    {
        if (*Tokenizer->At == '\n') 
        { 
            ++Tokenizer->LineCount; 
            Tokenizer->LineStart = Tokenizer->At + 1;
        }

        ++Tokenizer->At;
    }
}

static token GetToken(tokenizer* Tokenizer) 
{
    EatAllWhitespace(Tokenizer);

    token Token = {0};
    switch(*Tokenizer->At) 
    {
        case EOF:
        case '\0': 
        { 
            Token.Type = Token_EOF; 
        } break;

        case ';': { Token.Begin = Tokenizer->At++; Token.Type = Token_Semicolon; } break;
        case ':': { Token.Begin = Tokenizer->At++; Token.Type = Token_Colon; } break;
        case ',': { Token.Begin = Tokenizer->At++; Token.Type = Token_Comma; } break;
        case '.': { Token.Begin = Tokenizer->At++; Token.Type = Token_Period; } break;
        case '=': { Token.Begin = Tokenizer->At++; Token.Type = Token_Equals; } break;
        case '+': { Token.Begin = Tokenizer->At++; Token.Type = Token_Plus; } break;
        case '*': { Token.Begin = Tokenizer->At++; Token.Type = Token_Asterisk; } break;
        case '&': { Token.Begin = Tokenizer->At++; Token.Type = Token_Ampersand; } break;
        case '@': { Token.Begin = Tokenizer->At++; Token.Type = Token_Arobase; } break;
        case '{': { Token.Begin = Tokenizer->At++; Token.Type = Token_OpenCurlyBrace; } break;
        case '}': { Token.Begin = Tokenizer->At++; Token.Type = Token_CloseCurlyBrace; } break;
        case '(': { Token.Begin = Tokenizer->At++; Token.Type = Token_OpenParenthesis; } break;
        case ')': { Token.Begin = Tokenizer->At++; Token.Type = Token_CloseParenthesis; } break;
        case '[': { Token.Begin = Tokenizer->At++; Token.Type = Token_OpenBracket; } break;
        case ']': { Token.Begin = Tokenizer->At++; Token.Type = Token_CloseBracket; } break;

        case '/': 
        {
            Token.Begin = Tokenizer->At;
            if (Tokenizer->At[1] == '/') {
                Token.Type = Token_Comment;
                twc_string Text = {0};
                Text.Ptr = ++Tokenizer->At + 1;
                while (*Tokenizer->At && *Tokenizer->At != '\r' && *Tokenizer->At != '\n') {
                    ++Tokenizer->At;
                }
                Text.Size = Tokenizer->At - Text.Ptr;
                if (*Tokenizer->At == '\r') { ++Tokenizer->At; }
                if (*Tokenizer->At == '\n') { ++Tokenizer->At; }

                Token.Text.Exists = true;
                Token.Text.Value = Text;
            }
            else if (Tokenizer->At[1] == '*') {
                Token.Type = Token_Comment;
                twc_string Text = {0};
                Tokenizer->At += 2;
                Text.Ptr = Tokenizer->At;
                // No comment nesting in C, so we'll just parse along on our merry way until we find
                // any close-comment pattern
                while (*Tokenizer->At) {
                    if (Tokenizer->At[0] == '*' && Tokenizer->At[1] == '/') {
                        break;
                    }
                    ++Tokenizer->At;
                }
                Text.Size = Tokenizer->At - Text.Ptr;
                if (*Tokenizer->At) { Tokenizer->At += 2; }

                Token.Text.Exists = true;
                Token.Text.Value = Text;
            }
            else {
                ++Tokenizer->At;
                Token.Type = Token_Slash;
            }
        } break;

        case '#':
        {
            Token.Type = Token_Preprocessor;
            twc_string Text = {0};
            Text.Ptr = Token.Begin = Tokenizer->At;

            while (*Tokenizer->At && Tokenizer->At[0] != '\r' && Tokenizer->At[0] != '\n') 
            {
                if (Tokenizer->At[0] == '\\') {
                    if (Tokenizer->At[1] == '\r') {
                        ++Tokenizer->At;
                    }
                    if (Tokenizer->At[1] == '\n') {
                        ++Tokenizer->At;
                    }
                }
                ++Tokenizer->At;
            }
            Text.Size = Tokenizer->At - Text.Ptr;
            if (*Tokenizer->At == '\r') { Tokenizer->At++; }
            if (*Tokenizer->At == '\n') { Tokenizer->At++; }
        } break;

        case '"': 
        {
            Token.Begin = Tokenizer->At;
            Token.Type = Token_String;
            twc_string Text = {0};
            Text.Ptr = ++Tokenizer->At;
            while(*Tokenizer->At && *Tokenizer->At != '"') 
            {
                if (Tokenizer->At[0] == '\\' && 
                    (Tokenizer->At[1] == '\\' || Tokenizer->At[1] == '"')) 
                {
                    Tokenizer->At += 2;
                    continue;
                }
                Tokenizer->At += 1;
            }
            Text.Size = Tokenizer->At - Text.Ptr;
            if (*Tokenizer->At) { ++Tokenizer->At; } // To eat the closing quotation mark

            Token.Text.Exists = true;
            Token.Text.Value = Text;
        } break;

        default: 
        {
            if (CharIsAlpha(*Tokenizer->At) || *Tokenizer->At == '_' || *Tokenizer->At == '$') {
                Token.Type = Token_Identifier;
                Token.Begin = Tokenizer->At;

                twc_string Text = {0};
                Text.Ptr = Tokenizer->At;

                while (CharIsAlpha(*Tokenizer->At) || 
                       CharIsDigit(*Tokenizer->At) || 
                       *Tokenizer->At == '_' || *Tokenizer->At == '$') 
                {
                    ++Tokenizer->At;
                }

                Text.Size = Tokenizer->At - Text.Ptr;
                Token.Text.Exists = true;
                Token.Text.Value = Text;
            }
            else {
                Token.Type = Token_Unknown;
                Token.Begin = Tokenizer->At;
                ++Tokenizer->At;
            }
        } break;
    }

    return Token;
}

static token PeekToken(tokenizer* Tokenizer) 
{
    tokenizer Before = *Tokenizer;
    token Result = GetToken(Tokenizer);
    *Tokenizer = Before;
    return Result;
}

static token RequireToken(tokenizer* Tokenizer, token_type Type)
{
    token Result = GetToken(Tokenizer);
    if (Result.Type != Type) {
        //TODO error reporting
        DEBUG_BREAK;
    }
    return Result;
}

#if 0
static token RequireKeyword(tokenizer* Tokenizer, const char* Keyword) 
{
    twc_string KeywordString = { strlen(Keyword), Keyword };
    token Result = GetToken(Tokenizer);
    if (Result.Text.Exists == false || 
        twc_StringCompare(KeywordString, Result.Text.Value) != 0) {
        //TODO error reporting
        DEBUG_BREAK;
    }
    return Result;
}

static void EatUntilKeyword(tokenizer* Tokenizer, const char* Keyword)
{
    twc_string KeywordString = { strlen(Keyword), Keyword };
    token Next = PeekToken(Tokenizer);
    while (Next.Type != Token_EOF &&
           !(Next.Text.Exists && twc_StringCompare(Next.Text.Value, KeywordString) == 0)) {
        GetToken(Tokenizer);
        Next = PeekToken(Tokenizer);
    }
}
#endif
