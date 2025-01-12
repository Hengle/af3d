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

#include "SceneEnvironment.h"
#include "LightProbeComponent.h"
#include "Light.h"
#include "HardwareResourceManager.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "Settings.h"
#include "Logger.h"
#include "ShaderDataTypes.h"

namespace af3d
{
    namespace
    {
        struct LightsSSBOUpdate : boost::noncopyable
        {
            HardwareDataBufferPtr ssbo;
            bool recreate = false;
            std::pair<int, int> indexRange{(std::numeric_limits<int>::max)(), 0};
            std::vector<std::pair<int, ShaderClusterLightImpl>> lights;
        };

        struct ProbesSSBOUpdate : boost::noncopyable
        {
            HardwareDataBufferPtr ssbo;
            std::vector<ShaderClusterProbe> probes;
        };

        using ProbePair = std::pair<ShaderClusterProbe, LightProbeComponent*>;
    };

    SceneEnvironment::SceneEnvironment()
    : lightsSSBO_(hwManager.createDataBuffer(HardwareBuffer::Usage::DynamicDraw, sizeof(ShaderClusterLight) + sizeof(std::uint32_t) * (settings.maxImmCameras + 1))),
      probesSSBO_(hwManager.createDataBuffer(HardwareBuffer::Usage::DynamicDraw, sizeof(ShaderClusterProbe))),
      irradianceTexture_(textureManager.createRenderTexture(TextureTypeCubeMapArray,
          settings.lightProbe.irradianceResolution, settings.lightProbe.irradianceResolution, settings.cluster.maxProbes, GL_RGB16F, GL_RGB, GL_FLOAT)),
      specularTexture_(textureManager.createRenderTexture(TextureTypeCubeMapArray,
          settings.lightProbe.specularResolution, settings.lightProbe.specularResolution, settings.cluster.maxProbes, GL_RGB16F, GL_RGB, GL_FLOAT, true))
    {
        for (int i = 0; i < static_cast<int>(settings.cluster.maxLights); ++i) {
            lightsFreeIndices_.insert(i);
        }
        // 0 - reserved for global probe.
        for (int i = 1; i < static_cast<int>(settings.cluster.maxProbes); ++i) {
            probesFreeIndices_.insert(i);
        }
    }

    SceneEnvironment::~SceneEnvironment()
    {
        btAssert(lights_.empty());
        btAssert(probes_.empty());
        btAssert(globalProbe_ == nullptr);
    }

    void SceneEnvironment::update(float realDt, float dt)
    {
        realDt_ = realDt;
        dt_ = dt;
        time_ += dt;

        immCameras_.clear();
        immCameras_[0] = 0; // 0 is special, all cameras without an imm index map into it.
    }

    void SceneEnvironment::preSwap()
    {
        defaultVa_.upload();
        preSwapLights();
        preSwapProbes();
        shadowMgr_.preSwap();
    }

    int SceneEnvironment::addLight(Light* light)
    {
        if (lightsFreeIndices_.empty()) {
            LOG4CPLUS_WARN(logger(), "Too many lights...");
            return -1;
        }
        int idx = *lightsFreeIndices_.begin();
        lightsFreeIndices_.erase(lightsFreeIndices_.begin());
        lights_.insert(light);
        lightsRemovedIndices_.erase(idx);
        return idx;
    }

    void SceneEnvironment::removeLight(Light* light)
    {
        if (lights_.erase(light) <= 0) {
            return;
        }
        btAssert(light->index() >= 0);
        bool res = lightsFreeIndices_.insert(light->index()).second;
        btAssert(res);
        lightsRemovedIndices_.insert(light->index());
    }

    LightProbeRenderTarget SceneEnvironment::addLightProbe(LightProbeComponent* probe)
    {
        if (probe->isGlobal()) {
            globalProbe_ = probe;
            probesNeedUpdate_ = true;
            return LightProbeRenderTarget(0, irradianceTexture_, specularTexture_);
        } else {
            if (probesFreeIndices_.empty()) {
                LOG4CPLUS_WARN(logger(), "Too many probes...");
                return LightProbeRenderTarget(1, irradianceTexture_, specularTexture_);
            }
            int idx = *probesFreeIndices_.begin();
            probesFreeIndices_.erase(probesFreeIndices_.begin());
            probes_.insert(probe);
            probesToCheck_.insert(probe);
            probesNeedUpdate_ = true;
            return LightProbeRenderTarget(idx, irradianceTexture_, specularTexture_);
        }
    }

    void SceneEnvironment::removeLightProbe(LightProbeComponent* probe)
    {
        if (probe == globalProbe_) {
            globalProbe_ = nullptr;
            probesNeedUpdate_ = true;
        } else {
            if (probes_.erase(probe) <= 0) {
                return;
            }
            btAssert(probe->index() > 0);
            bool res = probesFreeIndices_.insert(probe->index()).second;
            btAssert(res);
            probesToCheck_.erase(probe);
            probesNeedUpdate_ = true;
        }
    }

    void SceneEnvironment::updateLightProbes()
    {
        if (globalProbe_) {
            if (globalProbe_->recreate()) {
                updateProbeTextures(globalProbe_);
            }
        }
        for (auto probe : probes_) {
            if (probe->recreate()) {
                updateProbeTextures(probe);
            }
        }
    }

    int SceneEnvironment::allocImmCameraIdx(ACookie camCookie)
    {
        auto it = immCameras_.find(camCookie);
        if (it == immCameras_.end()) {
            if (static_cast<std::uint32_t>(immCameras_.size()) >= (settings.maxImmCameras + 1)) {
                LOG4CPLUS_WARN(logger(), "Too many imm cameras...");
                return -1;
            }
            int idx = immCameras_.size();
            immCameras_[camCookie] = idx;
            return idx;
        }
        return it->second;
    }

    int SceneEnvironment::getImmCameraIdx(ACookie camCookie) const
    {
        auto it = immCameras_.find(camCookie);
        if (it == immCameras_.end()) {
            return 0;
        }
        return it->second;
    }

    void SceneEnvironment::preSwapLights()
    {
        bool recreate = lightsSSBO_->setValid();
        if (lights_.empty() && lightsRemovedIndices_.empty() && !recreate) {
            return;
        }

        auto upd = std::make_shared<LightsSSBOUpdate>();
        upd->ssbo = lightsSSBO_;
        upd->recreate = recreate;
        upd->lights.reserve(lights_.size() + lightsRemovedIndices_.size());
        for (auto light : lights_) {
            upd->indexRange.first = (std::min)(upd->indexRange.first, light->index());
            upd->indexRange.second = (std::max)(upd->indexRange.second, light->index());
            upd->lights.emplace_back(light->index(), ShaderClusterLightImpl());
            light->setupCluster(upd->lights.back().second);
        }
        for (auto idx : lightsRemovedIndices_) {
            upd->indexRange.first = (std::min)(upd->indexRange.first, idx);
            upd->indexRange.second = (std::max)(upd->indexRange.second, idx);
            upd->lights.emplace_back(idx, ShaderClusterLightImpl());
        }
        lightsRemovedIndices_.clear();
        renderer.scheduleHwOp([upd](HardwareContext& ctx) {
            char* ptr;
            if (upd->recreate) {
                upd->ssbo->resize(settings.cluster.maxLights, ctx);
                ptr = (char*)upd->ssbo->lock(HardwareBuffer::WriteOnly, ctx);
                auto ptrBase = ptr;
                for (int i = 0; i < upd->ssbo->count(ctx); ++i, ptr += upd->ssbo->elementSize()) {
                    ((ShaderClusterLight*)ptr)->enabled = 0;
                }
                ptr = ptrBase + upd->indexRange.first * upd->ssbo->elementSize();
            } else {
                btAssert(upd->indexRange.second >= upd->indexRange.first);
                ptr = (char*)upd->ssbo->lock(upd->indexRange.first,
                    upd->indexRange.second - upd->indexRange.first + 1, HardwareBuffer::ReadWrite, ctx);
            }
            for (const auto& lp : upd->lights) {
                auto dest = ptr + (lp.first - upd->indexRange.first) * upd->ssbo->elementSize();
                std::memcpy(dest, &lp.second, upd->ssbo->elementSize());
            }
            upd->ssbo->unlock(ctx);
        });
    }

    void SceneEnvironment::preSwapProbes()
    {
        if (irradianceTexture_->generation() != irradianceTextureGeneration_) {
            // Need to re-upload default textures.
            irradianceTextureGeneration_ = irradianceTexture_->generation();
            if (globalProbe_) {
                updateProbeTextures(globalProbe_);
            }
            for (auto probe : probes_) {
                updateProbeTextures(probe);
            }
        } else {
            for (auto probe : probesToCheck_) {
                updateProbeTextures(probe);
            }
        }

        probesToCheck_.clear();

        bool needUpdate = probesNeedUpdate_;
        probesNeedUpdate_ = false;
        if (globalProbe_) {
            needUpdate |= globalProbe_->resetDirty();
        }
        for (auto probe : probes_) {
            needUpdate |= probe->resetDirty();
        }

        bool recreate = probesSSBO_->setValid();

        if (!needUpdate && !recreate) {
            return;
        }

        std::vector<ProbePair> probes;
        probes.reserve(probes_.size());

        for (auto probe : probes_) {
            probes.emplace_back(ShaderClusterProbe(), probe);
            probe->setupCluster(probes.back().first);
        }

        std::sort(probes.begin(), probes.end(), [](const ProbePair& a, const ProbePair& b) {
            return a.second->bounds().getArea() > b.second->bounds().getArea();
        });

        auto upd = std::make_shared<ProbesSSBOUpdate>();
        upd->ssbo = probesSSBO_;
        upd->probes.resize(settings.cluster.maxProbes);

        if (globalProbe_) {
            globalProbe_->setupCluster(upd->probes[0]);
        }
        for (size_t i = 0; i < probes.size(); ++i) {
            upd->probes[i + 1] = probes[i].first;
        }

        LOG4CPLUS_TRACE(logger(), "Updating probes ssbo (" << probes.size() << ")...");

        renderer.scheduleHwOp([upd](HardwareContext& ctx) {
            upd->ssbo->reload(upd->probes.size(), &upd->probes[0], ctx);
        });
    }

    void SceneEnvironment::updateProbeTextures(LightProbeComponent* probe)
    {
        if (!probe->hasIrradiance()) {
            Color color = gammaToLinear(probe->ambientColor());
            std::vector<Byte> data(settings.lightProbe.irradianceResolution * settings.lightProbe.irradianceResolution * 3 * (TextureCubeFaceMax + 1) * sizeof(float));
            for (size_t i = 0; i < data.size(); i += 3 * sizeof(float)) {
                *(float*)&data[i + sizeof(float) * 0] = color.x();
                *(float*)&data[i + sizeof(float) * 1] = color.y();
                *(float*)&data[i + sizeof(float) * 2] = color.z();
            }
            irradianceTexture_->update(GL_RGB, GL_FLOAT, std::move(data), 0, probe->index());
        }
        if (!probe->hasSpecular()) {
            Color color = gammaToLinear(probe->specularColor());
            std::uint32_t mip = 0;
            while (std::uint32_t sz = textureMipSize(settings.lightProbe.specularResolution, mip)) {
                std::vector<Byte> data(sz * sz * 3 * (TextureCubeFaceMax + 1) * sizeof(float));
                for (size_t i = 0; i < data.size(); i += 3 * sizeof(float)) {
                    *(float*)&data[i + sizeof(float) * 0] = color.x();
                    *(float*)&data[i + sizeof(float) * 1] = color.y();
                    *(float*)&data[i + sizeof(float) * 2] = color.z();
                }
                specularTexture_->update(GL_RGB, GL_FLOAT, std::move(data), mip, probe->index());
                ++mip;
            }
        }
    }
}
