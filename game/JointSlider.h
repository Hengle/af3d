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

#ifndef _JOINTSLIDER_H_
#define _JOINTSLIDER_H_

#include "Joint.h"

namespace af3d
{
    class JointSlider : public std::enable_shared_from_this<JointSlider>,
        public Joint
    {
    public:
        struct Config
        {
            float linearSoftness = SLIDER_CONSTRAINT_DEF_SOFTNESS;
            float linearRestitution = SLIDER_CONSTRAINT_DEF_RESTITUTION;
            float linearDamping = SLIDER_CONSTRAINT_DEF_DAMPING;
        };

        explicit JointSlider(const SceneObjectPtr& objectA, const SceneObjectPtr& objectB,
            bool collideConnected = false);
        ~JointSlider() = default;

        static const AClass& staticKlass();

        static AObjectPtr create(const APropertyValueMap& propVals);

        AObjectPtr sharedThis() override { return shared_from_this(); }

        btSliderConstraint* constraint() override { return constraint_; }

        void render(bool drawA, PhysicsDebugDraw& dd, const btVector3& c, float sz) override;

        inline const btTransform& frameA() const { return frameA_; }
        void setFrameA(const btTransform& value);

        inline const btTransform& frameB() const { return frameB_; }
        void setFrameB(const btTransform& value);

        inline float lowerLinearLimit() const { return lowerLinearLimit_; }
        void setLowerLinearLimit(float value);

        inline float upperLinearLimit() const { return upperLinearLimit_; }
        void setUpperLinearLimit(float value);

        inline const Config& dirConfig() const { return dirConfig_; }
        void setDirConfig(const Config& value);

        inline const Config& limConfig() const { return limConfig_; }
        void setLimConfig(const Config& value);

        inline const Config& orthoConfig() const { return orthoConfig_; }
        void setOrthoConfig(const Config& value);

        btTransform worldFrameA() const;
        void setWorldFrameA(const btTransform& value);

        btTransform worldFrameB() const;
        void setWorldFrameB(const btTransform& value);

        APropertyValue propertyLocalFrameAGet(const std::string&) const { return frameA(); }
        void propertyLocalFrameASet(const std::string&, const APropertyValue& value) { setFrameA(value.toTransform()); }

        APropertyValue propertyLocalFrameBGet(const std::string&) const { return frameB(); }
        void propertyLocalFrameBSet(const std::string&, const APropertyValue& value) { setFrameB(value.toTransform()); }

        APropertyValue propertyWorldFrameAGet(const std::string&) const { return worldFrameA(); }
        void propertyWorldFrameASet(const std::string&, const APropertyValue& value) { setWorldFrameA(value.toTransform()); }

        APropertyValue propertyWorldFrameBGet(const std::string&) const { return worldFrameB(); }
        void propertyWorldFrameBSet(const std::string&, const APropertyValue& value) { setWorldFrameB(value.toTransform()); }

        APropertyValue propertyLowerLinearLimitGet(const std::string&) const { return lowerLinearLimit(); }
        void propertyLowerLinearLimitSet(const std::string&, const APropertyValue& value) { setLowerLinearLimit(value.toFloat()); }

        APropertyValue propertyUpperLinearLimitGet(const std::string&) const { return upperLinearLimit(); }
        void propertyUpperLinearLimitSet(const std::string&, const APropertyValue& value) { setUpperLinearLimit(value.toFloat()); }

        APropertyValue propertyDirLinearSoftnessGet(const std::string&) const { return dirConfig_.linearSoftness; }
        void propertyDirLinearSoftnessSet(const std::string&, const APropertyValue& value)
        {
            auto cfg = dirConfig();
            cfg.linearSoftness = value.toFloat();
            setDirConfig(cfg);
        }

        APropertyValue propertyDirLinearRestitutionGet(const std::string&) const { return dirConfig_.linearRestitution; }
        void propertyDirLinearRestitutionSet(const std::string&, const APropertyValue& value)
        {
            auto cfg = dirConfig();
            cfg.linearRestitution = value.toFloat();
            setDirConfig(cfg);
        }

        APropertyValue propertyDirLinearDampingGet(const std::string&) const { return dirConfig_.linearDamping; }
        void propertyDirLinearDampingSet(const std::string&, const APropertyValue& value)
        {
            auto cfg = dirConfig();
            cfg.linearDamping = value.toFloat();
            setDirConfig(cfg);
        }

        APropertyValue propertyLimLinearSoftnessGet(const std::string&) const { return limConfig_.linearSoftness; }
        void propertyLimLinearSoftnessSet(const std::string&, const APropertyValue& value)
        {
            auto cfg = limConfig();
            cfg.linearSoftness = value.toFloat();
            setLimConfig(cfg);
        }

        APropertyValue propertyLimLinearRestitutionGet(const std::string&) const { return limConfig_.linearRestitution; }
        void propertyLimLinearRestitutionSet(const std::string&, const APropertyValue& value)
        {
            auto cfg = limConfig();
            cfg.linearRestitution = value.toFloat();
            setLimConfig(cfg);
        }

        APropertyValue propertyLimLinearDampingGet(const std::string&) const { return limConfig_.linearDamping; }
        void propertyLimLinearDampingSet(const std::string&, const APropertyValue& value)
        {
            auto cfg = limConfig();
            cfg.linearDamping = value.toFloat();
            setLimConfig(cfg);
        }

        APropertyValue propertyOrthoLinearSoftnessGet(const std::string&) const { return orthoConfig_.linearSoftness; }
        void propertyOrthoLinearSoftnessSet(const std::string&, const APropertyValue& value)
        {
            auto cfg = orthoConfig();
            cfg.linearSoftness = value.toFloat();
            setOrthoConfig(cfg);
        }

        APropertyValue propertyOrthoLinearRestitutionGet(const std::string&) const { return orthoConfig_.linearRestitution; }
        void propertyOrthoLinearRestitutionSet(const std::string&, const APropertyValue& value)
        {
            auto cfg = orthoConfig();
            cfg.linearRestitution = value.toFloat();
            setOrthoConfig(cfg);
        }

        APropertyValue propertyOrthoLinearDampingGet(const std::string&) const { return orthoConfig_.linearDamping; }
        void propertyOrthoLinearDampingSet(const std::string&, const APropertyValue& value)
        {
            auto cfg = orthoConfig();
            cfg.linearDamping = value.toFloat();
            setOrthoConfig(cfg);
        }

    private:
        void doRefresh(bool forceDelete) override;

        void doAdopt(bool withEdit) override;

        void doAbandon() override;

        static const btTransform& fixup();
        btTransform frameAWithFixup() const;
        btTransform frameBWithFixup() const;

        btTransform frameA_ = btTransform::getIdentity();
        btTransform frameB_ = btTransform::getIdentity();
        float lowerLinearLimit_ = -1.0f;
        float upperLinearLimit_ = 1.0f;

        Config dirConfig_;
        Config limConfig_;
        Config orthoConfig_;

        btSliderConstraint* constraint_ = nullptr;

        SceneObjectPtr editA_;
        SceneObjectPtr editB_;
    };

    using JointSliderPtr = std::shared_ptr<JointSlider>;

    ACLASS_DECLARE(JointSlider)
}

#endif