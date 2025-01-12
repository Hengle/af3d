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

#include "RenderComponent.h"
#include "SceneObject.h"

namespace af3d
{
    ACLASS_DEFINE_BEGIN_ABSTRACT(RenderComponent, Component)
    ACLASS_PROPERTY(RenderComponent, Visible, AProperty_Visible, "Component is visible", Bool, true, General, APropertyEditable)
    ACLASS_DEFINE_END(RenderComponent)

    RenderComponent::RenderComponent(const AClass& klass, bool renderAlways)
    : Component(klass),
      renderAlways_(renderAlways)
    {
        camFilter_.layers().set(CameraLayer::General);
        camFilter_.layers().set(CameraLayer::Main);
        camFilter_.layers().set(CameraLayer::LightProbe);
    }

    const AClass& RenderComponent::staticKlass()
    {
        return AClass_RenderComponent;
    }

    const CameraFilter& RenderComponent::getCameraFilterWithFixup() const
    {
        if (cameraFilter().layers()[CameraLayer::LightProbe] && parent() && (parent()->bodyType() != BodyType::Static)) {
            // Non-static objects should never go into light probes.
            tmpCamFilter_ = cameraFilter();
            tmpCamFilter_.layers().reset(CameraLayer::LightProbe);
            return tmpCamFilter_;
        } else {
            return cameraFilter();
        }
    }
}
