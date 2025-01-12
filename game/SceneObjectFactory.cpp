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

#include "SceneObjectFactory.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "AssetManager.h"
#include "TextureManager.h"
#include "RenderMeshComponent.h"
#include "RenderQuadComponent.h"
#include "RenderFilterComponent.h"
#include "RenderSkyBoxComponent.h"
#include "PhysicsBodyComponent.h"
#include "CollisionSensorComponent.h"
#include "CollisionShapeBox.h"
#include "PhysicsJointComponent.h"
#include "JointPointToPoint.h"
#include "CameraComponent.h"
#include "CameraUsageComponent.h"
#include "TVComponent.h"
#include "LightProbeComponent.h"
#include "Settings.h"
#include "Utils.h"
#include "Logger.h"
#include "Const.h"
#include "af3d/Utils.h"

namespace af3d
{
    SceneObjectFactory sceneObjectFactory;

    template <>
    Single<SceneObjectFactory>* Single<SceneObjectFactory>::single = nullptr;

    bool SceneObjectFactory::init()
    {
        LOG4CPLUS_DEBUG(logger(), "sceneObjectFactory: init...");
        return true;
    }

    void SceneObjectFactory::shutdown()
    {
        LOG4CPLUS_DEBUG(logger(), "sceneObjectFactory: shutdown...");
    }

    SceneObjectPtr SceneObjectFactory::createDummy()
    {
        auto obj = std::make_shared<SceneObject>();
        obj->setBodyType(BodyType::Kinematic);

        obj->addComponent(std::make_shared<PhysicsBodyComponent>());

        return obj;
    }

    SceneObjectPtr SceneObjectFactory::createColoredBox(const btVector3& size, const Color& color1, const Color& color2)
    {
        auto mesh = meshManager.createBoxMesh(size,
            materialManager.matUnlitVCDefault(),
            {
                color2,
                color1,
                color2,
                color1,
                color1,
                color2
            });

        auto obj = std::make_shared<SceneObject>();
        auto rc = std::make_shared<RenderMeshComponent>();
        rc->setMesh(mesh);
        obj->addComponent(rc);

        return obj;
    }

    SceneObjectPtr SceneObjectFactory::createInstance(const std::string& assetPath)
    {
        auto sa = assetManager.getSceneObjectAsset(assetPath);

        if (!sa) {
            return std::make_shared<SceneObject>();
        }

        return sa->root();
    }

    SceneObjectPtr SceneObjectFactory::createSensor(bool allowSensor)
    {
        auto obj = std::make_shared<SceneObject>();

        auto c = std::make_shared<CollisionSensorComponent>();
        c->setAllowSensor(allowSensor);

        obj->addComponent(c);

        return obj;
    }

    SceneObjectPtr SceneObjectFactory::createTestRef(const SceneObjectPtr& other1, const SceneObjectPtr& other2)
    {
        auto obj = std::make_shared<SceneObject>();

        return obj;
    }

    SceneObjectPtr SceneObjectFactory::createLinkedBoxes(const btVector3& size, const Color& color1, const Color& color2)
    {
        auto obj = createColoredBox(size, color1, color2);
        obj->setBodyType(BodyType::Dynamic);
        auto pc = std::make_shared<PhysicsBodyComponent>();
        pc->addShape(std::make_shared<CollisionShapeBox>(size));
        obj->addComponent(pc);

        auto obj2 = createColoredBox(size, color2, color1);
        obj2->setPos(-btVector3_forward * size);
        obj2->setBodyType(BodyType::Dynamic);
        pc = std::make_shared<PhysicsBodyComponent>();
        pc->addShape(std::make_shared<CollisionShapeBox>(size));
        obj2->addComponent(pc);

        obj->addObject(obj2);

        auto j = std::make_shared<JointPointToPoint>(obj, obj2);
        j->setPivotA(-btVector3_forward * size * 0.5f);
        j->setPivotB(btVector3_forward * size * 0.5f);

        obj->addComponent(
            std::make_shared<PhysicsJointComponent>(Joints{j}));

        return obj;
    }

    SceneObjectPtr SceneObjectFactory::createTestCamera(const Color& clearColor, const Color& ambientColor, float scale)
    {
        auto obj = std::make_shared<SceneObject>();

        auto cam = std::make_shared<Camera>();
        cam->setOrder(camOrderTestCamera);
        cam->setAspect(settings.viewAspect);
        cam->setRenderTarget(AttachmentPoint::Color0, RenderTarget(textureManager.createRenderTextureScaled(TextureType2D, scale, 0, GL_RGB16F, GL_RGB, GL_FLOAT)));
        cam->setClearColor(AttachmentPoint::Color0, clearColor);
        cam->setAmbientColor(ambientColor);

        obj->addComponent(std::make_shared<CameraComponent>(cam));

        auto c = std::make_shared<CameraUsageComponent>(cam);
        obj->addComponent(c);

        if (settings.editor.enabled && !settings.editor.playing) {
            auto mesh = meshManager.loadConvertedMesh("camera.fbx", MaterialTypeUnlit);
            auto rc = std::make_shared<RenderMeshComponent>();
            rc->cameraFilter().layers() = CameraLayer::Main;
            rc->setMesh(mesh);
            obj->addComponent(rc);
        }

        return obj;
    }

    SceneObjectPtr SceneObjectFactory::createTestCameraDisplay(const SceneObjectPtr& camObj, float scale)
    {
        CameraUsageComponentPtr c;

        if (camObj) {
            c = camObj->findComponent<CameraUsageComponent>();
        }

        RenderFilterComponentPtr filterRc;

        auto obj = std::make_shared<SceneObject>();

        auto mesh = meshManager.loadMesh("tv.fbx");
        if (c && c->camera()->renderTarget()) {
            int i;
            for (i = 0; i < static_cast<int>(mesh->subMeshes().size()); ++i) {
                if (mesh->subMeshes()[i]->material()->name() == "tv.fbx/video") {
                    break;
                }
            }
            mesh = mesh->clone([i](int j, const MaterialPtr& m) {
                if (i == j) {
                    return m->convert(MaterialTypeUnlit);
                } else {
                    return m->clone();
                }
            });

            filterRc = std::make_shared<RenderFilterComponent>(MaterialTypeFilterVHS);
            filterRc->material()->setTimeOffset(getRandom(0.0f, 5.0f));
            filterRc->material()->setTextureBinding(SamplerName::Main,
                TextureBinding(c->camera()->renderTarget().texture(),
                    SamplerParams(GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST)));
            filterRc->material()->setTextureBinding(SamplerName::Noise,
                TextureBinding(textureManager.loadTexture("noise1.png"),
                    SamplerParams(GL_LINEAR)));
            filterRc->camera()->setOrder(camOrderTestDisplayFilter);
            filterRc->camera()->setRenderTarget(AttachmentPoint::Color0, RenderTarget(textureManager.createRenderTextureScaled(TextureType2D,
                static_cast<float>(settings.viewHeight) / c->camera()->renderTarget().texture()->height(), 0, GL_RGB16F, GL_RGB, GL_FLOAT)));

            mesh->subMeshes()[i]->material()->setTextureBinding(SamplerName::Main,
                TextureBinding(filterRc->camera()->renderTarget().texture(),
                    SamplerParams(GL_LINEAR)));
        }

        auto rc = std::make_shared<RenderMeshComponent>();
        rc->setMesh(mesh);
        rc->setScale(btVector3_one * scale);
        obj->addComponent(rc);
        if (c) {
            obj->addComponent(std::make_shared<TVComponent>(c, mesh->aabb().scaledAt0(rc->scale()), filterRc));
        }

        return obj;
    }

    SceneObjectPtr SceneObjectFactory::createLightProbe(const AABB& bounds, bool spherical, const Color& ambientColor, const Color& specularColor)
    {
        auto obj = std::make_shared<SceneObject>();

        obj->addComponent(std::make_shared<LightProbeComponent>(bounds, spherical, ambientColor, specularColor));

        return obj;
    }

    SceneObjectPtr SceneObjectFactory::createSkyBox(const std::string& texturePath)
    {
        auto obj = std::make_shared<SceneObject>();

        obj->addComponent(std::make_shared<RenderSkyBoxComponent>(
            textureManager.loadTexture(texturePath)));

        return obj;
    }

    SCENEOBJECT_DEFINE_BEGIN(Dummy)
    {
        return sceneObjectFactory.createDummy();
    }
    SCENEOBJECT_DEFINE_PROPS(Dummy)
    SCENEOBJECT_DEFINE_END(Dummy)

    SCENEOBJECT_DEFINE_BEGIN(ColoredBox)
    {
        return sceneObjectFactory.createColoredBox(
            params.get("size").toVec3(),
            params.get("color1").toColor(),
            params.get("color2").toColor());
    }
    SCENEOBJECT_DEFINE_PROPS(ColoredBox)
    SCENEOBJECT_PARAM(ColoredBox, "size", "Box size", Vec3f, btVector3(1.0f, 2.0f, 3.0f))
    SCENEOBJECT_PARAM(ColoredBox, "color1", "Box color #1", ColorRGB, Color(1.0f, 0.0f, 0.0f, 1.0f))
    SCENEOBJECT_PARAM(ColoredBox, "color2", "Box color #2", ColorRGB, Color(0.0f, 1.0f, 0.0f, 1.0f))
    SCENEOBJECT_DEFINE_END(ColoredBox)

    SCENEOBJECT_DEFINE_BEGIN(Instance)
    {
        return sceneObjectFactory.createInstance(
            params.get("asset").toString());
    }
    SCENEOBJECT_DEFINE_PROPS(Instance)
    SCENEOBJECT_PARAM(Instance, "asset", "Asset path", String, "empty.af3")
    SCENEOBJECT_DEFINE_END(Instance)

    SCENEOBJECT_DEFINE_BEGIN(Sensor)
    {
        return sceneObjectFactory.createSensor(params.get("allow sensors").toBool());
    }
    SCENEOBJECT_DEFINE_PROPS_NO_RESTRICT(Sensor)
    SCENEOBJECT_PARAM(Sensor, "allow sensors", "React on other sensors", Bool, false)
    SCENEOBJECT_DEFINE_END(Sensor)

    SCENEOBJECT_DEFINE_BEGIN(TestRef)
    {
        return sceneObjectFactory.createTestRef(params.get("other1").toObject<SceneObject>(),
            params.get("other2").toObject<SceneObject>());
    }
    SCENEOBJECT_DEFINE_PROPS(TestRef)
    SCENEOBJECT_PARAM(TestRef, "other1", "Other1", SceneObject, SceneObjectPtr())
    SCENEOBJECT_PARAM(TestRef, "other2", "Other2", SceneObject, SceneObjectPtr())
    SCENEOBJECT_DEFINE_END(TestRef)

    SCENEOBJECT_DEFINE_BEGIN(LinkedBoxes)
    {
        return sceneObjectFactory.createLinkedBoxes(
            params.get("size").toVec3(),
            params.get("color1").toColor(),
            params.get("color2").toColor());
    }
    SCENEOBJECT_DEFINE_PROPS(LinkedBoxes)
    SCENEOBJECT_PARAM(LinkedBoxes, "size", "Box size", Vec3f, btVector3(1.0f, 2.0f, 3.0f))
    SCENEOBJECT_PARAM(LinkedBoxes, "color1", "Box color #1", ColorRGB, Color(1.0f, 0.0f, 0.0f, 1.0f))
    SCENEOBJECT_PARAM(LinkedBoxes, "color2", "Box color #2", ColorRGB, Color(0.0f, 1.0f, 0.0f, 1.0f))
    SCENEOBJECT_DEFINE_END(LinkedBoxes)

    SCENEOBJECT_DEFINE_BEGIN(TestCamera)
    {
        return sceneObjectFactory.createTestCamera(params.get("clear color").toColor(),
            params.get("ambient color").toColor(), params.get("scale").toFloat());
    }
    SCENEOBJECT_DEFINE_PROPS(TestCamera)
    SCENEOBJECT_PARAM(TestCamera, "clear color", "Clear color", ColorRGB, Color(0.23f, 0.23f, 0.23f, 1.0f))
    SCENEOBJECT_PARAM(TestCamera, "ambient color", "Ambient color", ColorRGB, Color(0.2f, 0.2f, 0.2f, 1.0f))
    SCENEOBJECT_PARAM(TestCamera, "scale", "Scale", Float, 2.0f)
    SCENEOBJECT_DEFINE_END(TestCamera)

    SCENEOBJECT_DEFINE_BEGIN(TestCameraDisplay)
    {
        return sceneObjectFactory.createTestCameraDisplay(params.get("camera object").toObject<SceneObject>(),
            params.get("scale").toFloat());
    }
    SCENEOBJECT_DEFINE_PROPS(TestCameraDisplay)
    SCENEOBJECT_PARAM(TestCameraDisplay, "camera object", "Camera object", SceneObject, SceneObjectPtr())
    SCENEOBJECT_PARAM(TestCameraDisplay, "scale", "Scale", Float, 1.0f)
    SCENEOBJECT_DEFINE_END(TestCameraDisplay)

    SCENEOBJECT_DEFINE_BEGIN(LightProbe)
    {
        return sceneObjectFactory.createLightProbe(
            AABB(params.get("lower bound").toVec3(), params.get("upper bound").toVec3()),
            params.get("spherical").toBool(),
            params.get("ambient color").toColor(), params.get("specular color").toColor());
    }
    SCENEOBJECT_DEFINE_PROPS(LightProbe)
    SCENEOBJECT_PARAM(LightProbe, AProperty_Name, "Object name", String, "")
    SCENEOBJECT_PARAM(LightProbe, "spherical", "Use spherical capture", Bool, false)
    SCENEOBJECT_PARAM(LightProbe, "lower bound", "Lower bound", Vec3f, btVector3(-0.5f, -0.5f, -0.5f))
    SCENEOBJECT_PARAM(LightProbe, "upper bound", "Upper bound", Vec3f, btVector3(0.5f, 0.5f, 0.5f))
    SCENEOBJECT_PARAM(LightProbe, "ambient color", "Ambient color", ColorRGB, Color(1.0f, 1.0f, 1.0f, 1.0f))
    SCENEOBJECT_PARAM(LightProbe, "specular color", "Specular color", ColorRGB, Color(0.5f, 0.5f, 0.5f, 1.0f))
    SCENEOBJECT_DEFINE_END(LightProbe)

    SCENEOBJECT_DEFINE_BEGIN(SkyBox)
    {
        return sceneObjectFactory.createSkyBox(
            params.get("texture").toString());
    }
    SCENEOBJECT_DEFINE_PROPS_NO_RESTRICT(SkyBox)
    SCENEOBJECT_PARAM(SkyBox, "texture", "Texture path", String, "empty.hdr")
    SCENEOBJECT_DEFINE_END(SkyBox)
}
