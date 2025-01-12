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

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "HardwareContext.h"
#include "RenderNode.h"
#include "af3d/Single.h"
#include <mutex>
#include <condition_variable>
#include <list>

namespace af3d
{
    class Renderer : public Single<Renderer>
    {
    public:
        using HwOpFn = std::function<void(HardwareContext&)>;

        Renderer() = default;
        ~Renderer();

        bool init();

        void shutdown();

        bool reload(HardwareContext& ctx);

        void scheduleHwOp(const HwOpFn& hwOp); // Will get executed in 'render'.
        void scheduleHwOpSync(HwOpFn hwOp); // Will get executed in 'render'.
        void swap(const RenderNodeList& rnl);
        void cancelSwap(HardwareContext& ctx);

        bool render(HardwareContext& ctx);
        void cancelRender();

    private:
        using RenderOpFn = std::function<bool(HardwareContext&)>;
        using RenderOpList = std::list<RenderOpFn>;

        void doRender(const RenderNodePtr& rn, HardwareContext& ctx);

        std::mutex mtx_;
        std::condition_variable cond_;
        bool cancelSwap_ = false;
        bool cancelRender_ = false;
        bool rendering_ = false;
        RenderOpList ops_;
        std::uint64_t lastTimeUs_ = 0;
    };

    extern Renderer renderer;
}

#endif
