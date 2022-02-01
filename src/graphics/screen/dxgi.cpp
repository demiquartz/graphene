/** @file
 * @brief スクリーンマネージャ(DXGI)
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene/graphics/screen/dxgi.hpp>
#include "../../config.hpp"

#include <wrl/client.h>
#include <dxgi1_6.h>
#include <d3d11.h>

#ifndef NDEBUG
#include <iostream>
#endif

namespace Graphene::Graphics {

class ScreenDXGI final : public Screen {
public:
    ScreenDXGI(
        UniqueWindow&& window
    ) : Window_(std::move(window)) {
        D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };
        D3D_FEATURE_LEVEL featureLevel;
        auto hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
#ifdef NDEBUG
            0,
#else
            D3D11_CREATE_DEVICE_DEBUG,
#endif
            featureLevels,
            sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL),
            D3D11_SDK_VERSION,
            device.GetAddressOf(),
            &featureLevel,
            context.GetAddressOf()
        );
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
        Microsoft::WRL::ComPtr<IDXGIFactory7> factory;
        hr = CreateDXGIFactory2(
            DXGI_CREATE_FACTORY_DEBUG,
            __uuidof(IDXGIFactory7),
            reinterpret_cast<void**>(factory.GetAddressOf())
        );
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
        DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
        swapchainDesc.Width              = 0;
        swapchainDesc.Height             = 0;
        swapchainDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapchainDesc.Stereo             = FALSE;
        swapchainDesc.SampleDesc.Count   = 1;
        swapchainDesc.SampleDesc.Quality = 0;
        swapchainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchainDesc.BufferCount        = 2;
        swapchainDesc.Scaling            = DXGI_SCALING_STRETCH;
        swapchainDesc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapchainDesc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapchainDesc.Flags              = 0;
        hr = factory->CreateSwapChainForHwnd(
            device.Get(),
            std::any_cast<HWND>(Window_->GetHandle()),
            &swapchainDesc,
            nullptr,
            nullptr,
            reinterpret_cast<IDXGISwapChain1**>(SwapChain_.GetAddressOf())
        );
        if (FAILED(hr)) {
            throw std::system_error(hr, std::system_category());
        }
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
        device->CreateRenderTargetView(backBuffer.Get(), nullptr, targetView.GetAddressOf());
        context->OMSetRenderTargets(1, targetView.GetAddressOf(), nullptr);
        Window_->SetResizeCallback([this](std::size_t width, std::size_t height) {
            BOOL fullscreen;
            if (SUCCEEDED(SwapChain_->GetFullscreenState(&fullscreen, nullptr))) {
                if (fullscreen) Window_->HideCursor();
                else            Window_->ShowCursor();
            }
            context->OMSetRenderTargets(0, nullptr, nullptr);
            targetView.Reset();
            SwapChain_->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
            Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
            SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
            device->CreateRenderTargetView(backBuffer.Get(), nullptr, targetView.GetAddressOf());
            context->OMSetRenderTargets(1, targetView.GetAddressOf(), nullptr);
        });
        Window_->ShowWindow();
    }

    virtual ~ScreenDXGI() override {
    }

    virtual bool ShouldClose(void) override {
        return Window_->ShouldClose();
    }

    virtual void CancelClose(void) override {
        Window_->CancelClose();
    }

    virtual void Present(void) override {
        static std::size_t frame;
        float color[4];
        {
            float h = frame++ % 360;
            if (h <  60) {
                color[0] = 1.0f;
                color[1] = h / 60.0f;
                color[2] = 0.0f;
                color[3] = 1.0f;
            } else
            if (h < 120) {
                color[0] = (120.0f - h) / 60.0f;
                color[1] = 1.0f;
                color[2] = 0.0f;
                color[3] = 1.0f;
            } else
            if (h < 180) {
                color[0] = 0.0f;
                color[1] = 1.0f;
                color[2] = (h - 120.0f) / 60.0f;
                color[3] = 1.0f;
            } else
            if (h < 240) {
                color[0] = 0.0f;
                color[1] = (240.0f - h) / 60.0f;
                color[2] = 1.0f;
                color[3] = 1.0f;
            } else
            if (h < 300) {
                color[0] = (h - 240.0f) / 60.0f;
                color[1] = 0.0f;
                color[2] = 1.0f;
                color[3] = 1.0f;
            } else {
                color[0] = 1.0f;
                color[1] = 0.0f;
                color[2] = (360.0f - h) / 60.0f;
                color[3] = 1.0f;

            }
        }
        context->ClearRenderTargetView(targetView.Get(), color);
        auto hr = SwapChain_->Present(1, 0);
        if (FAILED(hr)) {
#ifndef NDEBUG
            std::cout << std::hex << "HRESULT: " << hr << std::endl;
#endif
        }
        Window_->PollEvents();
    }

private:
    UniqueWindow                            Window_;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> SwapChain_;

    Microsoft::WRL::ComPtr<ID3D11Device>           device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>    context;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> targetView;
};

UniqueScreen ScreenBuilderDXGI::Build(WindowBuilder& windowBuilder) const {
    windowBuilder.SetVisible(false);
    return std::make_unique<ScreenDXGI>(
        windowBuilder.Build()
    );
}

} // Graphene::Graphics
