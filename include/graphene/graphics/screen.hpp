/** @file
 * @brief スクリーンマネージャ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_SCREEN_HPP
#define GRAPHENE_GRAPHICS_SCREEN_HPP

#include <graphene/graphics/window.hpp>

namespace Graphene::Graphics {

/**
 * @brief スクリーンインタフェース
 *
 * 画面更新を行うためのインタフェースです。
 */
class Screen {
public:
    /**
     * @brief デストラクタ
     */
    virtual ~Screen();

    /**
     * @brief 画面の終了確認
     *
     * 終了処理が必要か問い合わせます。 @n
     * trueの場合は終了処理を行ってください。
     *
     * @retval true  必要
     * @retval false 不要
     */
    virtual bool ShouldClose(void) = 0;

    /**
     * @brief 画面の終了取消
     *
     * 終了処理を取り消します。 @n
     * ShouldClose()がtrueを返したが、
     * 終了させたくない場合に呼び出してください。
     *
     * @return なし
     */
    virtual void CancelClose(void) = 0;

    /**
     * @brief 画面の更新
     *
     * 画面を更新します。
     *
     * @return なし
     */
    virtual void Present(void) = 0;
};

/**
 * @brief Screenの固有ポインタ
 */
using UniqueScreen = std::unique_ptr<Screen>;

/**
 * @brief スクリーンビルダクラス
 *
 * スクリーンオブジェクトを生成するための抽象クラスです。
 */
class ScreenBuilder {
public:
    /**
     * @brief コンストラクタ
     */
    ScreenBuilder() noexcept;

    /**
     * @brief デストラクタ
     */
    virtual ~ScreenBuilder();

    /**
     * @brief スクリーンオブジェクトの生成
     *
     * スクリーンオブジェクトを生成します。 @n
     * この関数はウィンドウビルダのパラメータを
     * 部分的に変更する場合があります。
     *
     * @param [in] windowBuilder ウィンドウビルダ
     * @return スクリーンオブジェクト
     */
    virtual UniqueScreen Build(WindowBuilder& windowBuilder) const = 0;
};

} // Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_SCREEN_HPP
