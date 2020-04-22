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

#include "editor/EditModeObjectImpl.h"
#include "Scene.h"
#include "SceneObject.h"

namespace af3d { namespace editor
{
    EditModeObjectImpl::EditModeObjectImpl(Workspace* workspace)
    : EditModeImpl(workspace, "object")
    {
    }

    EditModeObject::TList EditModeObjectImpl::hoveredTyped() const
    {
        TList res;
        for (const auto& witem : hovered()) {
            res.push_back(witem.lock<SceneObject>());
        }
        return res;
    }

    EditModeObject::TList EditModeObjectImpl::selectedTyped() const
    {
        TList res;
        for (const auto& witem : selected()) {
            res.push_back(witem.lock<SceneObject>());
        }
        return res;
    }

    EditMode::Item EditModeObjectImpl::rayCast(const Frustum& frustum, const Ray& ray) const
    {
        Item res;

        scene()->rayCastRender(frustum, ray, [&res](const RenderComponentPtr& r, const AObjectPtr&, const btVector3&, float dist) {
            if ((r->aflags() & AObjectMarkerObject) == 0) {
                return -1.0f;
            }
            res = Item(r->parent()->shared_from_this());
            return dist;
        });

        return res;
    }

    bool EditModeObjectImpl::isValid(const Item& item) const
    {
        return !!aobjectCast<SceneObject>(item.obj());
    }

    bool EditModeObjectImpl::isAlive(const Item& item) const
    {
        auto sObj = std::static_pointer_cast<SceneObject>(item.obj());
        return sObj->scene();
    }
} }
