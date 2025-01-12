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

#ifndef _RENDERTARGET_H_
#define _RENDERTARGET_H_

#include "HardwareRenderTarget.h"
#include "Texture.h"

namespace af3d
{
    class RenderTarget
    {
    public:
        RenderTarget() = default;
        explicit RenderTarget(const TexturePtr& texture,
            GLint level = 0,
            TextureCubeFace cubeFace = TextureCubeXP,
            GLint layer = 0)
        : texture_(texture),
          level_(level),
          cubeFace_(cubeFace),
          layer_(layer)
        {
        }
        ~RenderTarget() = default;

        inline const TexturePtr& texture() const { return texture_; }
        inline GLint level() const { return level_; }
        inline TextureCubeFace cubeFace() const { return cubeFace_; }
        inline GLint layer() const { return layer_; }

        inline std::uint32_t width() const { return texture_ ? textureMipSize(texture_->width(), level_) : 0; }
        inline std::uint32_t height() const { return texture_ ? textureMipSize(texture_->height(), level_) : 0; }

        typedef void (*unspecified_bool_type)();
        static void unspecified_bool_true() {}

        operator unspecified_bool_type() const
        {
            return texture_ ? unspecified_bool_true : 0;
        }

        bool operator!() const
        {
            return !texture_;
        }

        inline HardwareRenderTarget toHardware() const
        {
            return texture_ ? HardwareRenderTarget(texture_->hwTex(), level_, cubeFace_, layer_) : HardwareRenderTarget();
        }

    private:
        TexturePtr texture_;
        GLint level_ = 0;
        TextureCubeFace cubeFace_ = TextureCubeXP;
        GLint layer_ = 0;
    };

    struct LightProbeRenderTarget
    {
        LightProbeRenderTarget() = default;
        LightProbeRenderTarget(int index,
            const TexturePtr& irradianceTexture,
            const TexturePtr& specularTexture)
        : index(index),
          irradianceTexture(irradianceTexture),
          specularTexture(specularTexture) {}

        int index = -1;
        TexturePtr irradianceTexture;
        TexturePtr specularTexture;
    };
}

#endif
