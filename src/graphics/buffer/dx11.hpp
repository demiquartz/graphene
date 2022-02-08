/** @file
 * @brief バッファオブジェクト(DX11)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_BUFFER_DX11_HPP
#define GRAPHENE_GRAPHICS_BUFFER_DX11_HPP

#include <graphene/graphics/types.hpp>
#include <graphene/graphics/buffer.hpp>

#include <wrl/client.h>
#include <d3d11.h>

namespace Graphene::Graphics {

/**
 * @brief バッファクラス(DX11)
 *
 * バッファ情報を格納するクラスです。
 */
class BufferDX11 final : public Buffer {
public:
    /**
     * @brief コンストラクタ
     *
     * @param [in] device デバイス(DX11)
     * @param [in] data   データ
     * @param [in] size   サイズ(bytes)
     * @param [in] mode   アクセスモード
     * @throw std::runtime_error オブジェクト生成失敗
     */
    BufferDX11(
        Microsoft::WRL::ComPtr<ID3D11Device> device,
        const void*                          data,
        std::size_t                          size,
        AccessMode                           mode
    );

    Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer_;
};

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_BUFFER_DX11_HPP
