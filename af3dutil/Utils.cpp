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

#include "Logger.h"
#include "af3d/Utils.h"
#include "af3d/Vector2.h"
#include "af3d/Vector3.h"
#include "af3d/Vector4.h"
#include <chrono>

namespace af3d
{
    const Vector2f Vector2f_zero(0.0f, 0.0f);
    const Vector2i Vector2i_zero(0, 0);

    const Vector3f Vector3f_zero(0.0f, 0.0f, 0.0f);
    const Vector3i Vector3i_zero(0, 0, 0);
    const btVector3 btVector3_zero(0.0f, 0.0f, 0.0f);
    const btVector3 btVector3_up(0.0f, 1.0f, 0.0f);
    const btVector3 btVector3_down(0.0f, -1.0f, 0.0f);
    const btVector3 btVector3_forward(0.0f, 0.0f, -1.0f);
    const btVector3 btVector3_back(0.0f, 0.0f, 1.0f);
    const btVector3 btVector3_right(1.0f, 0.0f, 0.0f);
    const btVector3 btVector3_left(-1.0f, 0.0f, 0.0f);
    const btVector3 btVector3_one(1.0f, 1.0f, 1.0f);

    const Vector4f Vector4f_zero(0.0f, 0.0f, 0.0f, 0.0f);
    const Vector4i Vector4i_zero(0, 0, 0, 0);
    const Color Color_zero(0.0f, 0.0f, 0.0f, 0.0f);

    static std::chrono::steady_clock::time_point startTime;

    void initTimeUs()
    {
        startTime = std::chrono::steady_clock::now();
    }

    std::uint64_t getTimeUs()
    {
        std::chrono::steady_clock::duration d =
           std::chrono::steady_clock::now() - startTime;

        return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
    }

    float getRandom(float minVal, float maxVal)
    {
        float r = static_cast<float>(rand() % RAND_MAX) / static_cast<float>(RAND_MAX);
        return minVal + (maxVal - minVal) * r;
    }

    int getRandomInt(int minVal, int maxVal)
    {
        return minVal + (rand() % (maxVal - minVal + 1));
    }

    bool readStream(std::istream& is, std::string& str)
    {
        str.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());
        return !is.fail();
    }

    void makeLookDir(const btVector3& dir, const btVector3& up, btMatrix3x3& basis)
    {
        auto vForward = btZeroNormalized(-dir);
        auto vRight = up.cross(vForward);
        btZeroNormalize(vRight);
        auto vUp = vForward.cross(vRight);

        basis[0][0] = vRight[0];
        basis[1][0] = vRight[1];
        basis[2][0] = vRight[2];

        basis[0][1] = vUp[0];
        basis[1][1] = vUp[1];
        basis[2][1] = vUp[2];

        basis[0][2] = vForward[0];
        basis[1][2] = vForward[1];
        basis[2][2] = vForward[2];
    }

    void makeLookDir(const btVector3& dir, const btVector3& up, btQuaternion& rotation)
    {
        btMatrix3x3 m;
        makeLookDir(dir, up, m);
        m.getRotation(rotation);
    }

    void makeLookDir(const btVector3& pos, const btVector3& dir, const btVector3& up, btTransform& xf)
    {
        makeLookDir(dir, up, xf.getBasis());
        xf.setOrigin(pos);
    }

    btTransform makeLookDir(const btVector3& pos, const btVector3& dir, const btVector3& up)
    {
        btTransform xf;
        makeLookDir(pos, dir, up, xf);
        return xf;
    }

    btTransform makeLookAt(const btVector3& pos, const btVector3& target, const btVector3& up)
    {
        return makeLookDir(pos, target - pos, up);
    }
}
