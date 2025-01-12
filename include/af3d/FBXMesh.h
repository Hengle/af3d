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

#ifndef _AF3D_FBX_MESH_H_
#define _AF3D_FBX_MESH_H_

#include "af3d/FBXEntity.h"
#include "af3d/Vector2.h"
#include "af3d/Vector3.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace af3d
{
    class FBXMesh : public FBXEntity,
        boost::noncopyable
    {
    public:
        FBXMesh() = default;
        ~FBXMesh() = default;

        inline const std::vector<Vector3f>& vertices() const { return vertices_; }
        inline std::vector<Vector3f>& vertices() { return vertices_; }

        inline const std::vector<std::uint16_t>& indices() const { return indices_; }
        inline std::vector<std::uint16_t>& indices() { return indices_; }

        inline const std::vector<Vector3f>& normals() const { return normals_; }
        inline std::vector<Vector3f>& normals() { return normals_; }

        inline const std::vector<Vector2f>& uvs() const { return uvs_; }
        inline std::vector<Vector2f>& uvs() { return uvs_; }

    private:
        std::vector<Vector3f> vertices_;
        std::vector<std::uint16_t> indices_;
        std::vector<Vector3f> normals_;
        std::vector<Vector2f> uvs_;
    };

    using FBXMeshPtr = std::shared_ptr<FBXMesh>;
}

#endif
