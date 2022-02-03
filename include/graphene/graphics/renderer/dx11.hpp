/** @file
 * @brief グラフィックスレンダラ(DX11)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_RENDERER_DX11_HPP
#define GRAPHENE_GRAPHICS_RENDERER_DX11_HPP

#include <graphene/graphics/renderer.hpp>

namespace Graphene::Graphics {

/**
 * @brief レンダラビルダクラス(DX11)
 *
 * レンダラオブジェクトを生成するための抽象クラスです。
 */
class RendererBuilderDX11 final : public RendererBuilder {
public:
    /**
     * @brief レンダラオブジェクトの生成
     *
     * レンダラオブジェクトを生成します。 @n
     * この関数はウィンドウビルダのパラメータを
     * 部分的に変更する場合があります。
     *
     * @param [in] windowBuilder ウィンドウビルダ
     * @return レンダラオブジェクト(DX11)
     */
    virtual SharedRenderer Build(WindowBuilder& windowBuilder) const override;
};

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_RENDERER_DX11_HPP
