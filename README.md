# Majdan Packed Array library

*A library for accessing arrays of packed unsigned integers width 1 to 64 bit lengths.*

*There are two versions: general and static.
With general version you can create packed arrays with any bit length. With static version you have to compile in the bit length which will be fixed during runtime. *

For example a **4 bit general packed array with capacity for 3 elements**:
```
#include <stdio.h>
#include "mpa.h"

int main()
{
  packed_array *pa = pa_create(4,3);
  pa_set(pa, 0, 12)
  pa_set(pa, 1, 3)
  pa_set(pa, 2, 15)
  int i;
  for(i=0; i<3; i++)
    printf("%d ", (int)pa_get(pa, i));
  pa_delete(pa);
}
```

**Result:**
12 3 15

**How it looks like in memory (32 bit CPU)?**

| Physical array index | uint32_t (MSB to LSB) |
|:----------------------------:|:-------------------------:|
| 0 | 1100 0011 1111 0000 0000 0000 0000 0000 |

**How it looks like for you (via the library)?**

| Logical array index | Value |
|:------------------------:|:----------:|
| 0 | 12 |
| 1 | 3 |
| 2 | 15 |

 ## Usage

### Creating a packed array

#### General version

```
packed_array *pa = pa_create(BITS, CAPACITY)
```
BITS - Bit length of elements of the packed array (1 to 64 bits)
CAPACITY - How many elements you want to add

#### Static version

```
packed_array *pa = pa_create(CAPACITY);
```
CAPACITY - How many elements you want to add

### Getting an element of packed array

```
uint_fast32_t elem;
elem = pa_get(pa, INDEX);
```
pa - Pointer to the packed_array
INDEX - Logical index of the element

### Setting an element of packed array

```
pa_set(pa, INDEX, VALUE);
```
pa - Pointer to the packed_array
INDEX - Logical index of the element
VALUE - Value to set

**The VALUE have to be in range otherwise it may overwrite the neighbors!**
For the 4 bit packed array example: you can store an element with 2^4 - 1 = 15  maximum value, so the range is 0 to 15.

### Deleting a packed array

```
pa_delete(pa);
```
pa - Pointer to the packed_array

## Build

Make  the packed array library, tests and examples:
```
make
```

Make the static packed array library with bit length of 12:
```
make static-shared static-lib static-obj CFLAGS='-DLOG_BITSIZE=12'
```

Run the self tests:
```
./bin/mpa-test
./bin/mpa-static-test8
./bin/mpa-static-test15
./bin/mpa-static-test16
./bin/mpa-static-test32
```

Run the examples:
```
./examples/bin/example01
./examples/bin/example02
```

To clean:
```
make clean
```

## Using in your program

### General version

Content of **yourprogram.c**:
```
#include "mpa.h"

void main()
{
  // ..
}
```

### Static version

Content of **yourprogram.c**:
```
#include "mpa-static.h"

void main()
{
  // ..
}
```

### Compiling with it

#### General version

1. Copy **mpa-library** folder in your program's source directory
2. In your source folder:
```
gcc -I mpa-library/src yourprogram.c mpa-library/src/mpa.c
```

#### Static version

1. Copy **mpa-library** folder in your program's source directory
2. In your source folder:
```
gcc -DLOG_BITS=4 -I mpa-library/src yourprogram.c mpa-library/src/mpa-static.c
```

Change 4 to the desired bit length!

### Using the compiled .o file

#### General version

1. Copy **mpa-library** folder in your program's source directory
2. Run **make general-obj** in the **mpa-library** folder
3. In your source folder:
```
gcc -I mpa-library/src yourprogram.c mpa-library/bin/mpa.o
```

#### Static version

1. Copy **mpa-library** folder in your program's source directory
2. Run **make static-obj CFLAGS='-DLOG_BITSIZE=4** in the **mpa-library** folder

Change 4 to the desired bit length!
3. In your source folder:
```
gcc -I mpa-library/src yourprogram.c mpa-library/bin/mpa-static.o
```

## Author

Written by Andras Majdan.

License: GNU General Public License Version 3

Report bugs to <majdan.andras@gmail.com>

## See also

[gpakosz/PackedArray](https://github.com/andmaj/bitmapdd) - Another great packed array implementation written by Gregory Pakosz
