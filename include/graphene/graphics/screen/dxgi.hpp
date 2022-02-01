/** @file
 * @brief スクリーンマネージャ(DXGI)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_SCREEN_DXGI_HPP
#define GRAPHENE_GRAPHICS_SCREEN_DXGI_HPP

#include <graphene/graphics/screen.hpp>

namespace Graphene::Graphics {

/**
 * @brief スクリーンビルダクラス(DXGI)
 *
 * スクリーンオブジェクトを生成するためのクラスです。
 */
class ScreenBuilderDXGI final : public ScreenBuilder {
public:
    /**
     * @brief スクリーンオブジェクトの生成
     *
     * スクリーンオブジェクトを生成します。
     *
     * @return スクリーンオブジェクト(DXGI)
     */
    virtual UniqueScreen Build(WindowBuilder& windowBuilder) const override;
};

} // Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_SCREEN_DXGI_HPP
