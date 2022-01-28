/** @file
 * @brief ウィンドウマネージャ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene/version.hpp>
#include <graphene/graphics/window.hpp>

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_REFRESH_RATE   60
#define DEFAULT_VISIBLE      true

namespace Graphene::Graphics {

Window::~Window() {
}

WindowBuilder::WindowBuilder() noexcept :
TitleName_   (SYSNAME              ),
ClientWidth_ (DEFAULT_WINDOW_WIDTH ),
ClientHeight_(DEFAULT_WINDOW_HEIGHT),
RefreshRate_ (DEFAULT_REFRESH_RATE ),
Visible_     (DEFAULT_VISIBLE      ) {
}

WindowBuilder::~WindowBuilder() {
}

WindowBuilder& WindowBuilder::SetTitleName(std::string name) noexcept {
    TitleName_ = name;
    return *this;
}

WindowBuilder& WindowBuilder::SetClientWidth(std::size_t width) noexcept {
    ClientWidth_ = width;
    return *this;
}

WindowBuilder& WindowBuilder::SetClientHeight(std::size_t height) noexcept {
    ClientHeight_ = height;
    return *this;
}

WindowBuilder& WindowBuilder::SetRefreshRate(std::size_t rate) noexcept {
    RefreshRate_ = rate;
    return *this;
}

WindowBuilder& WindowBuilder::SetVisible(bool visible) noexcept {
    Visible_ = visible;
    return *this;
}

} // Graphene::Graphics
