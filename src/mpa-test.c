/**
 * Majdan Packed Array library
 * General library test
 * 
 * Written by Andras Majdan
 * License: GNU General Public License Version 3
 * 
 * Report bugs to <majdan.andras@gmail.com>
 */

#include <stdio.h>
#include <limits.h>
#include <inttypes.h>

#include "mpa.h"

int main()
{
	int max_bitsize = sizeof(uint_fast32_t)*CHAR_BIT;
	
	int i, n, val;
	packed_array *pa;
	
	for(i=1; i<=max_bitsize; i++)
	{
		printf("Creating %d bit packed array with 1000 elements\n", i);
		pa = pa_create(i, 1000);
		
		printf("Setting 1000 elements of the array\n");
		for(n=0; n<1000; n++)
			pa_set(pa, n, n % (i+1));
		
		printf("Getting and checking 1000 elements of the array\n");
		for(n=0; n<1000; n++)
		{
			val = pa_get(pa, n);
			if(val != n % (i+1))
			{
				fprintf(stderr, "ERROR: %d.element does not match\n", n);
				fprintf(stderr, "%d instead of %d\n", val, n % (i+1));
				return 1;
			}
		}
		
		printf("Deleting the array\n");
		pa_delete(pa);
	}
	
	return 0;
}
