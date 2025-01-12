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

#ifndef _ASSET_MODEL_H_
#define _ASSET_MODEL_H_

#include "Asset.h"
#include "MaterialType.h"

namespace af3d
{
    class AssetModel : public std::enable_shared_from_this<AssetModel>,
        public Asset
    {
    public:
        AssetModel();
        ~AssetModel() = default;

        static const AClass& staticKlass();

        static AObjectPtr create(const APropertyValueMap& propVals);

        AObjectPtr sharedThis() override { return shared_from_this(); }

        inline MaterialTypeName materialTypeName() const { return matTypeName_; }
        inline void setMaterialTypeName(MaterialTypeName value) { matTypeName_ = value; }

        inline bool flipUV() const { return flipUV_; }
        inline void setFlipUV(bool value) { flipUV_ = value; }

        inline bool ignoreTransforms() const { return ignoreTransforms_; }
        inline void setIgnoreTransforms(bool value) { ignoreTransforms_ = value; }

    private:
        MaterialTypeName matTypeName_ = MaterialTypeBasic;
        bool flipUV_ = false;
        bool ignoreTransforms_ = true;
    };

    using AssetModelPtr = std::shared_ptr<AssetModel>;

    ACLASS_DECLARE(AssetModel)
}

#endif
