/** @file
 * @brief データ入出力
 * @author Takaaki Sato
 * @copyright Copyright (c) 2020 Demiquartz &lt;info@demiquartz.jp&gt;@n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_STREAM_STREAM_HPP
#define GRAPHENE_STREAM_STREAM_HPP

#include <memory>
#include <string>

namespace Graphene::Stream {

/**
 * @brief ストリームインタフェース
 *
 * ファイルやアーカイブの読み書きなどを共通のインタフェースで行えるようにします。 @n
 * ネットワーク経由でのデータの送受信もこのインタフェースを使用します。
 */
class Stream {
public:
    /**
     * @brief デストラクタ
     */
    virtual ~Stream() {}

    /**
     * @brief ストリームを閉じる
     *
     * 開かれているストリームを閉じます。 @n
     * この関数を呼び出さなくてもオブジェクトの削除と同時に自動で閉じますが、
     * すぐにストリームの所有権を放棄したい場合などに呼び出してください。 @n
     * 閉じたあとのストリーム動作は未定義です。
     *
     * @return なし
     */
    virtual void Close(void) = 0;

    /**
     * @brief ストリームのデータを同期
     *
     * 現在開かれているストリームのデータを同期します。 @n
     * ストリームが開かれていない場合は失敗します。 @n
     * この関数が呼ばれるまで書き込みが完了しない場合があります。
     *
     * @retval true  成功
     * @retval false 失敗
     */
    virtual bool Flush(void) = 0;

    /**
     * @brief ストリームからデータを読み込む
     *
     * ストリームから指定したサイズ分データを読み込みます。 @n
     * ストリームが開かれていない場合は何もせずに0を返します。 @n
     * dataがnullptrの場合は何もせずに0を返します。
     *
     * @param [out] data 読み込むデータを格納するポインタ
     * @param [in]  size 読み込むサイズ(byte)
     * @return 実際に読み込まれたサイズ(byte)
     */
    virtual std::size_t Read(void* data, std::size_t size) = 0;

    /**
     * @brief ストリームにデータを書き込む
     *
     * ストリームから指定したサイズ分データを書き込みます。 @n
     * ストリームが開かれていない場合は何もせずに0を返します。 @n
     * dataがnullptrの場合は何もせずに0を返します。
     *
     * @param [in] data 書き込むデータが格納されたポインタ
     * @param [in] size 書き込むサイズ(byte)
     * @return 実際に書き込まれたサイズ(byte)
     */
    virtual std::size_t Write(const void* data, std::size_t size) = 0;

    /**
     * @brief ストリームポインタの位置を移動
     *
     * 現在開かれているストリームのポインタの位置を移動します。 @n
     * ストリームが開かれていない場合は失敗します。
     *
     * @param [in] offset ストリームの先頭からの位置(byte)
     * @retval true  成功
     * @retval false 失敗
     */
    virtual bool Seek(std::size_t offset) = 0;

    /**
     * @brief ストリームポインタの位置を取得
     *
     * 現在開かれているストリームのポインタの位置を取得します。 @n
     * ストリームが開かれていない場合は常に0を返します。
     *
     * @return ストリームの先頭からの位置(byte)
     */
    virtual std::size_t Tell(void) const = 0;

    /**
     * @brief ストリームサイズを取得
     *
     * 現在開かれているストリームのサイズを取得します。 @n
     * ストリームが開かれていない場合は常に0を返します。
     *
     * @return offset ストリームのサイズ(byte)
     */
    virtual std::size_t Size(void) const = 0;
};

/**
 * @brief Streamの共有ポインタ
 */
using SharedStream = std::shared_ptr<Stream>;

/**
 * @brief ストリームファクトリインタフェース
 *
 * ストリームインスタンスを生成するためのファクトリインタフェースです。
 */
class StreamFactory {
public:
    /**
     * @brief デストラクタ
     */
    virtual ~StreamFactory() {}

    /**
     * @brief ストリームを開く
     *
     * パスで指定したストリームを開きます。 @n
     * オープンモードは以下の6つの中からサポートしているモードを使用することを推奨します。
     * -# "r" @n
     *    読み込みモードで開きます。 @n
     *    ストリームが存在しない場合は失敗します。
     * -# "w" @n
     *    書き込みモードで開きます。 @n
     *    ストリームが存在しない場合は新規作成します。 @n
     *    ストリームが存在する場合は初期化されます。
     * -# "a" @n
     *    書き込みモードで開きます。 @n
     *    ストリームが存在しない場合は新規作成します。 @n
     *    ストリームが存在する場合は追記されます。
     * -# "r+" @n
     *    読み書きモードで開きます。 @n
     *    ストリームが存在しない場合は失敗します。
     * -# "w+" @n
     *    読み書きモードで開きます。 @n
     *    ストリームが存在しない場合は新規作成します。 @n
     *    ストリームが存在する場合は初期化されます。
     * -# "a+" @n
     *    読み書きモードで開きます。 @n
     *    ストリームが存在しない場合は新規作成します。 @n
     *    ストリームが存在する場合は追記されます。
     *
     * fopenの形式に合わせていますが、[テキスト|バイナリ]の区別はされません。 @n
     * バイナリモードのみをサポートし、"b"オプションは記述不要です。
     *
     * @param [in] path ストリーム識別文字列
     * @param [in] mode オープンモード
     * @return Streamの共有ポインタ
     */
    virtual SharedStream Open(const std::string& path, const std::string& mode) = 0;
};

/**
 * @brief StreamFactoryの共有ポインタ
 */
using SharedStreamFactory = std::shared_ptr<StreamFactory>;

} // namespace Graphene::Stream

#endif // GRAPHENE_STREAM_STREAM_HPP
