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

#include "RenderFilterComponent.h"

namespace af3d
{
    ACLASS_DEFINE_BEGIN(RenderFilterComponent, RenderComponent)
    ACLASS_DEFINE_END(RenderFilterComponent)

    RenderFilterComponent::RenderFilterComponent()
    : RenderComponent(AClass_RenderFilterComponent, true)
    {
        cameraFilter().layers().resetAll();
    }

    const AClass& RenderFilterComponent::staticKlass()
    {
        return AClass_RenderFilterComponent;
    }

    AObjectPtr RenderFilterComponent::create(const APropertyValueMap& propVals)
    {
        auto obj = std::make_shared<RenderFilterComponent>();
        obj->propertiesSet(propVals);
        return obj;
    }

    void RenderFilterComponent::update(float dt)
    {
    }

    void RenderFilterComponent::render(RenderList& rl, void* const* parts, size_t numParts)
    {
        static Vector3f p0{-1.0f, -1.0f, 0.0f};
        static Vector3f p1{1.0f, -1.0f, 0.0f};
        static Vector3f p2{1.0f, 1.0f, 0.0f};
        static Vector3f p3{-1.0f, 1.0f, 0.0f};

        static Vector2f uv0{0.0f, 0.0f};
        static Vector2f uv1{1.0f, 0.0f};
        static Vector2f uv2{1.0f, 1.0f};
        static Vector2f uv3{0.0f, 1.0f};

        auto rop = rl.addGeometry(material_, GL_TRIANGLES);

        rop.addVertex(p0, uv0, color());
        rop.addVertex(p1, uv1, color());
        rop.addVertex(p2, uv2, color());

        rop.addVertex(p0, uv0, color());
        rop.addVertex(p2, uv2, color());
        rop.addVertex(p3, uv3, color());
    }

    std::pair<AObjectPtr, float> RenderFilterComponent::testRay(const Frustum& frustum, const Ray& ray, void* part)
    {
        return std::make_pair(AObjectPtr(), 0.0f);
    }

    void RenderFilterComponent::onRegister()
    {
    }

    void RenderFilterComponent::onUnregister()
    {
    }
}