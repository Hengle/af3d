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

#ifndef _COMPONENTMANAGER_H_
#define _COMPONENTMANAGER_H_

#include "Component.h"

namespace af3d
{
    class Scene;
    class RenderList;

    class ComponentManager : boost::noncopyable
    {
    public:
        ComponentManager() = default;
        virtual ~ComponentManager() = default;

        virtual void cleanup() = 0;

        virtual void addComponent(const ComponentPtr& component) = 0;

        virtual void removeComponent(const ComponentPtr& component) = 0;

        virtual void freezeComponent(const ComponentPtr& component) = 0;

        virtual void thawComponent(const ComponentPtr& component) = 0;

        virtual bool update(float dt) = 0;

        virtual void debugDraw(RenderList& rl) = 0;

        inline Scene* scene() { return scene_; }
        inline void setScene(Scene* value) { scene_ = value; }

    private:
        Scene* scene_ = nullptr;
    };

    using ComponentManagerPtr = std::shared_ptr<ComponentManager>;
}

#endif
