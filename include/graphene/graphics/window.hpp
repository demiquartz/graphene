/** @file
 * @brief ウィンドウマネージャ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_WINDOW_HPP
#define GRAPHENE_GRAPHICS_WINDOW_HPP

#include <memory>

namespace Graphene::Graphics {

/**
 * @brief ウィンドウインタフェース
 *
 * ウィンドウの表示や状態の確認と更新を行うためのインタフェースです。
 */
class Window {
public:
    /**
     * @brief デストラクタ
     */
    virtual ~Window() {}

    /**
     * @brief ウィンドウの終了確認
     *
     * 終了処理が必要か問い合わせます。 @n
     * trueの場合は終了処理を行ってください。
     *
     * @retval true  必要
     * @retval false 不要
     */
    virtual bool ShouldClose(void) = 0;

    /**
     * @brief ウィンドウの終了取消
     *
     * 終了処理を取り消します。 @n
     * ShouldClose()がtrueを返したが、
     * 終了させたくない場合に呼び出してください。
     *
     * @return なし
     */
    virtual void CancelClose(void) = 0;

    /**
     * @brief フレームバッファの更新
     *
     * フレームバッファを更新します。
     *
     * @return なし
     */
    virtual void SwapBuffers(void) = 0;
};

/**
 * @brief Windowの共有ポインタ
 */
using SharedWindow = std::shared_ptr<Window>;

} // Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_WINDOW_HPP
