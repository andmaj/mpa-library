/**
 * Majdan Packed Array library
 * Static library test
 * 
 * Written by Andras Majdan
 * License: GNU General Public License Version 3
 * 
 * Report bugs to <majdan.andras@gmail.com>
 */

/* -DLOG_BITSIZE parameter is given for the compiler */

#include <stdio.h>
#include <limits.h>
#include <inttypes.h>

#include "mpa-static.h"

int main()
{
	int n, val;
	packed_array *pa;
	
	printf("Creating %i bit packed array with 1000 elements\n", LOG_BITSIZE);
	pa = pa_create(1000);
		
	printf("Setting 1000 elements of the array\n");
	for(n=0; n<1000; n++)
		pa_set(pa, n, n % (LOG_BITSIZE+1));
		
	printf("Getting and checking 1000 elements of the array\n");
	for(n=0; n<1000; n++)
	{
		val = pa_get(pa, n);
		if(val != n % (LOG_BITSIZE+1))
		{
			fprintf(stderr, "ERROR: %d.element does not match\n", n);
			fprintf(stderr, "%d instead of %d\n", val, n % (LOG_BITSIZE+1));
			return 1;
		}
	}
		
	printf("Deleting the array\n");
	pa_delete(pa);
	
	return 0;
}
