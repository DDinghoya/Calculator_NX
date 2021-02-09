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

#include <borealis/core/i18n.hpp>
#include <borealis/core/logger.hpp>
#include <borealis/platforms/glfw/glfw_platform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// glfw video and input code inspired from the glfw hybrid app by fincs
// https://github.com/fincs/hybrid_app

namespace brls
{

static void glfwErrorCallback(int errorCode, const char* description)
{
    Logger::error("glfw: error {}: {}", errorCode, description);
}

GLFWPlatform::GLFWPlatform()
{
    // Init glfw
    glfwSetErrorCallback(glfwErrorCallback);
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);

    if (!glfwInit())
    {
        Logger::error("glfw: failed to initialize");
        return;
    }

    // Misc
    glfwSetTime(0.0);

    // Platform impls
    this->fontLoader  = new GLFWFontLoader();
    this->audioPlayer = new NullAudioPlayer();
}

void GLFWPlatform::createWindow(std::string windowTitle, uint32_t windowWidth, uint32_t windowHeight)
{
    this->videoContext = new GLFWVideoContext(windowTitle, windowWidth, windowHeight);
    this->inputManager = new GLFWInputManager(this->videoContext->getGLFWWindow());
}

std::string GLFWPlatform::getName()
{
    return "GLFW";
}

bool GLFWPlatform::mainLoopIteration()
{
    bool isActive;
    do
    {
        isActive = !glfwGetWindowAttrib(this->videoContext->getGLFWWindow(), GLFW_ICONIFIED);

        if (isActive)
            glfwPollEvents();
        else
            glfwWaitEvents();
    } while (!isActive);

    return !glfwWindowShouldClose(this->videoContext->getGLFWWindow());
}

AudioPlayer* GLFWPlatform::getAudioPlayer()
{
    return this->audioPlayer;
}

VideoContext* GLFWPlatform::getVideoContext()
{
    return this->videoContext;
}

InputManager* GLFWPlatform::getInputManager()
{
    return this->inputManager;
}

FontLoader* GLFWPlatform::getFontLoader()
{
    return this->fontLoader;
}

ThemeVariant GLFWPlatform::getThemeVariant()
{
    char* themeEnv = getenv("BOREALIS_THEME");
    if (themeEnv != nullptr && !strcasecmp(themeEnv, "DARK"))
        return ThemeVariant::DARK;
    else
        return ThemeVariant::LIGHT;
}

std::string GLFWPlatform::getLocale()
{
    return LOCALE_DEFAULT;
}

GLFWPlatform::~GLFWPlatform()
{
    delete this->audioPlayer;
    delete this->videoContext;
    delete this->inputManager;
}

} // namespace brls
