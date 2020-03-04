/*
 * Copyright (c) 2020, Stanislav Vorobiov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _AF3D_TYPES_H_
#define _AF3D_TYPES_H_

#include "bullet/LinearMath/btTransformUtil.h"
#include <string>
#include <vector>
#include <array>
#include <cstdio>
#include <cinttypes>

namespace af3d
{
    using Byte = std::uint8_t;

    // This function is used to ensure that a floating point number is not a NaN or infinity.
    // Called it 'btIsValid' after 'b2IsValid', really strange that bullet physics doesn't have this.
    inline bool btIsValid(float x)
    {
#if defined(__GNUC__) && ((__GNUC__*100 + __GNUC_MINOR__) >= 406)
/*
 * Fix that type-punned pointer crap on GCC...
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif
        union {
            float f;
            std::int32_t i;
        } v = { x };
        return (v.i & 0x7f800000) != 0x7f800000;
#if defined(__GNUC__) && ((__GNUC__*100 + __GNUC_MINOR__) >= 406)
#pragma GCC diagnostic pop
#endif
    }
}

#endif
