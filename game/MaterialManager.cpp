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

#include "MaterialManager.h"
#include "HardwareResourceManager.h"
#include "TextureManager.h"
#include "Logger.h"
#include "Platform.h"
#include "Settings.h"
#include "af3d/Assert.h"

namespace af3d
{
    static const struct {
        const char* vert;
        const char* frag;
        const char* compute;
        const char* header;
    } shaders[MaterialTypeMax + 1] = {
        {"shaders/basic.vert", "shaders/basic.frag", nullptr, nullptr},
        {"shaders/basic.vert", "shaders/basic.frag", nullptr, "#define NM 1\n"},
        {"shaders/unlit.vert", "shaders/unlit.frag", nullptr, nullptr},
        {"shaders/unlit-vc.vert", "shaders/unlit-vc.frag", nullptr, nullptr},
        {"shaders/imm.vert", "shaders/imm.frag", nullptr, nullptr},
        {"shaders/outline.vert", "shaders/outline.frag", nullptr, nullptr},
        {"shaders/grid.vert", "shaders/grid.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-vhs.frag", nullptr, nullptr},
        {"shaders/filter-cubemap.vert", "shaders/filter-irradiance-conv.frag", nullptr, nullptr},
        {"shaders/basic.vert", "shaders/pbr.frag", nullptr, nullptr},
        {"shaders/basic.vert", "shaders/pbr.frag", nullptr, "#define NM 1\n"},
        {"shaders/filter.vert", "shaders/filter-cube2equirect.frag", nullptr, nullptr},
        {"shaders/filter-cubemap.vert", "shaders/filter-equirect2cube.frag", nullptr, nullptr},
        {"shaders/filter-cubemap.vert", "shaders/filter-specularcm.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-specularlut.frag", nullptr, nullptr},
        {"shaders/filter-fxaa.vert", "shaders/filter-fxaa.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-tone-mapping.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-gaussian-blur.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-bloom-pass1.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-bloom-pass2.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-taa.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-downscale.frag", nullptr, nullptr},
        {"shaders/skybox.vert", "shaders/skybox.frag", nullptr, nullptr},
        {"shaders/basic.vert", "shaders/pbr.frag", nullptr, "#define FAST 1\n"},
        {"shaders/basic.vert", "shaders/pbr.frag", nullptr, "#define FAST 1\n#define NM 1\n"},
        {nullptr, nullptr, "shaders/cluster-build.comp", nullptr},
        {nullptr, nullptr, "shaders/cluster-cull.comp", nullptr},
        {"shaders/prepass1.vert", "shaders/prepass.frag", nullptr, nullptr},
        {"shaders/prepass2.vert", "shaders/prepass.frag", nullptr, nullptr},
        {"shaders/prepass-ws.vert", "shaders/prepass.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-composite.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-ssao.frag", nullptr, nullptr},
        {"shaders/filter.vert", "shaders/filter-ssao-blur.frag", nullptr, nullptr},
        {"shaders/prepass1.vert", nullptr, nullptr, "#define SHADOW 1\n"},
        {"shaders/prepass2.vert", nullptr, nullptr, "#define SHADOW 1\n"},
        {"shaders/prepass-ws.vert", nullptr, nullptr, "#define SHADOW 1\n"}
    };

    MaterialManager materialManager;

    template <>
    Single<MaterialManager>* Single<MaterialManager>::single = nullptr;

    MaterialManager::~MaterialManager()
    {
        runtime_assert(cachedMaterials_.empty());
        runtime_assert(immediateMaterials_.empty());
    }

    bool MaterialManager::init()
    {
        LOG4CPLUS_DEBUG(logger(), "materialManager: init...");

        glslCommonHeader_ = "#version 430 core\n";
        glslCommonHeader_ += "#define CLUSTER_GRID_X " + std::to_string(settings.cluster.gridSize.x()) + "\n";
        glslCommonHeader_ += "#define CLUSTER_GRID_Y " + std::to_string(settings.cluster.gridSize.y()) + "\n";
        glslCommonHeader_ += "#define CLUSTER_GRID_Z " + std::to_string(settings.cluster.gridSize.z()) + "\n";
        glslCommonHeader_ += "#define CLUSTER_CULL_X " + std::to_string(settings.cluster.gridSize.x() / settings.cluster.cullNumGroups.x()) + "\n";
        glslCommonHeader_ += "#define CLUSTER_CULL_Y " + std::to_string(settings.cluster.gridSize.y() / settings.cluster.cullNumGroups.y()) + "\n";
        glslCommonHeader_ += "#define CLUSTER_CULL_Z " + std::to_string(settings.cluster.gridSize.z() / settings.cluster.cullNumGroups.z()) + "\n";
        glslCommonHeader_ += "#define SPECULAR_CM_LEVELS " + std::to_string(settings.lightProbe.specularMipLevels - 1) + "\n";
        glslCommonHeader_ += "#define MAX_IMM_CAMERAS " + std::to_string(settings.maxImmCameras) + "\n";
        glslCommonHeader_ += "#define CSM_NUM_SPLITS " + std::to_string(settings.csm.numSplits) + "\n";
        glslCommonHeader_ += "#line 1\n";

        for (int i = MaterialTypeFirst; i <= MaterialTypeMax; ++i) {
            MaterialTypeName name = static_cast<MaterialTypeName>(i);
            materialTypes_[name] = std::make_shared<MaterialType>(name, hwManager.createProgram(),
                (shaders[name].compute != nullptr));
        }

        return true;
    }

    void MaterialManager::shutdown()
    {
        LOG4CPLUS_DEBUG(logger(), "materialManager: shutdown...");
        matImmDefault_[0][0].reset();
        matImmDefault_[0][1].reset();
        matImmDefault_[1][0].reset();
        matImmDefault_[1][1].reset();
        matUnlitVCDefault_.reset();
        matOutlineInactive_.reset();
        matOutlineHovered_.reset();
        matOutlineSelected_.reset();
        matClusterCull_.reset();
        matPrepassWS_.reset();
        matPrepass_[0].reset();
        matPrepass_[1].reset();
        matShadowWS_.reset();
        matShadow_[0].reset();
        matShadow_[1].reset();

        runtime_assert(immediateMaterials_.empty());
        cachedMaterials_.clear();
        for (int i = MaterialTypeFirst; i <= MaterialTypeMax; ++i) {
            MaterialTypeName name = static_cast<MaterialTypeName>(i);
            materialTypes_[name].reset();
        }
    }

    void MaterialManager::reload()
    {
        LOG4CPLUS_DEBUG(logger(), "materialManager: reload...");
    }

    bool MaterialManager::renderReload(HardwareContext& ctx)
    {
        LOG4CPLUS_DEBUG(logger(), "materialManager: render reload...");

        for (const auto& mat : materialTypes_) {
            std::vector<HardwareShaderPtr> hwShaders;

            if (shaders[mat->name()].compute) {
                LOG4CPLUS_DEBUG(logger(), "materialManager: loading " << shaders[mat->name()].compute << "...");

                PlatformIFStream isCompute(shaders[mat->name()].compute);

                std::string computeSource;

                if (!readStream(isCompute, computeSource)) {
                    LOG4CPLUS_ERROR(logger(), "Unable to read \"" << shaders[mat->name()].compute << "\"");
                    return false;
                }

                if (shaders[mat->name()].header) {
                    computeSource = shaders[mat->name()].header + computeSource;
                }

                computeSource = glslCommonHeader_ + computeSource;

                auto computeShader = hwManager.createShader(HardwareShader::Type::Compute);

                if (!computeShader->compile(computeSource, ctx)) {
                    return false;
                }

                hwShaders.push_back(computeShader);
            } else {
                LOG4CPLUS_DEBUG(logger(), "materialManager: loading " << shaders[mat->name()].vert << "...");

                PlatformIFStream isVert(shaders[mat->name()].vert);

                std::string vertSource;

                if (!readStream(isVert, vertSource)) {
                    LOG4CPLUS_ERROR(logger(), "Unable to read \"" << shaders[mat->name()].vert << "\"");
                    return false;
                }

                std::string fragSource;

                if (shaders[mat->name()].frag) {
                    LOG4CPLUS_DEBUG(logger(), "materialManager: loading " << shaders[mat->name()].frag << "...");

                    PlatformIFStream isFrag(shaders[mat->name()].frag);

                    if (!readStream(isFrag, fragSource)) {
                        LOG4CPLUS_ERROR(logger(), "Unable to read \"" << shaders[mat->name()].frag << "\"");
                        return false;
                    }
                }

                if (shaders[mat->name()].header) {
                    vertSource = shaders[mat->name()].header + vertSource;
                    if (!fragSource.empty()) {
                        fragSource = shaders[mat->name()].header + fragSource;
                    }
                }

                vertSource = glslCommonHeader_ + vertSource;
                if (!fragSource.empty()) {
                    fragSource = glslCommonHeader_ + fragSource;
                }

                auto vertexShader = hwManager.createShader(HardwareShader::Type::Vertex);

                if (!vertexShader->compile(vertSource, ctx)) {
                    return false;
                }

                if (!fragSource.empty()) {
                    auto fragmentShader = hwManager.createShader(HardwareShader::Type::Fragment);

                    if (!fragmentShader->compile(fragSource, ctx)) {
                        return false;
                    }

                    hwShaders.push_back(fragmentShader);
                }

                hwShaders.push_back(vertexShader);
            }

            if (!mat->reload(hwShaders, ctx)) {
                return false;
            }

            mat->setDefaultUniform(UniformName::MainColor, gammaToLinear(Color_one));
            mat->setDefaultUniform(UniformName::SpecularColor, gammaToLinear(Color_zero));
            mat->setDefaultUniform(UniformName::Shininess, 1.0f);
            mat->setDefaultUniform(UniformName::EmissiveFactor, 1.0f);
        }

        if (!matUnlitVCDefault_) {
            matImmDefault_[0][0] = createMaterial(MaterialTypeImm);
            matImmDefault_[0][0]->setBlendingParams(BlendingParams(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            matImmDefault_[0][0]->setDepthTest(false);
            matImmDefault_[0][0]->setCullFaceMode(0);

            matImmDefault_[0][1] = createMaterial(MaterialTypeImm);
            matImmDefault_[0][1]->setBlendingParams(BlendingParams(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            matImmDefault_[0][1]->setDepthTest(false);

            matImmDefault_[1][0] = createMaterial(MaterialTypeImm);
            matImmDefault_[1][0]->setBlendingParams(BlendingParams(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            matImmDefault_[1][0]->setCullFaceMode(0);

            matImmDefault_[1][1] = createMaterial(MaterialTypeImm);
            matImmDefault_[1][1]->setBlendingParams(BlendingParams(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

            matUnlitVCDefault_ = createMaterial(MaterialTypeUnlitVC);

            matOutlineInactive_ = createMaterial(MaterialTypeOutline);
            matOutlineInactive_->params().setUniform(UniformName::MainColor, gammaToLinear(settings.editor.outlineColorInactive));
            matOutlineInactive_->setCullFaceMode(GL_FRONT);
            matOutlineInactive_->setBlendingParams(BlendingParams(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

            matOutlineHovered_ = createMaterial(MaterialTypeOutline);
            matOutlineHovered_->params().setUniform(UniformName::MainColor, gammaToLinear(settings.editor.outlineColorHovered));
            matOutlineHovered_->setCullFaceMode(GL_FRONT);
            matOutlineHovered_->setBlendingParams(BlendingParams(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

            matOutlineSelected_ = createMaterial(MaterialTypeOutline);
            matOutlineSelected_->params().setUniform(UniformName::MainColor, gammaToLinear(settings.editor.outlineColorSelected));
            matOutlineSelected_->setCullFaceMode(GL_FRONT);
            matOutlineSelected_->setBlendingParams(BlendingParams(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

            matClusterCull_ = createMaterial(MaterialTypeClusterCull);

            matPrepassWS_ = createMaterial(MaterialTypePrepassWS);
            matPrepass_[0] = createMaterial(MaterialTypePrepass1);
            matPrepass_[1] = createMaterial(MaterialTypePrepass2);

            matShadowWS_ = createMaterial(MaterialTypeShadowWS);
            matShadow_[0] = createMaterial(MaterialTypeShadow1);
            matShadow_[1] = createMaterial(MaterialTypeShadow2);
        }

        return true;
    }

    MaterialTypePtr MaterialManager::getMaterialType(MaterialTypeName name)
    {
        return materialTypes_[name];
    }

    MaterialPtr MaterialManager::getMaterial(const std::string& name)
    {
        auto it = cachedMaterials_.find(name);
        if (it == cachedMaterials_.end()) {
            return MaterialPtr();
        }
        return it->second;
    }

    MaterialPtr MaterialManager::loadImmMaterial(const TexturePtr& tex, const SamplerParams& params, bool depthTest)
    {
        runtime_assert(!tex->name().empty());

        std::string matName = "_bmatImm/" + tex->name() + "@" + params.toString() + (depthTest ? "-d" : "");

        auto it = cachedMaterials_.find(matName);
        if (it != cachedMaterials_.end()) {
            return it->second;
        }

        auto mat = createMaterial(MaterialTypeImm, matName);

        mat->setTextureBinding(SamplerName::Main, TextureBinding(tex, params));
        mat->setBlendingParams(BlendingParams(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        mat->setDepthTest(depthTest);

        return mat;
    }

    MaterialPtr MaterialManager::createMaterial(MaterialTypeName typeName, const std::string& name)
    {
        auto material = std::make_shared<Material>(this, name, getMaterialType(typeName));

        if (name.empty()) {
            immediateMaterials_.insert(material.get());
        } else {
            if (!cachedMaterials_.emplace(name, material).second) {
                LOG4CPLUS_ERROR(logger(), "Material " << name << " already exists");
                return MaterialPtr();
            }

            LOG4CPLUS_DEBUG(logger(), "materialManager: creating material (" << typeName << ") " << name << "...");
        }

        return material;
    }

    bool MaterialManager::onMaterialClone(const MaterialPtr& material)
    {
        if (material->name().empty()) {
            immediateMaterials_.insert(material.get());
            return true;
        }

        if (!cachedMaterials_.emplace(material->name(), material).second) {
            LOG4CPLUS_ERROR(logger(), "Material " << material->name() << " already exists");
            return false;
        }

        return true;
    }

    void MaterialManager::onMaterialDestroy(Material* material)
    {
        immediateMaterials_.erase(material);
    }
}
