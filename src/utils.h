#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <zlib.h>
#include <filesystem>

namespace utils {

    inline bool exists(const std::string& path) {
        return std::filesystem::exists(path);
    }

    inline void create_dir(const std::string& path) {
        std::filesystem::create_directories(path);
    }

    inline std::vector<std::string> read_lines(const std::string& path) {
        std::ifstream in(path);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(in, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    inline void append_line(const std::string& path, const std::string& line) {
        std::ofstream out(path, std::ios::app);
        out << line << "\n";
    }
     inline std::string sha1(const std::string& data) {
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(reinterpret_cast<const unsigned char*>(data.c_str()),
             data.size(), hash);

        std::ostringstream oss;
        for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
            oss << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(hash[i]);

        return oss.str();
    }

    inline std::string read_file(const std::string& path) {
        std::ifstream in(path, std::ios::binary);
        std::ostringstream ss;
        ss << in.rdbuf();
        return ss.str();
    }

    inline std::string compress(const std::string& data) {
        uLongf compressedSize = compressBound(data.size());
        std::vector<Bytef> buffer(compressedSize);

        ::compress(buffer.data(), &compressedSize,
                   reinterpret_cast<const Bytef*>(data.data()),
                   data.size());

        return std::string(reinterpret_cast<char*>(buffer.data()), compressedSize);
    }

    inline void write_binary(const std::string& path, const std::string& data) {
        std::ofstream out(path, std::ios::binary);
        out.write(data.data(), data.size());
    }
}


