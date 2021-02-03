/*
    Borealis, a Nintendo Switch UI Library
    Copyright (C) 2021  natinusala

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <borealis/core/video.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace brls
{

// GLFW Video Context
class GLFWVideoContext : public VideoContext
{
  public:
    GLFWVideoContext(std::string windowTitle, uint32_t windowWidth, uint32_t windowHeight);
    ~GLFWVideoContext();

    NVGcontext* getNVGContext() override;

    void clear(NVGcolor color) override;
    void beginFrame() override;
    void endFrame() override;
    void resetState() override;

    GLFWwindow* getGLFWWindow();

  private:
    GLFWwindow* window     = nullptr;
    NVGcontext* nvgContext = nullptr;
};

} // namespace brls
