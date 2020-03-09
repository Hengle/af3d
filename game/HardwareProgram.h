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

#ifndef _HARDWARE_PROGRAM_H_
#define _HARDWARE_PROGRAM_H_

#include "HardwareShader.h"
#include <type_traits>

namespace af3d
{
    enum class VertexAttribName
    {
        Pos = 0,
        UV,
        Normal,
        Diffuse,
        Specular,
        Max = Specular
    };

    enum class UniformName
    {
        ProjMatrix = 0,
        Time,
        MaxAuto = Time,
        AmbientColor,
        DiffuseColor,
        SpecularColor,
        Max = SpecularColor
    };

    struct VariableInfo
    {
        VariableInfo() = default;
        VariableInfo(GLenum type, GLint size)
        : type(type),
          size(size) {}

        GLenum type;
        GLint size;
    };

    static_assert(std::is_pod<VariableInfo>::value, "VariableInfo must be POD type");

    class HardwareProgram : public HardwareResource
    {
    public:
        explicit HardwareProgram(HardwareResourceManager* mgr);
        ~HardwareProgram();

        static inline bool isAuto(UniformName name) { return name <= UniformName::MaxAuto; }

        void invalidate(HardwareContext& ctx) override;

        GLuint id(HardwareContext& ctx) const override;

        void attachShader(const HardwareShaderPtr& shader, HardwareContext& ctx);

        bool link(HardwareContext& ctx);

    private:
        std::vector<std::pair<GLuint, HardwareShaderPtr>> shaders_;
        GLuint id_ = 0;
    };

    using HardwareProgramPtr = std::shared_ptr<HardwareProgram>;
}

#endif
