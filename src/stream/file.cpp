/**
 * @brief ファイル入出力
 * @author Takaaki Sato
 * @copyright Copyright (c) 2020 Demiquartz &lt;info@demiquartz.jp&gt;@n
 * Distributed under the MIT License (See accompanying file LICENSE
 * or copy at https://opensource.org/licenses/MIT)
 */
#include <graphene/stream/file.hpp>
#include <system_error>
#include <cstdio>
#include <sys/stat.h>

namespace Graphene::Stream {

class File final : public Stream {
public:
    File(const std::string& path, const std::string& mode) :
    Handle_(std::fopen(path.c_str(), AddBinaryMode(mode).c_str())) {
        if (!Handle_) {
            throw std::system_error(errno, std::generic_category(), path);
        }
    }

    virtual ~File() override {
        Close();
    }

    virtual void Close(void) override {
        if (Handle_) {
            std::fclose(Handle_);
            Handle_ = nullptr;
        }
    }

    virtual bool Flush(void) override {
        return std::fflush(Handle_) == 0;
    }

    virtual std::size_t Read(void* data, std::size_t size) override {
        return std::fread(data, 1, size, Handle_);
    }

    virtual std::size_t Write(const void* data, std::size_t size) override {
        return std::fwrite(data, 1, size, Handle_);
    }

    virtual bool Seek(std::size_t offset) override {
        return std::fseek(Handle_, offset, SEEK_SET) == 0;
    }

    virtual std::size_t Tell(void) const override {
        return std::ftell(Handle_);
    }

    virtual std::size_t Size(void) const override {
        struct stat s;
        return fstat(fileno(Handle_), &s) == 0 ? s.st_size : 0;
    }

private:
    std::string AddBinaryMode(const std::string& mode) {
        return mode.find('b') == std::string::npos ? mode + 'b' : mode;
    }

private:
    FILE* Handle_;
};

SharedStream FileFactory::Open(const std::string& path, const std::string& mode) {
    return std::make_shared<File>(path, mode);
}

} // namespace Graphene::Stream
