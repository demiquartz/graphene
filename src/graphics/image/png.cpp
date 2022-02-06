/** @file
 * @brief PNG画像データ
 * @author Takaaki Sato
 * @copyright (c) 2022 Demiquartz &lt;info@demiquartz.jp&gt; @n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene/graphics/image/png.hpp>
#include <vector>
#include <png.h>
#include "../detail/pixconv.hpp"

namespace Graphene::Graphics {

class ImagePNG final : public Image {
public:
    ImagePNG(Stream::SharedStream stream, PixelFormat format) {
        auto et = ErrorTypeRuntime;
        auto em = "Failed to create png read structure.";
        auto ip = static_cast<png_infop>(nullptr);
        auto rp = png_create_read_struct(
            PNG_LIBPNG_VER_STRING, &em,
            [](png_structp rp, png_const_charp em) {
                *static_cast<png_const_charpp>(png_get_error_ptr(rp)) = em;
                png_longjmp(rp, 1);
            },
#ifdef NDEBUG
            [](png_structp rp, png_const_charp em) {
            }
#else
            nullptr
#endif
        );
        if (!rp || setjmp(png_jmpbuf(rp))) {
            auto rpp = rp ? &rp : nullptr;
            auto ipp = ip ? &ip : nullptr;
            png_destroy_read_struct(rpp, ipp, nullptr);
            auto message = std::string("LoadImagePNG: ") + em;
            if (et == ErrorTypeRuntime) throw std::runtime_error(message);
            else                        throw std::logic_error  (message);
        }
        if (!(ip = png_create_info_struct(rp))) {
            png_error(rp, "Failed to create png read structure.");
        }
        png_set_read_fn(
            rp, stream.get(),
            [](png_structp rp, png_bytep data, png_size_t size) {
                auto stream = static_cast<Stream::Stream*>(png_get_io_ptr(rp));
                if (stream->Read(data, size) < size) {
                    png_error(rp, "Failed to read png stream.");
                }
            }
        );
        {
            png_read_info(rp, ip);
            auto ctype = png_get_color_type(rp, ip);
            if ( ctype & PNG_COLOR_MASK_PALETTE) png_set_palette_to_rgb(rp                      );
            if (~ctype & PNG_COLOR_MASK_COLOR  ) png_set_gray_to_rgb   (rp                      );
            if (~ctype & PNG_COLOR_MASK_ALPHA  ) png_set_add_alpha     (rp, ~0, PNG_FILLER_AFTER);
            png_read_update_info(rp, ip);
            Length_[0] = png_get_image_width (rp, ip);
            Length_[1] = png_get_image_height(rp, ip);
            Rank_      = 2;
            switch (png_get_bit_depth(rp, ip)) {
            case 8:  Format_ = RGBA8888; break;
            case 16: Format_ = RGBAUN16; break;
            default:
                et = ErrorTypeLogic;
                png_error(rp, "Unexpected format.");
            }
            format  = Detail::GetConvertibleFormat(format, Format_);
            Stride_ = (Detail::GetBytesPerPixel(format) * Length_[0] + 3) & ~3; // align to 4 bytes
            Image_.resize(Stride_ * Length_[1]);
            if (format == Format_) {
                for (std::size_t y = 0, h = Length_[1]; y < h; ++y) {
                    png_read_row(rp, reinterpret_cast<png_bytep>(Image_.data() + y * Stride_), nullptr);
                }
            } else {
                if (!ConvertAndRead(rp, format)) {
                    et = ErrorTypeLogic;
                    png_error(rp, "Unsupported conversion patterns.");
                }
                Format_ = format;
            }
            png_read_end(rp, nullptr);
        }
        png_destroy_read_struct(&rp, &ip, nullptr);
    }

    virtual const void* Data(void) const override {
        return Image_.data();
    }

    virtual std::size_t Size(void) const override {
        return Image_.size();
    }

    virtual std::size_t Rank(void) const override {
        return Rank_;
    }

    virtual std::size_t Length(std::size_t axis) const override {
        return Rank_ > axis ? Length_[axis] : 1;
    }

    virtual std::size_t Stride(void) const override {
        return Stride_;
    }

    virtual PixelFormat Format(void) const override {
        return Format_;
    }

private:
    template<class T>
    bool ConvertAndRead(png_structp rp) {
        // TODO: 並列化可能？
        std::vector<png_byte> buffer(Detail::GetBytesPerPixel(Format_) * Length_[0]);
        switch (Format_) {
        case RGBAUN16:
            for (std::size_t y = 0, h = Length_[1]; y < h; ++y) {
                png_read_row(rp, buffer.data(), nullptr);
                Detail::ConvertPixelFormat(
                    reinterpret_cast<T*>(Image_.data() + y * Stride_),
                    reinterpret_cast<Detail::rgba_un16*>(buffer.data()),
                    Length_[0]
                );
            }
            return true;
        case RGBA8888:
            for (std::size_t y = 0, h = Length_[1]; y < h; ++y) {
                png_read_row(rp, buffer.data(), nullptr);
                Detail::ConvertPixelFormat(
                    reinterpret_cast<T*>(Image_.data() + y * Stride_),
                    reinterpret_cast<Detail::rgba_8888*>(buffer.data()),
                    Length_[0]
                );
            }
            return true;
        default:
            return false;
        }
    }

    bool ConvertAndRead(png_structp rp, PixelFormat format) {
        switch (format) {
        case RGBAFP32: return ConvertAndRead<Detail::rgba_fp32>(rp);
        case ARGBFP32: return ConvertAndRead<Detail::argb_fp32>(rp);
        case RGBAFP16: return ConvertAndRead<Detail::rgba_fp16>(rp);
        case ARGBFP16: return ConvertAndRead<Detail::argb_fp16>(rp);
        case RGBAUN16: return ConvertAndRead<Detail::rgba_un16>(rp);
        case ARGBUN16: return ConvertAndRead<Detail::argb_un16>(rp);
        case RGBA8888: return ConvertAndRead<Detail::rgba_8888>(rp);
        case ARGB8888: return ConvertAndRead<Detail::argb_8888>(rp);
        case RGBA4444: return ConvertAndRead<Detail::rgba_4444>(rp);
        case ARGB4444: return ConvertAndRead<Detail::argb_4444>(rp);
        case RGBA5551: return ConvertAndRead<Detail::rgba_5551>(rp);
        case ARGB1555: return ConvertAndRead<Detail::argb_1555>(rp);
        case RGBA5650: return ConvertAndRead<Detail::rgba_5650>(rp);
        case ARGB0565: return ConvertAndRead<Detail::argb_0565>(rp);
        default:       return false;
        }
    }

    enum ErrorType {
        ErrorTypeRuntime,
        ErrorTypeLogic
    };

    std::vector<std::byte> Image_;
    std::size_t            Rank_;
    std::size_t            Length_[2];
    std::size_t            Stride_;
    PixelFormat            Format_;
};

SharedImage LoadImagePNG(Stream::SharedStream stream, PixelFormat format) {
    return std::make_shared<ImagePNG>(stream, format);
}

} // namespace Graphene::Graphics
