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
 * @brief ブレンドモード列挙型
 */
enum BlendMode {
    BlendModeNone,       ///< 上書き
    BlendModeAdd,        ///< 加算
    BlendModeAlpha,      ///< アルファ
    BlendModePreMulAlpha ///< 乗算済みアルファ
};

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
     * @brief ブレンドモードの設定
     *
     * ブレンドモードを設定します。
     *
     * @param [in] mode ブレンドモード
     * @return なし
     */
    virtual void SetBlendMode(BlendMode mode) = 0;

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
     * @brief ビューポートの設定
     *
     * ビューポートを設定します。 @n
     * x,y,width,heightは仮想スクリーン座標系で指定します。 @n
     * minDepth,maxDepthは[0.0f,1.0f]の範囲で指定します。
     *
     * @param [in] x        X座標
     * @param [in] y        Y座標
     * @param [in] width    横幅
     * @param [in] height   縦幅
     * @param [in] minDepth 最小深度
     * @param [in] maxDepth 最大深度
     * @return なし
     */
    virtual void SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) = 0;

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

    /**
     * @brief 動作確認用
     *
     * 動作確認用の一時的な実装です。 @n
     * インターフェースの仕様が決まったら削除します。
     *
     * @return なし
     */
    virtual void DebugDraw(void) = 0;
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
     * @brief 仮想スクリーン横幅の設定
     *
     * 仮想スクリーン横幅を設定します。 @n
     * 仮想スクリーン座標系は解像度が変化したときに自動でスクリーンサイズにフィットします。 @n
     * 0を指定した場合Build関数を呼び出した時点でのスクリーン横幅が使用されます。
     *
     * @param [in] width 仮想スクリーン横幅
     * @return 自己参照
     */
    virtual RendererBuilder& SetVirtualWidth(std::size_t width) noexcept final;

    /**
     * @brief 仮想スクリーン縦幅の設定
     *
     * 仮想スクリーン縦幅を設定します。 @n
     * 仮想スクリーン座標系は解像度が変化したときに自動でスクリーンサイズにフィットします。 @n
     * 0を指定した場合Build関数を呼び出した時点でのスクリーン縦幅が使用されます。
     *
     * @param [in] height 仮想スクリーン縦幅
     * @return 自己参照
     */
    virtual RendererBuilder& SetVirtualHeight(std::size_t height) noexcept final;

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

protected:
    std::size_t VirtualWidth_;
    std::size_t VirtualHeight_;
};

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_RENDERER_HPP
