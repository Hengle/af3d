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

#ifndef _MESH_H_
#define _MESH_H_

#include "Resource.h"
#include "SubMesh.h"
#include "af3d/AABB.h"

namespace af3d
{
    class MeshManager;

    class Mesh : public std::enable_shared_from_this<Mesh>,
        public Resource
    {
    public:
        Mesh(MeshManager* mgr,
            const std::string& name,
            const AABB& aabb,
            const std::vector<SubMeshPtr>& subMeshes,
            const ResourceLoaderPtr& loader = ResourceLoaderPtr());
        ~Mesh();

        virtual ResourcePtr sharedThis() override { return shared_from_this(); }

        inline const AABB& aabb() const { return aabb_; }

        const std::vector<SubMeshPtr>& subMeshes() const { return subMeshes_; }

    private:
        MeshManager* mgr_;
        AABB aabb_;
        std::vector<SubMeshPtr> subMeshes_;
    };

    using MeshPtr = std::shared_ptr<Mesh>;
}

#endif
