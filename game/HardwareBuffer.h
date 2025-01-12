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

#ifndef _HARDWARE_BUFFER_H_
#define _HARDWARE_BUFFER_H_

#include "HardwareResource.h"

namespace af3d
{
    class HardwareBuffer : public HardwareResource
    {
    public:
        enum class Usage
        {
            StaticDraw = 0,
            DynamicDraw,
            StreamDraw,
            StaticCopy
        };

        enum Access
        {
            WriteOnly = 0,
            ReadOnly,
            ReadWrite
        };

        HardwareBuffer(HardwareResourceManager* mgr, Usage usage, GLsizeiptr elementSize);
        ~HardwareBuffer();

        static GLbitfield glAccess(Access access);

        inline Usage usage() const { return usage_; }

        inline GLsizeiptr elementSize() const { return elementSize_; }

        inline GLsizeiptr count(HardwareContext& ctx) const { return count_; }

        inline GLsizeiptr sizeInBytes(HardwareContext& ctx) const { return count_ * elementSize_; }

        GLenum glUsage() const;

        GLuint id(HardwareContext& ctx) const override;

        void resize(GLsizeiptr cnt, HardwareContext& ctx);

        void reload(GLsizeiptr cnt, const GLvoid* data, HardwareContext& ctx);

        void upload(GLintptr offset, GLsizeiptr cnt, const GLvoid* data, HardwareContext& ctx);

        GLvoid* lock(GLintptr offset, GLsizeiptr cnt, Access access, HardwareContext& ctx);

        GLvoid* lock(Access access, HardwareContext& ctx);

        void unlock(HardwareContext& ctx);

    private:
        void doInvalidate(HardwareContext& ctx) override;

        virtual void doResize(HardwareContext& ctx) = 0;

        virtual void doReload(GLsizeiptr cnt, const GLvoid* data, HardwareContext& ctx) = 0;

        virtual void doUpload(GLintptr offset, GLsizeiptr cnt, const GLvoid* data, HardwareContext& ctx) = 0;

        virtual GLvoid* doLock(GLintptr offset, GLsizeiptr cnt, Access access, HardwareContext& ctx) = 0;

        virtual void doUnlock(HardwareContext& ctx) = 0;

        void createBuffer();

        Usage usage_ = Usage::StaticDraw;
        GLsizeiptr elementSize_ = 0;
        GLsizeiptr count_ = 0;
        GLuint id_ = 0;
        bool locked_ = false;
    };

    using HardwareBufferPtr = std::shared_ptr<HardwareBuffer>;
}

#endif
