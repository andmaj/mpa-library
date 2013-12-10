/**
 * Majdan Packed Array library
 * Static version
 * 
 * Written by Andras Majdan
 * License: GNU General Public License Version 3
 * 
 * Report bugs to <majdan.andras@gmail.com>
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>

#include "mpa-static.h"

/* Check for predefined PHY_BITSIZE */
#ifndef PHY_BITSIZE
/* Warning: 
   This macro will fail on architectures other than 32 or 64 bits.
   In this case predefine PHY_BITSIZE!
    
   Because of preprocessor limitations cannot use:
   #define PHY_BITSIZE (sizeof(uint_fast32_t) * CHAR_BIT)
   See: http://c-faq.com/cpp/ifexpr.html
*/
#if UINT_FAST32_MAX > 0xFFFFFFFF
#define PHY_BITSIZE 64
#else
#define PHY_BITSIZE 32
#endif
/* End of warning */

#endif

#if LOG_BITSIZE > PHY_BITSIZE
#error LOG_BITSIZE cannot be bigger than PHY_BITSIZE
#endif

/**
 * Create a packed array
 * 
 * @log_count	Count of logical elements
 * 
 * @return Pointer to the packed array
 */
packed_array *pa_create(uint_fast32_t log_count)
{
	packed_array *pa = (packed_array*) malloc( sizeof(packed_array) );
	pa->log_count = log_count;

	pa->phy_bitsize = PHY_BITSIZE;
	pa->phy_count = log_count * LOG_BITSIZE / PHY_BITSIZE + 1;

	pa->phy_mem = (uint_fast32_t*) calloc( pa->phy_count, sizeof(uint_fast32_t) );
	return pa;
}

/**
 * Free up the memory used by the packed array
 * 
 * @pa	Pointer to the packed array to delete
 */
void pa_delete(packed_array *pa)
{
	free(pa->phy_mem);
	free(pa);
}

/**
 * Get an element from the packed array
 * 
 * @pa			Pointer to the packed array
 * @log_index	Logical index of the element
 * 
 * @return Value of the element
 */
#if PHY_BITSIZE == LOG_BITSIZE
#define PA_GET
uint_fast32_t pa_get(packed_array *pa, uint_fast32_t log_index)
{
	return pa->phy_mem[log_index];
}
#endif

#if PHY_BITSIZE == 64
#if LOG_BITSIZE == 32
#define PA_GET
/* 64 bit -> 32 bit */
uint_fast32_t pa_get(packed_array *pa, uint_fast32_t log_index)
{	
	if(log_index & 1)
		return pa->phy_mem[log_index >> 1] & 0x00000000FFFFFFFF;
	else
		return (pa->phy_mem[log_index >> 1] & 0xFFFFFFFF00000000) >> 32;
}
#elif LOG_BITSIZE == 16
#define PA_GET
/* 64 bit -> 16 bit */
uint_fast32_t pa_get(packed_array *pa, uint_fast32_t log_index)
{
	static const uint64_t mask[4] = { 
		0xFFFF000000000000,
		0x0000FFFF00000000,
		0x00000000FFFF0000,
		0x000000000000FFFF
	};
	
	static const uint64_t bitshift[4] = { 48, 32, 16, 0 };
	
	return (pa->phy_mem[log_index/4] & mask[log_index & 3]) >> 
		bitshift[log_index & 3];
}
#elif LOG_BITSIZE == 8
#define PA_GET
/* 64 bit -> 8 bit */
uint_fast32_t pa_get(packed_array *pa, uint_fast32_t log_index)
{
	static const uint64_t mask[8] = { 
		0xFF00000000000000,
		0x00FF000000000000,
		0x0000FF0000000000,
		0x000000FF00000000,
		0x00000000FF000000,
		0x0000000000FF0000,
		0x000000000000FF00,
		0x00000000000000FF
	};
	
	static const uint64_t bitshift[8] = { 56, 48, 40, 32, 24, 16, 8, 0 };

	return (pa->phy_mem[log_index/8] & mask[log_index & 7]) >>
		bitshift[log_index & 7];
}
#endif
#elif PHY_BITSIZE == 32
#if LOG_BITSIZE == 16
#define PA_GET
/* 32 bit -> 16 bit */
uint_fast32_t pa_get(packed_array *pa, uint_fast32_t log_index)
{	
	if(log_index & 1)
		return pa->phy_mem[log_index >> 1] & 0x0000FFFFF;
	else
		return (pa->phy_mem[log_index >> 1] & 0xFFFF0000) >> 16;
}
#elif LOG_BITSIZE == 8
#define PA_GET	
/* 32 bit -> 8 bit */
uint_fast32_t pa_get(packed_array *pa, uint_fast32_t log_index)
{
	static const uint32_t mask[4] = { 
		0xFF000000,
		0x00FF0000,
		0x0000FF00,
		0x000000FF
	};
	
	static const uint32_t bitshift[4] = { 24, 16, 8, 0 };
	
	return pa->phy_mem[log_index/4] & mask[log_index & 3] >>
		bitshift[log_index & 3];
}
#endif
#endif

#ifndef PA_GET
uint_fast32_t pa_get(packed_array *pa, uint_fast32_t log_index)
{
	uint_fast32_t phy_index = log_index * LOG_BITSIZE / PHY_BITSIZE;
	uint_fast32_t phy_remain = log_index * LOG_BITSIZE % PHY_BITSIZE;
	
	uint_fast32_t mask;

    if(phy_remain > (PHY_BITSIZE - LOG_BITSIZE))
    {
    	/* Shared between [phy_index] and [phy_index+1] element */
    	mask = ~(uint_fast32_t)0 >> (2*PHY_BITSIZE - LOG_BITSIZE - phy_remain);
    	return (pa->phy_mem[phy_index] >> phy_remain) |
    		   ((pa->phy_mem[phy_index+1] & mask) << (PHY_BITSIZE - phy_remain));
    }
    else
    {
    	/* Stored only in [phy_index] */
    	mask = ~(uint_fast32_t)0 >> (2*PHY_BITSIZE - LOG_BITSIZE - phy_remain);
    	return (pa->phy_mem[phy_index] & mask) >> phy_remain;
    }
}
#endif

/**
 * Set an element of the packed array
 * 
 * @pa			Pointer to the packed array
 * @log_index	Logical index of the element
 * @value		New value of the element
 */
#if PHY_BITSIZE == LOG_BITSIZE
#define PA_SET
void pa_set(packed_array *pa, uint_fast32_t log_index, uint_fast32_t value)
{
	pa->phy_mem[log_index] = value;
}
#endif
 
#if PHY_BITSIZE == 64
#if LOG_BITSIZE == 32
#define PA_SET
/* 64 bit -> 32 bit */
void pa_set(packed_array *pa, uint_fast32_t log_index, uint_fast32_t value)
{
	if(log_index & 1)
		pa->phy_mem[log_index >> 1] = 
			(pa->phy_mem[log_index >> 1] & 0xFFFFFFFF00000000) | value;
	else
		pa->phy_mem[log_index >> 1] = 
			(pa->phy_mem[log_index >> 1] & 0x00000000FFFFFFFF) | 
			(value << 32);
}
#elif LOG_BITSIZE == 16
#define PA_SET
/* 64 bit -> 16 bit */
void pa_set(packed_array *pa, uint_fast32_t log_index, uint_fast32_t value)
{
	static const uint64_t mask[4] = { 
		0x0000FFFFFFFFFFFF,
		0xFFFF0000FFFFFFFF,
		0xFFFFFFFF0000FFFF,
		0xFFFFFFFFFFFF0000
	};
	
	static const uint64_t bitshift[4] = { 48, 32, 16, 0 };
	
	pa->phy_mem[log_index/4] = 
		(pa->phy_mem[log_index/4] & mask[log_index & 3]) |
		(value << bitshift[log_index & 3]);
}
#elif LOG_BITSIZE == 8
#define PA_SET
/* 64 bit -> 8 bit */
void pa_set(packed_array *pa, uint_fast32_t log_index, uint_fast32_t value)
{
	static const uint64_t mask[8] = { 
		0x00FFFFFFFFFFFFFF,
		0xFF00FFFFFFFFFFFF,
		0xFFFF00FFFFFFFFFF,
		0xFFFFFF00FFFFFFFF,
		0xFFFFFFFF00FFFFFF,
		0xFFFFFFFFFF00FFFF,
		0xFFFFFFFFFFFF00FF,
		0xFFFFFFFFFFFFFF00
	};
	
	static const uint64_t bitshift[8] = { 56, 48, 40, 32, 24, 16, 8, 0 };
	
	pa->phy_mem[log_index/8] = 
		(pa->phy_mem[log_index/8] & mask[log_index & 7]) |
		(value << bitshift[log_index & 7]);
}
#endif
#elif PHY_BITSIZE == 32
#if LOG_BITSIZE == 16
#define PA_SET
/* 32 bit -> 16 bit */
void pa_set(packed_array *pa, uint_fast32_t log_index, uint_fast32_t value)
{
	if(log_index & 1)
		pa->phy_mem[log_index >> 1] = 
			(pa->phy_mem[log_index >> 1] & 0xFFFF0000) | value;
	else
		pa->phy_mem[log_index >> 1] = 
			(pa->phy_mem[log_index >> 1] & 0x0000FFFF) | (value << 16);
}
#elif LOG_BITSIZE == 8
#define PA_SET
/* 32 bit -> 8 bit */
void pa_set(packed_array *pa, uint_fast32_t log_index, uint_fast32_t value)
{
	static const uint32_t mask[4] = { 
		0x00FFFFFF,
		0xFF00FFFF,
		0xFFFF00FF,
		0xFFFFFF00
	};
	
	static const uint32_t bitshift[4] = { 24, 16, 8, 0 };
	
	pa->phy_mem[log_index/4] = 
		(pa->phy_mem[log_index/4] & mask[log_index & 3]) |
		(value << bitshift[log_index & 3]);
}
#endif
#endif
 
#ifndef PA_SET
void pa_set(packed_array *pa, uint_fast32_t log_index, uint_fast32_t value)
{
	uint_fast32_t phy_index = log_index * LOG_BITSIZE / PHY_BITSIZE;
	uint_fast32_t phy_remain = log_index * LOG_BITSIZE % PHY_BITSIZE;
	uint_fast32_t mask;

    if(phy_remain > (PHY_BITSIZE - LOG_BITSIZE))
    {
    	/* Shared between [phy_index] and [phy_index+1] element */
    	mask = ~(uint_fast32_t)0 >> (PHY_BITSIZE-phy_remain);
        pa->phy_mem[phy_index] = (pa->phy_mem[phy_index] & mask) | (value << phy_remain);
        mask = ~(uint_fast32_t)0 << (LOG_BITSIZE-(PHY_BITSIZE-phy_remain));
        pa->phy_mem[phy_index+1] = (pa->phy_mem[phy_index+1] & mask) | (value >> (PHY_BITSIZE-phy_remain));
    }
    else
    {
    	/* Stored only in [phy_index] */
    	mask = ~((~(uint_fast32_t)0 >> (PHY_BITSIZE - LOG_BITSIZE)) << phy_remain);

    	pa->phy_mem[phy_index] = (pa->phy_mem[phy_index] & mask) | (value << phy_remain);
    }
}
#endif
