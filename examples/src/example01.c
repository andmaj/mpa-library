/**
 * Majdan Packed Array library
 * Example 01 - General library test
 * 
 * Written by Andras Majdan
 * License: GNU General Public License Version 3
 * 
 * Report bugs to <majdan.andras@gmail.com>
 */

#include <stdio.h>
#include <inttypes.h>

#include "mpa.h"

int main()
{
	int i;
	int val = 0;
	
	printf("Creating a 15 bit packed array with 5 elements\n");
	packed_array *pa = pa_create(15, 5);
	
	for(i=0; i<5; i++)
	{
		printf("Setting %d. element to %d\n", i, val);
		pa_set(pa, i, val);
		val += 3;
	}
	
	for(i=0; i<5; i++)
	{
		printf("Getting %d. element\n", i);
		printf("> %d\n", (int)pa_get(pa, i));
	}
	
	printf("Deleting the packed array\n");
	pa_delete(pa);
	 
	return 0;
}
