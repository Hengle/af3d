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

#ifndef _APROPERTY_VALUE_H_
#define _APROPERTY_VALUE_H_

#include "AWeakObject.h"
#include "af3d/Vector2.h"
#include "af3d/Vector3.h"
#include "af3d/Vector4.h"
#include <boost/operators.hpp>
#include <unordered_map>

namespace af3d
{
    class APropertyValue : public boost::totally_ordered<APropertyValue>
    {
    public:
        enum Type
        {
            None = 0,
            Int,
            Float,
            String,
            Vec2f,
            Vec3f,
            Vec3i,
            Vec4f,
            Object,
            WeakObject,
            Transform,
            Quaternion,
            Array
        };

        APropertyValue();
        APropertyValue(int val);
        APropertyValue(float val);
        APropertyValue(const char* val);
        APropertyValue(const std::string& val);
        APropertyValue(const Vector2f& val);
        APropertyValue(const Vector3f& val);
        APropertyValue(const Vector3i& val);
        APropertyValue(const btVector3& val);
        APropertyValue(const Vector4f& val);
        APropertyValue(const AObjectPtr& val);
        APropertyValue(const AWeakObject& val);
        APropertyValue(const btTransform& val);
        APropertyValue(const btQuaternion& val);
        APropertyValue(const std::vector<APropertyValue>& val);

        ~APropertyValue() = default;

        inline Type type() const { return type_; }

        inline bool empty() const { return type_ == None; }

        bool convertibleTo(Type other) const;
        APropertyValue convertTo(Type other) const;

        bool toBool() const;
        int toInt() const;
        float toFloat() const;
        std::string toString() const;
        Vector2f toVec2f() const;
        Vector3f toVec3f() const;
        Vector3i toVec3i() const;
        btVector3 toVec3() const;
        Vector4f toVec4f() const;
        inline Color toColor() const { return toVec4f(); }
        AObjectPtr toObject() const;
        AWeakObject toWeakObject() const;
        btTransform toTransform() const;
        btQuaternion toQuaternion() const;
        std::vector<APropertyValue> toArray() const;

        template <class T>
        std::shared_ptr<T> toObject() const;

        bool operator<(const APropertyValue& rhs) const;
        bool operator==(const APropertyValue& rhs) const;

        // Refresh all objects from their cookies.
        void refreshObjects();

    private:
        union Pod
        {
            Pod() = default;
            explicit Pod(int value) : intVal(value) {}
            explicit Pod(float value) : floatVal(value) {}
            explicit Pod(const Vector2f& value) : vec2fVal(value) {}
            explicit Pod(const Vector3f& value) : vec3fVal(value) {}
            explicit Pod(const Vector3i& value) : vec3iVal(value) {}
            explicit Pod(const Vector4f& value) : vec4fVal(value) {}

            int intVal;
            float floatVal;
            Vector2f vec2fVal;
            Vector3f vec3fVal;
            Vector3i vec3iVal;
            Vector4f vec4fVal;
        };

        Type type_;
        Pod pod_;
        std::string str_;
        AObjectPtr obj_;
        AWeakObject wobj_;
        btTransform xf_;
        btQuaternion rot_;
        std::vector<APropertyValue> arr_;
    };
}

#endif
