/** @file
 * @brief セットアップ(GLFW)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <system_error>
#include <GLFW/glfw3.h>
#include "glfw.hpp"

namespace Graphene::Setup {

void InitializeGLFW(void) {
    if (glfwInit() == GLFW_FALSE) {
        const char* description;
        glfwGetError(&description);
        throw std::runtime_error(description);
    }
    if (std::atexit(glfwTerminate)) {
        throw std::system_error(errno, std::generic_category());
    }
}

} // Graphene::Setup
