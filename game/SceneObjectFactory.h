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

#ifndef _SCENEOBJECTFACTORY_H_
#define _SCENEOBJECTFACTORY_H_

#include "af3d/Types.h"
#include "af3d/Single.h"
#include "af3d/AABB.h"
#include "SceneObject.h"

namespace af3d
{
    class SceneObjectFactory : public Single<SceneObjectFactory>
    {
    public:
        SceneObjectFactory() = default;
        ~SceneObjectFactory() = default;

        bool init();

        void shutdown();

        SceneObjectPtr createDummy();

        SceneObjectPtr createColoredBox(const btVector3& size, const Color& color1, const Color& color2);

        SceneObjectPtr createInstance(const std::string& assetPath);

        SceneObjectPtr createSensor(bool allowSensor);

        SceneObjectPtr createTestRef(const SceneObjectPtr& other1, const SceneObjectPtr& other2);

        SceneObjectPtr createLinkedBoxes(const btVector3& size, const Color& color1, const Color& color2);

        SceneObjectPtr createTestCamera(const Color& clearColor, const Color& ambientColor, float scale);

        SceneObjectPtr createTestCameraDisplay(const SceneObjectPtr& camObj, float scale);

        SceneObjectPtr createLightProbe(const AABB& bounds, bool spherical, const Color& ambientColor, const Color& specularColor);

        SceneObjectPtr createSkyBox(const std::string& texturePath);
    };

    extern SceneObjectFactory sceneObjectFactory;
}

#endif
