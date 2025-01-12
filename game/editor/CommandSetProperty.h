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

#ifndef _EDITOR_COMMAND_SETPROPERTY_H_
#define _EDITOR_COMMAND_SETPROPERTY_H_

#include "editor/Command.h"
#include "AObject.h"
#include <unordered_set>

namespace af3d { namespace editor
{
    class CommandSetProperty : public Command
    {
    public:
        CommandSetProperty(Scene* scene,
            const AObjectPtr& obj,
            const std::string& propName, const APropertyValue& propValue);
        ~CommandSetProperty() = default;

        bool redo() override;

        bool undo() override;

    private:
        ACommandPtr setValue(const AObjectPtr& obj, APropertyValue& value);

        bool fixForParam(APropertyValue& value);

        static bool reachableViaParams(const AWeakObject& from, const AWeakObject& to,
            std::unordered_set<ACookie>& visitedObjs);

        static bool reachableViaParams(const APropertyValue& value, const AWeakObject& to,
            std::unordered_set<ACookie>& visitedObjs);

        AWeakObject wobj_;
        std::string name_;
        APropertyValue prevValue_;
        APropertyValue value_;
        ACommandPtr cmd_;
        const bool isParam_;
        bool first_ = true;
    };
} }

#endif
