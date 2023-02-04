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
        fluxsort 1.1.5.4
*/

#ifndef FLUXSORT_H
#define FLUXSORT_H

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef int CMPFUNC(const void *a, const void *b);

// #define cmp(a,b) (*(a) > *(b))

#ifndef QUADSORT_H
#include "quadsort.h"
#endif

// When sorting an array of 32/64 bit pointers, like a string array, QUAD_CACHE
// needs to be adjusted in quadsort.h and here for proper performance when
// sorting large arrays.

#ifdef cmp
#define QUAD_CACHE 4294967295
#else
// #define QUAD_CACHE 131072
#define QUAD_CACHE 262144
// #define QUAD_CACHE 524288
// #define QUAD_CACHE 4294967295
#endif

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

#include "fluxsort.c"

#undef VAR
#undef FUNC

// fluxsort_prim

#define VAR int
#define FUNC(NAME) NAME##_int32
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "fluxsort.c"
#undef cmp
#else
#include "fluxsort.c"
#endif
#undef VAR
#undef FUNC

#define VAR unsigned int
#define FUNC(NAME) NAME##_uint32
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "fluxsort.c"
#undef cmp
#else
#include "fluxsort.c"
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

#include "fluxsort.c"

#undef VAR
#undef FUNC

// fluxsort_prim

#define VAR long long
#define FUNC(NAME) NAME##_int64
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "fluxsort.c"
#undef cmp
#else
#include "fluxsort.c"
#endif
#undef VAR
#undef FUNC

#define VAR unsigned long long
#define FUNC(NAME) NAME##_uint64
#ifndef cmp
#define cmp(a, b) (*(a) > *(b))
#include "fluxsort.c"
#undef cmp
#else
#include "fluxsort.c"
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

#include "fluxsort.c"

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

#include "fluxsort.c"

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

#define VAR long double
#define FUNC(NAME) NAME##128

#include "fluxsort.c"

#undef VAR
#undef FUNC

//////////////////////////////////////////////////////////////////////////
// ┌────────────────────────────────────────────────────────────────────┐//
// │███████┐██┐     ██┐   ██┐██┐  ██┐███████┐ ██████┐ ██████┐ ████████┐ │//
// │██┌────┘██│     ██│   ██│└██┐██┌┘██┌────┘██┌───██┐██┌──██┐└──██┌──┘ │//
// │█████┐  ██│     ██│   ██│ └███┌┘ ███████┐██│   ██│██████┌┘   ██│    │//
// │██┌──┘  ██│     ██│   ██│ ██┌██┐ └────██│██│   ██│██┌──██┐   ██│    │//
// │██│     ███████┐└██████┌┘██┌┘ ██┐███████│└██████┌┘██│  ██│   ██│    │//
// │└─┘     └──────┘ └─────┘ └─┘  └─┘└──────┘ └─────┘ └─┘  └─┘   └─┘    │//
// └────────────────────────────────────────────────────────────────────┘//
//////////////////////////////////////////////////////////////////////////

void fluxsort(void *array, size_t nmemb, size_t size, CMPFUNC *cmp) {
    if (nmemb < 2) {
        return;
    }

    switch (size) {
    case sizeof(char):
        return fluxsort8(array, nmemb, cmp);

    case sizeof(short):
        return fluxsort16(array, nmemb, cmp);

    case sizeof(int):
        return fluxsort32(array, nmemb, cmp);

    case sizeof(long long):
        return fluxsort64(array, nmemb, cmp);

#if __amd64__
        /* long double isn't native on non-x64 systems */
    case sizeof(long double):
        return fluxsort128(array, nmemb, cmp);
#endif

    default:
        return assert(size == sizeof(char) || size == sizeof(short) ||
                      size == sizeof(int) || size == sizeof(long long) ||
                      size == sizeof(long double));
    }
}

// This must match quadsort_prim()

void fluxsort_prim(void *array, size_t nmemb, size_t size) {
    if (nmemb < 2) {
        return;
    }

    switch (size) {
    case 4:
        fluxsort_int32(array, nmemb, NULL);
        return;
    case 5:
        fluxsort_uint32(array, nmemb, NULL);
        return;
    case 8:
        fluxsort_int64(array, nmemb, NULL);
        return;
    case 9:
        fluxsort_uint64(array, nmemb, NULL);
        return;
    default:
        assert(size == sizeof(int) || size == sizeof(int) + 1 ||
               size == sizeof(long long) || size == sizeof(long long) + 1);
        return;
    }
}

#undef QUAD_CACHE

#endif
