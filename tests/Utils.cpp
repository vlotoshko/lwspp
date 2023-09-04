/*
 * EasyWebsockets - C++ wrapper for the libwebsockets library
 *
 * Copyright (C) 2023 - 2023 Volodymyr Lotoshko <vlotoshko@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifdef __linux__

#include <array>
#include <climits>
#include <unistd.h>

#elif _WIN32

#include <windows.h>

#endif

#include "Utils.hpp"

namespace ews::tests
{

auto getDirectoryFromFilePath(const std::string& filePath) -> std::string;

#ifdef __linux__

auto getExecDirectory() -> std::string
{
    std::array<char, PATH_MAX> path{};
    size_t count = readlink( "/proc/self/exe", path.data(), PATH_MAX );
    auto execPath = std::string {path.data(), ((count > 0) ? count : 0)};

    return getDirectoryFromFilePath(execPath);
}

#elif _WIN32

auto getExecDirectory() -> std::string
{
    char result[ MAX_PATH ];
    auto execPath = std::string(result, GetModuleFileName(nullptr, result, MAX_PATH));

    return getDirectoryFromFilePath(execPath);
}
#else

auto getExecDirectory() -> std::string
{
    throw std::runtime_error("Cannot get exec directory: unsupported platform");
}

#endif

// expected input: <direcory path>/<file name>
// returns: <direcory path>/
auto getDirectoryFromFilePath(const std::string& filePath) -> std::string
{
    auto pos = filePath.rfind('/');
    if (pos != std::string::npos)
    {
        if (pos == filePath.length())
        {
            return filePath;
        }
        return filePath.substr(0, pos+1);
    }
    return std::string{};
}

} // namespace ews::tests
