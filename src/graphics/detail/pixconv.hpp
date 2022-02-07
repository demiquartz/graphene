/** @file
 * @brief ピクセルフォーマット変換
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#ifndef GRAPHENE_GRAPHICS_DETAIL_PIXCONV_HPP
#define GRAPHENE_GRAPHICS_DETAIL_PIXCONV_HPP

#include <limits>
#include <half.hpp>

namespace Graphene::Graphics::Detail {

//==============================================================================
// 内部型
//==============================================================================
using half_float::half;
using _fp32 = float;
using _fp16 = half;
using _un16 = std::uint16_t;
using _un8  = std::uint8_t;

//==============================================================================
// 宣言部
//==============================================================================
struct rgba_fp32;
struct argb_fp32;
struct rgba_fp16;
struct argb_fp16;
struct rgba_un16;
struct argb_un16;
struct rgba_8888;
struct argb_8888;
struct rgba_4444;
struct argb_4444;
struct rgba_5551;
struct argb_1555;
struct rgba_5650;
struct argb_0565;

template<class T, class U>
struct xxxx_fpxx {
    operator rgba_fp32() const;
    operator argb_fp32() const;
    operator rgba_un16() const;
    operator argb_un16() const;
};
struct rgba_fp32 : public xxxx_fpxx<rgba_fp32, _fp32> {
    using base_type = rgba_fp32;
    _fp32 r, g, b, a;
    rgba_fp32(_fp32 r, _fp32 g, _fp32 b, _fp32 a) : r(r), g(g), b(b), a(a) {}
};
struct argb_fp32 : public xxxx_fpxx<argb_fp32, _fp32> {
    using base_type = argb_fp32;
    _fp32 a, r, g, b;
    argb_fp32(_fp32 a, _fp32 r, _fp32 g, _fp32 b) : a(a), r(r), g(g), b(b) {}
};
struct rgba_fp16 : public xxxx_fpxx<rgba_fp16, _fp16> {
    using base_type = rgba_fp32;
    _fp16 r, g, b, a;
    rgba_fp16& operator=(const rgba_fp32& pixel);
};
struct argb_fp16 : public xxxx_fpxx<argb_fp16, _fp16> {
    using base_type = argb_fp32;
    _fp16 a, r, g, b;
    argb_fp16& operator=(const argb_fp32& pixel);
};

template<class T>
struct xxxx_un16 {
    operator rgba_fp32() const;
    operator argb_fp32() const;
    operator rgba_un16() const;
    operator argb_un16() const;
};
struct rgba_un16 : public xxxx_un16<rgba_un16> {
    using base_type = rgba_un16;
    _un16 r, g, b, a;
    rgba_un16(_un16 r, _un16 g, _un16 b, _un16 a) : r(r), g(g), b(b), a(a) {}
};
struct argb_un16 : public xxxx_un16<argb_un16> {
    using base_type = argb_un16;
    _un16 a, r, g, b;
    argb_un16(_un16 a, _un16 r, _un16 g, _un16 b) : a(a), r(r), g(g), b(b) {}
};

template<class T>
struct xxxx_8888 {
    operator rgba_fp32() const;
    operator argb_fp32() const;
    operator rgba_un16() const;
    operator argb_un16() const;
};
struct rgba_8888 : public xxxx_8888<rgba_8888> {
    using base_type = rgba_un16;
    _un8 r, g, b, a;
    rgba_8888& operator=(const rgba_un16& pixel);
};
struct argb_8888 : public xxxx_8888<argb_8888> {
    using base_type = argb_un16;
    _un8 a, r, g, b;
    argb_8888& operator=(const argb_un16& pixel);
};

template<class T>
struct xxxx_4444 {
    operator rgba_fp32() const;
    operator argb_fp32() const;
    operator rgba_un16() const;
    operator argb_un16() const;
};
struct rgba_4444 : public xxxx_4444<rgba_4444> {
    using base_type = rgba_un16;
    std::uint16_t r : 4, g : 4, b : 4, a : 4;
    rgba_4444& operator=(const rgba_un16& pixel);
};
struct argb_4444 : public xxxx_4444<argb_4444> {
    using base_type = argb_un16;
    std::uint16_t a : 4, r : 4, g : 4, b : 4;
    argb_4444& operator=(const argb_un16& pixel);
};

template<class T>
struct xxxx_5551 {
    operator rgba_fp32() const;
    operator argb_fp32() const;
    operator rgba_un16() const;
    operator argb_un16() const;
};
struct rgba_5551 : public xxxx_5551<rgba_5551> {
    using base_type = rgba_un16;
    std::uint16_t r : 5, g : 5, b : 5, a : 1;
    rgba_5551& operator=(const rgba_un16& pixel);
};
struct argb_1555 : public xxxx_5551<argb_1555> {
    using base_type = argb_un16;
    std::uint16_t a : 1, r : 5, g : 5, b : 5;
    argb_1555& operator=(const argb_un16& pixel);
};

template<class T>
struct xxxx_5650 {
    operator rgba_fp32() const;
    operator argb_fp32() const;
    operator rgba_un16() const;
    operator argb_un16() const;
};
struct rgba_5650 : public xxxx_5650<rgba_5650> {
    using base_type = rgba_un16;
    std::uint16_t r : 5, g : 6, b : 5;
    rgba_5650& operator=(const rgba_un16& pixel);
};
struct argb_0565 : public xxxx_5650<argb_0565> {
    using base_type = argb_un16;
    std::uint16_t r : 5, g : 6, b : 5;
    argb_0565& operator=(const argb_un16& pixel);
};

//==============================================================================
// 実装部
//==============================================================================
template<class T, class U>
xxxx_fpxx<T, U>::operator rgba_fp32() const {
    return {
        reinterpret_cast<const T*>(this)->r,
        reinterpret_cast<const T*>(this)->g,
        reinterpret_cast<const T*>(this)->b,
        reinterpret_cast<const T*>(this)->a
    };
}
template<class T, class U>
xxxx_fpxx<T, U>::operator argb_fp32() const {
    return {
        reinterpret_cast<const T*>(this)->a,
        reinterpret_cast<const T*>(this)->r,
        reinterpret_cast<const T*>(this)->g,
        reinterpret_cast<const T*>(this)->b
    };
}
template<class T, class U>
xxxx_fpxx<T, U>::operator rgba_un16() const {
    return {
        static_cast<_un16>(std::clamp<U>(reinterpret_cast<const T*>(this)->r, 0, 1) * std::numeric_limits<_un16>::max()),
        static_cast<_un16>(std::clamp<U>(reinterpret_cast<const T*>(this)->g, 0, 1) * std::numeric_limits<_un16>::max()),
        static_cast<_un16>(std::clamp<U>(reinterpret_cast<const T*>(this)->b, 0, 1) * std::numeric_limits<_un16>::max()),
        static_cast<_un16>(std::clamp<U>(reinterpret_cast<const T*>(this)->a, 0, 1) * std::numeric_limits<_un16>::max())
    };
}
template<class T, class U>
xxxx_fpxx<T, U>::operator argb_un16() const {
    return {
        static_cast<_un16>(std::clamp<U>(reinterpret_cast<const T*>(this)->a, 0, 1) * std::numeric_limits<_un16>::max()),
        static_cast<_un16>(std::clamp<U>(reinterpret_cast<const T*>(this)->r, 0, 1) * std::numeric_limits<_un16>::max()),
        static_cast<_un16>(std::clamp<U>(reinterpret_cast<const T*>(this)->g, 0, 1) * std::numeric_limits<_un16>::max()),
        static_cast<_un16>(std::clamp<U>(reinterpret_cast<const T*>(this)->b, 0, 1) * std::numeric_limits<_un16>::max())
    };
}
rgba_fp16& rgba_fp16::operator=(const rgba_fp32& pixel) {
    r = pixel.r;
    g = pixel.g;
    b = pixel.b;
    a = pixel.a;
    return *this;
}
argb_fp16& argb_fp16::operator=(const argb_fp32& pixel) {
    a = pixel.a;
    r = pixel.r;
    g = pixel.g;
    b = pixel.b;
    return *this;
}

template<class T>
xxxx_un16<T>::operator rgba_fp32() const {
    return {
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / std::numeric_limits<_un16>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / std::numeric_limits<_un16>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / std::numeric_limits<_un16>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->a) / std::numeric_limits<_un16>::max()
    };
}
template<class T>
xxxx_un16<T>::operator argb_fp32() const {
    return {
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->a) / std::numeric_limits<_un16>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / std::numeric_limits<_un16>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / std::numeric_limits<_un16>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / std::numeric_limits<_un16>::max()
    };
}
template<class T>
xxxx_un16<T>::operator rgba_un16() const {
    return {
        reinterpret_cast<const T*>(this)->r,
        reinterpret_cast<const T*>(this)->g,
        reinterpret_cast<const T*>(this)->b,
        reinterpret_cast<const T*>(this)->a
    };
}
template<class T>
xxxx_un16<T>::operator argb_un16() const {
    return {
        reinterpret_cast<const T*>(this)->a,
        reinterpret_cast<const T*>(this)->r,
        reinterpret_cast<const T*>(this)->g,
        reinterpret_cast<const T*>(this)->b
    };
}

template<class T>
xxxx_8888<T>::operator rgba_fp32() const {
    return {
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / std::numeric_limits<_un8>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / std::numeric_limits<_un8>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / std::numeric_limits<_un8>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->a) / std::numeric_limits<_un8>::max()
    };
}
template<class T>
xxxx_8888<T>::operator argb_fp32() const {
    return {
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->a) / std::numeric_limits<_un8>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / std::numeric_limits<_un8>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / std::numeric_limits<_un8>::max(),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / std::numeric_limits<_un8>::max()
    };
}
template<class T>
xxxx_8888<T>::operator rgba_un16() const {
    return {
        static_cast<_un16>(reinterpret_cast<const T*>(this)->r * 0x101),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->g * 0x101),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->b * 0x101),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->a * 0x101)
    };
}
template<class T>
xxxx_8888<T>::operator argb_un16() const {
    return {
        static_cast<_un16>(reinterpret_cast<const T*>(this)->a * 0x101),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->r * 0x101),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->g * 0x101),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->b * 0x101)
    };
}
rgba_8888& rgba_8888::operator=(const rgba_un16& pixel) {
    r = pixel.r >> 8;
    g = pixel.g >> 8;
    b = pixel.b >> 8;
    a = pixel.a >> 8;
    return *this;
}
argb_8888& argb_8888::operator=(const argb_un16& pixel) {
    a = pixel.a >> 8;
    r = pixel.r >> 8;
    g = pixel.g >> 8;
    b = pixel.b >> 8;
    return *this;
}

template<class T>
xxxx_4444<T>::operator rgba_fp32() const {
    return {
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / 0xf,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / 0xf,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / 0xf,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->a) / 0xf
    };
}
template<class T>
xxxx_4444<T>::operator argb_fp32() const {
    return {
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->a) / 0xf,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / 0xf,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / 0xf,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / 0xf
    };
}
template<class T>
xxxx_4444<T>::operator rgba_un16() const {
    return {
        static_cast<_un16>(reinterpret_cast<const T*>(this)->r * 0x1111),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->g * 0x1111),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->b * 0x1111),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->a * 0x1111)
    };
}
template<class T>
xxxx_4444<T>::operator argb_un16() const {
    return {
        static_cast<_un16>(reinterpret_cast<const T*>(this)->a * 0x1111),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->r * 0x1111),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->g * 0x1111),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->b * 0x1111)
    };
}
rgba_4444& rgba_4444::operator=(const rgba_un16& pixel) {
    r = pixel.r >> 12;
    g = pixel.g >> 12;
    b = pixel.b >> 12;
    a = pixel.a >> 12;
    return *this;
}
argb_4444& argb_4444::operator=(const argb_un16& pixel) {
    a = pixel.a >> 12;
    r = pixel.r >> 12;
    g = pixel.g >> 12;
    b = pixel.b >> 12;
    return *this;
}

template<class T>
xxxx_5551<T>::operator rgba_fp32() const {
    return {
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / 0x1f,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / 0x1f,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / 0x1f,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->a)
    };
}
template<class T>
xxxx_5551<T>::operator argb_fp32() const {
    return {
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->a),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / 0x1f,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / 0x1f,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / 0x1f
    };
}
template<class T>
xxxx_5551<T>::operator rgba_un16() const {
    return {
        static_cast<_un16>(reinterpret_cast<const T*>(this)->r * 0x8421 >> 4),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->g * 0x8421 >> 4),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->b * 0x8421 >> 4),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->a * 0xffff)
    };
}
template<class T>
xxxx_5551<T>::operator argb_un16() const {
    return {
        static_cast<_un16>(reinterpret_cast<const T*>(this)->a * 0xffff),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->r * 0x8421 >> 4),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->g * 0x8421 >> 4),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->b * 0x8421 >> 4)
    };
}
rgba_5551& rgba_5551::operator=(const rgba_un16& pixel) {
    r = pixel.r >> 11;
    g = pixel.g >> 11;
    b = pixel.b >> 11;
    a = pixel.a >> 15;
    return *this;
}
argb_1555& argb_1555::operator=(const argb_un16& pixel) {
    a = pixel.a >> 15;
    r = pixel.r >> 11;
    g = pixel.g >> 11;
    b = pixel.b >> 11;
    return *this;
}

template<class T>
xxxx_5650<T>::operator rgba_fp32() const {
    return {
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / 0x1f,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / 0x3f,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / 0x1f,
        static_cast<_fp32>(1)
    };
}
template<class T>
xxxx_5650<T>::operator argb_fp32() const {
    return {
        static_cast<_fp32>(1),
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->r) / 0x1f,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->g) / 0x3f,
        static_cast<_fp32>(reinterpret_cast<const T*>(this)->b) / 0x1f
    };
}
template<class T>
xxxx_5650<T>::operator rgba_un16() const {
    return {
        static_cast<_un16>(reinterpret_cast<const T*>(this)->r * 0x8421 >> 4),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->g * 0x1041 >> 2),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->b * 0x8421 >> 4),
        std::numeric_limits<_un16>::max()
    };
}
template<class T>
xxxx_5650<T>::operator argb_un16() const {
    return {
        std::numeric_limits<_un16>::max(),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->r * 0x8421 >> 4),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->g * 0x1041 >> 2),
        static_cast<_un16>(reinterpret_cast<const T*>(this)->b * 0x8421 >> 4)
    };
}
rgba_5650& rgba_5650::operator=(const rgba_un16& pixel) {
    r = pixel.r >> 11;
    g = pixel.g >> 10;
    b = pixel.b >> 11;
    return *this;
}
argb_0565& argb_0565::operator=(const argb_un16& pixel) {
    r = pixel.r >> 11;
    g = pixel.g >> 10;
    b = pixel.b >> 11;
    return *this;
}

rgba_fp32 BurnAlpha(const rgba_fp32& pixel) {
    return {
        pixel.a * pixel.r,
        pixel.a * pixel.g,
        pixel.a * pixel.b,
        pixel.a
    };
}
argb_fp32 BurnAlpha(const argb_fp32& pixel) {
    return {
        pixel.a,
        pixel.a * pixel.r,
        pixel.a * pixel.g,
        pixel.a * pixel.b
    };
}
rgba_un16 BurnAlpha(const rgba_un16& pixel) {
    return {
        static_cast<_un16>(pixel.a * pixel.r / 0xffff),
        static_cast<_un16>(pixel.a * pixel.g / 0xffff),
        static_cast<_un16>(pixel.a * pixel.b / 0xffff),
        static_cast<_un16>(pixel.a)
    };
}
argb_un16 BurnAlpha(const argb_un16& pixel) {
    return {
        static_cast<_un16>(pixel.a),
        static_cast<_un16>(pixel.a * pixel.r / 0xffff),
        static_cast<_un16>(pixel.a * pixel.g / 0xffff),
        static_cast<_un16>(pixel.a * pixel.b / 0xffff)
    };
}

/**
 * @brief ピクセルフォーマットの変換
 *
 * ピクセルフォーマットを変換します。
 *
 * @param [out] dst 出力配列
 * @param [in]  src 入力配列
 * @param [in]  n   配列の長さ
 */
template<bool PMA, class T, class U>
void ConvertPixelFormat(T* dst, const U* src, std::size_t n) {
    if constexpr (PMA) {
        for (std::size_t i = 0; i < n; ++i) {
            *dst++ = BurnAlpha(static_cast<T::base_type>(*src++));
        }
    } else {
        for (std::size_t i = 0; i < n; ++i) {
            *dst++ = static_cast<T::base_type>(*src++);
        }
    }
}

/**
 * @brief 変換可能ピクセルフォーマットの取得
 *
 * 変換対象形式を元に実際に変換可能な形式を取得します。
 *
 * @param [in] target 変換対象型式
 * @param [in] source 変換元形式
 * @return 変換可能形式
 */
PixelFormat GetConvertibleFormat(PixelFormat target, PixelFormat source) {
    switch (target) {
    case XXXX0000:
        return source;
    case RGBA0000:
        switch (source) {
        case RGBAFP32:
        case ARGBFP32:
            return RGBAFP32;
        case RGBAFP16:
        case ARGBFP16:
            return RGBAFP16;
        case RGBAUN16:
        case ARGBUN16:
            return RGBAUN16;
        case RGBA8888:
        case ARGB8888:
            return RGBA8888;
        case RGBA4444:
        case ARGB4444:
            return RGBA4444;
        case RGBA5551:
        case ARGB1555:
            return RGBA5551;
        case RGBA5650:
        case ARGB0565:
            return RGBA5650;
        default:
            return source;
        }
    case ARGB0000:
        switch (source) {
        case RGBAFP32:
        case ARGBFP32:
            return ARGBFP32;
        case RGBAFP16:
        case ARGBFP16:
            return ARGBFP16;
        case RGBAUN16:
        case ARGBUN16:
            return ARGBUN16;
        case RGBA8888:
        case ARGB8888:
            return ARGB8888;
        case RGBA4444:
        case ARGB4444:
            return ARGB4444;
        case RGBA5551:
        case ARGB1555:
            return ARGB1555;
        case RGBA5650:
        case ARGB0565:
            return ARGB0565;
        default:
            return source;
        }
    default:
        return target;
    }
}

/**
 * @brief ピクセルフォーマットサイズの取得
 *
 * 指定したピクセルフォーマットを格納するのに必要な、
 * 1ピクセルあたりのバイト数を取得します。
 *
 * @param [in] format ピクセルフォーマット
 * @return 1ピクセルあたりのバイト数
 */
std::size_t GetBytesPerPixel(PixelFormat format) {
    switch (format) {
    case RGBAFP32:
    case ARGBFP32:
        return 16;
    case RGBAFP16:
    case ARGBFP16:
    case RGBAUN16:
    case ARGBUN16:
        return 8;
    case RGBA8888:
    case ARGB8888:
        return 4;
    case RGBA4444:
    case ARGB4444:
    case RGBA5551:
    case ARGB1555:
    case RGBA5650:
    case ARGB0565:
        return 2;
    default:
        return 0;
    }
}

} // namespace Graphene::Graphics::Detail

#endif // GRAPHENE_GRAPHICS_DETAIL_PIXCONV_HPP
