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

#include "PointLight.h"

namespace af3d
{
    ACLASS_DEFINE_BEGIN(PointLight, Light)
    ACLASS_PROPERTY(PointLight, Radius, "radius", "Radius", Float, 1.0f, Position, APropertyEditable)
    ACLASS_DEFINE_END(PointLight)

    PointLight::PointLight()
    : Light(AClass_PointLight, TypeId, false)
    {
        setRadius(1.0f);
    }

    const AClass& PointLight::staticKlass()
    {
        return AClass_PointLight;
    }

    AObjectPtr PointLight::create(const APropertyValueMap& propVals)
    {
        auto obj = std::make_shared<PointLight>();
        obj->propertiesSet(propVals);
        return obj;
    }

    void PointLight::setRadius(float value)
    {
        radius_ = value;
        setLocalAABBImpl(AABB(-btVector3_one * value, btVector3_one * value));
    }

    void PointLight::doSetupCluster(ShaderClusterLightImpl& cLight) const
    {
        cLight.dir = Vector4f(radius_, 0.0f, 0.0f, 0.0f);
    }
}
