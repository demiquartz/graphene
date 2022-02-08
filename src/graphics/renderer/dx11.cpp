/** @file
 * @brief グラフィックスレンダラ(DX11)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene/graphics/renderer/dx11.hpp>
#include "../texture/dx11.hpp"
#include "../../config.hpp"
#include "../../resource.h"

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

    virtual SharedTexture GenerateTexture(SharedImage image) override {
        if (image) {
            try {
                return std::make_shared<TextureDX11>(image, Device_);
            }
            catch (std::exception&) {
#ifndef NDEBUG
                throw;
#endif
            }
        }
        return nullptr;
    }

    virtual void BindTexture(SharedTexture texture) override {
        auto textureDX11 = dynamic_cast<TextureDX11*>(texture.get());
        if (textureDX11) {
            DeviceContext_->PSSetShaderResources(0, 1, textureDX11->GetTexture().GetAddressOf());
            DeviceContext_->PSSetSamplers       (0, 1, textureDX11->GetSampler().GetAddressOf());
        }
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

    virtual void DebugDraw(void) override {
        struct Vertex {
            float pos[4];
            float tex[2];
        };
        Vertex vertexList[] {
            { { -1.0f,  1.0f, 0.5f, 1.0f }, { 0.0f, 0.0f } },
            { {  1.0f,  1.0f, 0.5f, 1.0f }, { 1.0f, 0.0f } },
            { { -1.0f, -1.0f, 0.5f, 1.0f }, { 0.0f, 1.0f } },
            { {  1.0f, -1.0f, 0.5f, 1.0f }, { 1.0f, 1.0f } }
        };

        static Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        if (!buffer) {
            D3D11_BUFFER_DESC bufferDesc;
            bufferDesc.ByteWidth           = sizeof(vertexList);
            bufferDesc.Usage               = D3D11_USAGE_DEFAULT;
            bufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.CPUAccessFlags      = 0;
            bufferDesc.MiscFlags           = 0;
            bufferDesc.StructureByteStride = 0;
            D3D11_SUBRESOURCE_DATA subResourceData;
            subResourceData.pSysMem          = vertexList;
            subResourceData.SysMemPitch      = 0;
            subResourceData.SysMemSlicePitch = 0;
            auto hr = Device_->CreateBuffer(&bufferDesc, &subResourceData, buffer.GetAddressOf());
            if (FAILED(hr)) return;
        }

        static Microsoft::WRL::ComPtr<ID3DBlob> vshader;
        if (!vshader) {
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
            if (FAILED(hr)) return;
#ifndef NDEBUG
            if (message) {
                std::cout << std::string(static_cast<const char*>(message->GetBufferPointer()), message->GetBufferSize()) << std::endl;
            }
#endif
        }

        static Microsoft::WRL::ComPtr<ID3DBlob> pshader;
        if (!pshader) {
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
            if (FAILED(hr)) return;
#ifndef NDEBUG
            if (message) {
                std::cout << std::string(static_cast<const char*>(message->GetBufferPointer()), message->GetBufferSize()) << std::endl;
            }
#endif
        }

        static Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
        if (!layout) {
            D3D11_INPUT_ELEMENT_DESC vertexDesc[] {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            };
            auto hr = Device_->CreateInputLayout(
                vertexDesc,
                ARRAYSIZE(vertexDesc),
                vshader->GetBufferPointer(),
                vshader->GetBufferSize(),
                layout.GetAddressOf()
            );
            if (FAILED(hr)) return;
        }

        static Microsoft::WRL::ComPtr<ID3D11VertexShader> vobject;
        if (!vobject) {
            auto hr = Device_->CreateVertexShader(vshader->GetBufferPointer(), vshader->GetBufferSize(), nullptr, vobject.GetAddressOf());
            if (FAILED(hr)) return;
        }

        static Microsoft::WRL::ComPtr<ID3D11PixelShader> pobject;
        if (!pobject) {
            auto hr = Device_->CreatePixelShader(pshader->GetBufferPointer(), pshader->GetBufferSize(), nullptr, pobject.GetAddressOf());
            if (FAILED(hr)) return;
        }

        UINT strides = sizeof(Vertex);
        UINT offsets = 0;
        DeviceContext_->IASetInputLayout(layout.Get());
        DeviceContext_->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &strides, &offsets);
        DeviceContext_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        DeviceContext_->VSSetShader(vobject.Get(), nullptr, 0);
        DeviceContext_->PSSetShader(pobject.Get(), nullptr, 0);
        DeviceContext_->Draw(4, 0);
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
