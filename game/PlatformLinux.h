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

#ifndef _PLATFORMLINUX_H_
#define _PLATFORMLINUX_H_

#include "Platform.h"

namespace af3d
{
    class PlatformLinux : public Platform
    {
    public:
        PlatformLinux() = default;
        ~PlatformLinux() = default;

        bool init(const std::string& assetsPath);

        void shutdown();

        virtual const std::string& assetsPath() const override { return assetsPath_; }

        virtual std::streambuf* openFile(const std::string& fileName) override;

        virtual bool changeVideoMode(bool fullscreen, int videoMode, int msaaMode, bool vsync, bool trilinearFilter) override;

        virtual std::string readUserConfig() const override;
        virtual bool writeUserConfig(const std::string& value) override;

        virtual std::string readUserSaveData() const override;
        virtual bool writeUserSaveData(const std::string& value) override;

    private:
        std::string assetsPath_;

        static std::string readUserFile(const std::string& fileName);

        static bool writeUserFile(const std::string& fileName, const std::string& value);
    };

    #define platformLinux dynamic_cast<af3d::PlatformLinux*>(af3d::platform.get())
}

#endif
