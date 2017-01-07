#ifndef JSON_H
#define JSON_H

#ifdef __cplusplus
extern "C" {
#endif

#define JSON_API extern

typedef unsigned char byte;

typedef enum
{
    JSON_NULL = 0,
    JSON_TRUE,
    JSON_FALSE,
    JSON_STRING,
    JSON_NUMBER,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_PAIR,
} json_type;

struct json_pair;
struct json_value;

typedef struct json_value
{
    json_type Type;
    byte Payload[];
} json_value;

typedef struct
{
    json_type Type; // = JSON_STRING

    size_t Length;
    const char* Start;
} json_string;

typedef struct json_pair
{
    json_type Type; // = JSON_PAIR

    json_string Key;
    json_value* Value;

    struct json_pair* NextInHash;
    struct json_pair* Next;
} json_pair;

typedef struct
{
    json_type Type; // = JSON_OBJECT

    struct json_pair* First;

    size_t PairTableSize;
    struct json_pair** Pairs;
} json_object;

typedef struct
{
    json_type Type; // = JSON_ARRAY

    size_t Length;
    struct json_value** Values;
} json_array;

typedef struct {
    json_type Type; // = JSON_NUMBER

    double Value;
} json_number;

typedef enum
{
    ERROR_NONE,
    ERROR_SYNTAX,
} json_parser_error_id;

typedef struct
{
    json_parser_error_id Id;
    char* Message;
} json_parse_error;

typedef struct
{
    json_parse_error Error;
    json_value* Document;
} json_result;

#define JSON_TYPE(valptr) (*((json_type*)(valptr)))

JSON_API size_t
json_Hash(json_string Key);

JSON_API json_value*
json_Lookup(json_object* Object, json_string Key);

JSON_API json_string
json_String(const char* CString);

/* Simple parse, no options.
 * WARNING: Will actually go through the entire document twice!
 * This is because it uses information from an attempted parse with no memory
 * available to try to get an exact estimate for the amount of memory needed for
 * the parse proper. You should use the extended function below if you wish to
 * pass in an arbitrary memory block/arbitrary amount of memory.
 * Because of this, it may be inefficient for large documents.
 */
JSON_API json_result 
json_Parse(const char* Data, int DataSize);

/* Extended parse options.
 * The OutScratchUsed parameter will be equal to the number of bytes of memory
 * needed to fully parse the document. If ScratchSize is less than this value, 
 * parsing will halt early and you will be given an incomplete document. You can
 * call this function with a ScratchSize of 0 to receive an exact estimate of
 * how much memory is needed.
 */
JSON_API json_result
json_ParseExtended(const char* Data, int DataSize,
                   void* ScratchMemory, size_t ScratchSize, 
                   size_t* OutScratchUsed, bool RetainSourceStrings);

#ifdef __cplusplus
}
#endif

#endif
