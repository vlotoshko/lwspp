/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Apr, 2023
 */

#pragma once

#include <string>
#include <vector>

#include "easywebsocket/client/TypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief The IEventHandler class defines an interface for implementing client behavior.
 * Users of the library must implement this interface themselves.
 */
class IEventHandler
{
public:
    IEventHandler() = default;
    virtual ~IEventHandler() = default;

    IEventHandler(IEventHandler&&) = default;
    auto operator=(IEventHandler&&) noexcept -> IEventHandler& = default;

    IEventHandler(const IEventHandler&) = delete;
    auto operator=(const IEventHandler&) noexcept -> IEventHandler& = delete;

public:
    // Invoked when the client receives binary data from the server.
    virtual void onBinaryDataReceive(const std::vector<char>& data, size_t bytesRemains) noexcept = 0;
    // Invoked when the client receives text data from the server. This method expects valid UTF-8 text.
    virtual void onTextDataReceive(const std::string& message, size_t bytesRemains) noexcept = 0;

    virtual void onConnect(ISessionInfoPtr) noexcept = 0;
    virtual void onDisconnect() noexcept = 0;
    virtual void onError(const std::string& errorMessage) noexcept = 0;
    virtual void onWarning(const std::string& errorMessage) noexcept = 0;
};

} // namespace ews::cli
