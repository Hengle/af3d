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

#include "RenderList.h"

namespace af3d
{
    RenderImm::RenderImm(const MaterialPtr& material,
        GLenum primitiveMode,
        float depthValue,
        const ScissorParams& scissorParams,
        RenderList& rl)
    : material_(material),
      primitiveMode_(primitiveMode),
      depthValue_(depthValue),
      scissorParams_(scissorParams),
      rl_(rl),
      startVertices_(rl_.env()->defaultVa().data().vertices.size())
    {
    }

    RenderImm::~RenderImm()
    {
        VertexArraySlice vaSlice(rl_.env()->defaultVa().vaNoEbo(),
            startVertices_,
            rl_.env()->defaultVa().data().vertices.size() - startVertices_,
            0);
        rl_.addGeometry(material_, vaSlice, primitiveMode_, depthValue_, scissorParams_);
    }

    std::vector<VertexImm>& RenderImm::vertices()
    {
        return rl_.env()->defaultVa().data().vertices;
    }

    void RenderImm::addLine(const btVector3& pos, const btVector3& dir, const btVector3& up, const Color& c, bool withCovers)
    {
        auto pos2 = pos + dir;

        auto dirN = dir.normalized();

        auto up2 = up.rotate(dirN, SIMD_HALF_PI);
        auto up3 = -up;
        auto up4 = -up2;

        if (withCovers) {
            // Back cover
            addVertex(pos, Vector2f_zero, c);
            addVertex(pos + up2, Vector2f_zero, c);
            addVertex(pos + up, Vector2f_zero, c);

            addVertex(pos, Vector2f_zero, c);
            addVertex(pos + up3, Vector2f_zero, c);
            addVertex(pos + up2, Vector2f_zero, c);

            addVertex(pos + up4, Vector2f_zero, c);
            addVertex(pos + up3, Vector2f_zero, c);
            addVertex(pos, Vector2f_zero, c);

            addVertex(pos + up4, Vector2f_zero, c);
            addVertex(pos, Vector2f_zero, c);
            addVertex(pos + up, Vector2f_zero, c);

            // Front cover
            addVertex(pos2 + up, Vector2f_zero, c);
            addVertex(pos2 + up2, Vector2f_zero, c);
            addVertex(pos2, Vector2f_zero, c);

            addVertex(pos2 + up2, Vector2f_zero, c);
            addVertex(pos2 + up3, Vector2f_zero, c);
            addVertex(pos2, Vector2f_zero, c);

            addVertex(pos2, Vector2f_zero, c);
            addVertex(pos2 + up3, Vector2f_zero, c);
            addVertex(pos2 + up4, Vector2f_zero, c);

            addVertex(pos2 + up, Vector2f_zero, c);
            addVertex(pos2, Vector2f_zero, c);
            addVertex(pos2 + up4, Vector2f_zero, c);
        }

        // Side 1
        addVertex(pos + up2, Vector2f_zero, c);
        addVertex(pos2 + up, Vector2f_zero, c);
        addVertex(pos + up, Vector2f_zero, c);

        addVertex(pos + up2, Vector2f_zero, c);
        addVertex(pos2 + up2, Vector2f_zero, c);
        addVertex(pos2 + up, Vector2f_zero, c);

        // Side 2
        addVertex(pos + up3, Vector2f_zero, c);
        addVertex(pos2 + up2, Vector2f_zero, c);
        addVertex(pos + up2, Vector2f_zero, c);

        addVertex(pos + up3, Vector2f_zero, c);
        addVertex(pos2 + up3, Vector2f_zero, c);
        addVertex(pos2 + up2, Vector2f_zero, c);

        // Side 3
        addVertex(pos + up4, Vector2f_zero, c);
        addVertex(pos2 + up4, Vector2f_zero, c);
        addVertex(pos + up3, Vector2f_zero, c);

        addVertex(pos + up3, Vector2f_zero, c);
        addVertex(pos2 + up4, Vector2f_zero, c);
        addVertex(pos2 + up3, Vector2f_zero, c);

        // Side 4
        addVertex(pos + up, Vector2f_zero, c);
        addVertex(pos2 + up, Vector2f_zero, c);
        addVertex(pos + up4, Vector2f_zero, c);

        addVertex(pos + up4, Vector2f_zero, c);
        addVertex(pos2 + up, Vector2f_zero, c);
        addVertex(pos2 + up4, Vector2f_zero, c);
    }

    void RenderImm::addArrow(const btVector3& pos, const btVector3& dir, const btVector3& up, const Color& c)
    {
        auto pos2 = pos + dir;

        auto dirN = dir.normalized();

        auto up2 = up.rotate(dirN, SIMD_2_PI / 3);
        auto up3 = up.rotate(dirN, -SIMD_2_PI / 3);

        addVertex(pos + up3, Vector2f_zero, c);
        addVertex(pos + up2, Vector2f_zero, c);
        addVertex(pos + up, Vector2f_zero, c);

        addVertex(pos + up2, Vector2f_zero, c);
        addVertex(pos2, Vector2f_zero, c);
        addVertex(pos + up, Vector2f_zero, c);

        addVertex(pos2, Vector2f_zero, c);
        addVertex(pos + up2, Vector2f_zero, c);
        addVertex(pos + up3, Vector2f_zero, c);

        addVertex(pos2, Vector2f_zero, c);
        addVertex(pos + up3, Vector2f_zero, c);
        addVertex(pos + up, Vector2f_zero, c);
    }

    void RenderImm::addQuadArrow(const btVector3& pos, const btVector3& dir, const btVector3& up, const Color& c)
    {
        auto pos2 = pos + dir;

        auto dirN = dir.normalized();

        auto up2 = up.rotate(dirN, SIMD_HALF_PI);
        auto up3 = -up;
        auto up4 = -up2;

        // Cover
        addVertex(pos, Vector2f_zero, c);
        addVertex(pos + up2, Vector2f_zero, c);
        addVertex(pos + up, Vector2f_zero, c);

        addVertex(pos, Vector2f_zero, c);
        addVertex(pos + up3, Vector2f_zero, c);
        addVertex(pos + up2, Vector2f_zero, c);

        addVertex(pos + up4, Vector2f_zero, c);
        addVertex(pos + up3, Vector2f_zero, c);
        addVertex(pos, Vector2f_zero, c);

        addVertex(pos + up4, Vector2f_zero, c);
        addVertex(pos, Vector2f_zero, c);
        addVertex(pos + up, Vector2f_zero, c);

        // Side
        addVertex(pos + up3, Vector2f_zero, c);
        addVertex(pos2, Vector2f_zero, c);
        addVertex(pos + up2, Vector2f_zero, c);

        addVertex(pos + up2, Vector2f_zero, c);
        addVertex(pos2, Vector2f_zero, c);
        addVertex(pos + up, Vector2f_zero, c);

        addVertex(pos + up4, Vector2f_zero, c);
        addVertex(pos2, Vector2f_zero, c);
        addVertex(pos + up3, Vector2f_zero, c);

        addVertex(pos + up, Vector2f_zero, c);
        addVertex(pos2, Vector2f_zero, c);
        addVertex(pos + up4, Vector2f_zero, c);
    }

    void RenderImm::addLineArrow(const btVector3& pos, const btVector3& dir, const btVector3& up, const Vector2f& arrowSize, const Color& c)
    {
        addLine(pos, dir, up, c);
        addArrow(pos + dir, dir.normalized() * arrowSize.x(), up.normalized() * arrowSize.y(), c);
    }

    void RenderImm::addLineBox(const btVector3& pos, const btVector3& dir, const btVector3& up, const btVector3& boxSize, const Color& c)
    {
        addLine(pos, dir, up, c);

        auto dirN = dir.normalized();
        auto upN = up.normalized();
        auto rightN = dirN.cross(upN);
        addBox(pos + dir - upN * boxSize.y() * 0.5f - rightN * boxSize.x() * 0.5f, {rightN * boxSize.x(), dirN * boxSize.z(), upN * boxSize.y()}, c);
    }

    void RenderImm::addQuad(const btVector3& pos, const std::array<btVector3, 2>& dirs, const Color& c)
    {
        auto p2 = pos + dirs[0];
        auto p3 = pos + dirs[1];
        auto p4 = p2 + dirs[1];

        addVertex(pos, Vector2f_zero, c);
        addVertex(p2, Vector2f_zero, c);
        addVertex(p4, Vector2f_zero, c);

        addVertex(pos, Vector2f_zero, c);
        addVertex(p4, Vector2f_zero, c);
        addVertex(p3, Vector2f_zero, c);
    }

    void RenderImm::addBox(const btVector3& pos, const std::array<btVector3, 3>& dirs, const Color& c)
    {
        auto p2 = pos + dirs[0];
        auto p3 = pos + dirs[1];
        auto p4 = p2 + dirs[1];

        auto t = pos + dirs[2];
        auto t2 = p2 + dirs[2];
        auto t3 = p3 + dirs[2];
        auto t4 = p4 + dirs[2];

        // Top
        addVertex(t, Vector2f_zero, c);
        addVertex(t2, Vector2f_zero, c);
        addVertex(t4, Vector2f_zero, c);

        addVertex(t, Vector2f_zero, c);
        addVertex(t4, Vector2f_zero, c);
        addVertex(t3, Vector2f_zero, c);

        // Bottom
        addVertex(p4, Vector2f_zero, c);
        addVertex(p2, Vector2f_zero, c);
        addVertex(pos, Vector2f_zero, c);

        addVertex(p3, Vector2f_zero, c);
        addVertex(p4, Vector2f_zero, c);
        addVertex(pos, Vector2f_zero, c);

        // Right
        addVertex(p2, Vector2f_zero, c);
        addVertex(p4, Vector2f_zero, c);
        addVertex(t4, Vector2f_zero, c);

        addVertex(p2, Vector2f_zero, c);
        addVertex(t4, Vector2f_zero, c);
        addVertex(t2, Vector2f_zero, c);

        // Left
        addVertex(pos, Vector2f_zero, c);
        addVertex(t3, Vector2f_zero, c);
        addVertex(p3, Vector2f_zero, c);

        addVertex(pos, Vector2f_zero, c);
        addVertex(t, Vector2f_zero, c);
        addVertex(t3, Vector2f_zero, c);

        // Front
        addVertex(pos, Vector2f_zero, c);
        addVertex(p2, Vector2f_zero, c);
        addVertex(t2, Vector2f_zero, c);

        addVertex(pos, Vector2f_zero, c);
        addVertex(t2, Vector2f_zero, c);
        addVertex(t, Vector2f_zero, c);

        // Back
        addVertex(p3, Vector2f_zero, c);
        addVertex(t3, Vector2f_zero, c);
        addVertex(t4, Vector2f_zero, c);

        addVertex(p3, Vector2f_zero, c);
        addVertex(t4, Vector2f_zero, c);
        addVertex(p4, Vector2f_zero, c);
    }

    void RenderImm::addRing(const btVector3& pos, const btVector3& up, float radius, const Color& c, int numSegments)
    {
        btQuaternion rot(up, SIMD_2_PI / numSegments);

        auto p = btZeroNormalized(btPerpendicular(up)) * radius;

        auto dirN = btZeroNormalized(p.cross(up));

        auto pUp1 = pos + p + up;
        auto pUp2 = pos + p + up.rotate(dirN, SIMD_2_PI / 3);
        auto pUp3 = pos + p + up.rotate(dirN, -SIMD_2_PI / 3);

        for (int i = 0; i < numSegments; ++i) {
            p = quatRotate(rot, p);
            dirN = quatRotate(rot, dirN);

            auto up1 = pos + p + up;
            auto up2 = pos + p + up.rotate(dirN, SIMD_2_PI / 3);
            auto up3 = pos + p + up.rotate(dirN, -SIMD_2_PI / 3);

            // Side 1
            addVertex(pUp3, Vector2f_zero, c);
            addVertex(up1, Vector2f_zero, c);
            addVertex(pUp1, Vector2f_zero, c);

            addVertex(pUp3, Vector2f_zero, c);
            addVertex(up3, Vector2f_zero, c);
            addVertex(up1, Vector2f_zero, c);

            // Side 2
            addVertex(pUp1, Vector2f_zero, c);
            addVertex(up1, Vector2f_zero, c);
            addVertex(pUp2, Vector2f_zero, c);

            addVertex(up1, Vector2f_zero, c);
            addVertex(up2, Vector2f_zero, c);
            addVertex(pUp2, Vector2f_zero, c);

            // Side 3
            addVertex(pUp2, Vector2f_zero, c);
            addVertex(up2, Vector2f_zero, c);
            addVertex(up3, Vector2f_zero, c);

            addVertex(pUp2, Vector2f_zero, c);
            addVertex(up3, Vector2f_zero, c);
            addVertex(pUp3, Vector2f_zero, c);

            pUp1 = up1;
            pUp2 = up2;
            pUp3 = up3;
        }
    }

    void RenderImm::addCircle(const btVector3& pos, const btVector3& up, const Color& c, int numSegments)
    {
        btQuaternion rot(up, SIMD_2_PI / numSegments);

        auto p = btZeroNormalized(btPerpendicular(up)) * up.length();

        auto p1 = pos + p;

        for (int i = 0; i < numSegments; ++i) {
            p = quatRotate(rot, p);

            auto p2 = pos + p;

            addVertex(p1, Vector2f_zero, c);
            addVertex(p2, Vector2f_zero, c);
            addVertex(pos, Vector2f_zero, c);

            p1 = p2;
        }
    }

    RenderList::RenderList(const CameraPtr& camera, const SceneEnvironmentPtr& env)
    : camera_(camera),
      env_(env)
    {
    }

    void RenderList::addGeometry(const Matrix4f& modelMat, const Matrix4f& prevModelMat,
        const AABB& aabb, const MaterialPtr& material,
        const VertexArraySlice& vaSlice, GLenum primitiveMode, float depthValue,
        const ScissorParams& scissorParams)
    {
        geomList_.emplace_back(modelMat, prevModelMat, aabb, material, vaSlice, primitiveMode, depthValue, scissorParams);
    }

    void RenderList::addGeometry(const MaterialPtr& material,
        const VertexArraySlice& vaSlice, GLenum primitiveMode, float depthValue,
        const ScissorParams& scissorParams)
    {
        geomList_.emplace_back(material, vaSlice, primitiveMode, depthValue, scissorParams);
    }

    RenderImm RenderList::addGeometry(const MaterialPtr& material,
        GLenum primitiveMode,
        float depthValue, const ScissorParams& scissorParams)
    {
        return RenderImm(material, primitiveMode, depthValue, scissorParams, *this);
    }

    VertexArraySlice RenderList::createGeometry(const VertexImm* vertices, std::uint32_t numVertices,
        const std::uint16_t* indices, std::uint32_t numIndices)
    {
        auto startVertices = env_->defaultVa().data().vertices.size();

        env_->defaultVa().data().vertices.insert(env_->defaultVa().data().vertices.end(), vertices, vertices + numVertices);
        if (indices) {
            auto startIndices = env_->defaultVa().data().indices.size();

            env_->defaultVa().data().indices.insert(env_->defaultVa().data().indices.end(), indices, indices + numIndices);

            return VertexArraySlice(env_->defaultVa().va(),
                startIndices,
                env_->defaultVa().data().indices.size() - startIndices,
                startVertices);
        } else {
            return VertexArraySlice(env_->defaultVa().vaNoEbo(),
                startVertices,
                env_->defaultVa().data().vertices.size() - startVertices,
                0);
        }
    }

    void RenderList::addLight(const LightPtr& light)
    {
        lightList_.push_back(light);
    }
}
