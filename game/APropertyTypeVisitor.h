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

#ifndef _APROPERTY_TYPE_VISITOR_H_
#define _APROPERTY_TYPE_VISITOR_H_

#include "af3d/Types.h"

namespace af3d
{
    class APropertyType;
    class APropertyTypeBool;
    class APropertyTypeInt;
    class APropertyTypeFloat;
    class APropertyTypeString;
    class APropertyTypeVec2f;
    class APropertyTypeVec3f;
    class APropertyTypeVec3i;
    class APropertyTypeVec4f;
    class APropertyTypeColor;
    class APropertyTypeEnum;
    class APropertyTypeObject;
    class APropertyTypeTransform;
    class APropertyTypeQuaternion;
    class APropertyTypeArray;

    class APropertyTypeVisitor
    {
    public:
        APropertyTypeVisitor() = default;
        virtual ~APropertyTypeVisitor() = default;

        virtual void visitBool(const APropertyTypeBool& type) = 0;
        virtual void visitInt(const APropertyTypeInt& type) = 0;
        virtual void visitFloat(const APropertyTypeFloat& type) = 0;
        virtual void visitString(const APropertyTypeString& type) = 0;
        virtual void visitVec2f(const APropertyTypeVec2f& type) = 0;
        virtual void visitVec3f(const APropertyTypeVec3f& type) = 0;
        virtual void visitVec3i(const APropertyTypeVec3i& type) = 0;
        virtual void visitVec4f(const APropertyTypeVec4f& type) = 0;
        virtual void visitColor(const APropertyTypeColor& type) = 0;
        virtual void visitEnum(const APropertyTypeEnum& type) = 0;
        virtual void visitObject(const APropertyTypeObject& type) = 0;
        virtual void visitTransform(const APropertyTypeTransform& type) = 0;
        virtual void visitQuaternion(const APropertyTypeQuaternion& type) = 0;
        virtual void visitArray(const APropertyTypeArray& type) = 0;
    };
}

#endif
