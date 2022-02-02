/** @file
 * @brief グラフェンライブラリ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene.hpp>
#include "config.hpp"
#include "version.hpp"

#if USE_GLFW
#include "setup/glfw.hpp"
#endif

#ifndef NDEBUG
#include <iostream>
#endif

namespace Graphene {

void Initialize(void) {
#ifndef NDEBUG
    std::clog << SYSNAME << " " << REVISION << std::endl;
#endif
#if USE_GLFW
    Setup::InitializeGLFW();
#endif
}

const char* GetVersion(void) noexcept {
    return VERSION;
}

std::size_t GetMajorVersion(void) noexcept {
    return VERSION_MAJOR;
}

std::size_t GetMinorVersion(void) noexcept {
    return VERSION_MINOR;
}

std::size_t GetPatchVersion(void) noexcept {
    return VERSION_PATCH;
}

} // Graphene
