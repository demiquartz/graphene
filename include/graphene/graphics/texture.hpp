/** @file
 * @brief テクスチャオブジェクト
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_TEXTURE_HPP
#define GRAPHENE_GRAPHICS_TEXTURE_HPP

#include <memory>

namespace Graphene::Graphics {

/**
 * @brief テクスチャインタフェース
 *
 * テクスチャ情報を格納するインタフェースです。
 */
class Texture {
public:
    /**
     * @brief デストラクタ
     */
    virtual ~Texture() {}
};

/**
 * @brief Textureの共有ポインタ
 */
using SharedTexture = std::shared_ptr<Texture>;

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_TEXTURE_HPP
