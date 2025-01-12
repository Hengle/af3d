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

#include "RenderNode.h"
#include "TextureManager.h"
#include "Logger.h"

namespace af3d
{
    DrawBufferBinding::DrawBufferBinding(const AttachmentPoints& drawBuffers, const HardwareProgram::Outputs& outputs)
    {
        mask = 0;

        if (drawBuffers.empty()) {
            numBuffers = -1;
            return;
        }

        numBuffers = 0;
        int n = 0;
        for (int i = static_cast<int>(AttachmentPoint::Color0);
            (i <= static_cast<int>(AttachmentPoint::Max)) && (numBuffers < static_cast<int>(outputs.size())); ++i) {
            AttachmentPoint p = static_cast<AttachmentPoint>(i);
            if (drawBuffers[p]) {
                if (outputs.count(n) > 0) {
                    buffers[numBuffers++] = glAttachmentPoint(p);
                    mask |= (1 << n);
                }
                ++n;
            }
        }
    }

    RenderNode::RenderNode(const AABB2i& viewport, const AttachmentPoints& clearMask, const AttachmentColors& clearColors, const HardwareMRT& mrt)
    : type_(Type::Root),
      viewport_(viewport),
      clearMask_(clearMask),
      clearColors_(clearColors),
      mrt_(mrt),
      numDraws_(0)
    {
    }

    void RenderNode::add(RenderNode&& tmpNode, int pass, const DrawBufferBinding& drawBufferBinding,
        const MaterialTypePtr& matType,
        const MaterialParams& matParams,
        const BlendingParams& matBlendingParams,
        bool matDepthTest,
        bool matDepthWrite,
        GLenum matCullFaceMode,
        GLenum depthFunc, float depthValue, bool flipCull,
        std::vector<HardwareTextureBinding>&& textures, std::vector<StorageBufferBinding>&& storageBuffers,
        const VertexArraySlice& vaSlice, GLenum primitiveMode, const ScissorParams& scissorParams,
        MaterialParams&& materialParamsAuto)
    {
        btAssert(type_ == Type::Root);

        RenderNode* node = this;

        node = node->insertPass(std::move(tmpNode), pass);
        node = node->insertDepthTest(std::move(tmpNode), matDepthTest, depthFunc);
        node = node->insertDepth(std::move(tmpNode), depthValue);
        node = node->insertBlendingParams(std::move(tmpNode), matBlendingParams);

        if (flipCull) {
            if (matCullFaceMode == GL_FRONT) {
                matCullFaceMode = GL_BACK;
            } else if (matCullFaceMode == GL_BACK) {
                matCullFaceMode = GL_FRONT;
            }
        }

        node = node->insertCullFace(std::move(tmpNode), matCullFaceMode);
        node = node->insertMaterialType(std::move(tmpNode), matType);
        node = node->insertTextures(std::move(tmpNode), std::move(textures));
        node = node->insertVertexArray(std::move(tmpNode), vaSlice.va(), std::move(storageBuffers));
        node = node->insertDraw(std::move(tmpNode), numDraws_++);

        node->va_ = vaSlice.va();
        node->scissorParams_ = scissorParams;
        node->materialParams_ = matParams;
        node->materialParamsAuto_ = std::move(materialParamsAuto);
        node->draw_.bufferBinding = drawBufferBinding;
        node->draw_.primitiveMode = primitiveMode;
        node->draw_.start = vaSlice.start();
        node->draw_.count = vaSlice.count();
        node->draw_.baseVertex = vaSlice.baseVertex();
        node->draw_.depthWrite = matDepthWrite;
    }

    void RenderNode::add(RenderNode&& tmpNode, int pass, const MaterialPtr& material,
        const VertexArrayPtr& va,
        std::vector<StorageBufferBinding>&& storageBuffers,
        const Vector3i& computeNumGroups,
        MaterialParams&& materialParamsAuto)
    {
        btAssert(type_ == Type::Root);
        btAssert(material->type()->isCompute());

        RenderNode* node = this;

        node = node->insertPass(std::move(tmpNode), pass);
        node = node->insertDepthTest(std::move(tmpNode), false, 0);
        node = node->insertDepth(std::move(tmpNode), 0.0f);
        node = node->insertBlendingParams(std::move(tmpNode), BlendingParams());
        node = node->insertCullFace(std::move(tmpNode), 0);
        node = node->insertMaterialType(std::move(tmpNode), material->type());
        node = node->insertTextures(std::move(tmpNode), std::vector<HardwareTextureBinding>{});
        node = node->insertVertexArray(std::move(tmpNode), va, std::move(storageBuffers));
        node = node->insertDraw(std::move(tmpNode), numDraws_++);

        node->materialParams_ = material->params();
        node->materialParamsAuto_ = std::move(materialParamsAuto);
        node->computeNumGroups_ = computeNumGroups;
    }

    bool RenderNode::operator<(const RenderNode& other) const
    {
        switch (type_) {
        case Type::Pass: return comparePass(other);
        case Type::DepthTest: return compareDepthTest(other);
        case Type::Depth: return compareDepth(other);
        case Type::BlendingParams: return compareBlendingParams(other);
        case Type::CullFace: return compareCullFace(other);
        case Type::MaterialType: return compareMaterialType(other);
        case Type::Textures: return compareTextures(other);
        case Type::VertexArray: return compareVertexArray(other);
        case Type::Draw: return compareDraw(other);
        case Type::Root:
        default:
            runtime_assert(false);
        }
        return false;
    }

    void RenderNode::apply(HardwareContext& ctx) const
    {
        switch (type_) {
        case Type::Root:
            applyRoot(ctx);
            break;
        case Type::Pass:
            applyPass(ctx);
            break;
        case Type::DepthTest:
            applyDepthTest(ctx);
            break;
        case Type::Depth:
            applyDepth(ctx);
            break;
        case Type::BlendingParams:
            applyBlendingParams(ctx);
            break;
        case Type::CullFace:
            applyCullFace(ctx);
            break;
        case Type::MaterialType:
            applyMaterialType(ctx);
            break;
        case Type::Textures:
            applyTextures(ctx);
            break;
        case Type::VertexArray:
            applyVertexArray(ctx);
            break;
        case Type::Draw:
            applyDraw(ctx);
            break;
        default:
            runtime_assert(false);
        }
        for (const auto& node : children_) {
            node.apply(ctx);
        }
        if (type_ == Type::VertexArray) {
            applyVertexArrayDone(ctx);
        }
    }

    bool RenderNode::comparePass(const RenderNode& other) const
    {
        return pass_ < other.pass_;
    }

    bool RenderNode::compareDepthTest(const RenderNode& other) const
    {
        if (depthTest_ != other.depthTest_) {
            return (int)depthTest_ > (int)other.depthTest_;
        }
        return depthFunc_ < other.depthFunc_;
    }

    bool RenderNode::compareDepth(const RenderNode& other) const
    {
        return depth_ < other.depth_;
    }

    bool RenderNode::compareBlendingParams(const RenderNode& other) const
    {
        return blendingParams_ < other.blendingParams_;
    }

    bool RenderNode::compareCullFace(const RenderNode& other) const
    {
        return cullFaceMode_ < other.cullFaceMode_;
    }

    bool RenderNode::compareMaterialType(const RenderNode& other) const
    {
        return materialType_ < other.materialType_;
    }

    bool RenderNode::compareTextures(const RenderNode& other) const
    {
        return textures_ < other.textures_;
    }

    bool RenderNode::compareVertexArray(const RenderNode& other) const
    {
        if (va_ != other.va_) {
            return va_ < other.va_;
        } else {
            return storageBuffers_ < other.storageBuffers_;
        }
    }

    bool RenderNode::compareDraw(const RenderNode& other) const
    {
        return draw_.idx < other.draw_.idx;
    }

    RenderNode* RenderNode::insertPass(RenderNode&& tmpNode, int pass)
    {
        tmpNode.type_ = Type::Pass;
        tmpNode.pass_ = pass;
        return insertImpl(std::move(tmpNode));
    }

    RenderNode* RenderNode::insertDepthTest(RenderNode&& tmpNode, bool depthTest, GLenum depthFunc)
    {
        tmpNode.type_ = Type::DepthTest;
        tmpNode.depthTest_ = depthTest;
        tmpNode.depthFunc_ = depthFunc;
        return insertImpl(std::move(tmpNode));
    }

    RenderNode* RenderNode::insertDepth(RenderNode&& tmpNode, float depth)
    {
        tmpNode.type_ = Type::Depth;
        tmpNode.depth_ = depth;
        return insertImpl(std::move(tmpNode));
    }

    RenderNode* RenderNode::insertBlendingParams(RenderNode&& tmpNode, const BlendingParams& blendingParams)
    {
        tmpNode.type_ = Type::BlendingParams;
        tmpNode.blendingParams_ = blendingParams;
        return insertImpl(std::move(tmpNode));
    }

    RenderNode* RenderNode::insertCullFace(RenderNode&& tmpNode, GLenum cullFaceMode)
    {
        tmpNode.type_ = Type::CullFace;
        tmpNode.cullFaceMode_ = cullFaceMode;
        return insertImpl(std::move(tmpNode));
    }

    RenderNode* RenderNode::insertMaterialType(RenderNode&& tmpNode, const MaterialTypePtr& materialType)
    {
        tmpNode.type_ = Type::MaterialType;
        tmpNode.materialType_ = materialType;
        return insertImpl(std::move(tmpNode));
    }

    RenderNode* RenderNode::insertTextures(RenderNode&& tmpNode, std::vector<HardwareTextureBinding>&& textures)
    {
        tmpNode.type_ = Type::Textures;
        tmpNode.textures_ = std::move(textures);
        return insertImpl(std::move(tmpNode));
    }

    RenderNode* RenderNode::insertVertexArray(RenderNode&& tmpNode, const VertexArrayPtr& va, std::vector<StorageBufferBinding>&& storageBuffers)
    {
        tmpNode.type_ = Type::VertexArray;
        tmpNode.va_ = va;
        tmpNode.storageBuffers_ = std::move(storageBuffers);
        return insertImpl(std::move(tmpNode));
    }

    RenderNode* RenderNode::insertDraw(RenderNode&& tmpNode, int drawIdx)
    {
        tmpNode.type_ = Type::Draw;
        tmpNode.draw_.idx = drawIdx;
        return insertImpl(std::move(tmpNode));
    }

    RenderNode* RenderNode::insertImpl(RenderNode&& tmpNode)
    {
        auto res = children_.insert(std::move(tmpNode));
        return const_cast<RenderNode*>(&*res.first);
    }

    void RenderNode::applyRoot(HardwareContext& ctx) const
    {
        //LOG4CPLUS_DEBUG(logger(), "draw(" << numDraws_ << ")");
        bool haveFb = ctx.setMRT(mrt_);
        ogl.Viewport(viewport_.lowerBound[0], viewport_.lowerBound[1],
            viewport_.upperBound[0] - viewport_.lowerBound[0],
            viewport_.upperBound[1] - viewport_.lowerBound[1]);
        GLbitfield mask = 0;
        if (clearMask_[AttachmentPoint::Depth]) {
            mask |= GL_DEPTH_BUFFER_BIT;
        }
        if (clearMask_[AttachmentPoint::Stencil]) {
            mask |= GL_STENCIL_BUFFER_BIT;
        }
        if (mask) {
            ogl.Clear(mask);
        }
        for (int i = static_cast<int>(AttachmentPoint::Color0); i <= static_cast<int>(AttachmentPoint::Max); ++i) {
            AttachmentPoint p = static_cast<AttachmentPoint>(i);
            if (clearMask_[p]) {
                if (haveFb) {
                    GLenum buffer = glAttachmentPoint(p);
                    ogl.DrawBuffers(1, &buffer);
                } else {
                    btAssert(p == AttachmentPoint::Color0);
                }
                Color c = gammaToLinear(clearColors_[i]);
                ogl.ClearColor(c[0], c[1], c[2], c[3]);
                ogl.Clear(GL_COLOR_BUFFER_BIT);
            }
        }
    }

    void RenderNode::applyPass(HardwareContext& ctx) const
    {
    }

    void RenderNode::applyDepthTest(HardwareContext& ctx) const
    {
        if (depthTest_) {
            ogl.DepthFunc(depthFunc_);
            ogl.Enable(GL_DEPTH_TEST);
        } else {
            ogl.Disable(GL_DEPTH_TEST);
        }
    }

    void RenderNode::applyDepth(HardwareContext& ctx) const
    {
    }

    void RenderNode::applyBlendingParams(HardwareContext& ctx) const
    {
        if (blendingParams_.isEnabled()) {
            ogl.BlendFuncSeparate(blendingParams_.blendSfactor, blendingParams_.blendDfactor,
                blendingParams_.blendSfactorAlpha, blendingParams_.blendDfactorAlpha);
            ogl.Enable(GL_BLEND);
        } else {
            ogl.Disable(GL_BLEND);
        }
    }

    void RenderNode::applyCullFace(HardwareContext& ctx) const
    {
        if (cullFaceMode_) {
            ogl.CullFace(cullFaceMode_);
            ogl.Enable(GL_CULL_FACE);
        } else {
            ogl.Disable(GL_CULL_FACE);
        }
    }

    void RenderNode::applyMaterialType(HardwareContext& ctx) const
    {
        ogl.UseProgram(materialType_->prog()->id(ctx));
    }

    void RenderNode::applyTextures(HardwareContext& ctx) const
    {
        for (int i = 0; i < static_cast<int>(textures_.size()); ++i) {
            ctx.setActiveTextureUnit(i);
            GLuint id = textures_[i].tex ? textures_[i].tex->id(ctx) : 0;
            if (id == 0) {
                id = textureManager.white1x1()->hwTex()->id(ctx);
                ctx.bindSampler(i, SamplerParams(GL_NEAREST, GL_NEAREST));
                ctx.bindTexture(textureManager.white1x1()->type(), id);
            } else {
                ctx.bindSampler(i, textures_[i].params);
                ctx.bindTexture(textures_[i].tex->type(), id);
            }
        }
    }

    void RenderNode::applyVertexArray(HardwareContext& ctx) const
    {
        ogl.BindVertexArray(va_->vao(ctx)->id(ctx));
        for (const auto& bb : storageBuffers_) {
            ogl.BindBufferBase(GL_SHADER_STORAGE_BUFFER,
                HardwareProgram::getStorageBufferIndex(bb.first), bb.second->id(ctx));
        }
    }

    void RenderNode::applyVertexArrayDone(HardwareContext& ctx) const
    {
        ogl.BindVertexArray(0);
    }

    void RenderNode::applyDraw(HardwareContext& ctx) const
    {
        materialParamsAuto_.apply(ctx);
        materialParams_.apply(ctx);

        if (computeNumGroups_) {
            ogl.DispatchCompute(computeNumGroups_->x(), computeNumGroups_->y(), computeNumGroups_->z());
            ogl.MemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            return;
        }

        if (scissorParams_.enabled) {
            ogl.Scissor(scissorParams_.x, scissorParams_.y, scissorParams_.width, scissorParams_.height);
            ogl.Enable(GL_SCISSOR_TEST);
        }

        if (!draw_.depthWrite) {
            ogl.DepthMask(GL_FALSE);
        }

        if (draw_.bufferBinding.numBuffers >= 0) {
            ogl.DrawBuffers(draw_.bufferBinding.numBuffers, &draw_.bufferBinding.buffers[0]);
        }

        if (draw_.count == 0) {
            if (va_->ebo()) {
                ogl.DrawElements(draw_.primitiveMode, va_->ebo()->count(ctx),
                    va_->ebo()->glDataType(),
                    (const void*)(va_->ebo()->elementSize() * draw_.start));
            } else {
                // FIXME: Empty draw, optimize this out!
            }
        } else {
            if (va_->ebo()) {
                ogl.DrawElementsBaseVertex(draw_.primitiveMode, draw_.count,
                    va_->ebo()->glDataType(),
                    (void*)(va_->ebo()->elementSize() * draw_.start), draw_.baseVertex);
            } else {
                ogl.DrawArrays(draw_.primitiveMode, draw_.start, draw_.count);
            }
        }

        if (!draw_.depthWrite) {
            ogl.DepthMask(GL_TRUE);
        }

        if (scissorParams_.enabled) {
            ogl.Disable(GL_SCISSOR_TEST);
        }
    }
}
