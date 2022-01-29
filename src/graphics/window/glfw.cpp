/** @file
 * @brief ウィンドウマネージャ(GLFW)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene/graphics/window/glfw.hpp>
#include "../../config.hpp"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Graphene::Graphics {

class WindowGLFW final : public Window {
public:
    WindowGLFW(
        const std::string& title,
        std::size_t        width,
        std::size_t        height,
        bool               visible
    ) {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RESIZABLE,                     GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, visible ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_CLIENT_API,                   GLFW_NO_API);
        Window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!Window_) {
            const char* description;
            glfwGetError(&description);
            throw std::runtime_error(description);
        }
    }

    virtual ~WindowGLFW() override {
        glfwDestroyWindow(Window_);
    }

    virtual std::any GetHandle(void) override {
#ifdef GLFW_EXPOSE_NATIVE_WIN32
        return glfwGetWin32Window(Window_);
#endif
#ifdef GLFW_EXPOSE_NATIVE_X11
        return glfwGetX11Window(Window_);
#endif
        return nullptr;
    }
    virtual bool ShouldClose(void) override {
        return glfwWindowShouldClose(Window_) != GLFW_FALSE;
    }

    virtual void CancelClose(void) override {
        glfwSetWindowShouldClose(Window_, GLFW_FALSE);
    }

    virtual void PollEvents(void) override {
        glfwPollEvents();
    }

    virtual void Show(void) override {
        glfwShowWindow(Window_);
    }

    virtual void Hide(void) override {
        glfwHideWindow(Window_);
    }

private:
    GLFWwindow* Window_;
};

UniqueWindow WindowBuilderGLFW::Build(void) const {
    return std::make_unique<WindowGLFW>(
        TitleName_,
        ClientWidth_,
        ClientHeight_,
        Visible_
    );
}

} // Graphene::Graphics
