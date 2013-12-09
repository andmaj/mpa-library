/**
 * Majdan Packed Array library
 * General version
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

#include "mpa.h"

/* Check for predefined PHY_BITSIZE */
#ifndef PHY_BITSIZE
#define PHY_BITSIZE (sizeof(uint_fast32_t) * CHAR_BIT)
#endif

/**
 * Create a packed array
 * 
 * @log_bitsize Bitsize of logical elements
 * @log_count	Count of logical elements
 * 
 * @return Pointer to the packed array
 */
packed_array *pa_create(uint_fast32_t log_bitsize, uint_fast32_t log_count)
{
	packed_array *pa = malloc( sizeof(packed_array) );
	pa->log_bitsize = log_bitsize;
	pa->log_count = log_count;

	pa->phy_bitsize = PHY_BITSIZE;
	pa->phy_count = log_count * log_bitsize / PHY_BITSIZE + 1;

	pa->phy_mem = calloc( pa->phy_count, sizeof(uint_fast32_t) );
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
uint_fast32_t pa_get(packed_array *pa, uint_fast32_t log_index)
{
	uint_fast32_t phy_index = log_index * pa->log_bitsize / PHY_BITSIZE;
	uint_fast32_t phy_remain = log_index * pa->log_bitsize % PHY_BITSIZE;
	
	uint_fast32_t mask;

    if(phy_remain > (PHY_BITSIZE - pa->log_bitsize))
    {
    	/* Shared between [phy_index] and [phy_index+1] element */
    	mask = ~(uint_fast32_t)0 >> (2*PHY_BITSIZE - pa->log_bitsize - phy_remain);
    	return (pa->phy_mem[phy_index] >> phy_remain) |
    		   ((pa->phy_mem[phy_index+1] & mask) << (PHY_BITSIZE - phy_remain));
    }
    else
    {
    	/* Stored only in [phy_index] */
    	mask = ~(uint_fast32_t)0 >> (2*PHY_BITSIZE - pa->log_bitsize - phy_remain);
    	return (pa->phy_mem[phy_index] & mask) >> phy_remain;
    }
}

/**
 * Set an element of the packed array
 * 
 * @pa			Pointer to the packed array
 * @log_index	Logical index of the element
 * @value		New value of the element
 */
void pa_set(packed_array *pa, uint_fast32_t log_index, uint_fast32_t value)
{
	uint_fast32_t phy_index = log_index * pa->log_bitsize / PHY_BITSIZE;
	uint_fast32_t phy_remain = log_index * pa->log_bitsize % PHY_BITSIZE;
	uint_fast32_t mask;

    if(phy_remain > (PHY_BITSIZE - pa->log_bitsize))
    {
    	/* Shared between [phy_index] and [phy_index+1] element */
    	mask = ~(uint_fast32_t)0 >> (PHY_BITSIZE-phy_remain);
        pa->phy_mem[phy_index] = (pa->phy_mem[phy_index] & mask) | (value << phy_remain);
        mask = ~(uint_fast32_t)0 << (pa->log_bitsize-(PHY_BITSIZE-phy_remain));
        pa->phy_mem[phy_index+1] = (pa->phy_mem[phy_index+1] & mask) | (value >> (PHY_BITSIZE-phy_remain));
    }
    else
    {
    	/* Stored only in [phy_index] */
    	mask = ~((~(uint_fast32_t)0 >> (PHY_BITSIZE - pa->log_bitsize)) << phy_remain);

    	pa->phy_mem[phy_index] = (pa->phy_mem[phy_index] & mask) | (value << phy_remain);
    }
}
