//------------------------------------------------------------------------------------
// Copyright (C) Arm Limited, 2019-2020 All rights reserved.
//
// The example code is provided to you as an aid to learning when working
// with Arm-based technology, including but not limited to programming tutorials.
//
// Arm hereby grants to you, subject to the terms and conditions of this Licence,
// a non-exclusive, non-transferable, non-sub-licensable, free-of-charge copyright
// licence, to use, copy and modify the Software solely for the purpose of internal
// demonstration and evaluation.
//
// You accept that the Software has not been tested by Arm therefore the Software
// is provided "as is", without warranty of any kind, express or implied. In no
// event shall the authors or copyright holders be liable for any claim, damages
// or other liability, whether in action or contract, tort or otherwise, arising
// from, out of or in connection with the Software or the use of Software.
//------------------------------------------------------------------------------------

#include <inttypes.h>
#include "vecdot.h"

void calc_vecdot_ref(int64_t n, float16_t* restrict a, float16_t* restrict b, float32_t* c)
{
    float32_t acc = 0;

    for(int64_t i = 0; i < n; ++i)
    {
        acc += (float32_t)a[i] * (float32_t)b[i];
    }

    *c = acc;
}