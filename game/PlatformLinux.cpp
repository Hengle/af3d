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

#include "PlatformLinux.h"
#include "Logger.h"
#include "af3d/Utils.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sys/stat.h>
#include <errno.h>

namespace af3d
{
    PlatformPtr platform(new PlatformLinux());

    bool PlatformLinux::init(const std::string& assetsPath)
    {
        initTimeUs();
        assetsPath_ = assetsPath;
        return true;
    }

    void PlatformLinux::shutdown()
    {
    }

    std::streambuf* PlatformLinux::openFile(const std::string& fileName)
    {
        std::filebuf* buf = new std::filebuf();

        auto fn = boost::replace_all_copy(fileName, "\\", "/");

        if (!buf->open((assetsPath_ + "/" + fn).c_str(), std::ios::in | std::ios::binary)) {
            delete buf;
            buf = nullptr;
        }

        return buf;
    }

    std::string PlatformLinux::readUserConfig() const
    {
        return readUserFile("user.ini");
    }

    bool PlatformLinux::writeUserConfig(const std::string& value)
    {
        return true;//writeUserFile("user.ini", value);
    }

    std::string PlatformLinux::readUserSaveData() const
    {
        return readUserFile("savedata");
    }

    bool PlatformLinux::writeUserSaveData(const std::string& value)
    {
        return writeUserFile("savedata", value);
    }

    std::string PlatformLinux::readUserFile(const std::string& fileName)
    {
        const char* p = getenv("HOME");

        if (!p) {
            LOG4CPLUS_WARN(logger(), "$HOME not defined, not reading " << fileName);
            return "";
        }

        std::string fp = /*std::string(p) + "/.af3d/" + */fileName;

        std::ifstream is(fp.c_str());

        if (!is) {
            LOG4CPLUS_WARN(logger(), fp << " not found");
            return "";
        }

        std::string str;

        str.assign(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());

        if (is.fail()) {
            LOG4CPLUS_WARN(logger(), "Error reading " << fp);
            return "";
        }

        return str;
    }

    bool PlatformLinux::writeUserFile(const std::string& fileName, const std::string& value)
    {
        const char* p = getenv("HOME");

        if (!p) {
            LOG4CPLUS_WARN(logger(), "$HOME not defined, not writing " << fileName);
            return false;
        }

        std::string d = std::string(p) + "/.af3d";

        /*if ((mkdir(d.c_str(), 0700) != 0) && (errno != EEXIST)) {
            LOG4CPLUS_WARN(logger(), "Cannot create " << d << ", not writing " << fileName);
            return false;
        }*/

        std::string fp = /*d + "/" + */fileName;

        mode_t oldMask = ::umask(~(S_IRUSR | S_IWUSR));

        try {
            std::ofstream os(fp.c_str(),
                std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);

            if (!os) {
                throw std::runtime_error("");
            }

            os.exceptions(std::ios::failbit | std::ios::badbit);

            os.write(value.c_str(), value.size());

            os.close();
        } catch (...) {
            LOG4CPLUS_WARN(logger(), "Error writing " << fp);

            ::umask(oldMask);

            return false;
        }

        ::umask(oldMask);

        return true;
    }
}
