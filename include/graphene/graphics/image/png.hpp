/** @file
 * @brief PNG画像データ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_IMAGE_PNG_HPP
#define GRAPHENE_GRAPHICS_IMAGE_PNG_HPP

#include <graphene/graphics/image.hpp>
#include <graphene/stream/stream.hpp>

namespace Graphene::Graphics {

/**
 * @brief PNG画像の読み込み
 *
 * PNG画像を読み込みます。
 *
 * @param [in] stream 入力ストリーム
 * @param [in] format ピクセルフォーマット
 * @return イメージオブジェクト
 * @throw std::exception オブジェクト生成失敗
 */
SharedImage LoadImagePNG(Stream::SharedStream stream, PixelFormat format);

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_IMAGE_PNG_HPP
