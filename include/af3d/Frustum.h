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

#ifndef _AF3D_FRUSTUM_H_
#define _AF3D_FRUSTUM_H_

#include "af3d/Types.h"
#include "af3d/Vector2.h"
#include "af3d/Matrix4.h"
#include "af3d/AABB.h"
#include "af3d/Plane.h"

namespace af3d
{
    enum class ProjectionType
    {
        Perspective = 0,
        Orthographic
    };

    class Frustum
    {
    public:
        enum class Plane
        {
            Near = 0,
            Far,
            Left,
            Right,
            Top,
            Bottom,
            Max = Bottom
        };

        using Planes = std::array<btPlane, 6>;
        using Corners = std::array<btVector3, 8>;

        Frustum() = default;
        ~Frustum() = default;

        inline ProjectionType projectionType() const { return projectionType_; }
        void setProjectionType(ProjectionType value);

        inline float fov() const { return fov_; }
        void setFov(float value);

        inline float orthoHeight() const { return orthoHeight_; }
        void setOrthoHeight(float value);

        inline float aspect() const { return aspect_; }
        void setAspect(float value);

        inline float nearDist() const { return nearDist_; }
        void setNearDist(float value);

        inline float farDist() const { return farDist_; }
        void setFarDist(float value);

        inline bool flipY() const { return flipY_; }
        void setFlipY(bool value);

        inline const btTransform& transform() const { return xf_; }
        void setTransform(const btTransform& value);

        inline const Vector2f& jitter() const { return jitter_; }
        void setJitter(const Vector2f& value);

        const Matrix4f& projMat() const;
        const Matrix4f& viewProjMat() const;
        const Matrix4f& jitteredViewProjMat() const;
        const Matrix4f& viewMat() const;
        const Planes& planes() const;
        const Corners& corners() const;

        inline const btPlane& plane(Plane p) const { return planes()[static_cast<int>(p)]; }

        inline const btVector3& corner(int i) const { return corners()[i]; }

        bool isVisible(const AABB& aabb) const;

        // How much world units are visible at 'worldPos'.
        Vector2f getExtents(const btVector3& worldPos) const;

    private:
        void projUpdated();

        void updateViewProjMat() const;

        inline btPlane& cachedPlane(Plane p) const { return cachedPlanes_[static_cast<int>(p)]; }

        ProjectionType projectionType_ = ProjectionType::Perspective;
        float fov_ = btRadians(45.0f);
        float orthoHeight_ = 40.0f;
        float aspect_ = 4.0f / 3.0f;
        float nearDist_ = 0.05f;
        float farDist_ = 300.0f;
        bool flipY_ = false;

        btTransform xf_ = btTransform::getIdentity();
        Vector2f jitter_ = Vector2f_zero;

        mutable Matrix4f cachedProjMat_;
        mutable Matrix4f cachedViewProjMat_;
        mutable Matrix4f cachedJitteredViewProjMat_;
        mutable Matrix4f cachedViewMat_;
        mutable Vector4f cachedProjParams_;
        mutable Planes cachedPlanes_;
        mutable Corners cachedCorners_;

        mutable bool recalcProjMat_ = true;
        mutable bool recalcViewProjMat_ = true;
        mutable bool recalcPlanes_ = true;
        mutable bool recalcCorners_ = true;
    };
}

#endif
