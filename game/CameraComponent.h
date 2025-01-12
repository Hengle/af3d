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

#ifndef _CAMERACOMPONENT_H_
#define _CAMERACOMPONENT_H_

#include "PhasedComponent.h"
#include "Camera.h"
#include "af3d/Ray.h"

namespace af3d
{
    class CameraComponent : public std::enable_shared_from_this<CameraComponent>,
        public PhasedComponent
    {
    public:
        explicit CameraComponent(const CameraPtr& camera);
        ~CameraComponent() = default;

        static const AClass& staticKlass();

        static AObjectPtr create(const APropertyValueMap& propVals);

        AObjectPtr sharedThis() override { return shared_from_this(); }

        void preRender(float dt) override;

        inline const CameraPtr& camera() const { return camera_; }

        Vector2f screenToViewport(const Vector2f& pt) const;

        // pt is pixels, (0,0) - top-left, (w,h) - bottom-right
        Ray screenPointToRay(const Vector2f& pt) const;

        // pt is in range 0.0-1.0, (0,0) - bottom-left, (1,1) - top-right
        Ray viewportPointToRay(const Vector2f& pt) const;

    private:
        void onRegister() override;

        void onUnregister() override;

        CameraPtr camera_;
    };

    using CameraComponentPtr = std::shared_ptr<CameraComponent>;

    ACLASS_DECLARE(CameraComponent)
}

#endif
