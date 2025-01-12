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

#include "APropertyType.h"
#include "AClass.h"

namespace af3d
{
    APropertyTypeBool::APropertyTypeBool()
    : APropertyType("Bool")
    {
    }

    void APropertyTypeBool::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitBool(*this);
    }

    APropertyTypeInt::APropertyTypeInt(int vMin, int vMax, APropertyUnit unit)
    : APropertyTypeNumeric<int>("Int", vMin, vMax, unit)
    {
    }

    APropertyTypeInt::APropertyTypeInt(APropertyUnit unit)
    : APropertyTypeNumeric<int>("Int", (std::numeric_limits<int>::min)(), (std::numeric_limits<int>::max)(), unit)
    {
    }

    void APropertyTypeInt::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitInt(*this);
    }

    APropertyTypeFloat::APropertyTypeFloat(float vMin, float vMax, APropertyUnit unit)
    : APropertyTypeNumeric<float>("Float", vMin, vMax, unit)
    {
    }

    APropertyTypeFloat::APropertyTypeFloat(APropertyUnit unit)
    : APropertyTypeNumeric<float>("Float", -(std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)(), unit)
    {
    }

    void APropertyTypeFloat::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitFloat(*this);
    }

    APropertyTypeString::APropertyTypeString(APropertyUnit unit)
    : APropertyType("String", unit)
    {
    }

    void APropertyTypeString::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitString(*this);
    }

    APropertyTypeVec2f::APropertyTypeVec2f()
    : APropertyType("Vec2f")
    {
    }

    void APropertyTypeVec2f::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitVec2f(*this);
    }

    APropertyTypeVec3f::APropertyTypeVec3f()
    : APropertyType("Vec3f")
    {
    }

    void APropertyTypeVec3f::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitVec3f(*this);
    }

    APropertyTypeVec3i::APropertyTypeVec3i()
    : APropertyType("Vec3i")
    {
    }

    void APropertyTypeVec3i::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitVec3i(*this);
    }

    APropertyTypeVec4f::APropertyTypeVec4f()
    : APropertyType("Vec4f")
    {
    }

    void APropertyTypeVec4f::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitVec4f(*this);
    }

    APropertyTypeColor::APropertyTypeColor(bool hasAlpha)
    : APropertyType("Color"),
      hasAlpha_(hasAlpha)
    {
    }

    void APropertyTypeColor::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitColor(*this);
    }

    APropertyTypeEnum::APropertyTypeEnum(const char* name, const Enumerators& enumerators)
    : APropertyType(name),
      enumerators_(enumerators)
    {
    }

    void APropertyTypeEnum::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitEnum(*this);
    }

    APropertyTypeObject::APropertyTypeObject(const char* name, const AClass& klass, bool isWeak)
    : APropertyType(name),
      klass_(klass),
      isWeak_(isWeak)
    {
    }

    void APropertyTypeObject::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitObject(*this);
    }

    APropertyTypeTransform::APropertyTypeTransform()
    : APropertyType("Transform")
    {
    }

    void APropertyTypeTransform::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitTransform(*this);
    }

    APropertyTypeQuaternion::APropertyTypeQuaternion()
    : APropertyType("Quaternion")
    {
    }

    void APropertyTypeQuaternion::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitQuaternion(*this);
    }

    APropertyTypeArray::APropertyTypeArray(const char* name, const APropertyType& type)
    : APropertyType(name),
      type_(type)
    {
    }

    void APropertyTypeArray::accept(APropertyTypeVisitor& visitor) const
    {
        visitor.visitArray(*this);
    }
}
