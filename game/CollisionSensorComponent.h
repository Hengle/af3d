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

#ifndef _COLLISIONSENSORCOMPONENT_H_
#define _COLLISIONSENSORCOMPONENT_H_

#include "CollisionComponent.h"
#include "SensorListener.h"
#include "SceneObject.h"
#include <unordered_map>

namespace af3d
{
    class CollisionSensorComponent : public std::enable_shared_from_this<CollisionSensorComponent>,
         public CollisionComponent
    {
    public:
        CollisionSensorComponent();
        ~CollisionSensorComponent() = default;

        static const AClass& staticKlass();

        static AObjectPtr create(const APropertyValueMap& propVals);

        AObjectPtr sharedThis() override { return shared_from_this(); }

        void updateContact(const Contact& contact, bool isNew) override;

        void endContact(const Contact& contact) override;

        inline const SensorListenerPtr& listener() const { return listener_; }
        inline void setListener(const SensorListenerPtr& value)
        {
            if (value) {
                value->incUseCount();
            }
            if (listener_) {
                listener_->decUseCount();
            }
            listener_ = value;
        }

        inline bool allowSensor() const { return allowSensor_; }
        inline void setAllowSensor(bool value) { allowSensor_ = value; }

    private:
        void onRegister() override;

        void onUnregister() override;

        std::unordered_map<SceneObjectPtr, int> counts_;
        std::unordered_map<std::uint64_t, SceneObjectPtr> contacts_;

        SensorListenerPtr listener_;

        bool allowSensor_ = false;
    };

    using CollisionSensorComponentPtr = std::shared_ptr<CollisionSensorComponent>;

    ACLASS_DECLARE(CollisionSensorComponent)
}

#endif
