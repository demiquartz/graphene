/** @file
 * @brief ウィンドウマネージャ(GLFW)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_WINDOW_GLFW_HPP
#define GRAPHENE_GRAPHICS_WINDOW_GLFW_HPP

#include <graphene/graphics/window.hpp>

namespace Graphene::Graphics {

/**
 * @brief ウィンドウビルダクラス(GLFW)
 *
 * ウィンドウオブジェクトを生成するためのクラスです。
 */
class WindowBuilderGLFW final : public WindowBuilder {
public:
    /**
     * @brief ウィンドウオブジェクトの生成
     *
     * ウィンドウオブジェクトを生成します。
     *
     * @return ウィンドウオブジェクト(GLFW)
     * @throw std::runtime_error オブジェクト生成失敗
     */
    virtual UniqueWindow Build(void) const override;
};

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_WINDOW_GLFW_HPP
