/* twitter.c
 *  by Andrew 'ChronalDragon' Chronister
 *
 * Implementations of all the functions defined in twitter.h
 * Includes SHA1 hashing and URL encoding utilities.
 */

#include <stdio.h> // For snprintf
#include <string.h> // For memcpy
#include <stdlib.h>
#include <malloc.h> // For alloca (MSVC)
#ifdef __GNUC__
#include <alloc.h> // alloca on GCC
#endif
#include <assert.h>
#include <time.h>   // For time(), needed for generating timestamps for OAuth
#include <curl/curl.h>
#include "twitter.h"

#include "byte_utils.c"

#define twc_malloc(n) State->MemAllocFunc(n)
#define twc_free(m) State->MemFreeFunc(m)

// Even though it's declared inline in the header, GCC (and others?) may need an
// extern declaration so they know in which TU to put the generated code.
extern twc_string twc_ToString(const char* CString);

const char* twc_HttpMethodString[3] = {
    "GET",
    "POST",
    "DELETE",
};

static inline int
ClampAmount(int Desired, int Available)
{
    if (Available < 0) { Available = 0; }
    return Available >= Desired ? Desired : Available;
}

twc_string TSTR(const char* String)
{
    twc_string Result = {0};
    Result.Ptr = String;
    Result.Size = strlen(Result.Ptr);
    return Result;
}

extern bool
twc_SerializeStatusId(twc_status_id StatusId, char Buf[TWC_STATUS_ID_BUFSIZE])
{
    return snprintf(Buf, TWC_STATUS_ID_BUFSIZE, "%"PRIu64, StatusId) > 0;
}

extern bool
twc_SerializePlaceId(twc_place_id StatusId, char Buf[TWC_STATUS_ID_BUFSIZE])
{
    return snprintf(Buf, TWC_STATUS_ID_BUFSIZE, "%"PRIx64, StatusId) > 0;
}

extern twc_status_id
twc_DeserializeStatusId(const char* Buf, int BufSize)
{
    twc_status_id Result = 0;
    const char* C = Buf + BufSize - 1;
    u64 PlaceValue = 1;
    while (Buf && *C && BufSize > 0 && *C >= '0' && *C <= '9')
    {
        Result += PlaceValue * (*C - '0');
        PlaceValue *= 10;
        C--;
        BufSize--;
    }
    return Result;
}

#if 0
//TODO(chronister): Put the 30 in a #define and figure out what size it needs to be
static bool
twc_SerializeLatLong(double LatLong, char Buf[30])
{
    return snprintf(Buf, 30, "%.13f", LatLong) > 0;
}
#endif

//TODO(chronister): Put the 13 in a #define and figure out what size it needs to be
bool
twc_SerializeInt(int Val, char Buf[13])
{
    return snprintf(Buf, 13, "%d", Val) > 0;
}

#if 0
static int 
twc_CStringAlphaCompare(const char* A, const char* B)
{
    if (A == NULL && B == NULL) { return 0; }
    else if (A == NULL) { return 1; }
    else if (B == NULL) { return -1; }

    while (*A && *B) {
        if (*A > *B) { return -1; }
        else if (*A < *B) { return 1; }
        A++; B++; 
    } 

    return 0;
}
#endif

// Bits into bytes
#define BITS(bits) (bits/8)
#define SHA1_BLOCK_SIZE BITS(512)
#define MD5_BLOCK_SIZE BITS(512)

char Base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char HexChar[] = "0123456789ABCDEF";

u64
twc_Base64EncodedLength(twc_in u64 SourceLengthInBytes)
{
    u64 BlockSize = BITS(24);
    u64 BlockCount = SourceLengthInBytes / BlockSize;
    if (BlockCount * BlockSize < SourceLengthInBytes) ++BlockCount;

    return BlockCount * 4; // 4 6-bit base64 chars per block
}

u64
twc_Base64Encode(twc_in u8* SourceBytes, twc_in u64 SourceLength, twc_out char* DestBytes, u64 DestLength)
{
    u64 ResultLength = twc_Base64EncodedLength(SourceLength);

    u32 BlockSize = BITS(24);
    u64 BlockCount = ResultLength / 4;

    for (u32 BlockIndex = 0;
        BlockIndex < BlockCount;
        ++BlockIndex)
    {
        u32 Block = 0;
        int RemainingBytes = SourceLength - BlockIndex*BlockSize;

        if (BlockIndex*BlockSize + sizeof(u32) <= SourceLength)
        {
            Block = *(u32*)(SourceBytes + (BlockIndex*BlockSize)) & 0x00FFFFFF; // Get 24 bits of the message
            Block = SwapByteOrder(Block);
        }
        else
        {
            if (RemainingBytes > 0) { Block |= (SourceBytes[BlockIndex*BlockSize] << 24); }
            if (RemainingBytes > 1) { Block |= (SourceBytes[BlockIndex*BlockSize + 1] << 16); }
            if (RemainingBytes > 2) { Block |= (SourceBytes[BlockIndex*BlockSize + 2] << 8); }
        }

        // Read them out from the top, since we flipped the number above
        // to get the "big-endian" order
        u32 Bits1 = (Block & (63 << 26)) >> 26;
        u32 Bits2 = (Block & (63 << 20)) >> 20;
        u32 Bits3 = (Block & (63 << 14)) >> 14; 
        u32 Bits4 = (Block & (63 << 8)) >> 8;

        u32 ResultIndex = BlockIndex*4;
        u8 Bytes[4];
        Bytes[0] = Base64[Bits1];
        Bytes[1] = Base64[Bits2];
        Bytes[2] = Base64[Bits3];
        Bytes[3] = Base64[Bits4];

        if (RemainingBytes == 1) { Bytes[2] = '='; }
        if (RemainingBytes <= 2) { Bytes[3] = '='; }

        memcpy(DestBytes + ResultIndex, Bytes, 4);
    }

    return ResultLength;
}

static bool
CharIsUnreserved(char C)
{
    return (('A' <= C && C <= 'Z') || ('a' <= C && C <= 'z') ||
            ('0' <= C && C <= '9') ||
            (C == '-' || C == '_' || C == '.' || C == '~'));
}

u64
twc_URLEncodedLength(const char* SourceString, const u64 SourceLength)
{
    u64 Result = 0;
    for (u64 CharIndex = 0; CharIndex < SourceLength; ++CharIndex)
    {
        if (CharIsUnreserved(SourceString[CharIndex])) {
            Result += 1;
        }
        else {
            Result += 3;
        }
    }
    return Result;
}

u64
twc_URLEncode(const char* SourceString, const u64 SourceLength, char* DestBuffer, u64 DestBufferLen)
{
    u64 DestIndex = 0;
    for (u64 SourceIndex = 0; SourceIndex < SourceLength && DestIndex < DestBufferLen; ++SourceIndex)
    {
        if (CharIsUnreserved(SourceString[SourceIndex])) {
            if (DestIndex + 1 <= DestBufferLen) {
                DestBuffer[DestIndex] = SourceString[SourceIndex];
            }
            DestIndex += 1;
        }
        else {
            if (DestIndex + 3 <= DestBufferLen) {
                // NOTE(chronister): According to twitter's "percent-encoding" algorithm,
                // even multibyte characters should be considered byte by byte, so this will
                // be correct regardless.
                u8 CharValue = (u8)(SourceString[SourceIndex]);
                DestBuffer[DestIndex] = '%';
                DestBuffer[DestIndex + 1] = HexChar[(CharValue & 0xF0) >> 4];
                DestBuffer[DestIndex + 2] = HexChar[CharValue & 0x0F];
            }
            DestIndex += 3;
        }
    }
    return DestIndex;
}

//NOTE (IMPORTANT): THIS IS NOT CRYPTOGRAPHICALLY SECURE!
// First, SHA-1 is proven to have collisions with somewhat reasonable computation time. 
//   This shouldn't be used for anything which actually has sensitive information.
// Second, this implementation has not been thoroughly tested for its own security. 
//   Use at your own discretion.
twc_message_digest_sha1
twc_Hash_SHA1(twc_in u8* Bytes, twc_in u64 LengthInBytes)
{
    assert(Bytes);
    //We'll need to store this as number of bits
    //Also, I really hope no one has _that much_ memory. 
    //If it's actually that high, it might be bogus for other reasons.
    assert(LengthInBytes < UINT64_MAX/8); 

    u32 H[5] = { 0x67452301,
                 0xEFCDAB89,
                 0x98BADCFE,
                 0x10325476,
                 0xC3D2E1F0 };

    u32 K[4] = { 0x5A827999,
                0x6ED9EBA1,
                0x8F1BBCDC,
                0xCA62C1D6 };

    const u32 ChunkSize = SHA1_BLOCK_SIZE;
    u8 MemBuf[SHA1_BLOCK_SIZE];

    u64 NumberOfChunks = LengthInBytes / ChunkSize + 1; // Must leave room for the final 1-bit and message length at the end
    u32 CalculationWords[80];

    assert(ChunkSize == 16*sizeof(u32)); // This assumption is baked into the algorithm going forward

    for (u32 ChunkIndex = 0;
        ChunkIndex < NumberOfChunks;
        ++ChunkIndex)
    {
        memset(MemBuf, 0, ChunkSize);
        u64 ChunkOffset = ChunkIndex*ChunkSize;
        s64 Remaining = LengthInBytes - ChunkOffset;

        if (Remaining >= ChunkSize)
        {
            memcpy(MemBuf, Bytes + ChunkOffset, ChunkSize);
        }
        else if (Remaining <= 0)
        {
            if (Remaining == 0)
            {
                MemBuf[0] = 0x80;   
            }
            // Else, the 1 bit was included at the end of the last chunk.

            * (u64*)(MemBuf + (ChunkSize - sizeof(u64))) = SwapByteOrder64(LengthInBytes*8);
        }
        else
        {
            memcpy(MemBuf, Bytes + ChunkOffset, Remaining);
            MemBuf[Remaining] = 0x80;
            if ((Remaining + 1) <= (s32)(ChunkSize - sizeof(u64)))
            {
                * (u64*)(MemBuf + (ChunkSize - sizeof(u64))) = SwapByteOrder64(LengthInBytes*8);
            }
            else { ++NumberOfChunks; }
        }

        for(u32 t = 0; t < 16; t++)
        {
            CalculationWords[t]  = MemBuf[t * 4] << 24;
            CalculationWords[t] |= MemBuf[t * 4 + 1] << 16;
            CalculationWords[t] |= MemBuf[t * 4 + 2] << 8;
            CalculationWords[t] |= MemBuf[t * 4 + 3];
        }

        for (u32 WordIndex = 16;
            WordIndex < 80;
            ++WordIndex)
        {
            CalculationWords[WordIndex] = 
                RotateLeft(CalculationWords[WordIndex - 3] ^
                           CalculationWords[WordIndex - 8] ^
                           CalculationWords[WordIndex - 14] ^
                           CalculationWords[WordIndex - 16], 1);
        }

        u32 A = H[0];
        u32 B = H[1];
        u32 C = H[2];
        u32 D = H[3];
        u32 E = H[4];
        u32 F = 0x0;
        u32 Ki = 0x0;

        for (u32 WordIndex = 0;
            WordIndex < 80;
            ++WordIndex)
        { 
            if (0 <= WordIndex && WordIndex < 20)
            {
                F = (B & C) | ((~B) & D);
                Ki = 0;
            }
            else if (20 <= WordIndex && WordIndex < 40)
            {
                F = (B ^ C ^ D);
                Ki = 1;
            }
            else if (40 <= WordIndex && WordIndex < 60)
            {
                F = (B & C) | (B & D) | (C & D);
                Ki = 2;
            }
            else if (60 <= WordIndex && WordIndex < 80)
            {
                F = (B ^ C ^ D);
                Ki = 3;
            }

            u32 Temp = (RotateLeft(A, 5) + F + E + K[Ki] + CalculationWords[WordIndex]);
            E = D;
            D = C;
            C = RotateLeft(B, 30);
            B = A;
            A = Temp;
        }

        H[0] += A;
        H[1] += B;
        H[2] += C;
        H[3] += D;
        H[4] += E;
    }

    twc_message_digest_sha1 Hash = {0};
    for(int i = 0; i < countof(Hash.Bytes); ++i)
    {
        Hash.Bytes[i] = H[i>>2] >> 8 * ( 3 - ( i & 0x03 ) ); // Gotta flip back to little endian here
    }

    return Hash;
}

twc_message_digest_sha1
twc_MessageAuthenticationCode_SHA1(twc_in u8* Key, twc_in u32 KeyLength, twc_in u8* Message, twc_in u32 MessageLength)
{
    twc_message_digest_sha1 KeyHash;
    u8 KeyBuf[SHA1_BLOCK_SIZE];

    u32 KeyLen = KeyLength;

    if (KeyLen > SHA1_BLOCK_SIZE)
    {
        KeyHash = twc_Hash_SHA1(Key, KeyLen);
        Key = KeyHash.Bytes;
        KeyLen = countof(KeyHash.Bytes);
    }
    if (KeyLen <= SHA1_BLOCK_SIZE)
    {
        memset(KeyBuf, 0, countof(KeyBuf));
        memcpy(KeyBuf, Key, KeyLen);
        Key = KeyBuf;
        KeyLen = SHA1_BLOCK_SIZE;
    }
    
    u8 OuterPad[SHA1_BLOCK_SIZE];
    u8 InnerPad[SHA1_BLOCK_SIZE];
    for (u32 ByteIndex = 0;
        ByteIndex < KeyLen;
        ++ByteIndex)
    {
        OuterPad[ByteIndex] = Key[ByteIndex] ^ 0x5c;
        InnerPad[ByteIndex] = Key[ByteIndex] ^ 0x36;
    }

    u32 IntermediateBuf1Count = countof(InnerPad) + MessageLength;
    u8* IntermediateBuf1 = (u8*)malloc(IntermediateBuf1Count);

    memcpy(IntermediateBuf1, InnerPad, countof(InnerPad));
    memcpy(IntermediateBuf1+countof(InnerPad), Message, MessageLength);

    twc_message_digest_sha1 Intermediate = twc_Hash_SHA1(IntermediateBuf1, IntermediateBuf1Count);

    u8 IntermediateBuf2[countof(OuterPad) + countof(Intermediate.Bytes)];
    memcpy(IntermediateBuf2, OuterPad, countof(OuterPad));
    memcpy(IntermediateBuf2+countof(OuterPad), Intermediate.Bytes, countof(Intermediate.Bytes));

    twc_message_digest_sha1 Result = twc_Hash_SHA1(IntermediateBuf2, countof(IntermediateBuf2));
    free(IntermediateBuf1);
    return Result;
}

extern void
PrintDigest(twc_message_digest_sha1 Digest)
{
    for(int i = 0; i < 20; ++i)
    {
        printf("%02x", Digest.Bytes[i]);
    }
    printf("\n");
}

extern int
twc_FillURLParameter(const char* URL, twc_string ParamValue, twc_strbuf Dest)
{
    const char* C;
    for (C = URL; *C && (*C != ':' || (*C == ':' && *(C + 1) == '/' && *(C + 2) == '/')); ++C);
    int PrefixLen = C - URL;
    int ParamLen = (int)ParamValue.Size;

    memcpy(Dest.Ptr, URL, ClampAmount(PrefixLen, (int)Dest.Size));
    memcpy(Dest.Ptr + PrefixLen, ParamValue.Ptr, ClampAmount(ParamLen, (int)Dest.Size - PrefixLen));
    
    for (; *C && *C != '.' && *C != '/'; ++C);
    int SuffixLen = (int)strlen(C);

    memcpy(Dest.Ptr + PrefixLen + ParamLen, C, ClampAmount(SuffixLen, (int)Dest.Size - PrefixLen - ParamLen));

    return PrefixLen + ParamLen + SuffixLen;
}

extern int
twc_URLParameterFilledLength(const char* URL, twc_string ParamValue)
{
    twc_strbuf NullBuf = {0};
    return twc_FillURLParameter(URL, ParamValue, NullBuf);
}

extern twc_key_value_list
twc_KeyValueList_Append(twc_key_value_list List, twc_key_value_pair* Item)
{
    Item->Next = NULL;
    if (List == NULL) { return Item; }
    else if (List->Next == NULL) {
        List->Next = Item;
        return List;
    }
    else { return twc_KeyValueList_Append(List->Next, Item); }
}

extern twc_key_value_list
twc_KeyValueList_InsertSorted(twc_key_value_list List, twc_key_value_pair* Item)
{
    Item->Next = NULL;
    if (List == NULL) { return Item; }
    else {
        int Compare = twc_StringCompare(List->Key, Item->Key);
        if (Compare > 0) {
            Item->Next = List;
            return Item;
        }
        else {
            List->Next = twc_KeyValueList_InsertSorted(List->Next, Item);
            return List;
        }
    }
}

extern void
twc_InitEx(twc_state* State, twc_oauth_keys Keys,
         char* ErrorBuf, size_t ErrorBufSize,
         twc_malloc_func Alloc, twc_free_func Free)
{
    State->MemAllocFunc = Alloc;
    State->MemFreeFunc = Free;

    State->Keys = Keys;

    State->cURL_Error.Size = ErrorBufSize;
    State->cURL_Error.Ptr = ErrorBuf;
    State->cURL_Data.Size = 0;
    State->cURL_Data.Ptr = NULL;

    State->cURL = curl_easy_init();

    // Turn off verify peer
    // TODO(chronister): Necessary?
    curl_easy_setopt(State->cURL, CURLOPT_SSL_VERIFYPEER, 0);

    // Set curl buffers and callbacks
    curl_easy_setopt(State->cURL, CURLOPT_ERRORBUFFER, State->cURL_Error);
    curl_easy_setopt(State->cURL, CURLOPT_WRITEFUNCTION, twc_cURL_Callback);
    curl_easy_setopt(State->cURL, CURLOPT_WRITEDATA, State);
}

extern void
twc_Init(twc_state* State, twc_oauth_keys Keys)
{
    // TODO what should this use if you don't want to use the CRT
    twc_InitEx(State, Keys, (char*)malloc(1024), 1024, malloc, free);
}

extern size_t
twc_cURL_Callback(char* Data, size_t Data_ItemSize, size_t Data_NumItems, void* StatePtr)
{
    twc_state* State = (twc_state*)StatePtr;
    size_t DataSize = Data_ItemSize * Data_NumItems;

    size_t TotalSize = State->cURL_Data.Size + DataSize;

    // TODO(chronister): Would it be more performant to maintain a largish (~100kb) buffer and only
    // grow if bigger than that?
    // Keeping in mind that when the user consumes the data they want to be able to continue using
    // it past when new data comes in....
    void* NewMem = twc_malloc(TotalSize);
    if (NewMem == NULL) 
    {
        fprintf(stderr, "Couldn't allocate memory for incoming cURL data!");
        return 0;
    }
    if (State->cURL_Data.Ptr != NULL) {
        memcpy(NewMem, State->cURL_Data.Ptr, State->cURL_Data.Size);
    }
    memcpy((char*)NewMem + State->cURL_Data.Size, Data, DataSize);

    twc_free(State->cURL_Data.Ptr);
    State->cURL_Data.Ptr = NewMem;
    State->cURL_Data.Size = TotalSize;
    return DataSize;
}

extern twc_buffer
twc_ConsumeData(twc_state* State)
{
    twc_buffer Result = {0};
    Result = State->cURL_Data;

    State->cURL_Data.Size = 0;
    State->cURL_Data.Ptr = NULL;

    return Result;
}

extern void
twc_DiscardData(twc_state* State)
{
    twc_free(State->cURL_Data.Ptr);
    State->cURL_Data.Size = 0;
    State->cURL_Data.Ptr = NULL;
}

extern void 
twc_Close(twc_state* State)
{
    twc_free(State->cURL_Data.Ptr);
    twc_free(State->cURL_Error.Ptr);
    curl_easy_cleanup(State->cURL);

    State->cURL_Data.Size = 0;
    State->cURL_Data.Ptr = NULL;
    State->cURL_Error.Size = 0;
    State->cURL_Error.Ptr = NULL;
    State->cURL = NULL;
    State->MemAllocFunc = NULL;
    State->MemFreeFunc = NULL;
}

extern twc_key_value_pair
twc_KeyValue(const char* Key, const char* Value)
{
    twc_key_value_pair Result;
    Result.Key = TSTR(Key);
    Result.Value = TSTR(Value);
    Result.Next = NULL;
    return Result;
}

extern twc_key_value_pair
twc_KeyValueStr(const char* Key, twc_string Value)
{
    twc_key_value_pair Result;
    Result.Key = TSTR(Key);
    Result.Value = Value;
    Result.Next = NULL;
    return Result;
}

static inline int
twc_KeyValue_StringLength(const char* Key, const char* Value)
{
    return (int)(strlen(Key) + strlen(Value));
}


extern int
twc_KeyValueList_ToString(twc_in twc_key_value_list Params,
                          twc_in char* KeyValueSeparator, twc_in char* PairSeparator, 
                          twc_in bool QuoteValue, twc_in bool URLEncodeKeyValues, twc_out twc_strbuf Strbuf)
{
    int PairSepLen = (int)strlen(PairSeparator);
    int KeyValueSepLen = (int)strlen(KeyValueSeparator);

    int TotalSize = 0;
    twc_key_value_pair* Param;
    for (Param = Params; Param != NULL; Param = Param->Next)
    {
        if (Param != Params) {
            strncpy(Strbuf.Ptr + TotalSize, PairSeparator, ClampAmount(PairSepLen, (int)Strbuf.Size - TotalSize));
            TotalSize += PairSepLen;
        }

        if (URLEncodeKeyValues) 
        {
            int URLEncodedLength = twc_URLEncodedLength(Param->Key.Ptr, (int)Param->Key.Size);
            twc_URLEncode(Param->Key.Ptr, Param->Key.Size, Strbuf.Ptr + TotalSize, ClampAmount(URLEncodedLength, (int)Strbuf.Size - TotalSize));
            TotalSize += URLEncodedLength;
        }
        else 
        {
            strncpy(Strbuf.Ptr + TotalSize, Param->Key.Ptr, ClampAmount((int)Param->Key.Size, (int)Strbuf.Size - TotalSize));
            TotalSize += Param->Key.Size;
        }

        strncpy(Strbuf.Ptr + TotalSize, KeyValueSeparator, ClampAmount(KeyValueSepLen, (int)Strbuf.Size - TotalSize));
        TotalSize += KeyValueSepLen;

        if (QuoteValue) {
            strncpy(Strbuf.Ptr + TotalSize, "\"", ClampAmount(1, (int)Strbuf.Size - TotalSize));
            TotalSize += 1;
        }

        if (URLEncodeKeyValues) 
        {
            int URLEncodedLength = twc_URLEncodedLength(Param->Value.Ptr, (int)Param->Value.Size);
            twc_URLEncode(Param->Value.Ptr, Param->Value.Size, Strbuf.Ptr + TotalSize, ClampAmount(URLEncodedLength, (int)Strbuf.Size - TotalSize));
            TotalSize += URLEncodedLength;
        }
        else 
        {
            strncpy(Strbuf.Ptr + TotalSize, Param->Value.Ptr, ClampAmount((int)Param->Value.Size, (int)Strbuf.Size - TotalSize));
            TotalSize += Param->Value.Size;
        }

        if (QuoteValue) {
            strncpy(Strbuf.Ptr + TotalSize, "\"", ClampAmount(1, (int)Strbuf.Size - TotalSize));
            TotalSize += 1;
        }
    }

    return TotalSize;
}

extern int
twc_KeyValueList_StringLength(twc_in twc_key_value_list Params, 
                              twc_in char* KeyValueSeparator, twc_in char* PairSeparator,
                              twc_in bool QuoteValue, twc_in bool URLEncode)
{
    twc_strbuf NullBuf = {0};
    return twc_KeyValueList_ToString(Params, KeyValueSeparator, PairSeparator, QuoteValue, URLEncode, NullBuf);
}

extern int
twc_GenerateOAuthSignature(twc_in twc_http_method ReqType, twc_in char* BareURL, 
                           twc_in twc_key_value_list Params, 
                           twc_in char* ConsumerSecret, twc_in char* TokenSecret, 
                           twc_out char* Signature)
{
    char* Base;
    switch(ReqType) {
        case TWC_HTTP_GET: {
            Base = "GET&";
        } break;

        case TWC_HTTP_POST: {
            Base = "POST&";
        } break;

        case TWC_HTTP_DELETE: {
            Base = "DELETE&";
        } break;

        default: {
            return -1;
        } break;
    }
    int BaseLen = (int)strlen(Base);

    twc_strbuf ParamString;
    ParamString.Size = twc_KeyValueList_StringLength(Params, "=", "&", false, true);
    ParamString.Ptr = alloca(ParamString.Size);

    twc_KeyValueList_ToString(Params, "=", "&", false, true, ParamString);

    int EncodedURLLen = twc_URLEncodedLength(BareURL, strlen(BareURL));
    char* EncodedURL = alloca(EncodedURLLen);
    twc_URLEncode(BareURL, (int)strlen(BareURL), EncodedURL, EncodedURLLen);

    int EncodedParamsLen = twc_URLEncodedLength(ParamString.Ptr, ParamString.Size);
    char* EncodedParams = alloca(EncodedParamsLen);
    twc_URLEncode(ParamString.Ptr, ParamString.Size, EncodedParams, EncodedParamsLen);

    int SigContentLen = BaseLen + EncodedURLLen + 1 + EncodedParamsLen;
    char* SigContent = alloca(SigContentLen);
    strncpy(SigContent, Base, BaseLen);
    strncpy(SigContent + BaseLen, EncodedURL, EncodedURLLen);
    strncpy(SigContent + BaseLen + EncodedURLLen, "&", 1);
    strncpy(SigContent + BaseLen + EncodedURLLen + 1, EncodedParams, EncodedParamsLen);

    //printf("SIG CONTENT: \n%.*s\n", SigContentLen, SigContent);

    int ConsumerSecretLen = (int)strlen(ConsumerSecret);
    int TokenSecretLen = (int)strlen(TokenSecret);
    int SigningKeyLen = ConsumerSecretLen + 1 + TokenSecretLen;
    char* SigningKey = alloca(SigningKeyLen); // Temporary
    strncpy(SigningKey, ConsumerSecret, ConsumerSecretLen);
    strncpy(SigningKey + ConsumerSecretLen, "&", 1);
    strncpy(SigningKey + ConsumerSecretLen + 1, TokenSecret, TokenSecretLen);

    twc_message_digest_sha1 SigDigest = twc_MessageAuthenticationCode_SHA1((u8*)SigningKey, SigningKeyLen, (u8*)SigContent, SigContentLen);

    int SigBase64Len = twc_Base64EncodedLength(sizeof(SigDigest));
    char* SigBase64 = malloc(SigBase64Len); // Temporary
    twc_Base64Encode(SigDigest.Bytes, sizeof(SigDigest), SigBase64, SigBase64Len);

    int SignatureLen = twc_URLEncodedLength(SigBase64, SigBase64Len);
    twc_URLEncode(SigBase64, SigBase64Len, Signature, SignatureLen);

    return SignatureLen;
}

extern int
twc_OAuthSignatureMaxLength()
{
    // This is an UPPER LIMIT! A fairly pessimistic one, too. It assumes that
    // the base64-encoded hash digest is essentially all slashes.
    // Wouldn't be able to calculate actual size without actually doing the 
    // whole hash directly. 
    // If you want to know the actual length, without having a buffer available,
    // call twc_GenerateOAuthSignature with a NULL buffer.
    
    return 3 * twc_Base64EncodedLength(sizeof(twc_message_digest_sha1));
}

extern int
twc_GenerateOAuthHeader(twc_in twc_http_method ReqType, twc_in char* BaseURL, twc_in twc_key_value_list ParamsSorted, 
                   twc_in twc_oauth_keys Keys, twc_out twc_strbuf OAuthHeader)
{
    srand(time(NULL));
    char Nonce[17]; // Enough to hold a 64-bit number in base 16
    memset(Nonce, '\0', sizeof(Nonce));
    snprintf(Nonce, 17, "%"PRIx64, rand() + ((u64)rand() << 16) + ((u64)rand() << 32) + ((u64)rand() << 48)); 

    char Timestamp[22]; // Enough to hold a 64-bit number in base 10
    memset(Timestamp, '\0', sizeof(Timestamp));
    snprintf(Timestamp, 22, "%"PRIu64, time(NULL));

    twc_key_value_list AllParams = ParamsSorted;

    twc_key_value_pair ConsumerKeyValue = twc_KeyValue(TWC_OAUTH_CONSUMERKEY, Keys.ConsumerKey);
    twc_key_value_pair NonceKeyValue = twc_KeyValue(TWC_OAUTH_NONCE, Nonce);
    twc_key_value_pair SignatureMethodKeyValue = twc_KeyValue(TWC_OAUTH_SIGNATUREMETHOD, "HMAC-SHA1");
    twc_key_value_pair TimestampKeyValue = twc_KeyValue(TWC_OAUTH_TIMESTAMP, Timestamp);
    twc_key_value_pair TokenKeyValue = twc_KeyValue(TWC_OAUTH_TOKEN, Keys.TokenKey);
    twc_key_value_pair OAuthVersionKeyValue = twc_KeyValue(TWC_OAUTH_VERSION, "1.0");
    
    AllParams = twc_KeyValueList_InsertSorted(AllParams, &ConsumerKeyValue);
    AllParams = twc_KeyValueList_InsertSorted(AllParams, &NonceKeyValue);
    AllParams = twc_KeyValueList_InsertSorted(AllParams, &SignatureMethodKeyValue);
    AllParams = twc_KeyValueList_InsertSorted(AllParams, &TimestampKeyValue);
    AllParams = twc_KeyValueList_InsertSorted(AllParams, &TokenKeyValue);
    AllParams = twc_KeyValueList_InsertSorted(AllParams, &OAuthVersionKeyValue);

    int SignatureLen = twc_OAuthSignatureMaxLength();
    char* Signature = malloc(SignatureLen + 1);
    memset(Signature, '\0', SignatureLen + 1);
    twc_GenerateOAuthSignature(ReqType, BaseURL, AllParams, Keys.ConsumerSecret, Keys.TokenSecret, Signature);

    twc_key_value_pair SignatureKeyValue = twc_KeyValue(TWC_OAUTH_SIGNATURE, Signature);

    twc_key_value_list AuthParams = NULL;

    //TODO: OAuth pin?
    AuthParams = twc_KeyValueList_InsertSorted(AuthParams, &ConsumerKeyValue);
    AuthParams = twc_KeyValueList_InsertSorted(AuthParams, &NonceKeyValue);
    AuthParams = twc_KeyValueList_InsertSorted(AuthParams, &SignatureKeyValue);
    AuthParams = twc_KeyValueList_InsertSorted(AuthParams, &SignatureMethodKeyValue);
    AuthParams = twc_KeyValueList_InsertSorted(AuthParams, &TimestampKeyValue);
    AuthParams = twc_KeyValueList_InsertSorted(AuthParams, &TokenKeyValue);
    AuthParams = twc_KeyValueList_InsertSorted(AuthParams, &OAuthVersionKeyValue);

    char* AuthPrefix = "Authorization: OAuth ";
    int AuthPrefixLen = (int)strlen(AuthPrefix);
    strncpy(OAuthHeader.Ptr, AuthPrefix, ClampAmount(AuthPrefixLen, (int)OAuthHeader.Size));

    twc_strbuf ParamListBuf;
    ParamListBuf.Ptr = OAuthHeader.Ptr + AuthPrefixLen;
    ParamListBuf.Size = (int)OAuthHeader.Size - AuthPrefixLen;
    if (ParamListBuf.Size < 0) { ParamListBuf.Size = 0; }
    int ParamListLen = twc_KeyValueList_ToString(AuthParams, "=", ", ", true, false, ParamListBuf);
    return ParamListLen + AuthPrefixLen;
}

extern int
twc_OAuthHeaderMaxLength(twc_in twc_oauth_keys Keys)
{
    int KeyValueLengths[8]; 
    KeyValueLengths[0] = (int)strlen("Authorization: OAuth ");
    KeyValueLengths[1] = 16; // Nonce
    KeyValueLengths[2] = 22; // Timestamp
    KeyValueLengths[3] = twc_KeyValue_StringLength(TWC_OAUTH_CONSUMERKEY, Keys.ConsumerKey);
    KeyValueLengths[4] = twc_KeyValue_StringLength(TWC_OAUTH_TOKEN, Keys.TokenKey);
    KeyValueLengths[5] = twc_KeyValue_StringLength(TWC_OAUTH_SIGNATUREMETHOD, "HMAC-SHA1");
    KeyValueLengths[6] = twc_KeyValue_StringLength(TWC_OAUTH_VERSION, "1.0");
    KeyValueLengths[7] = twc_KeyValue_StringLength(TWC_OAUTH_SIGNATURE, "") + twc_OAuthSignatureMaxLength();
    // TODO: OAuth pin?

    int TotalLength = 0;
    for (int i = 0; i < countof(KeyValueLengths); ++i) {
        TotalLength += KeyValueLengths[i] + 1; // Account for "="
        if (i > 0) {
            TotalLength += 2; // Account for ", "
        }
    }

    return TotalLength;
}

extern twc_call_result
twc_Media_Upload(twc_state* State, twc_in char* Filename, twc_in twc_buffer FileContents)
{
    twc_call_result Result = {0};
    
    curl_easy_setopt(State->cURL, CURLOPT_URL, TWC_URL_MEDIA_UPLOAD);
    curl_easy_setopt(State->cURL, CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(State->cURL, CURLOPT_POST, 1);

    struct curl_httppost* FormPost = NULL;
    struct curl_httppost* LastPtr = NULL;

    if (FileContents.Size > 0)
    {
        curl_formadd(&FormPost, &LastPtr,
                     CURLFORM_COPYNAME, "media",
                     CURLFORM_BUFFER, "",
                     CURLFORM_BUFFERPTR, FileContents.Ptr,
                     CURLFORM_BUFFERLENGTH, FileContents.Size,
                     CURLFORM_END);
    }
    else if (Filename != NULL)
    {
        curl_formadd(&FormPost, &LastPtr,
                     CURLFORM_COPYNAME, "media",
                     CURLFORM_FILE, Filename,
                     CURLFORM_END);
    }
    else
    {
        fprintf(stderr, "Must provide either filename or filled buffer\n");
        return Result;
    }

    curl_easy_setopt(State->cURL, CURLOPT_HTTPPOST, FormPost);

    //@COPYPASTA from below
    curl_easy_setopt(State->cURL, CURLOPT_ENCODING, TWC_ENCODING);

    struct curl_slist* OAuthHeaderList = NULL;
    twc_strbuf OAuthHeader;
    OAuthHeader.Size = twc_OAuthHeaderMaxLength(State->Keys);
    OAuthHeader.Ptr = (char*)alloca(OAuthHeader.Size + 1);
    memset(OAuthHeader.Ptr, '\0', OAuthHeader.Size);

    twc_GenerateOAuthHeader(TWC_HTTP_POST, TWC_URL_MEDIA_UPLOAD, NULL, State->Keys, OAuthHeader);
    OAuthHeaderList = curl_slist_append(OAuthHeaderList, OAuthHeader.Ptr);

    if (OAuthHeaderList != NULL)
    {
        curl_easy_setopt(State->cURL, CURLOPT_HTTPHEADER, OAuthHeaderList);
    }

    if (curl_easy_perform(State->cURL) == CURLE_OK)
    {
        // Transfer complete, write function should be done writing
        twc_buffer Data = twc_ConsumeData(State);
        Result.Data = Data;

        return Result;
    }
    Result.Error = TWC_Error_cURL_Error;
    Result.ErrorStr.Ptr = State->cURL_Error.Ptr;
    Result.ErrorStr.Size = State->cURL_Error.Size;
    twc_DiscardData(State);

    return Result;
}

extern twc_call_result
twc_MakeCall(twc_state* State, twc_http_method Method, char* BaseURL, twc_key_value_list Params)
{
    twc_call_result Result = {0};
    printf("\n\nTWITTER API CALL: %s\n", BaseURL);

    if (Method == TWC_HTTP_GET)
    {
        if (Params != NULL) {
            int BaseLen = (int)strlen(BaseURL);

            int FullLen = BaseLen + 1 + twc_KeyValueList_StringLength(Params, "=", "&", false, true);
            char* FullURL = (char*)alloca(FullLen + 1);
            memset(FullURL, '\0', FullLen + 1);
            
            memcpy(FullURL, BaseURL, BaseLen);
            memcpy(FullURL + BaseLen, "?", 1);

            twc_strbuf ParamBuf;
            ParamBuf.Size = FullLen - (BaseLen + 1);
            ParamBuf.Ptr = FullURL + BaseLen + 1;
            twc_KeyValueList_ToString(Params, "=", "&", false, true, ParamBuf);

            printf("FULL URL: %s\n", FullURL);

            curl_easy_setopt(State->cURL, CURLOPT_URL, FullURL);
        }
        else {
            printf("FULL URL: %s\n", BaseURL);
            curl_easy_setopt(State->cURL, CURLOPT_URL, BaseURL);
        }
        curl_easy_setopt(State->cURL, CURLOPT_HTTPGET, 1);
    }
    else if (Method == TWC_HTTP_POST)
    {
        curl_easy_setopt(State->cURL, CURLOPT_URL, BaseURL);

        if (Params != NULL) {
            twc_strbuf ParamBuf;
            ParamBuf.Size = twc_KeyValueList_StringLength(Params, "=", "&", false, true);
            ParamBuf.Ptr = alloca(ParamBuf.Size + 1);
            memset(ParamBuf.Ptr, '\0', ParamBuf.Size + 1);
            twc_KeyValueList_ToString(Params, "=", "&", false, true, ParamBuf);

            //printf("PARAMS: %s\n", ParamBuf.Ptr);

            curl_easy_setopt(State->cURL, CURLOPT_POSTFIELDS, ParamBuf.Ptr);
        }
        else { 
            curl_easy_setopt(State->cURL, CURLOPT_POSTFIELDS, "");
        }
        curl_easy_setopt(State->cURL, CURLOPT_POST, 1);
    }
    else
    {
        assert(!"Unsupported HTTP method!");
    }

    curl_easy_setopt(State->cURL, CURLOPT_ENCODING, TWC_ENCODING);

    struct curl_slist* OAuthHeaderList = NULL;
    twc_strbuf OAuthHeader;
    OAuthHeader.Size = twc_OAuthHeaderMaxLength(State->Keys);
    OAuthHeader.Ptr = (char*)alloca(OAuthHeader.Size + 1);
    memset(OAuthHeader.Ptr, '\0', OAuthHeader.Size);

    twc_GenerateOAuthHeader(Method, BaseURL, Params, State->Keys, OAuthHeader);
    //printf("OAUTH HEADERS: \n%.*s\n", (int)OAuthHeader.Size, OAuthHeader.Ptr);
    OAuthHeaderList = curl_slist_append(OAuthHeaderList, OAuthHeader.Ptr);

    if (OAuthHeaderList != NULL)
    {
        curl_easy_setopt(State->cURL, CURLOPT_HTTPHEADER, OAuthHeaderList);
    }

    if (curl_easy_perform(State->cURL) == CURLE_OK)
    {
        // Transfer complete, write function should be done writing
        twc_buffer Data = twc_ConsumeData(State);
        Result.Data = Data;

        return Result;
    }
    Result.Error = TWC_Error_cURL_Error;
    Result.ErrorStr.Ptr = State->cURL_Error.Ptr;
    Result.ErrorStr.Size = State->cURL_Error.Size;
    twc_DiscardData(State);

    return Result;
}

/* Templates for the code generator
 * Should not be compiled directly.
 *
 * The code generator will parse through this file and use these for each
 * function it generates that corresponds to an API endpoint. For example,
 * twc_Statuses_Update will use the body of twc_param_serialization(twc_string)
 * to generate the appropriate lines of code to add the StatusId to the
 * parameter list when making a call to statuses/update.json.
 *
 * There are two kinds of templates. Parameter templates, and URL slug
 * templates.
 *
 * Parameter templates -- defined with twc_param_serialization(T) -- are used to
 *   serialize the contents of a parameter of type T to a string and add the
 *   result to the parameter list that will become the GET or POST parameter of
 *   the API call.
 * Expected behavior:
 *   The local variable ParamList should be replaced by a pointer to a local
 *   twc_KeyValue struct that is the new head of the linked list, containing the
 *   serialized parameter.
 *
 * URL slug templates -- defined with twc_url_serialization(T) -- are used to
 *   serialize a parameter corresponding to a URL slug (e.g. :id in
 *   statuses/destroy/:id) and fill the URL string with the serialized parameter
 *   in place of the slug.
 * Expected behavior:
 *   The local variable FilledURL should be initialized to a twc_strbuf
 *   holding a stack-allocated string which has the URL with the correct slug
 *   parameter filled in.
 *   For example, "https://api.twitter.com/1.1/statuses/destroy/20.json"
 *
 * Available parameters:
 *
 *   @ParamName@ -- The full name of the variable containing the parameter value
 *     May be a member of the Params struct or may be separate if the parameter
 *     is required.
 *     e.g. Params.StatusId. 
 *
 *   @FieldName@ -- Just the name of the field in the parameter struct, if
 *     applicable. (If parameter is required, i.e. is an argument in function
 *     header, will be the same as @ParamName@.)
 *     e.g. StatusId
 *
 *   @ParamString@ -- The raw name of the parameter as expected by twitter, in a
 *     string.
 *     e.g. "status_id"
 *
 *   @EndpointURL@ -- The string literal constant giving the current endpoint's
 *     full URL. Defined in twitter.h
 *     e.g. TWC_URL_STATUSES_UPDATE
 */
#define TWC_CODEGEN_TEMPLATES 0
#if TWC_CODEGEN_TEMPLATES

twc_param_serialization(bool$) 
{ 
    twc_key_value_pair @FieldName@_KV; 
    if (@ParamName@.Exists) {
        @FieldName@_KV = twc_KeyValue(@ParamString@, TWC_BOOLSTR(@ParamName@.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_param_serialization(int$)
{
    twc_key_value_pair @FieldName@_KV;
    if (@ParamName@.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(@ParamName@.Value, Buf);
        @FieldName@_KV = twc_KeyValue(@ParamString@, Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_param_serialization(int)
{
    twc_key_value_pair @FieldName@_KV;
    char Buf[13];
    memset(Buf, 0, sizeof(Buf));
    twc_SerializeInt(@ParamName@, Buf);
    @FieldName@_KV = twc_KeyValue(@ParamString@, Buf);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
}

twc_param_serialization(const char*)
{
    twc_key_value_pair @FieldName@_KV;
    if (@ParamName@ != NULL) {
        @FieldName@_KV = twc_KeyValue(@ParamString@, @ParamName@);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_param_serialization(twc_string)
{
    twc_key_value_pair @FieldName@_KV = twc_KeyValueStr(@ParamString@, @ParamName@);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
}

twc_param_serialization(twc_string$)
{
    twc_key_value_pair @FieldName@_KV;
    if (@ParamName@.Exists) {
        @FieldName@_KV = twc_KeyValueStr(@ParamString@, @ParamName@.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_param_serialization(twc_geo_coordinate$)
{
    twc_key_value_pair LatParam;
    twc_key_value_pair LongParam;
    if (@ParamName@.Exists) {
        twc_geo_coordinate @FieldName@_KV = @ParamName@.Value;
        char LatBuf[30]; // Enough to hold a serialized float at double precision
        char LongBuf[30];
        memset(LatBuf, 0, sizeof(LatBuf));
        memset(LongBuf, 0, sizeof(LongBuf));
        twc_SerializeLatLong(%s.Latitude, LatBuf);
        twc_SerializeLatLong(%s.Longitude, LongBuf);

        LatParam = twc_KeyValue(TWC_PARAM_LATITUDE, LatBuf);
        LongParam = twc_KeyValue(TWC_PARAM_LONGITUDE, LongBuf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &LatParam);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &LongParam);
    }
}

twc_param_serialization(twc_status_id)
{
    twc_key_value_pair @FieldName@_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(@ParamName@, Buf);
        @FieldName@_KV = twc_KeyValue(@ParamString@, Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_param_serialization(twc_user_id)
{
    twc_key_value_pair @FieldName@_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(@ParamName@, Buf);
        @FieldName@_KV = twc_KeyValue(@ParamString@, Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_param_serialization(twc_place_id)
{
    twc_key_value_pair @FieldName@_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(@ParamName@, Buf);
        @FieldName@_KV = twc_KeyValue(@ParamString@, Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_param_serialization(twc_status_id$)
{
    twc_key_value_pair @FieldName@_KV;
    if (@ParamName@.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(@ParamName@.Value, Buf);
        @FieldName@_KV = twc_KeyValue(@ParamString@, Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}


twc_param_serialization(twc_cursor_id$)
{
    twc_key_value_pair @FieldName@_KV;
    if (@ParamName@.Exists) {
        // Cursor ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(@ParamName@.Value, Buf);
        @FieldName@_KV = twc_KeyValue(@ParamString@, Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_param_serialization(twc_user_id$)
{
    twc_key_value_pair @FieldName@_KV;
    if (@ParamName@.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(@ParamName@.Value, Buf);
        @FieldName@_KV = twc_KeyValue(@ParamString@, Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_param_serialization(twc_place_id$)
{
    twc_key_value_pair @FieldName@_KV;
    if (@ParamName@.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializePlaceId(@ParamName@.Value, Buf);
        @FieldName@_KV = twc_KeyValue(@ParamString@, Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &@FieldName@_KV);
    }
}

twc_url_serialization(twc_status_id)
{
    twc_strbuf FilledURL;
    {
        char IdBuf[TWC_STATUS_ID_BUFSIZE];
        memset(IdBuf, '\0', sizeof(IdBuf));
        twc_SerializeStatusId(@ParamName@, IdBuf);

        FilledURL.Size = twc_URLParameterFilledLength(@EndpointURL@, TSTR(IdBuf));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(@EndpointURL@, TSTR(IdBuf), FilledURL);
    }
}

twc_url_serialization(twc_place_id)
{
    twc_strbuf FilledURL;
    {
        char IdBuf[TWC_STATUS_ID_BUFSIZE];
        memset(IdBuf, '\0', sizeof(IdBuf));
        twc_SerializeStatusId(@ParamName@, IdBuf);

        FilledURL.Size = twc_URLParameterFilledLength(@EndpointURL@, TSTR(IdBuf));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(@EndpointURL@, TSTR(IdBuf), FilledURL);
    }
}

twc_url_serialization(int)
{
    twc_strbuf FilledURL;
    {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(@ParamName@, Buf);

        FilledURL.Size = twc_URLParameterFilledLength(@EndpointURL@, TSTR(Buf));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(@EndpointURL@, TSTR(Buf), FilledURL);
    }
}

twc_url_serialization(twc_string)
{
    twc_strbuf FilledURL;
    {
        FilledURL.Size = twc_URLParameterFilledLength(@EndpointURL@, @ParamName@);
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(@EndpointURL@, @ParamName@, FilledURL);
    }
}

twc_url_serialization(const char*)
{
    twc_strbuf FilledURL;
    {
        FilledURL.Size = twc_URLParameterFilledLength(@EndpointURL@, TSTR(@ParamName@));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(@EndpointURL@, TSTR(@ParamName@), FilledURL);
    }
}
#endif

#ifndef TWC_CODEGEN_FIRST_PASS
#include "twitter_api.c"
#endif
