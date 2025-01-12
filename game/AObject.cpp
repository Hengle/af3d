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

#include "AObject.h"
#include "af3d/Utils.h"
#include <atomic>

namespace af3d
{
    const APropertyTypeObject APropertyType_AObject{"AObject", AClass_AObject};
    const APropertyTypeArray APropertyType_ArrayAObject{"ArrayAObject", APropertyType_AObject};

    ACLASS_DEFINE_BEGIN_ABSTRACT(AObject, Null)
    ACLASS_PROPERTY(AObject, Name, AProperty_Name, "Object name", String, "", General, APropertyEditable)
    ACLASS_DEFINE_END(AObject)

    static std::atomic<ACookie> nextCookie{1};

    static std::mutex cookieToAObjMtx;
    static std::unordered_map<ACookie, AObject*> cookieToAObj;

    AObject::AObject(const AClass& klass)
    : klass_(&klass),
      cookie_(allocCookie())
    {
        ScopedLock lock(cookieToAObjMtx);
        cookieToAObj[cookie_] = this;
    }

    AObject::~AObject()
    {
        ScopedLock lock(cookieToAObjMtx);
        auto it = cookieToAObj.find(cookie_);
        if ((it != cookieToAObj.end()) && (it->second == this)) {
            cookieToAObj.erase(it);
        }
    }

    void AObject::setCookie(ACookie value)
    {
        runtime_assert(value > 0);
        ScopedLock lock(cookieToAObjMtx);
        cookieToAObj.erase(cookie_);
        cookie_ = value;
        cookieToAObj[cookie_] = this;
    }

    const AClass& AObject::staticKlass()
    {
        return AClass_AObject;
    }

    ACookie AObject::allocCookie()
    {
        return nextCookie++;
    }

    AObject* AObject::getByCookie(ACookie value)
    {
        ScopedLock lock(cookieToAObjMtx);
        auto it = cookieToAObj.find(value);
        return (it == cookieToAObj.end()) ? nullptr : it->second;
    }

    size_t AObject::getCount()
    {
        ScopedLock lock(cookieToAObjMtx);
        return cookieToAObj.size();
    }

    bool AObject::isSubClassOf(const AClass& value) const
    {
        return klass_->isSubClassOf(value);
    }

    bool AObject::propertyCanGet(const std::string& key) const
    {
        return klass_->propertyCanGet(key);
    }

    bool AObject::propertyCanSet(const std::string& key) const
    {
        return klass_->propertyCanSet(key);
    }

    const AProperty* AObject::propertyFind(const std::string& key) const
    {
        return klass_->propertyFind(key);
    }

    APropertyValue AObject::propertyGet(const std::string& key) const
    {
        return klass_->propertyGet(this, key);
    }

    ACommandPtr AObject::propertySet(const std::string& key, const APropertyValue& value)
    {
        return klass_->propertySet(this, key, value);
    }

    ACommandPtr AObject::propertySetOneOf(const std::string& key1, const std::string& key2, const APropertyValue& value)
    {
        if (propertyCanSet(key1)) {
            return propertySet(key1, value);
        } else {
            return propertySet(key2, value);
        }
    }

    void AObject::propertiesSet(const APropertyValueMap& propVals)
    {
        for (const auto& kv : propVals.items()) {
            propertySet(kv.first, kv.second);
        }
    }
}
