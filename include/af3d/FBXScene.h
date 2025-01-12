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

#ifndef _AF3D_FBX_SCENE_H_
#define _AF3D_FBX_SCENE_H_

#include "af3d/FBXMaterialTemplate.h"
#include "af3d/FBXTextureTemplate.h"
#include "af3d/FBXMesh.h"
#include <memory>

namespace af3d
{
    class FBXScene
    {
    public:
        FBXScene() = default;
        ~FBXScene() = default;

        inline const FBXMaterialTemplate& mtlTemplate() const { return mtlTemplate_; }
        inline FBXMaterialTemplate& mtlTemplate() { return mtlTemplate_; }

        inline const FBXTextureTemplate& texTemplate() const { return texTemplate_; }
        inline FBXTextureTemplate& texTemplate() { return texTemplate_; }

        inline const std::vector<FBXMeshPtr>& meshes() const { return meshes_; }
        inline const FBXMeshPtr& addMesh()
        {
            meshes_.push_back(std::make_shared<FBXMesh>());
            return meshes_.back();
        }

    private:
        FBXMaterialTemplate mtlTemplate_;
        FBXTextureTemplate texTemplate_;

        std::vector<FBXMeshPtr> meshes_;
    };

    using FBXScenePtr = std::shared_ptr<FBXScene>;
}

#endif
