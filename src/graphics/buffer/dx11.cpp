/** @file
 * @brief バッファオブジェクト(DX11)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <cstring>
#include "dx11.hpp"

namespace Graphene::Graphics {

BufferDX11::BufferDX11(
    Microsoft::WRL::ComPtr<ID3D11Device> device,
    const void*                          data,
    std::size_t                          size,
    AccessMode                           mode
) {
    D3D11_BUFFER_DESC desc;
    desc.ByteWidth           = size;
    desc.Usage               = D3D11_USAGE_DYNAMIC;
    desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags           = 0;
    desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA subres;
    subres.pSysMem          = data;
    subres.SysMemPitch      = 0;
    subres.SysMemSlicePitch = 0;
    auto hr = device->CreateBuffer(&desc, data ? &subres : nullptr, Buffer_.GetAddressOf());
    if (FAILED(hr)) throw std::system_error(hr, std::system_category());
}

} // namespace Graphene::Graphics
