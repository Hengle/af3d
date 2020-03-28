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

#include "ImGuiUtils.h"

namespace af3d { namespace ImGuiUtils
{
    struct InputTextCallback_UserData
    {
        std::string* Str;
        ImGuiInputTextCallback ChainCallback;
        void* ChainCallbackUserData;
    };

    APropertyEdit::APropertyEdit(const APropertyType& type)
    : type_(&type)
    {
    }

    bool APropertyEdit::update(APropertyValue& val, bool readOnly)
    {
        curVal_ = &val;
        curReadOnly_ = readOnly;
        curRet_ = false;
        type_->accept(*this);
        return curRet_;
    }

    void APropertyEdit::visitBool(const APropertyTypeBool& type)
    {
    }

    void APropertyEdit::visitInt(const APropertyTypeInt& type)
    {
    }

    void APropertyEdit::visitFloat(const APropertyTypeFloat& type)
    {
    }

    void APropertyEdit::visitString(const APropertyTypeString& type)
    {
        ImGuiInputTextFlags flags = 0;

        if (curReadOnly_) {
            flags |= ImGuiInputTextFlags_ReadOnly;
        }

        strVal_ = curVal_->toString();

        if (inputText("##str", strVal_, flags) && !curReadOnly_) {
            curRet_ = true;
            *curVal_ = APropertyValue(strVal_);
        }
    }

    void APropertyEdit::visitVec2f(const APropertyTypeVec2f& type)
    {
    }

    void APropertyEdit::visitVec3f(const APropertyTypeVec3f& type)
    {
    }

    void APropertyEdit::visitVec4f(const APropertyTypeVec4f& type)
    {
    }

    void APropertyEdit::visitColor(const APropertyTypeColor& type)
    {
    }

    void APropertyEdit::visitEnum(const APropertyTypeEnum& type)
    {
    }

    void APropertyEdit::visitObject(const APropertyTypeObject& type)
    {
    }

    void APropertyEdit::visitTransform(const APropertyTypeTransform& type)
    {
    }

    void APropertyEdit::visitArray(const APropertyTypeArray& type)
    {
    }

    static int inputTextCallback(ImGuiInputTextCallbackData* data)
    {
        InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
            // Resize string callback
            // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
            std::string* str = user_data->Str;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->c_str();
        } else if (user_data->ChainCallback) {
            // Forward to user callback, if any
            data->UserData = user_data->ChainCallbackUserData;
            return user_data->ChainCallback(data);
        }
        return 0;
    }

    bool inputText(const char* label, std::string& str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        btAssert((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = &str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputText(label, (char*)str.c_str(), str.capacity() + 1, flags, inputTextCallback, &cb_user_data);
    }

    bool inputTextMultiline(const char* label, std::string& str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        btAssert((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = &str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputTextMultiline(label, (char*)str.c_str(), str.capacity() + 1, size, flags, inputTextCallback, &cb_user_data);
    }

    bool inputTextWithHint(const char* label, const char* hint, std::string& str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
    {
        btAssert((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = &str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return ImGui::InputTextWithHint(label, hint, (char*)str.c_str(), str.capacity() + 1, flags, inputTextCallback, &cb_user_data);
    }
} }
