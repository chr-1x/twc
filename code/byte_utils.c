/* byte_utils.c
 *  by Andrew 'ChronalDragon' Chronister
 *
 *  A few small useful utilities needed for operations done in hashing.
 *  If intel intrinsics are available, those are used.
 *  (TODO(chronister): Necessary to do inline asm for gcc?
 */
#ifndef BYTEUTILS_H
#include "types.h"

#if _MSC_VER
#include "intrin.h"
#endif

extern inline u32 RotateLeft(u32 Value, s32 Amount); // Make GCC happy
inline u32
RotateLeft(u32 Value, s32 Amount)
{
#if _MSC_VER
	u32 Result = _rotl(Value, Amount);
#else
	Amount &= 31;
	u32 Result = ((Value << Amount) | Value >> (32 - Amount));
#endif

	return Result;
}

extern inline u32 SwapByteOrder(u32 A);
inline u32 
SwapByteOrder(u32 A)
{
#if USE_INTRINSICS
        return _byteswap_ulong(A);
#else
        return ((A & 0xFF000000) >> 24) |
               ((A & 0x00FF0000) >> 8) |
               ((A & 0x0000FF00) << 8) |
               ((A & 0x000000FF) << 24);
#endif
}

extern inline u64 SwapByteOrder64(u64 A);
inline u64 
SwapByteOrder64(u64 A)
{
#if USE_INTRINSICS
        return _byteswap_uint64(A);
#else
        return ((A & 0xFF00000000000000) >> 56) |
               ((A & 0x00FF000000000000) >> 40) |
               ((A & 0x0000FF0000000000) >> 24) |
               ((A & 0x000000FF00000000) >> 8) |
               ((A & 0x00000000FF000000) << 8) |
               ((A & 0x0000000000FF0000) << 24) |
               ((A & 0x000000000000FF00) << 40) |
               ((A & 0x00000000000000FF) << 56);
#endif
}

#define BYTEUTILS_H
#endif
