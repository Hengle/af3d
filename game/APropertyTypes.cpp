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

#include "APropertyTypes.h"

namespace af3d
{
    const APropertyTypeBool APropertyType_Bool;
    const APropertyTypeInt APropertyType_Int;
    const APropertyTypeInt APropertyType_UInt{0};
    const APropertyTypeFloat APropertyType_Float;
    const APropertyTypeFloat APropertyType_UFloat{0.0f};
    const APropertyTypeFloat APropertyType_FloatPercentage{0.0f, 1.0f};
    const APropertyTypeFloat APropertyType_FloatRadian{APropertyUnit::Radian};
    const APropertyTypeString APropertyType_String;
    const APropertyTypeString APropertyType_StringMesh{APropertyUnit::Mesh};
    const APropertyTypeVec2f APropertyType_Vec2f;
    const APropertyTypeVec3f APropertyType_Vec3f;
    const APropertyTypeVec3i APropertyType_Vec3i;
    const APropertyTypeVec4f APropertyType_Vec4f;
    const APropertyTypeColor APropertyType_ColorRGB{false};
    const APropertyTypeColor APropertyType_ColorRGBA{true};
    const APropertyTypeTransform APropertyType_Transform;
    const APropertyTypeQuaternion APropertyType_Quaternion;
    const APropertyTypeArray APropertyType_ArrayVec3f{"ArrayVec3f", APropertyType_Vec3f};
    const APropertyTypeArray APropertyType_ArrayVec3i{"ArrayVec3i", APropertyType_Vec3i};
    const APropertyTypeArray APropertyType_ArrayInt{"ArrayInt", APropertyType_Int};
    const APropertyTypeArray APropertyType_ArrayArrayInt{"ArrayArrayInt", APropertyType_ArrayInt};
    const APropertyTypeArray APropertyType_ArrayVec4f{"ArrayVec4f", APropertyType_Vec4f};
}
