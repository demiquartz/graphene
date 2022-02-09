/** @file
 * @brief グラフィックスレンダラ(DX11)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene/graphics/renderer/dx11.hpp>
#include "../buffer/dx11.hpp"
#include "../texture/dx11.hpp"
#include "../../config.hpp"
#include "../../resource.h"

#include <cstring>
#include <chrono>
#include <thread>
#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#ifndef NDEBUG
#include <iostream>
#endif

RESOURCE_EXTERN(graphics_shader_hlsl_test_vsh);
RESOURCE_EXTERN(graphics_shader_hlsl_test_psh);

namespace Graphene::Graphics {

class RendererDX11 final : public Renderer {
public:
    RendererDX11(
        UniqueWindow&& window,
        std::size_t    width,
        std::size_t    height
    ) :
    Window_(std::move(window)),
    VirtualW_(width  ? width  : Window_->GetClientWidth ()),
    VirtualH_(height ? height : Window_->GetClientHeight()),
    ClearColorR_(0.0f),
    ClearColorG_(0.0f),
    ClearColorB_(0.0f),
    ClearColorA_(1.0f),
    ViewportX_(0.0f),
    ViewportY_(0.0f),
    ViewportW_(VirtualW_),
    ViewportH_(VirtualH_),
    ViewportN_(0.0f),
    ViewportF_(1.0f) {
        InitializeDX11();
        InitializeDXGI();
        InitializeShader();
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
        Device_->CreateRenderTargetView(backBuffer.Get(), nullptr, RenderTargetView_.GetAddressOf());
        DeviceContext_->OMSetRenderTargets(1, RenderTargetView_.GetAddressOf(), nullptr);
        Window_->SetResizeCallback([this](std::size_t width, std::size_t height) {
            BOOL fullscreen;
            if (SUCCEEDED(SwapChain_->GetFullscreenState(&fullscreen, nullptr))) {
                if (fullscreen) Window_->HideCursor();
                else            Window_->ShowCursor();
            }
            DeviceContext_->OMSetRenderTargets(0, nullptr, nullptr);
            RenderTargetView_.Reset();
            SwapChain_->ResizeBuffers(2, width, height, DXGI_FORMAT_UNKNOWN, 0);
            Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
            SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
            Device_->CreateRenderTargetView(backBuffer.Get(), nullptr, RenderTargetView_.GetAddressOf());
            DeviceContext_->OMSetRenderTargets(1, RenderTargetView_.GetAddressOf(), nullptr);
            SetViewport(width, height);
        });
        SetViewport(Window_->GetClientWidth(), Window_->GetClientHeight());
        Window_->ShowWindow();
    }

    virtual ~RendererDX11() override {
    }

    virtual void SetBlendMode(BlendMode mode) override {
        D3D11_BLEND_DESC desc;
        desc.AlphaToCoverageEnable                 = FALSE;
        desc.IndependentBlendEnable                = FALSE;
        desc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        switch (mode) {
        case BlendModeAdd:
            desc.RenderTarget[0].BlendEnable = TRUE;
            desc.RenderTarget[0].SrcBlend    = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlend   = D3D11_BLEND_ONE;
            break;
        case BlendModeAlpha:
            desc.RenderTarget[0].BlendEnable = TRUE;
            desc.RenderTarget[0].SrcBlend    = D3D11_BLEND_SRC_ALPHA;
            desc.RenderTarget[0].DestBlend   = D3D11_BLEND_INV_SRC_ALPHA;
            break;
        case BlendModePreMulAlpha:
            desc.RenderTarget[0].BlendEnable = TRUE;
            desc.RenderTarget[0].SrcBlend    = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlend   = D3D11_BLEND_INV_SRC_ALPHA;
            break;
        default:
            desc.RenderTarget[0].BlendEnable = FALSE;
            desc.RenderTarget[0].SrcBlend    = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlend   = D3D11_BLEND_ZERO;
            break;
        }
        auto hr = Device_->CreateBlendState(&desc, BlendState_.GetAddressOf());
        if (SUCCEEDED(hr)) DeviceContext_->OMSetBlendState(BlendState_.Get(), {}, ~0);
    }

    virtual void SetClearColor(float red, float green, float blue, float alpha) override {
        ClearColorR_ = red;
        ClearColorG_ = green;
        ClearColorB_ = blue;
        ClearColorA_ = alpha;
    }

    virtual void SetViewport(float x, float y, float width, float height, float minDepth, float maxDepth) override {
        ViewportX_ = x;
        ViewportY_ = y;
        ViewportW_ = width;
        ViewportH_ = height;
        ViewportN_ = minDepth;
        ViewportF_ = maxDepth;
        SetViewport(Window_->GetClientWidth(), Window_->GetClientHeight());
    }

    virtual void SetPrimitiveTopology(PrimitiveTopology topology) override {
        D3D11_PRIMITIVE_TOPOLOGY pt;
        switch (topology) {
        case PrimitiveTopologyPointList:
            pt = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
            break;
        case PrimitiveTopologyLineList:
            pt = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            break;
        case PrimitiveTopologyLineStrip:
            pt = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
            break;
        case PrimitiveTopologyTriangleList:
            pt = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            break;
        case PrimitiveTopologyTriangleStrip:
            pt = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
            break;
        case PrimitiveTopologyLineListWithAdjacency:
            pt = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
            break;
        case PrimitiveTopologyLineStripWithAdjacency:
            pt = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
            break;
        case PrimitiveTopologyTriangleListWithAdjacency:
            pt = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ;
            break;
        case PrimitiveTopologyTriangleStripWithAdjacency:
            pt = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ;
            break;
        default:
            pt = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
            break;
        }
        DeviceContext_->IASetPrimitiveTopology(pt);
    }

    virtual SharedBuffer Allocate(const void* data, std::size_t size, AccessMode mode) override {
        try {
            return std::make_shared<BufferDX11>(Device_, data, size, mode);
        }
        catch (std::exception&) {
#ifndef NDEBUG
            throw;
#endif
        }
        return nullptr;
    }

    virtual SharedTexture Allocate(const SharedImage image, AccessMode mode) override {
        if (image) {
            try {
                return std::make_shared<TextureDX11>(Device_, image, mode);
            }
            catch (std::exception&) {
#ifndef NDEBUG
                throw;
#endif
            }
        }
        return nullptr;
    }

    virtual void Bind(SharedBuffer buffer, std::size_t stride) override {
        auto bufferDX11 = dynamic_cast<BufferDX11*>(buffer.get());
        if (bufferDX11) {
            UINT strideDX11 = stride;
            UINT offsetDX11 = 0;
            DeviceContext_->IASetVertexBuffers(0, 1, bufferDX11->Buffer_.GetAddressOf(), &strideDX11, &offsetDX11);
        }
    }

    virtual void Bind(SharedTexture texture) override {
        auto textureDX11 = dynamic_cast<TextureDX11*>(texture.get());
        if (textureDX11) {
            DeviceContext_->PSSetShaderResources(0, 1, textureDX11->GetTexture().GetAddressOf());
            DeviceContext_->PSSetSamplers       (0, 1, textureDX11->GetSampler().GetAddressOf());
        }
    }

    virtual bool Read(SharedBuffer buffer, std::size_t offset, void* data, std::size_t size) const override {
        throw std::logic_error("Unimplemented");
    }

    virtual bool Write(SharedBuffer buffer, std::size_t offset, const void* data, std::size_t size) override {
        auto bufferDX11 = dynamic_cast<BufferDX11*>(buffer.get());
        if (!bufferDX11 || !data || !size) {
            return false;
        }
        auto res = bufferDX11->Buffer_.Get();
        D3D11_MAPPED_SUBRESOURCE subres;
        if (FAILED(DeviceContext_->Map(res, 0, D3D11_MAP_WRITE_DISCARD, 0, &subres))) {
            return false;
        }
        std::memcpy(static_cast<std::byte*>(subres.pData) + offset, data, size);
        DeviceContext_->Unmap(res, 0);
        return true;
    }

    virtual bool Read(SharedBuffer texture, SharedImage image) const override {
        throw std::logic_error("Unimplemented");
    }

    virtual bool Write(SharedBuffer buffer, const SharedImage image) override {
        throw std::logic_error("Unimplemented");
    }

    virtual void Draw(std::size_t count, std::size_t start) override {
        DeviceContext_->Draw(count, start);
    }

    virtual bool ShouldQuit(void) override {
        return Window_->ShouldClose();
    }

    virtual void CancelQuit(void) override {
        Window_->CancelClose();
    }

    virtual void Present(void) override {
        auto hr = SwapChain_->Present(1, Active_ ? 0 : DXGI_PRESENT_TEST);
        Active_ = hr != DXGI_STATUS_OCCLUDED;
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
        Window_->PollEvents();
        if (Active_) {
            float clearColor[4] = { ClearColorR_, ClearColorG_, ClearColorB_, ClearColorA_ };
            DeviceContext_->ClearRenderTargetView(RenderTargetView_.Get(), clearColor);
        } else {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(250ms);
        }
    }

private:
    void InitializeDX11(void) {
        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };
#ifndef NDEBUG
        D3D_FEATURE_LEVEL featureLevel;
#endif
        auto hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
#ifndef NDEBUG
            D3D11_CREATE_DEVICE_DEBUG,
#else
            0,
#endif
            featureLevels,
            sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL),
            D3D11_SDK_VERSION,
            Device_.GetAddressOf(),
#ifndef NDEBUG
            &featureLevel,
#else
            nullptr,
#endif
            DeviceContext_.GetAddressOf()
        );
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
#ifndef NDEBUG
        std::clog << "FeatureLevel: " << featureLevel << std::endl;
#endif
    }

    void InitializeDXGI(void) {
        Microsoft::WRL::ComPtr<IDXGIFactory7> factory;
        auto hr = CreateDXGIFactory2(
#ifndef NDEBUG
            DXGI_CREATE_FACTORY_DEBUG,
#else
            0,
#endif
            IID_PPV_ARGS(factory.GetAddressOf())
        );
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
        DXGI_SWAP_CHAIN_DESC1 desc;
        desc.Width              = 0;
        desc.Height             = 0;
        desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo             = FALSE;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount        = 1;
        desc.Scaling            = DXGI_SCALING_STRETCH;
        desc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;
        desc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
        desc.Flags              = 0;
        hr = factory->CreateSwapChainForHwnd(
            Device_.Get(),
            std::any_cast<HWND>(Window_->GetHandle()),
            &desc,
            nullptr,
            nullptr,
            reinterpret_cast<IDXGISwapChain1**>(SwapChain_.GetAddressOf())
        );
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
    }

    // TODO: 暫定的に固定パイプラインにするが今後
    // シェーダオブジェクトで管理できるように変更予定
    void InitializeShader(void) {
        Microsoft::WRL::ComPtr<ID3DBlob> vshader;
        {
            Microsoft::WRL::ComPtr<ID3DBlob> message;
            auto hr = D3DCompile(
                RESOURCE_DATA(graphics_shader_hlsl_test_vsh),
                RESOURCE_SIZE(graphics_shader_hlsl_test_vsh),
                "test.vsh",
                nullptr,
                nullptr,
                "vs_main",
                "vs_5_0",
                D3DCOMPILE_OPTIMIZATION_LEVEL3,
                0,
                vshader.GetAddressOf(),
                message.GetAddressOf()
            );
#ifndef NDEBUG
            if (message) {
                std::cout << std::string(static_cast<const char*>(message->GetBufferPointer()), message->GetBufferSize()) << std::endl;
            }
#endif
            if (FAILED(hr)) {
                throw std::system_error(hr, std::system_category());
            }
        }

        Microsoft::WRL::ComPtr<ID3DBlob> pshader;
        {
            Microsoft::WRL::ComPtr<ID3DBlob> message;
            auto hr = D3DCompile(
                RESOURCE_DATA(graphics_shader_hlsl_test_psh),
                RESOURCE_SIZE(graphics_shader_hlsl_test_psh),
                "test.psh",
                nullptr,
                nullptr,
                "ps_main",
                "ps_5_0",
                D3DCOMPILE_OPTIMIZATION_LEVEL3,
                0,
                pshader.GetAddressOf(),
                message.GetAddressOf()
            );
#ifndef NDEBUG
            if (message) {
                std::cout << std::string(static_cast<const char*>(message->GetBufferPointer()), message->GetBufferSize()) << std::endl;
            }
#endif
            if (FAILED(hr)) {
                throw std::system_error(hr, std::system_category());
            }
        }

        Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
        {
            D3D11_INPUT_ELEMENT_DESC vertexDesc[] {
                { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR",    0, DXGI_FORMAT_R32_FLOAT,       0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };
            auto hr = Device_->CreateInputLayout(
                vertexDesc,
                ARRAYSIZE(vertexDesc),
                vshader->GetBufferPointer(),
                vshader->GetBufferSize(),
                layout.GetAddressOf()
            );
            if (FAILED(hr)) {
                throw std::system_error(hr, std::system_category());
            }
        }

        Microsoft::WRL::ComPtr<ID3D11VertexShader> vobject;
        {
            auto hr = Device_->CreateVertexShader(vshader->GetBufferPointer(), vshader->GetBufferSize(), nullptr, vobject.GetAddressOf());
            if (FAILED(hr)) {
                throw std::system_error(hr, std::system_category());
            }
        }

        Microsoft::WRL::ComPtr<ID3D11PixelShader> pobject;
        {
            auto hr = Device_->CreatePixelShader(pshader->GetBufferPointer(), pshader->GetBufferSize(), nullptr, pobject.GetAddressOf());
            if (FAILED(hr)) {
                throw std::system_error(hr, std::system_category());
            }
        }

        DeviceContext_->IASetInputLayout(layout.Get());
        DeviceContext_->VSSetShader(vobject.Get(), nullptr, 0);
        DeviceContext_->PSSetShader(pobject.Get(), nullptr, 0);
    }

    void SetViewport(float width, float height) {
        D3D11_VIEWPORT viewport;
        auto xScale       = width  / VirtualW_;
        auto yScale       = height / VirtualH_;
        viewport.TopLeftX = ViewportX_ * xScale;
        viewport.TopLeftY = ViewportY_ * yScale;
        viewport.Width    = ViewportW_ * xScale;
        viewport.Height   = ViewportH_ * yScale;
        viewport.MinDepth = ViewportN_;
        viewport.MaxDepth = ViewportF_;
        DeviceContext_->RSSetViewports(1, &viewport);
    }

    UniqueWindow                                   Window_;
    Microsoft::WRL::ComPtr<IDXGISwapChain4>        SwapChain_;
    Microsoft::WRL::ComPtr<ID3D11Device>           Device_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    DeviceContext_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView_;
    Microsoft::WRL::ComPtr<ID3D11BlendState>       BlendState_;
    float                                          VirtualW_;
    float                                          VirtualH_;
    float                                          ClearColorR_;
    float                                          ClearColorG_;
    float                                          ClearColorB_;
    float                                          ClearColorA_;
    float                                          ViewportX_;
    float                                          ViewportY_;
    float                                          ViewportW_;
    float                                          ViewportH_;
    float                                          ViewportN_;
    float                                          ViewportF_;
    bool                                           Active_;
};

SharedRenderer RendererBuilderDX11::Build(WindowBuilder& windowBuilder) const {
    windowBuilder.SetVisible(false);
    return std::make_unique<RendererDX11>(
        windowBuilder.Build(),
        VirtualWidth_,
        VirtualHeight_
    );
}

} // namespace Graphene::Graphics
