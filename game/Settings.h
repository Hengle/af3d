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

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "af3d/Single.h"
#include "af3d/AppConfig.h"
#include "Platform.h"
#include <set>

namespace af3d
{
    class Settings : public Single<Settings>
    {
    public:
        enum class AAMode
        {
            None = 0,
            FXAA = 1,
            TAA = 2
        };

        struct Physics
        {
            /*
             * Physics will step with this rate.
             */
            float fixedTimestep;

            /*
             * Maximum number of physics steps to avoid "spiral of death".
             */
            std::uint32_t maxSteps;
            float slowmoFactor;

            bool debugWireframe;
            bool debugAabb;
            bool debugContactPoints;
            bool debugNoDeactivation;
            bool debugJoints;
            bool debugJointLimits;
            bool debugNormals;
            bool debugFrames;
        };

        struct ImGui
        {
            bool drawCursor;
        };

        struct Editor
        {
            bool enabled;
            bool playing;
            bool disableSimulation;
            bool styledJson;
            float objMarkerSizeWorld;
            int objMarkerSizePixels;
            Color objMarkerColorInactive;
            Color objMarkerColorHovered;
            Color objMarkerColorSelected;
            Color outlineColorInactive;
            Color outlineColorHovered;
            Color outlineColorSelected;
            int lightMarkerSizePixels;
            float lightMarkerAlphaOff;
            float lightMarkerAlphaInactive;
            float lightMarkerAlphaHovered;
            float lightMarkerAlphaSelected;
            Color collisionColorOff;
            Color collisionColorInactive;
            Color collisionColorHovered;
            Color collisionColorSelected;
            float jointMarkerSizeWorld;
            int jointMarkerSizePixels;
            Color jointMarkerColorInactive;
            Color jointMarkerColorHovered;
            Color jointMarkerColorSelected;
            Color jointMarkerColorOff;
        };

        struct Cluster
        {
            Vector3i gridSize;
            Vector3i cullNumGroups;
            std::uint32_t numTiles;
            std::uint32_t maxLights;
            std::uint32_t maxLightsPerTile;
            std::uint32_t maxProbes;
            std::uint32_t maxProbesPerTile;
        };

        struct LightProbe
        {
            std::uint32_t irradianceResolution;
            std::uint32_t specularResolution;
            std::uint32_t specularMipLevels;
        };

        struct CSM
        {
            std::uint32_t maxCount;
            std::uint32_t numSplits;
            std::uint32_t resolution;
        };

        Settings() = default;
        ~Settings() = default;

        void init(const AppConfigPtr& appConfig);

        inline void setDeveloper(std::uint32_t value) { developer = value; }

#ifdef _WIN32
        std::string assets;
#endif
        std::uint32_t developer;
        std::uint32_t viewWidth;
        std::uint32_t viewHeight;
        float viewAspect;
        std::uint32_t profileReportTimeoutMs;
        std::uint32_t minRenderDt;
        std::uint32_t maxImmCameras;
        int videoMode;
        int msaaMode;
        bool vsync;
        bool fullscreen;
        bool trilinearFilter;
        AAMode aaMode;
        bool bloom;
        bool ssao;
        std::uint32_t viewX;
        std::uint32_t viewY;
        std::set<VideoMode> winVideoModes;
        Physics physics;
        ImGui imGui;
        Editor editor;
        Cluster cluster;
        LightProbe lightProbe;
        CSM csm;
    };

    extern Settings settings;
}

#endif
