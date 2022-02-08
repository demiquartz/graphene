/** @file
 * @brief テクスチャオブジェクト(DX11)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_TEXTURE_DX11_HPP
#define GRAPHENE_GRAPHICS_TEXTURE_DX11_HPP

#include <graphene/graphics/texture.hpp>
#include <graphene/graphics/image.hpp>

#include <wrl/client.h>
#include <d3d11.h>

namespace Graphene::Graphics {

/**
 * @brief テクスチャクラス(DX11)
 *
 * テクスチャ情報を格納するクラスです。
 */
class TextureDX11 final : public Texture {
public:
    /**
     * @brief コンストラクタ
     *
     * @param [in] image  イメージオブジェクト
     * @param [in] device デバイス(DX11)
     * @throw std::runtime_error オブジェクト生成失敗
     */
    TextureDX11(
        SharedImage                          image,
        Microsoft::WRL::ComPtr<ID3D11Device> device
    );

    /**
     * @brief テクスチャの取得
     *
     * テクスチャを取得します。
     *
     * @return テクスチャ(DX11)
     */
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
    GetTexture(void);

    /**
     * @brief サンプラの取得
     *
     * サンプラを取得します。
     *
     * @return サンプラ(DX11)
     */
    Microsoft::WRL::ComPtr<ID3D11SamplerState>
    GetSampler(void);

private:
    Microsoft::WRL::ComPtr<ID3D11Resource>           Texture_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureView_;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>       SamplerState_;
};

} // namespace Graphene::Graphics

#endif // GRAPHENE_GRAPHICS_TEXTURE_DX11_HPP
