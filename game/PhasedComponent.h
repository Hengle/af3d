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

#ifndef _PHASEDCOMPONENT_H_
#define _PHASEDCOMPONENT_H_

#include "PhasedComponentManager.h"

namespace af3d
{
    static const std::uint32_t phaseThink = 1U << 0;
    static const std::uint32_t phasePreRender = 1U << 1;

    class PhasedComponent : public Component
    {
    public:
        PhasedComponent(const AClass& klass, std::uint32_t phases, int order = 0);
        ~PhasedComponent() = default;

        static const AClass& staticKlass();

        inline std::uint32_t phases() const { return phases_; }
        inline int order() const { return order_; }

        PhasedComponentManager* manager() override { return manager_; }
        inline void setManager(PhasedComponentManager* value)
        {
            onSetManager(manager_, value);
        }

        virtual void update(float dt) {}

        virtual void preRender(float dt) {}

    private:
        std::uint32_t phases_;
        int order_;
        PhasedComponentManager* manager_ = nullptr;
    };

    ACLASS_DECLARE(PhasedComponent)
}

#endif
