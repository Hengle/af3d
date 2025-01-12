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

#include "AClass.h"
#include "AClassRegistry.h"

namespace af3d
{
    const AClass AClass_Null;

    AClass::AClass()
    : super_(*this)
    {
    }

    AClass::AClass(const std::string& name,
        const AClass& super,
        CreateFn createFn,
        const std::vector<PropertyDef>& propertyDefs)
    : name_(name),
      super_(super),
      createFn_(createFn)
    {
        properties_.reserve(propertyDefs.size());
        for (const auto& def : propertyDefs) {
            properties_.emplace_back(def.prop);
            runtime_assert(funcs_.emplace(def.prop.name(), def.funcs).second);
        }
        AClassRegistry::instance().classRegister(*this);
    }

    AClass::~AClass()
    {
        // Nothing we can do here actually...
    }

    const AClass* AClass::super() const
    {
        return (&super_ == &AClass_Null) ? nullptr : &super_;
    }

    bool AClass::isSubClassOf(const AClass& value) const
    {
        if (&super_ == this) {
            return false;
        } else if (&value == this) {
            return true;
        } else {
            return super_.isSubClassOf(value);
        }
    }

    APropertyList AClass::getProperties() const
    {
        if (&super_ == this) {
            return properties_;
        } else {
            auto superProps = super_.getProperties();
            size_t n = superProps.size();
            for (const auto& p : properties_) {
                AProperty* foundP = nullptr;
                for (size_t i = 0; i < n; ++i) {
                    if (p.name() == superProps[i].name()) {
                        foundP = &superProps[i];
                        break;
                    }
                }
                if (foundP) {
                    *foundP = p;
                } else {
                    superProps.push_back(p);
                }
            }
            return superProps;
        }
    }

    bool AClass::propertyCanGet(const std::string& key) const
    {
        if (&super_ == this) {
            return false;
        }

        auto it = funcs_.find(key);
        if (it == funcs_.end()) {
            return super_.propertyCanGet(key);
        }

        return !!it->second.getter;
    }

    bool AClass::propertyCanSet(const std::string& key) const
    {
        if (&super_ == this) {
            return false;
        }

        auto it = funcs_.find(key);
        if (it == funcs_.end()) {
            return super_.propertyCanSet(key);
        }

        return !!it->second.setter;
    }

    const AProperty* AClass::propertyFind(const std::string& key) const
    {
        if (&super_ == this) {
            return nullptr;
        } else {
            for (const auto& p : properties_) {
                if (p.name() == key) {
                    return &p;
                }
            }
            return super_.propertyFind(key);
        }
    }

    APropertyValue AClass::propertyGet(const AObject* obj, const std::string& key) const
    {
        if (&super_ == this) {
            return APropertyValue();
        }

        auto it = funcs_.find(key);
        if (it == funcs_.end()) {
            return super_.propertyGet(obj, key);
        }

        if (!it->second.getter) {
            return APropertyValue();
        }

        return (obj->*(it->second.getter))(key);
    }

    ACommandPtr AClass::propertySet(AObject* obj, const std::string& key, const APropertyValue& value) const
    {
        if (&super_ == this) {
            return ACommandPtr();
        }

        auto it = funcs_.find(key);
        if (it == funcs_.end()) {
            return super_.propertySet(obj, key, value);
        }

        if (!it->second.setter) {
            return ACommandPtr();
        }

        if (it->second.undoableSetter) {
            auto cmd = (obj->*(it->second.undoableSetter))(key, value);
            if (cmd) {
                cmd->redo();
            }
            return cmd;
        } else {
            (obj->*(it->second.setter))(key, value);
            return ACommandPtr();
        }
    }

    AObjectPtr AClass::create(const APropertyValueMap& propVals) const
    {
        return createFn_ ? createFn_(propVals) : AObjectPtr();
    }
}
