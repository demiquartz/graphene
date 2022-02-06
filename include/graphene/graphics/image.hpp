/** @file
 * @brief 画像データ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_IMAGE_HPP
#define GRAPHENE_GRAPHICS_IMAGE_HPP

#include <memory>
#include <graphene/graphics/types.hpp>

namespace Graphene::Graphics {

/**
 * @brief イメージインタフェース
 *
 * 画像データを扱うためのインタフェースです。
 */
class Image {
public:
    /**
     * @brief デストラクタ
     */
    virtual ~Image() {}

    /**
     * @brief データの取得
     */
    virtual const void* Data(void) const = 0;

    /**
     * @brief サイズの取得
     */
    virtual std::size_t Size(void) const = 0;

    /**
     * @brief 次元数の取得
     */
    virtual std::size_t Rank(void) const = 0;

    /**
     * @brief 各軸長の取得
     */
    virtual std::size_t Length(std::size_t axis) const = 0;

    /**
     * @brief 行間隔の取得
     */
    virtual std::size_t Stride(void) const = 0;

    /**
     * @brief フォーマットの取得
     */
    virtual PixelFormat Format(void) const = 0;
};

/**
 * @brief Imageの共有ポインタ
 */
using SharedImage = std::shared_ptr<Image>;

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_IMAGE_HPP
