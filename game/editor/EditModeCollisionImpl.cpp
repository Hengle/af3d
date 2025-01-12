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

#include "editor/EditModeCollisionImpl.h"
#include "Scene.h"
#include "SceneObject.h"

namespace af3d { namespace editor
{
    EditModeCollisionImpl::EditModeCollisionImpl(Workspace* workspace)
    : EditModeImpl(workspace, "collision")
    {
    }

    EditModeCollision::TList EditModeCollisionImpl::hoveredTyped() const
    {
        TList res;
        for (const auto& witem : hovered()) {
            res.push_back(witem.lock<CollisionShape>());
        }
        return res;
    }

    EditModeCollision::TList EditModeCollisionImpl::selectedTyped() const
    {
        TList res;
        for (const auto& witem : selected()) {
            res.push_back(witem.lock<CollisionShape>());
        }
        return res;
    }

    EditMode::Item EditModeCollisionImpl::rayCast(const Frustum& frustum, const Ray& ray) const
    {
        Item res;

        scene()->rayCast(ray.pos, ray.pos + ray.dir * 1000.0f, [&res](btCollisionShape* shape, const btVector3&, const btVector3&, float fraction) {
            auto s = CollisionShape::fromShape(shape);
            if ((s->aflags() & AObjectEditable) == 0) {
                return -1.0f;
            }
            res = Item(std::static_pointer_cast<CollisionShape>(s->sharedThis()));
            return fraction;
        });

        return res;
    }

    bool EditModeCollisionImpl::isValid(const Item& item) const
    {
        auto r = aobjectCast<CollisionShape>(item.obj());
        return r && ((r->aflags() & AObjectEditable) != 0);
    }

    bool EditModeCollisionImpl::isAlive(const Item& item) const
    {
        auto r = std::static_pointer_cast<CollisionShape>(item.obj());
        return r->active();
    }
} }
