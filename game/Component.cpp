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

#include "Component.h"
#include "SceneObject.h"

namespace af3d
{
    ACLASS_DEFINE_BEGIN_ABSTRACT(Component, AObject)
    ACLASS_PROPERTY_RO(Component, Parent, AProperty_Parent, "Parent", AObject, Hierarchy, APropertyTransient)
    ACLASS_DEFINE_END(Component)

    Component::Component(const AClass& klass)
    : AObject(klass)
    {
    }

    const AClass& Component::staticKlass()
    {
        return AClass_Component;
    }

    void Component::removeFromParent()
    {
        if (parent_) {
            parent_->removeComponent(std::static_pointer_cast<Component>(sharedThis()));
        }
    }

    void Component::debugDraw(RenderList& rl)
    {
    }

    void Component::onFreeze()
    {
    }

    void Component::onThaw()
    {
    }

    Scene* Component::scene()
    {
        return parent_ ? parent_->scene() : nullptr;
    }

    std::shared_ptr<SceneObject> Component::script_parent() const
    {
        if (parent_) {
            return parent_->shared_from_this();
        } else {
            return SceneObjectPtr();
        }
    }

    APropertyValue Component::propertyParentGet(const std::string&) const
    {
        return APropertyValue(script_parent());
    }
}
