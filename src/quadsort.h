/*
        Copyright (C) 2014-2022 Igor van den Hoven ivdhoven@gmail.com
*/

/*
        Permission is hereby granted, free of charge, to any person obtaining
        a copy of this software and associated documentation files (the
        "Software"), to deal in the Software without restriction, including
        without limitation the rights to use, copy, modify, merge, publish,
        distribute, sublicense, and/or sell copies of the Software, and to
        permit persons to whom the Software is furnished to do so, subject to
        the following conditions:

        The above copyright notice and this permission notice shall be
        included in all copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
        EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
        MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
        IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
        CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
        TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
        SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
        quadsort 1.1.5.4
*/

#ifndef QUADSORT_H
#define QUADSORT_H

#include <assert.h>
#include <errno.h>
#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>

typedef int CMPFUNC(const void *a, const void *b);

// #define cmp(a,b) (*(a) > *(b))

// When sorting an array of pointers, like a string array, the QUAD_CACHE needs
// to be set for proper performance when sorting large arrays.
// quadsort_prim() can be used to sort 32 and 64 bit primitives.

// With a 6 MB L3 cache a value of 262144 works well.

#ifdef cmp
#define QUAD_CACHE 4294967295
#else
// #define QUAD_CACHE 131072
#define QUAD_CACHE 262144
// #define QUAD_CACHE 524288
// #define QUAD_CACHE 4294967295
#endif

#define parity_merge_two(array, swap, x, y, ptl, ptr, pts, cmp)                \
    {                                                                          \
        ptl = array + 0;                                                       \
        ptr = array + 2;                                                       \
        pts = swap + 0;                                                        \
        x = cmp(ptl, ptr) <= 0;                                                \
        y = !x;                                                                \
        pts[x] = *ptr;                                                         \
        ptr += y;                                                              \
        pts[y] = *ptl;                                                         \
        ptl += x;                                                              \
        pts++;                                                                 \
        *pts = cmp(ptl, ptr) <= 0 ? *ptl : *ptr;                               \
                                                                               \
        ptl = array + 1;                                                       \
        ptr = array + 3;                                                       \
        pts = swap + 3;                                                        \
        x = cmp(ptl, ptr) <= 0;                                                \
        y = !x;                                                                \
        pts--;                                                                 \
        pts[x] = *ptr;                                                         \
        ptr -= x;                                                              \
        pts[y] = *ptl;                                                         \
        ptl -= y;                                                              \
        *pts = cmp(ptl, ptr) > 0 ? *ptl : *ptr;                                \
    }

#define parity_merge_four(array, swap, x, y, ptl, ptr, pts, cmp)               \
    {                                                                          \
        ptl = array + 0;                                                       \
        ptr = array + 4;                                                       \
        pts = swap;                                                            \
        x = cmp(ptl, ptr) <= 0;                                                \
        y = !x;                                                                \
        pts[x] = *ptr;                                                         \
        ptr += y;                                                              \
        pts[y] = *ptl;                                                         \
        ptl += x;                                                              \
        pts++;                                                                 \
        x = cmp(ptl, ptr) <= 0;                                                \
        y = !x;                                                                \
        pts[x] = *ptr;                                                         \
        ptr += y;                                                              \
        pts[y] = *ptl;                                                         \
        ptl += x;                                                              \
        pts++;                                                                 \
        x = cmp(ptl, ptr) <= 0;                                                \
        y = !x;                                                                \
        pts[x] = *ptr;                                                         \
        ptr += y;                                                              \
        pts[y] = *ptl;                                                         \
        ptl += x;                                                              \
        pts++;                                                                 \
        *pts = cmp(ptl, ptr) <= 0 ? *ptl : *ptr;                               \
                                                                               \
        ptl = array + 3;                                                       \
        ptr = array + 7;                                                       \
        pts = swap + 7;                                                        \
        x = cmp(ptl, ptr) <= 0;                                                \
        y = !x;                                                                \
        pts--;                                                                 \
        pts[x] = *ptr;                                                         \
        ptr -= x;                                                              \
        pts[y] = *ptl;                                                         \
        ptl -= y;                                                              \
        x = cmp(ptl, ptr) <= 0;                                                \
        y = !x;                                                                \
        pts--;                                                                 \
        pts[x] = *ptr;                                                         \
        ptr -= x;                                                              \
        pts[y] = *ptl;                                                         \
        ptl -= y;                                                              \
        x = cmp(ptl, ptr) <= 0;                                                \
        y = !x;                                                                \
        pts--;                                                                 \
        pts[x] = *ptr;                                                         \
        ptr -= x;                                                              \
        pts[y] = *ptl;                                                         \
        ptl -= y;                                                              \
        *pts = cmp(ptl, ptr) > 0 ? *ptl : *ptr;                                \
    }

//////////////////////////////////////////////////////////
// ┌───────────────────────────────────────────────────┐//
// │       ██████┐ ██████┐    ██████┐ ██████┐████████┐ │//
// │       └────██┐└────██┐   ██┌──██┐└─██┌─┘└──██┌──┘ │//
// │        █████┌┘ █████┌┘   ██████┌┘  ██│     ██│    │//
// │        └───██┐██┌───┘    ██┌──██┐  ██│     ██│    │//
// │       ██████┌┘███████┐   ██████┌┘██████┐   ██│    │//
// │       └─────┘ └──────┘   └─────┘ └─────┘   └─┘    │//
// └───────────────────────────────────────────────────┘//
//////////////////////////////////////////////////////////

#define VAR int
#define FUNC(NAME) NAME##32

#include "quadsort.c"

#undef VAR
#undef FUNC

// quadsort_prim

#define VAR int
#define FUNC(NAME) NAME##_int32
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "quadsort.c"
#undef cmp
#else
#include "quadsort.c"
#endif
#undef VAR
#undef FUNC

#define VAR unsigned int
#define FUNC(NAME) NAME##_uint32
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "quadsort.c"
#undef cmp
#else
#include "quadsort.c"
#endif
#undef VAR
#undef FUNC

#define VAR float
#define FUNC(NAME) NAME##_float32
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "quadsort.c"
#undef cmp
#else
#include "quadsort.c"
#endif
#undef VAR
#undef FUNC

//////////////////////////////////////////////////////////
// ┌───────────────────────────────────────────────────┐//
// │        █████┐ ██┐  ██┐   ██████┐ ██████┐████████┐ │//
// │       ██┌───┘ ██│  ██│   ██┌──██┐└─██┌─┘└──██┌──┘ │//
// │       ██████┐ ███████│   ██████┌┘  ██│     ██│    │//
// │       ██┌──██┐└────██│   ██┌──██┐  ██│     ██│    │//
// │       └█████┌┘     ██│   ██████┌┘██████┐   ██│    │//
// │        └────┘      └─┘   └─────┘ └─────┘   └─┘    │//
// └───────────────────────────────────────────────────┘//
//////////////////////////////////////////////////////////

#define VAR long long
#define FUNC(NAME) NAME##64

#include "quadsort.c"

#undef VAR
#undef FUNC

// quadsort_prim

#define VAR long long
#define FUNC(NAME) NAME##_int64
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "quadsort.c"
#undef cmp
#else
#include "quadsort.c"
#endif
#undef VAR
#undef FUNC

#define VAR unsigned long long
#define FUNC(NAME) NAME##_uint64
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "quadsort.c"
#undef cmp
#else
#include "quadsort.c"
#endif
#undef VAR
#undef FUNC

#define VAR double
#define FUNC(NAME) NAME##_double64
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "quadsort.c"
#undef cmp
#else
#include "quadsort.c"
#endif
#undef VAR
#undef FUNC

// This section is outside of 32/64 bit pointer territory, so no cache checks
// necessary, unless sorting 32+ byte structures.

#undef QUAD_CACHE
#define QUAD_CACHE 4294967295

//////////////////////////////////////////////////////////
// ┌────────────────────────────────────────────────────┐//
// │                █████┐    ██████┐ ██████┐████████┐  │//
// │               ██┌──██┐   ██┌──██┐└─██┌─┘└──██┌──┘  │//
// │               └█████┌┘   ██████┌┘  ██│     ██│     │//
// │               ██┌──██┐   ██┌──██┐  ██│     ██│     │//
// │               └█████┌┘   ██████┌┘██████┐   ██│     │//
// │                └────┘    └─────┘ └─────┘   └─┘     │//
// └────────────────────────────────────────────────────┘//
//////////////////////////////////////////////////////////

#define VAR char
#define FUNC(NAME) NAME##8

#include "quadsort.c"

#undef VAR
#undef FUNC

//////////////////////////////////////////////////////////
// ┌────────────────────────────────────────────────────┐//
// │           ▄██┐   █████┐    ██████┐ ██████┐████████┐│//
// │          ████│  ██┌───┘    ██┌──██┐└─██┌─┘└──██┌──┘│//
// │          └─██│  ██████┐    ██████┌┘  ██│     ██│   │//
// │            ██│  ██┌──██┐   ██┌──██┐  ██│     ██│   │//
// │          ██████┐└█████┌┘   ██████┌┘██████┐   ██│   │//
// │          └─────┘ └────┘    └─────┘ └─────┘   └─┘   │//
// └────────────────────────────────────────────────────┘//
//////////////////////////////////////////////////////////

#define VAR short
#define FUNC(NAME) NAME##16

#include "quadsort.c"

#undef VAR
#undef FUNC

//////////////////////////////////////////////////////////
// ┌────────────────────────────────────────────────────┐//
// │  ▄██┐  ██████┐  █████┐    ██████┐ ██████┐████████┐ │//
// │ ████│  └────██┐██┌──██┐   ██┌──██┐└─██┌─┘└──██┌──┘ │//
// │ └─██│   █████┌┘└█████┌┘   ██████┌┘  ██│     ██│    │//
// │   ██│  ██┌───┘ ██┌──██┐   ██┌──██┐  ██│     ██│    │//
// │ ██████┐███████┐└█████┌┘   ██████┌┘██████┐   ██│    │//
// │ └─────┘└──────┘ └────┘    └─────┘ └─────┘   └─┘    │//
// └────────────────────────────────────────────────────┘//
//////////////////////////////////////////////////////////

// 128 reflects the name, though the actual size is 80, 96, or 128 bits,
// depending on platform.

#define VAR long double
#define FUNC(NAME) NAME##128
#include "quadsort.c"
#undef VAR
#undef FUNC

///////////////////////////////////////////////////////////
// ┌─────────────────────────────────────────────────────┐//
// │ ██████┐██┐   ██┐███████┐████████┐ ██████┐ ███┐  ███┐│//
// │██┌────┘██│   ██│██┌────┘└──██┌──┘██┌───██┐████┐████││//
// │██│     ██│   ██│███████┐   ██│   ██│   ██│██┌███┌██││//
// │██│     ██│   ██│└────██│   ██│   ██│   ██│██│└█┌┘██││//
// │└██████┐└██████┌┘███████│   ██│   └██████┌┘██│ └┘ ██││//
// │ └─────┘ └─────┘ └──────┘   └─┘    └─────┘ └─┘    └─┘│//
// └─────────────────────────────────────────────────────┘//
///////////////////////////////////////////////////////////

/*
typedef struct {char bytes[32];} struct256;
#define VAR struct256
#define FUNC(NAME) NAME##256

#include "quadsort.c"

#undef VAR
#undef FUNC
*/

///////////////////////////////////////////////////////////////////////////////
// ┌─────────────────────────────────────────────────────────────────────────┐//
// │    ██████┐ ██┐   ██┐ █████┐ ██████┐ ███████┐ ██████┐ ██████┐ ████████┐  │//
// │   ██┌───██┐██│   ██│██┌──██┐██┌──██┐██┌────┘██┌───██┐██┌──██┐└──██┌──┘  │//
// │   ██│   ██│██│   ██│███████│██│  ██│███████┐██│   ██│██████┌┘   ██│     │//
// │   ██│▄▄ ██│██│   ██│██┌──██│██│  ██│└────██│██│   ██│██┌──██┐   ██│     │//
// │   └██████┌┘└██████┌┘██│  ██│██████┌┘███████│└██████┌┘██│  ██│   ██│     │//
// │    └──▀▀─┘  └─────┘ └─┘  └─┘└─────┘ └──────┘ └─────┘ └─┘  └─┘   └─┘     │//
// └─────────────────────────────────────────────────────────────────────────┘//
///////////////////////////////////////////////////////////////////////////////

void quadsort(void *array, size_t nmemb, size_t size, CMPFUNC *cmp) {
    if (nmemb < 2) {
        return;
    }

    switch (size) {
    case sizeof(char):
        quadsort8(array, nmemb, cmp);
        return;

    case sizeof(short):
        quadsort16(array, nmemb, cmp);
        return;

    case sizeof(int):
        quadsort32(array, nmemb, cmp);
        return;

    case sizeof(long long):
        quadsort64(array, nmemb, cmp);
        return;

#if __amd64__
        /* long double isn't native on non-x64 systems */
    case sizeof(long double):
        quadsort128(array, nmemb, cmp);
        return;
#endif

        //		case sizeof(struct256):
        //			quadsort256(array, nmemb, cmp);
        return;

    default:
        assert(size == sizeof(char) || size == sizeof(short) ||
               size == sizeof(int) || size == sizeof(long long) ||
               size == sizeof(long double));
        //			qsort(array, nmemb, size, cmp);
    }
}

// suggested size values for primitives:

//		case  0: unsigned char
//		case  1: signed char
//		case  2: signed short
//		case  3: unsigned short
//		case  4: signed int
//		case  5: unsigned int
//		case  6: float
//		case  7: double
//		case  8: signed long long
//		case  9: unsigned long long
//		case 16: long double

void quadsort_prim(void *array, size_t nmemb, size_t size) {
    if (nmemb < 2) {
        return;
    }

    switch (size) {
    case 4:
        quadsort_int32(array, nmemb, NULL);
        return;
    case 5:
        quadsort_uint32(array, nmemb, NULL);
        return;
    case 6:
        quadsort_float32(array, nmemb, NULL);
        return;
    case 7:
        quadsort_double64(array, nmemb, NULL);
        return;
    case 8:
        quadsort_int64(array, nmemb, NULL);
        return;
    case 9:
        quadsort_uint64(array, nmemb, NULL);
        return;
    default:
        assert(size == sizeof(int) || size == sizeof(int) + 1 ||
               size == sizeof(long long) || size == sizeof(long long) + 1);
        return;
    }
}

#undef QUAD_CACHE

#endif
