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

#ifndef _AF3D_RAY_H_
#define _AF3D_RAY_H_

#include "af3d/Types.h"
#include "af3d/AABB.h"
#include "af3d/Sphere.h"
#include "af3d/Plane.h"

namespace af3d
{
    // Raycasts from inside the volume always fail.
    using RayTestResult = std::pair<bool, float>; // (intersects, distance)

    class Ray
    {
    public:
        btVector3 pos;
        btVector3 dir;

        Ray() = default;
        Ray(const btVector3& pos, const btVector3& dir);

        bool empty() const;

        btVector3 getAt(float t) const;

        Ray getTransformed(const btTransform& xf) const;

        RayTestResult testAABB(const AABB& aabb) const;

        RayTestResult testSphere(const Sphere& s) const;

        RayTestResult testPlane(const btPlane& p) const;
    };

    extern const Ray Ray_empty;
}

#endif
