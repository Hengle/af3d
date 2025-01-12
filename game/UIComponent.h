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

#ifndef _UICOMPONENT_H_
#define _UICOMPONENT_H_

#include "UIComponentManager.h"
#include "RenderList.h"

namespace af3d
{
    class UIComponent : public Component
    {
    public:
        explicit UIComponent(const AClass& klass, int zOrder = 0);
        ~UIComponent() = default;

        static const AClass& staticKlass();

        UIComponentManager* manager() override { return manager_; }
        inline void setManager(UIComponentManager* value)
        {
            onSetManager(manager_, value);
        }

        inline int zOrder() const { return zOrder_; }

        inline bool visible() const { return visible_; }
        inline void setVisible(bool value) { visible_ = value; }

        virtual void update(float dt) {}

        virtual void render(RenderList& rl) {}

    private:
        int zOrder_ = 0;
        bool visible_ = true;
        UIComponentManager* manager_ = nullptr;
    };

    ACLASS_DECLARE(UIComponent)
}

#endif
