/** @file
 * @brief バッファオブジェクト
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_BUFFER_HPP
#define GRAPHENE_GRAPHICS_BUFFER_HPP

#include <memory>

namespace Graphene::Graphics {

/**
 * @brief バッファインタフェース
 *
 * バッファ情報を格納するインタフェースです。
 */
class Buffer {
public:
    /**
     * @brief デストラクタ
     */
    virtual ~Buffer() {}
};

/**
 * @brief Bufferの共有ポインタ
 */
using SharedBuffer = std::shared_ptr<Buffer>;

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_BUFFER_HPP
