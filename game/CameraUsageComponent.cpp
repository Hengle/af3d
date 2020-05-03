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

#include "CameraUsageComponent.h"
#include "Scene.h"

namespace af3d
{
    ACLASS_DEFINE_BEGIN(CameraUsageComponent, PhasedComponent)
    ACLASS_DEFINE_END(CameraUsageComponent)

    CameraUsageComponent::CameraUsageComponent(const CameraPtr& camera)
    : PhasedComponent(AClass_CameraUsageComponent, phasePreRender),
      camera_(camera)
    {
    }

    const AClass& CameraUsageComponent::staticKlass()
    {
        return AClass_CameraUsageComponent;
    }

    AObjectPtr CameraUsageComponent::create(const APropertyValueMap& propVals)
    {
        return AObjectPtr();
    }

    void CameraUsageComponent::preRender(float dt)
    {
        camera_->setTransform(parent()->smoothTransform());
    }

    void CameraUsageComponent::incUseCount()
    {
        ++useCount_;
        if ((useCount_ == 1) && scene()) {
            scene()->addCamera(camera_);
        }
    }

    void CameraUsageComponent::decUseCount()
    {
        --useCount_;
        if (useCount_ <= 0) {
            btAssert(useCount_ == 0);
            if (scene()) {
                scene()->removeCamera(camera_);
            }
        }
    }

    void CameraUsageComponent::onRegister()
    {
        camera_->setTransform(parent()->smoothTransform());
        if (useCount_ > 0) {
            scene()->addCamera(camera_);
        }
    }

    void CameraUsageComponent::onUnregister()
    {
        scene()->removeCamera(camera_);
    }
}
