/** @file
 * @brief グラフィックスレンダラ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_RENDERER_HPP
#define GRAPHENE_GRAPHICS_RENDERER_HPP

#include <graphene/graphics/window.hpp>

namespace Graphene::Graphics {

/**
 * @brief レンダラインタフェース
 *
 * グラフィックスの描画管理を行うためのインタフェースです。
 */
class Renderer {
public:
    /**
     * @brief デストラクタ
     */
    virtual ~Renderer();

    /**
     * @brief 背景色の設定
     *
     * 背景色を[0.0f,1.0f]の範囲で設定します。
     *
     * @param [in] red   赤色
     * @param [in] greem 緑色
     * @param [in] blue  青色
     * @param [in] alpha 透明度
     * @return なし
     */
    virtual void SetClearColor(float red, float green, float blue, float alpha) = 0;

    /**
     * @brief レンダラの終了確認
     *
     * 終了処理が必要か問い合わせます。 @n
     * trueの場合は終了処理を行ってください。
     *
     * @retval true  必要
     * @retval false 不要
     */
    virtual bool ShouldQuit(void) = 0;

    /**
     * @brief レンダラの終了取消
     *
     * 終了処理を取り消します。 @n
     * ShouldQuit()がtrueを返したが、
     * 終了させたくない場合に呼び出してください。
     *
     * @return なし
     */
    virtual void CancelQuit(void) = 0;

    /**
     * @brief フレームバッファの更新
     *
     * フレームバッファを更新します。
     *
     * @return なし
     */
    virtual void Present(void) = 0;
};

/**
 * @brief Rendererの共有ポインタ
 */
using SharedRenderer = std::shared_ptr<Renderer>;

/**
 * @brief レンダラビルダクラス
 *
 * レンダラオブジェクトを生成するための抽象クラスです。
 */
class RendererBuilder {
public:
    /**
     * @brief コンストラクタ
     */
    RendererBuilder() noexcept;

    /**
     * @brief デストラクタ
     */
    virtual ~RendererBuilder();

    /**
     * @brief レンダラオブジェクトの生成
     *
     * レンダラオブジェクトを生成します。 @n
     * この関数はウィンドウビルダのパラメータを
     * 部分的に変更する場合があります。
     *
     * @param [in] windowBuilder ウィンドウビルダ
     * @return レンダラオブジェクト
     */
    virtual SharedRenderer Build(WindowBuilder& windowBuilder) const = 0;
};

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_RENDERER_HPP
