/** @file
 * @brief グラフィックスレンダラ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene/graphics/renderer.hpp>

namespace Graphene::Graphics {

Renderer::~Renderer() {
}

RendererBuilder::RendererBuilder() noexcept :
VirtualWidth_ (0),
VirtualHeight_(0) {
}

RendererBuilder::~RendererBuilder() {
}

RendererBuilder& RendererBuilder::SetVirtualWidth(std::size_t width) noexcept {
    VirtualWidth_ = width;
    return *this;
}

RendererBuilder& RendererBuilder::SetVirtualHeight(std::size_t height) noexcept {
    VirtualHeight_ = height;
    return *this;
}

} // namespace Graphene::Graphics
