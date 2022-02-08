/** @file
 * @brief グラフィックス型定義
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_TYPES_HPP
#define GRAPHENE_GRAPHICS_TYPES_HPP

namespace Graphene::Graphics {

/**
 * @brief アクセスモード列挙型
 */
enum AccessMode {
    AccessModeDefault  = 0,      ///< 読み書き不可
    AccessModeWritable = 1 << 0, ///< 書き込み可能
    AccessModeReadable = 1 << 1, ///< 読み込み可能
    AccessModeDynamic  = 1 << 2  ///< 高頻度更新
};

/**
 * @brief ピクセルフォーマット列挙型
 */
enum PixelFormat {
    XXXX0000, ///< フォーマット未指定
    RGBA0000, ///< RGBAフォーマット(ビット深度未指定)
    BGRA0000, ///< BGRAフォーマット(ビット深度未指定)
    RGBAFP32, ///< 格納方式(FLOAT,LE): RR RR RR RR GG GG GG GG BB BB BB BB AA AA AA AA (hex)
    BGRAFP32, ///< 格納方式(FLOAT,LE): BB BB BB BB GG GG GG GG RR RR RR RR AA AA AA AA (hex)
    RGBAFP16, ///< 格納方式(FLOAT,LE): RR RR GG GG BB BB AA AA (hex)
    BGRAFP16, ///< 格納方式(FLOAT,LE): BB BB GG GG RR RR AA AA (hex)
    RGBAUN16, ///< 格納方式(UNORM,LE): RR RR GG GG BB BB AA AA (hex)
    BGRAUN16, ///< 格納方式(UNORM,LE): BB BB GG GG RR RR AA AA (hex)
    RGBA8888, ///< 格納方式(UNORM,LE): RR GG BB AA (hex)
    BGRA8888, ///< 格納方式(UNORM,LE): BB GG RR AA (hex)
    RGBA4444, ///< 格納方式(UNORM,LE): BBBBAAAA RRRRGGGG (bin)
    BGRA4444, ///< 格納方式(UNORM,LE): RRRRAAAA BBBBGGGG (bin)
    RGBA5551, ///< 格納方式(UNORM,LE): GGBBBBBA RRRRRGGG (bin)
    BGRA5551, ///< 格納方式(UNORM,LE): GGRRRRRA BBBBBGGG (bin)
    RGBA5650, ///< 格納方式(UNORM,LE): GGGBBBBB RRRRRGGG (bin)
    BGRA5650  ///< 格納方式(UNORM,LE): GGGRRRRR BBBBBGGG (bin)
};

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_TYPES_HPP
