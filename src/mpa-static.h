/**
 * Majdan Packed Array library header
 * Static version
 * 
 * Written by Andras Majdan
 * License: GNU General Public License Version 3
 * 
 * Report bugs to <majdan.andras@gmail.com>
 */

#ifndef MPA_STATIC_H_
#define MPA_STATIC_H_

#include <inttypes.h>

/* Check for predefined LOG_BITSIZE */
#ifndef LOG_BITSIZE
#warning LOG_BITSIZE is not defined, setting to default
#define LOG_BITSIZE (2 * CHAR_BIT)
#endif

typedef struct
{
  uint_fast32_t log_count;
  uint_fast32_t phy_bitsize;
  uint_fast32_t phy_count;
  uint_fast32_t *phy_mem;
}
packed_array;

packed_array *pa_create(uint_fast32_t log_count);
void          pa_delete(packed_array *pa);
uint_fast32_t pa_get   (packed_array *pa, uint_fast32_t log_index);
void          pa_set   (packed_array *pa, uint_fast32_t log_index, uint_fast32_t value);

#endif /* MPA_STATIC_H_ */
