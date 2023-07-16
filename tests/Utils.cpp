/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
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
