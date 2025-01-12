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

#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

#include "ResourceManager.h"
#include "Material.h"
#include "af3d/Single.h"
#include <unordered_map>
#include <unordered_set>

namespace af3d
{
    class MaterialManager : public ResourceManager,
                            public Single<MaterialManager>
    {
    public:
        MaterialManager() = default;
        ~MaterialManager();

        bool init() override;

        void shutdown() override;

        void reload() override;

        bool renderReload(HardwareContext& ctx) override;

        MaterialTypePtr getMaterialType(MaterialTypeName name);

        MaterialPtr getMaterial(const std::string& name);

        MaterialPtr loadImmMaterial(const TexturePtr& tex, const SamplerParams& params, bool depthTest);

        MaterialPtr createMaterial(MaterialTypeName typeName, const std::string& name = "");

        bool onMaterialClone(const MaterialPtr& material);

        void onMaterialDestroy(Material* material);

        inline const MaterialPtr& matImmDefault(bool depthTest, bool cullFace) const { return matImmDefault_[(int)depthTest][(int)cullFace]; }
        inline const MaterialPtr& matUnlitVCDefault() const { return matUnlitVCDefault_; }
        inline const MaterialPtr& matOutlineInactive() const { return matOutlineInactive_; }
        inline const MaterialPtr& matOutlineHovered() const { return matOutlineHovered_; }
        inline const MaterialPtr& matOutlineSelected() const { return matOutlineSelected_; }
        inline const MaterialPtr& matClusterCull() const { return matClusterCull_; }
        inline const MaterialPtr& matPrepassWS() const { return matPrepassWS_; }
        inline const MaterialPtr& matPrepass(int i) const { return matPrepass_[i]; }
        inline const MaterialPtr& matShadowWS() const { return matShadowWS_; }
        inline const MaterialPtr& matShadow(int i) const { return matShadow_[i]; }

    private:
        using MaterialTypes = std::array<MaterialTypePtr, MaterialTypeMax + 1>;
        using CachedMaterials = std::unordered_map<std::string, MaterialPtr>;
        using ImmediateMaterials = std::unordered_set<Material*>;

        std::string glslCommonHeader_;

        MaterialTypes materialTypes_;
        CachedMaterials cachedMaterials_;
        ImmediateMaterials immediateMaterials_;

        MaterialPtr matImmDefault_[2][2];
        MaterialPtr matUnlitVCDefault_;
        MaterialPtr matOutlineInactive_;
        MaterialPtr matOutlineHovered_;
        MaterialPtr matOutlineSelected_;
        MaterialPtr matClusterCull_;
        MaterialPtr matPrepassWS_;
        MaterialPtr matPrepass_[2];
        MaterialPtr matShadowWS_;
        MaterialPtr matShadow_[2];
    };

    extern MaterialManager materialManager;
}

#endif
