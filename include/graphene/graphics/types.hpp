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
 * @brief ピクセルフォーマット列挙型
 */
enum PixelFormat {
    XXXX0000, ///< フォーマット未指定
    RGBA0000, ///< RGBAフォーマット(ビット深度未指定)
    ARGB0000, ///< ARGBフォーマット(ビット深度未指定)
    RGBAFP32, ///< 格納方式(FLOAT,LE): RR RR RR RR GG GG GG GG BB BB BB BB AA AA AA AA (hex)
    ARGBFP32, ///< 格納方式(FLOAT,LE): AA AA AA AA RR RR RR RR GG GG GG GG BB BB BB BB (hex)
    RGBAFP16, ///< 格納方式(FLOAT,LE): RR RR GG GG BB BB AA AA (hex)
    ARGBFP16, ///< 格納方式(FLOAT,LE): AA AA RR RR GG GG BB BB (hex)
    RGBAUN16, ///< 格納方式(UNORM,LE): RR RR GG GG BB BB AA AA (hex)
    ARGBUN16, ///< 格納方式(UNORM,LE): AA AA RR RR GG GG BB BB (hex)
    RGBA8888, ///< 格納方式(UNORM,LE): RR GG BB AA (hex)
    ARGB8888, ///< 格納方式(UNORM,LE): AA RR GG BB (hex)
    RGBA4444, ///< 格納方式(UNORM,LE): BBBBAAAA RRRRGGGG (bin)
    ARGB4444, ///< 格納方式(UNORM,LE): GGGGBBBB AAAARRRR (bin)
    RGBA5551, ///< 格納方式(UNORM,LE): GGBBBBBA RRRRRGGG (bin)
    ARGB1555, ///< 格納方式(UNORM,LE): GGGBBBBB ARRRRRGG (bin)
    RGBA5650, ///< 格納方式(UNORM,LE): GGGBBBBB RRRRRGGG (bin)
    ARGB0565  ///< 格納方式(UNORM,LE): GGGBBBBB RRRRRGGG (bin)
};

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_TYPES_HPP
