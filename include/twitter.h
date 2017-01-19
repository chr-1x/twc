#ifndef TWITTER_H
/*    .    ,--._  
 *   .\'-__(  )/
 *   \_       ;  
 *    \_     /     1) Introduction
 *  -<__,--''  ----================================================================================--------
 *
 * Tweet With C (TWC) is a C99-compatible library for using the Twitter API. It is designed to allow you,
 * the programmer, to interact with the API at a variety of levels, from raw cURL calls to structured and
 * typechecked functions that marshal the parameters and block until a response is given.
 *
 * cURL is the sole dependency.
 *
 * ## Basic Usage
 * 
 * Before you can call any twitter API functions, you must create and initialize a
 * `twc_state` instance:
 * 
 *     twc_oauth_keys Keys; // Populate with your OAuth Consumer/Token keys
 *     // OAuth pin authentication coming eventually
 *     
 *     twc_state Twitter;
 *     twc_Init(&Twitter, Keys);
 * 
 * Now you can call any function in the Twitter API. For example, to double-check
 * your OAuth keys are working:
 * 
 *     twc_call_result VerifyResult = 
 *         twc_Account_VerifyCredentials(&Twitter, (twc_account_verifycredentials_params){});
 *     printf("Twitter: %.*s\n", (int)VerifyResult.Data.Size, VerifyResult.Data.Ptr);
 *     
 * Tweet With C returns all API call results as strings (`twc_string`). You can
 * parse them with your own JSON library, or you can parse them with the provided
 * JSON parser (also used in the code generator):
 * 
 *     json_result Result = json_Parse((char*)VerifyResult.Data.Ptr, (int)VerifyResult.Data.Size);
 * 
 * When you're done, to free allocated memory and do cURL cleanup, call `twc_Close`:
 * 
 *     twc_Close(&Twitter);
 * 
 * ## Advanced Usage
 * 
 * `twc_InitEx` takes several parameters which you can use to control how memory is
 * allocated during library use (by default, it uses `malloc` and `free`):
 * 
 *     twc_InitEx(&Twitter, Keys, MyErrorBuffer, sizeof(MyErrorBuffer), MyMalloc, MyFree);
 * 
 * If you don't want to use the provided API functions, or one doesn't yet exist
 * for the endpoint you're trying to access, you can make calls directly:
 * 
 *     twc_key_value_list MyParamList = NULL;
 *     // Load up some params...
 *     MyParamList = twc_KeyValueList_InsertSorted(ParamList, &SomeKeyValue);
 * 
 *     twc_MakeCall(&Twitter, TWC_HTTP_GET, "https://api.twitter.com/42.0/undocumented.json", MyParamList);
 *     
 * In the current version of the library, all API calls are synchronous (blocking).
 * In the future, support may be added for asynchronous calls based on cURL's
 * support for the same.
 * 
 * If you can't wait, you may want to look at `twitter.c` to see how `twc_MakeCall`
 * is implemented. All of the functions it calls are exposed in this interface, so
 * you can use the existing helper functions in your own version of it.
 * 
 *     1.1) Includes
 * ----==============================--------
 */

// TODO(chronister): Collapse this in?
#include "types.h"
// The only thing we depend on: cURL, for all our API call needs.
#include <curl/curl.h>

#include <string.h>

// NOTE: The generated twitter API functions are included in §2.2

/*
 *     1.2) API qualifiers
 * ----==============================--------
 *
 * A few macros that we use to provide more information about API functions and parameters.
 */

/* twc_out: 
 *   Used to mark parameters that the function will use to return results.
 * Rationale: Having pointer parameters is ambiguous, and putting out_ as a prefix to parameter names is
 *   unwieldy. */
#define twc_out 

/* twc_in: 
 *   (TODO(chronister): DO I want this?)
 *   Used to mark parameters that the function uses solely as input.
 * Rationale: Clarifies the meaning of pointer and structure parameters, and ensures they are not spuriously
 *   modified. */
#define twc_in const

#ifdef __GNUC__
#define twc_inline __attribute__((always_inline)) inline
#else
#ifdef _MSC_VER
#define twc_inline __forceinline
#endif
#endif

// Metaprogramming
#define twc_param_struct struct

// Ensure that no name-mangling occurs if compiling as C++
#ifdef __cplusplus
extern "C" {
#endif

/*    .    ,--._  
 *   .\'-__(  )/
 *   \_       ;  
 *    \_     /     2) Data Structures and Types  
 *  `<__,--''  ----================================================================================--------
 */

/*
 *     2.1) Generic data types
 * ----==============================--------
 *
 * Macro-based generics that define useful data structures and option types.
 */

#define twc_buffer_t(T) struct { size_t Size; T* Ptr; }

typedef twc_buffer_t(void) twc_buffer;
typedef twc_buffer_t(char) twc_strbuf;
typedef twc_buffer_t(const char) twc_string;

#define twc_option_t(T) struct { bool Exists; T Value; }

typedef twc_option_t(bool) bool$;
typedef twc_option_t(int) int$;
typedef twc_option_t(float) float$;
typedef twc_option_t(double) double$;
typedef twc_option_t(u64) u64$;
typedef twc_option_t(twc_string) twc_string$;

#define TWC_NONE(T) (T){ .Exists = false }
#define TWC_SOME(expr, T) (T){ .Exists = true, .Value = expr}
#define TWC_OPTION(cond, expr, T) ((cond) ? TWC_SOME((expr), T) : TWC_NONE(T))
#define TWC_NULLBUF (twc_buffer){ .Size = 0, .Ptr = NULL}

/*
 *     2.1) Concrete data types
 * ----==============================--------
 *
 * Ordinary C types and instantiations of the above that are used in twitter API
 * calls and in this library.
 */

typedef union
{
    struct {
        u8 R, G, B;
    };
    u32 Combined;
} twc_color;

typedef u64 twc_status_id;
typedef u64$ twc_status_id$;

typedef u64 twc_user_id;
typedef u64$ twc_user_id$;

typedef u64 twc_cursor_id;
typedef u64$ twc_cursor_id$;

// Note: unlike the previous three types of ID, place IDs are dealt with by the Twitter API as hexadecimal strings.
typedef u64 twc_place_id;
typedef u64$ twc_place_id$;

typedef struct {
    double Latitude;
    double Longitude;
} twc_geo_coordinate;
typedef twc_option_t(twc_geo_coordinate) twc_geo_coordinate$;

typedef struct twc_key_value_pair
{
    twc_string Key;
    twc_string Value;

    struct twc_key_value_pair* Next;
} twc_key_value_pair;

typedef twc_key_value_pair* twc_key_value_list;

typedef struct
{
    const char* ConsumerKey; 
    const char* ConsumerSecret;
    const char* TokenKey; 
    const char* TokenSecret;
} twc_oauth_keys;

typedef void* (*twc_malloc_func)(size_t BytesToAlloc);
typedef void (*twc_free_func)(void* Block);

typedef struct
{
    twc_malloc_func MemAllocFunc;
    twc_free_func MemFreeFunc;

    CURL* cURL;
    twc_strbuf cURL_Error;
    twc_buffer cURL_Data;
    twc_buffer cURL_HeaderBuf;

    // OAuth parameters
    twc_oauth_keys Keys;
} twc_state;

typedef enum
{
    TWC_HTTP_GET = 0,
    TWC_HTTP_POST,
    TWC_HTTP_DELETE,
} twc_http_method;
typedef twc_option_t(twc_http_method) twc_http_method$;

// Defined in twitter.c
extern const char* twc_HttpMethodString[3];

// For your reference (for now)
typedef enum 
{
    TWC_API_Error_Cant_Authenticate         = 32,
    TWC_API_Error_Page_Nonexistent          = 34,
    TWC_API_Error_Account_Suspended         = 64,
    TWC_API_Error_API_Inactive              = 68,
    TWC_API_Error_Hit_Rate_Limit            = 88,
    TWC_API_Error_Invalid_Token             = 89,
    TWC_API_Error_SSL_Required              = 92,
    TWC_API_Error_Over_Capacity             = 130,
    TWC_API_Error_Internal                  = 131,
    TWC_API_Error_Timestamp_Unacceptable    = 135,
    TWC_API_Error_Blocked                   = 136,
    TWC_API_Error_Cant_Follow               = 161,
    TWC_API_Error_Not_Authorized            = 179,
    TWC_API_Error_Over_Post_Limit           = 185,
    TWC_API_Error_Duplicate_Status          = 187,
    TWC_API_Error_Bad_Authentication        = 215,
    TWC_API_Error_Automated_Request         = 226,
    TWC_API_Error_Need_Login_Verify         = 231,
    TWC_API_Error_Retired_Endpoint          = 251,
    TWC_API_Error_App_Cant_Write            = 261,
    TWC_API_Error_Cant_Mute_Self            = 271,
    TWC_API_Error_Not_Muting                = 272,
    TWC_API_Error_DM_Over_Char_Limit        = 354,
} twitter_error_code;

typedef enum
{
    TWC_No_Problem = 0,
    TWC_Error_JSON_Parse = 8000,
    TWC_Error_cURL_Error,
    TWC_Error_API_Call_Fail,
} twc_error;

typedef struct
{
    twc_error Error;
    union {
        twc_buffer Data;
        twc_string ErrorStr;
    };
} twc_call_result;


/*    .    ,--._  
 *   .\'-__(  )/
 *   \_       ;  
 *    \_     /     2) TWC API Functions
 *  -<__,--''  ----================================================================================--------
 * 
 * The meat of the API. The functions are split into logistics (dealing with
 * library constructs), Twitter API calls (included in from twitter_api.h),
 * and utility functions (used for building or modifying library constructs,
 * or calling helper procs that the library uses itself internally).
 *
 * 
 *     2.1) TWC Logistics
 * ----==============================--------
 * 
 * Functions that allow the API to set up and tear down the persistent data it keeps in the twc_state
 * structure, including the saved cURL instance.
 */

/* twc_Init:
 *   Initializes the twc_state structure using the provided parameters. Should be done before any other API
 *   calls are made.
 * @Twitter: The state structure to initialize. Assume that this function will touch every member of the
 *   structure (even if simply to zero it out).
 * @Keys: A set of four OAuth keys (Token Key/Secret, Consumer Key/Secret) to
 *   use when making twitter API calls.
 */
extern void
twc_Init(twc_state* State, twc_oauth_keys Keys);

/* twc_InitEx:
 *   Like twc_Init, but more options to pick from. 
 * @Twitter: The state structure to initialize. Assume that this function will touch every member of the
 *   structure (even if simply to zero it out).
 * @Keys: A set of four OAuth keys (Token Key/Secret, Consumer Key/Secret) to
 *   use when making twitter API calls.
 * @ErrorBuf: A buffer of at least ErrorBufSize characters of memory to store
 *   cURL error messages in.
 * @Alloc: Function the lib can use to allocate memory (mainly for buffering in
 *   data from cURL)
 * @Free: Function the lib should use to free memory allocated with the above.
 */
extern void
twc_InitEx(twc_state* Twitter, twc_oauth_keys Keys,
           char* ErrorBuf, size_t ErrorBufSize,
           twc_malloc_func Alloc, twc_free_func Free);

/* twc_Close:
 *   Clean up resources initialized with twc_Init or twc_InitEx. May call the
 *   Free function passed in earlier.
 * @Twitter: The state structure initialized earlier with twc_Init.
 */
extern void
twc_Close(twc_state* Twitter);

/*
 *     2.2) Twitter API
 * ----==============================--------
 * 
 * Functions that map directly to twitter API calls.
 */

#ifndef TWC_CODEGEN_FIRST_PASS
#include "twitter_api.h"
#endif

/* twc_Media_Upload:
 *   Temporarily included here so that your code can create tweets with media
 *   attachments. Full media API coming...eventually.
 * @Twitter: library state structure.
 * @Filename: Name of a file to upload to twitter. Only used if FileContents are
 *   not given.
 * @FileContents: An arbitrary length blob of data. Takes precedence over
 *   Filename if both are given. Note that Twitter places an upper bound on file
 *   sizes. 
 */
extern twc_call_result
twc_Media_Upload(twc_state* Twitter, twc_in char* Filename, twc_in twc_buffer FileContents);

/*
 *     2.3) Utility
 * ----==============================--------
 * 
 * Functions used in the operation of other parts of the API.
 * These are exposed so that you can call them directly, if you like, if your needs differ from what the
 * higher level parts of the API offer.
 *
 *   2.3.1) General
 * --======================----
 */

extern twc_call_result
twc_MakeCall(twc_state* State, twc_http_method Method, twc_in char* BaseURL, twc_key_value_list Params);

extern size_t
twc_cURL_Callback(char* Data, size_t Data_ItemSize, size_t Data_NumItems, void* StatePtr);

extern twc_buffer
twc_ConsumeData(twc_state* State);

extern void
twc_DiscardData(twc_state* State);

extern bool
twc_SerializeStatusId(twc_status_id StatusId, char* Buf);

extern twc_status_id
twc_DeserializeStatusId(const char* Buf, int BufSize);

twc_inline twc_string
twc_ToString(const char* CString) { 
    twc_string Result;
    Result.Size = strlen(CString);
    Result.Ptr = CString;
    return Result;
}

twc_inline int
twc_StringCompare(twc_string A, twc_string B) {
    if (A.Size == 0 && B.Size == 0) { return 0; }
    else if (A.Size == 0) { return -1; }
    else if (B.Size == 0) { return 1; }

    const char* a = A.Ptr;
    const char* b = B.Ptr;
    while (a - A.Ptr < (ptrdiff_t)A.Size && b - B.Ptr < (ptrdiff_t)B.Size)
    {
        if (*a < *b) { return -1; }
        else if (*a > *b) { return 1;}
        ++a; ++b;
    }
    if (A.Size == B.Size) { return 0; }
    else if (A.Size < B.Size) { return -1; }
    else if (A.Size > B.Size) { return 1; }
    return 0;
}

extern int
twc_URLParameterFilledLength(const char* URL, twc_string ParamValue);

extern int
twc_FillURLParameter(const char* URL, twc_string ParamValue, twc_strbuf Dest);

/*
 *   2.3.2) Encoding / Decoding Utilities
 * --======================----
 * Functions for converting to/from encoded formats used in the twitter API. 
 */

extern u64
twc_Base64EncodedLength(twc_in u64 SourceLength);

extern u64
twc_Base64Encode(twc_in u8* SourceBytes, twc_in u64 SourceLength,
                 twc_out char* DestBytes, u64 DestLength);

extern u64
twc_URLEncodedLength(twc_in char* SourceString, twc_in u64 SourceLength);

extern u64
twc_URLEncode(twc_in char* SourceString, twc_in u64 SourceLength, 
              twc_out char* DestBuffer, u64 DestBufferLen);

/*
 *   2.3.3) Hashing Utilities
 * --======================----
 * SHA1 hashing functions used for generating message digests in OAuth requests.
 */

typedef union twc_message_digest_sha1
{
    u8 Bytes[20];
    u32 Words[5];
} twc_message_digest_sha1;

/*
 * NOTE (IMPORTANT): THIS IS NOT CRYPTOGRAPHICALLY SECURE!
 * First, SHA-1 is proven to have collisions with fairly reasonable computation time. 
 *  This shouldn't be used for anything which actually has sensitive information.
 *  The use in the Twitter API is as a message digest, which is a reasonable use case when
 *   security is not a huge concern.
 * Second, this implementation has not been thoroughly tested for its own security. 
 *  Use at your own discretion.
 */
twc_message_digest_sha1
twc_Hash_SHA1(twc_in u8* Bytes, twc_in u64 LengthInBytes);

twc_message_digest_sha1
twc_MessageAuthenticationCode_SHA1(twc_in u8* Key,      twc_in u32 KeyLength, 
                                   twc_in u8* Message,  twc_in u32 MessageLength);

/*   2.3.4) OAuth Utilities
 * --======================----
 * Use these if you need to sign an API request with OAuth manually.
 */

extern int
twc_OAuthSignatureMaxLength();

extern int
twc_GenerateOAuthSignature(twc_in twc_http_method ReqType, twc_in char* BareURL, 
                           twc_in twc_key_value_list Params, 
                           twc_in char* ConsumerSecret, twc_in char* TokenSecret, 
                           twc_out char* Signature);

extern int
twc_OAuthHeaderMaxLength(twc_in twc_oauth_keys Keys);

extern int
twc_GenerateOAuthHeader(twc_in twc_http_method ReqType, twc_in char* BaseURL, 
                        twc_in twc_key_value_list ParamsSorted, 
                        twc_in twc_oauth_keys Keys, twc_out twc_strbuf OAuthHeader);

/*   2.3.5) KeyValue/KeyValueList Helpers
 * --======================================----
 * Functions useful for building and modifying Key/Value pairs and the Key/Value linked lists that are used
 * several places in the lower-level parts of the API.
 */

extern twc_key_value_pair
twc_KeyValue(const char* Key, const char* Value);

extern twc_key_value_list
twc_KeyValueList_Append(twc_key_value_list List, twc_key_value_pair* Item);

extern twc_key_value_list
twc_KeyValueList_InsertSorted(twc_key_value_list List, twc_key_value_pair* Item);

extern int
twc_KeyValueList_ToString(twc_in twc_key_value_list Params,
                      twc_in char* KeyValueSeparator, twc_in char* PairSeparator, 
                      twc_in bool QuoteValue, twc_in bool URLEncode, twc_out twc_strbuf Strbuf);

extern int
twc_KeyValueList_StringLength(twc_in twc_key_value_list Params, 
                              twc_in char* KeyValueSeparator, twc_in char* PairSeparator,
                              twc_in bool QuoteValue, twc_in bool URLEncode);

/*    .    ,--._  
 *   .\'-__(  )/
 *   \_       ;  
 *    \_     /     3) Constants and Macros
 *  -<__,--''  ----================================================================================--------
 */

// The size, in characters, of the buffer that TWC will pass to cURL
#define TWC_ERR_BUFSIZE 1024

// The size, in characters, of buffers used to serialize status IDs to strings.
#define TWC_STATUS_ID_BUFSIZE 20

// An empty status ID (e.g. for a twc_status_id parameter which has not been set)
#define TWC_STATUS_ID_NULL ((twc_status_id)0)

/*
 *     3.1) Twitter URL Pieces
 * ----==============================--------
 *
 * URL pieces defined as preprocessor constants so that we can take advantage of compiler string 
 * concatenation.
 */

#define TWC_PROTOCOL                            "https"
#define TWC_FORMAT                              ".json"
#define TWC_ENCODING                            "" //TODO: Set to UTF16 maybe?

#define TWC_API_VER                             "1.1"
#define TWC_URL_BASE                            TWC_PROTOCOL "://" "api.twitter.com/" TWC_API_VER "/"
#define TWC_URL_MEDIA                           TWC_PROTOCOL "://" "upload.twitter.com/" TWC_API_VER "/"

/*
 *     3.2) Twitter API URLs
 * ----==============================--------
 *
 * Prebaked URLs that can be passed directly to twc_MakeCall if you like, or even directly to cURL in most
 * cases.
 */

#define TWC_URL_APPLICATION_RATELIMITSTATUS     TWC_URL_BASE "application/rate_limit_status"    TWC_FORMAT

#define TWC_URL_ACCOUNT_VERIFYCREDENTIALS       TWC_URL_BASE "account/verify_credentials"       TWC_FORMAT
#define TWC_URL_ACCOUNT_RATELIMITSTATUS         TWC_URL_BASE "account/rate_limit_status"        TWC_FORMAT
#define TWC_URL_ACCOUNT_SETTINGS                TWC_URL_BASE "account/settings"                 TWC_FORMAT
#define TWC_URL_ACCOUNT_UPDATEPROFILE           TWC_URL_BASE "account/update_profile"           TWC_FORMAT
#define TWC_URL_ACCOUNT_UPDATEPROFILEIMAGE      TWC_URL_BASE "account/update_profile_image"     TWC_FORMAT
#define TWC_URL_ACCOUNT_UPDATEPROFILEBANNER     TWC_URL_BASE "account/update_profile_banner"    TWC_FORMAT
#define TWC_URL_ACCOUNT_REMOVEPROFILEBANNER     TWC_URL_BASE "account/remove_profile_banner"    TWC_FORMAT
#define TWC_URL_ACCOUNT_UPDATEPROFILEBACKGROUNDIMAGE    TWC_URL_BASE "account/update_profile_background_image"    TWC_FORMAT

#define TWC_URL_STATUSES_UPDATE                 TWC_URL_BASE "statuses/update"                  TWC_FORMAT
#define TWC_URL_STATUSES_UPDATEWITHMEDIA        TWC_URL_BASE "statuses/update_with_media"       TWC_FORMAT
#define TWC_URL_STATUSES_SHOW_ID                TWC_URL_BASE "statuses/show"                    TWC_FORMAT
#define TWC_URL_STATUSES_OEMBED                 TWC_URL_BASE "statuses/oembed"                  TWC_FORMAT
#define TWC_URL_STATUSES_DESTROY_ID             TWC_URL_BASE "statuses/destroy/:id"             TWC_FORMAT
#define TWC_URL_STATUSES_RETWEET_ID             TWC_URL_BASE "statuses/retweet/:id"             TWC_FORMAT
#define TWC_URL_STATUSES_UNRETWEET_ID           TWC_URL_BASE "statuses/unretweet/:id"           TWC_FORMAT
#define TWC_URL_STATUSES_RETWEETERS_IDS         TWC_URL_BASE "statuses/retweeters/ids"          TWC_FORMAT

#define TWC_URL_STATUSES_LOOKUP                 TWC_URL_BASE "statuses/lookup"                  TWC_FORMAT
#define TWC_URL_STATUSES_USERTIMELINE           TWC_URL_BASE "statuses/user_timeline"           TWC_FORMAT
#define TWC_URL_STATUSES_MENTIONSTIMELINE       TWC_URL_BASE "statuses/mentions_timeline"       TWC_FORMAT
#define TWC_URL_STATUSES_HOMETIMELINE           TWC_URL_BASE "statuses/home_timeline"           TWC_FORMAT
#define TWC_URL_STATUSES_RETWEETSOFME           TWC_URL_BASE "statuses/retweets_of_me"          TWC_FORMAT
#define TWC_URL_STATUSES_RETWEETS_ID            TWC_URL_BASE "statuses/retweets/:id"            TWC_FORMAT

#define TWC_URL_DIRECTMESSAGES                  TWC_URL_BASE "direct_messages"                  TWC_FORMAT
#define TWC_URL_DIRECTMESSAGES_SENT             TWC_URL_BASE "direct_messages/sent"             TWC_FORMAT
#define TWC_URL_DIRECTMESSAGES_SHOW             TWC_URL_BASE "direct_messages/show"             TWC_FORMAT
#define TWC_URL_DIRECTMESSAGES_NEW              TWC_URL_BASE "direct_messages/new"              TWC_FORMAT
#define TWC_URL_DIRECTMESSAGES_DESTROY          TWC_URL_BASE "direct_messages/destroy"          TWC_FORMAT

#define TWC_URL_FRIENDS_IDS                     TWC_URL_BASE "friends/ids"                      TWC_FORMAT
#define TWC_URL_FRIENDS_LIST                    TWC_URL_BASE "friends/list"                     TWC_FORMAT
#define TWC_URL_FOLLOWERS_IDS                   TWC_URL_BASE "followers/ids"                    TWC_FORMAT
#define TWC_URL_FOLLOWERS_LIST                  TWC_URL_BASE "followers/list"                   TWC_FORMAT
#define TWC_URL_FRIENDSHIPS_LOOKUP              TWC_URL_BASE "friendships/lookup"               TWC_FORMAT
#define TWC_URL_FRIENDSHIPS_INCOMING            TWC_URL_BASE "friendships/incoming"             TWC_FORMAT
#define TWC_URL_FRIENDSHIPS_OUTGOING            TWC_URL_BASE "friendships/outgoing"             TWC_FORMAT
#define TWC_URL_FRIENDSHIPS_CREATE              TWC_URL_BASE "friendships/create"               TWC_FORMAT
#define TWC_URL_FRIENDSHIPS_DESTROY             TWC_URL_BASE "friendships/destroy"              TWC_FORMAT
#define TWC_URL_FRIENDSHIPS_UPDATE              TWC_URL_BASE "friendships/update"               TWC_FORMAT
#define TWC_URL_FRIENDSHIPS_SHOW                TWC_URL_BASE "friendships/show"                 TWC_FORMAT
#define TWC_URL_FRIENDSHIPS_NORETWEETS_IDS      TWC_URL_BASE "friendships/no_retweets/ids"      TWC_FORMAT

#define TWC_URL_USERS_LOOKUP                    TWC_URL_BASE "users/lookup"                     TWC_FORMAT
#define TWC_URL_USERS_SHOW                      TWC_URL_BASE "users/show"                       TWC_FORMAT
#define TWC_URL_USERS_SEARCH                    TWC_URL_BASE "users/search"                     TWC_FORMAT
#define TWC_URL_USERS_PROFILEBANNER             TWC_URL_BASE "users/profile_banner"             TWC_FORMAT
#define TWC_URL_USERS_REPORTSPAM                TWC_URL_BASE "users/report_spam"                TWC_FORMAT

#define TWC_URL_USERS_SUGGESTIONS_SLUG          TWC_URL_BASE "users/suggestions/:slug"          TWC_FORMAT
#define TWC_URL_USERS_SUGGESTIONS               TWC_URL_BASE "user/suggestions"                 TWC_FORMAT
#define TWC_URL_USERS_SUGGESTIONS_SLUG_MEMBERS  TWC_URL_BASE "users/suggestions/:slug/members"  TWC_FORMAT

#define TWC_URL_FAVORITES_LIST                  TWC_URL_BASE "favorites/list"                   TWC_FORMAT
#define TWC_URL_FAVORITES_DESTROY               TWC_URL_BASE "favorites/destroy"                TWC_FORMAT
#define TWC_URL_FAVORITES_CREATE                TWC_URL_BASE "favorites/create"                 TWC_FORMAT

#define TWC_URL_LISTS_LIST                      TWC_URL_BASE "lists/list"                       TWC_FORMAT
#define TWC_URL_LISTS_SHOW                      TWC_URL_BASE "lists/show"                       TWC_FORMAT
#define TWC_URL_LISTS_STATUSES                  TWC_URL_BASE "lists/statuses"                   TWC_FORMAT
#define TWC_URL_LISTS_OWNERSHIPS                TWC_URL_BASE "lists/ownerships"                 TWC_FORMAT
#define TWC_URL_LISTS_DESTROY                   TWC_URL_BASE "lists/destroy"                    TWC_FORMAT
#define TWC_URL_LISTS_UPDATE                    TWC_URL_BASE "lists/update"                     TWC_FORMAT
#define TWC_URL_LISTS_CREATE                    TWC_URL_BASE "lists/create"                     TWC_FORMAT

#define TWC_URL_LISTS_SUBSCRIPTIONS             TWC_URL_BASE "lists/subscriptions"              TWC_FORMAT
#define TWC_URL_LISTS_SUBSCRIBERS               TWC_URL_BASE "lists/subscribers"                TWC_FORMAT
#define TWC_URL_LISTS_SUBSCRIBERS_SHOW          TWC_URL_BASE "lists/subscribers/show"           TWC_FORMAT
#define TWC_URL_LISTS_SUBSCRIBERS_CREATE        TWC_URL_BASE "lists/subscribers/create"         TWC_FORMAT
#define TWC_URL_LISTS_SUBSCRIBERS_DESTROY       TWC_URL_BASE "lists/subscribers/destroy"        TWC_FORMAT

#define TWC_URL_LISTS_MEMBERSHIPS               TWC_URL_BASE "lists/memberships"                TWC_FORMAT
#define TWC_URL_LISTS_MEMBERS                   TWC_URL_BASE "lists/members"                    TWC_FORMAT
#define TWC_URL_LISTS_MEMBERS_SHOW              TWC_URL_BASE "lists/members/show"               TWC_FORMAT
#define TWC_URL_LISTS_MEMBERS_CREATE            TWC_URL_BASE "lists/members/create"             TWC_FORMAT
#define TWC_URL_LISTS_MEMBERS_CREATEALL         TWC_URL_BASE "lists/members/create_all"         TWC_FORMAT
#define TWC_URL_LISTS_MEMBERS_DESTROY           TWC_URL_BASE "lists/members/destroy"            TWC_FORMAT
#define TWC_URL_LISTS_MEMBERS_DESTROYALL        TWC_URL_BASE "lists/members/destroy_all"        TWC_FORMAT

#define TWC_URL_MUTES_USERS_CREATE              TWC_URL_BASE "mutes/users/create"               TWC_FORMAT
#define TWC_URL_MUTES_USERS_DESTROY             TWC_URL_BASE "mutes/users/destroy"              TWC_FORMAT
#define TWC_URL_MUTES_USERS_IDS                 TWC_URL_BASE "mutes/users/ids"                  TWC_FORMAT
#define TWC_URL_MUTES_USERS_LIST                TWC_URL_BASE "mutes/users/list"                 TWC_FORMAT

#define TWC_URL_BLOCKS_LIST                     TWC_URL_BASE "blocks/list"                      TWC_FORMAT
#define TWC_URL_BLOCKS_IDS                      TWC_URL_BASE "blocks/ids"                       TWC_FORMAT
#define TWC_URL_BLOCKS_CREATE                   TWC_URL_BASE "blocks/create"                    TWC_FORMAT
#define TWC_URL_BLOCKS_DESTROY                  TWC_URL_BASE "blocks/destroy"                   TWC_FORMAT

#define TWC_URL_SEARCH_TWEETS                   TWC_URL_BASE "search/tweets"                    TWC_FORMAT
#define TWC_URL_SAVEDSEARCHES_LIST              TWC_URL_BASE "saved_searches/list"              TWC_FORMAT
#define TWC_URL_SAVEDSEARCHES_SHOW_ID           TWC_URL_BASE "saved_searches/show/:id"          TWC_FORMAT
#define TWC_URL_SAVEDSEARCHES_CREATE            TWC_URL_BASE "saved_searches/create"            TWC_FORMAT
#define TWC_URL_SAVEDSEARCHES_DESTROY_ID        TWC_URL_BASE "saved_searches/destroy/:id"       TWC_FORMAT

#define TWC_URL_GEO_ID_PLACEID                  TWC_URL_BASE "geo/id/:place_id"                 TWC_FORMAT
#define TWC_URL_GEO_REVERSEGEOCODE              TWC_URL_BASE "geo/reverse_geocode"              TWC_FORMAT
#define TWC_URL_GEO_SEARCH                      TWC_URL_BASE "geo/search"                       TWC_FORMAT
#define TWC_URL_GEO_PLACE                       TWC_URL_BASE "geo/place"                        TWC_FORMAT

#define TWC_URL_TRENDS_PLACE                    TWC_URL_BASE "trends/place"                     TWC_FORMAT
#define TWC_URL_TRENDS_AVAILABLE                TWC_URL_BASE "trends/available"                 TWC_FORMAT
#define TWC_URL_TRENDS_CLOSEST                  TWC_URL_BASE "trends/closest"                   TWC_FORMAT

#define TWC_URL_HELP_CONFIGURATION              TWC_URL_BASE "help/configuration"               TWC_FORMAT
#define TWC_URL_HELP_LANGUAGES                  TWC_URL_BASE "help/languages"                   TWC_FORMAT
#define TWC_URL_HELP_PRIVACY                    TWC_URL_BASE "help/privacy"                     TWC_FORMAT
#define TWC_URL_HELP_TOS                        TWC_URL_BASE "help/tos"                         TWC_FORMAT

#define TWC_URL_COLLECTIONS_CREATE              TWC_URL_MEDIA "collections/create"              TWC_FORMAT
#define TWC_URL_COLLECTIONS_DESTROY             TWC_URL_MEDIA "collections/destroy"             TWC_FORMAT
#define TWC_URL_COLLECTIONS_LIST                TWC_URL_MEDIA "collections/list"                TWC_FORMAT
#define TWC_URL_COLLECTIONS_SHOW                TWC_URL_MEDIA "collections/show"                TWC_FORMAT
#define TWC_URL_COLLECTIONS_UPDATE              TWC_URL_MEDIA "collections/update"              TWC_FORMAT
#define TWC_URL_COLLECTIONS_ENTRIES             TWC_URL_MEDIA "collections/entries"             TWC_FORMAT
#define TWC_URL_COLLECTIONS_ENTRIES_ADD         TWC_URL_MEDIA "collections/entries/add"         TWC_FORMAT
#define TWC_URL_COLLECTIONS_ENTRIES_CURATE      TWC_URL_MEDIA "collections/entries/curate"      TWC_FORMAT
#define TWC_URL_COLLECTIONS_ENTRIES_MOVE        TWC_URL_MEDIA "collections/entries/move"        TWC_FORMAT
#define TWC_URL_COLLECTIONS_ENTRIES_REMOVE      TWC_URL_MEDIA "collections/entries/remove"      TWC_FORMAT

#define TWC_URL_PROJECTS                        TWC_URL_MEDIA "project"                         TWC_FORMAT

#define TWC_URL_MEDIA_UPLOAD                    TWC_URL_MEDIA "media/upload"                    TWC_FORMAT


/*
 *      3.3) URL Parameter Keys
 *  ----==============================--------
 */

// Authorization header
#define TWC_OAUTH_CONSUMERKEY                   "oauth_consumer_key"
#define TWC_OAUTH_NONCE                         "oauth_nonce"
#define TWC_OAUTH_SIGNATURE                     "oauth_signature"
#define TWC_OAUTH_SIGNATUREMETHOD               "oauth_signature_method"
#define TWC_OAUTH_VERSION                       "oauth_version"
#define TWC_OAUTH_TOKEN                         "oauth_token"
#define TWC_OAUTH_TIMESTAMP                     "oauth_timestamp"

#define TWC_TRUE_STR                            "true"
#define TWC_FALSE_STR                           "false"

#define TWC_BOOLSTR(b) ((b) ? TWC_TRUE_STR : TWC_FALSE_STR)

#ifdef __cplusplus
}
#endif

#define TWITTER_H
#endif
