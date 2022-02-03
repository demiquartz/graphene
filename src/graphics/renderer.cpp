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

RendererBuilder::RendererBuilder() noexcept {
}

RendererBuilder::~RendererBuilder() {
}

} // namespace Graphene::Graphics
