/** @file
 * @brief ウィンドウマネージャ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_WINDOW_HPP
#define GRAPHENE_GRAPHICS_WINDOW_HPP

#include <any>
#include <functional>
#include <memory>
#include <string>

namespace Graphene::Graphics {

/**
 * @brief ウィンドウインタフェース
 *
 * ウィンドウの表示や状態の確認と更新を行うためのインタフェースです。
 */
class Window {
public:
    /**
     * @brief リサイズコールバック関数オブジェクト
     *
     * @param [in] width  リサイズ後のクライアント領域の横幅
     * @param [in] height リサイズ後のクライアント領域の縦幅
     * @return なし
     */
    using ResizeCallback = std::function<void(std::size_t, std::size_t)>;

    /**
     * @brief デストラクタ
     */
    virtual ~Window();

    /**
     * @brief ウィンドウハンドルの取得
     *
     * ウィンドウハンドルを取得します。 @n
     * ハンドル型は環境により異なります。
     *
     * @return ウィンドウハンドル
     */
    virtual std::any GetHandle(void) = 0;

    /**
     * @brief リサイズコールバック関数の設定
     *
     * リサイズコールバック関数を設定します。 @n
     * ウィンドウサイズが変更されたときに呼び出されます。
     *
     * @param [in] callback リサイズコールバック関数
     * @return なし
     */
    virtual void SetResizeCallback(const ResizeCallback& callback) = 0;

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
     * @brief イベントの処理
     *
     * 待機中のイベントを処理します。 @n
     * 定期的に呼び出す必要があります。
     *
     * @return なし
     */
    virtual void PollEvents(void) = 0;

    /**
     * @brief カーソルの表示
     *
     * カーソルを表示します。
     *
     * @return なし
     */
    virtual void ShowCursor(void) = 0;

    /**
     * @brief カーソルの非表示
     *
     * カーソルを非表示にします。
     *
     * @return なし
     */
    virtual void HideCursor(void) = 0;

    /**
     * @brief ウィンドウの表示
     *
     * ウィンドウを表示します。
     *
     * @return なし
     */
    virtual void ShowWindow(void) = 0;

    /**
     * @brief ウィンドウの非表示
     *
     * ウィンドウを非表示にします。
     *
     * @return なし
     */
    virtual void HideWindow(void) = 0;
};

/**
 * @brief Windowの固有ポインタ
 */
using UniqueWindow = std::unique_ptr<Window>;

/**
 * @brief ウィンドウビルダクラス
 *
 * ウィンドウオブジェクトを生成するための抽象クラスです。
 */
class WindowBuilder {
public:
    /**
     * @brief コンストラクタ
     */
    WindowBuilder() noexcept;

    /**
     * @brief デストラクタ
     */
    virtual ~WindowBuilder();

    /**
     * @brief ウィンドウタイトル名の設定
     *
     * ウィンドウタイトル名を設定します。
     *
     * @param [in] name ウィンドウタイトル名
     * @return 自己参照
     */
    virtual WindowBuilder& SetTitleName(std::string name) noexcept final;

    /**
     * @brief クライアント領域の横幅の設定
     *
     * クライアント領域の横幅を設定します。
     *
     * @param [in] width クライアント領域の横幅
     * @return 自己参照
     */
    virtual WindowBuilder& SetClientWidth(std::size_t width) noexcept final;

    /**
     * @brief クライアント領域の縦幅の設定
     *
     * クライアント領域の縦幅を設定します。
     *
     * @param [in] height クライアント領域の縦幅
     * @return 自己参照
     */
    virtual WindowBuilder& SetClientHeight(std::size_t height) noexcept final;

    /**
     * @brief ウィンドウ表示状態の設定
     *
     * ウィンドウ表示状態を設定します。
     *
     * @param [in] visible ウィンドウ表示状態
     * @return 自己参照
     */
    virtual WindowBuilder& SetVisible(bool visible) noexcept final;

    /**
     * @brief ウィンドウオブジェクトの生成
     *
     * ウィンドウオブジェクトを生成します。
     *
     * @return ウィンドウオブジェクト
     */
    virtual UniqueWindow Build(void) const = 0;

protected:
    std::string TitleName_;
    std::size_t ClientWidth_;
    std::size_t ClientHeight_;
    bool        Visible_;
};

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_WINDOW_HPP
