/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <string>
#include <vector>

#include "easywebsocket/server/Types.hpp"
#include "easywebsocket/server/TypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief The IEventHandler class defines an interface for implementing server behavior.
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
    // Invoked when the server receives binary data from the client.
    virtual void onBinaryDataReceive(SessionId, const std::vector<char>& data, size_t bytesRemains) noexcept = 0;
    // Invoked when the server receives text data from the client. This method expects valid UTF-8 text.
    virtual void onTextDataReceive(SessionId, const std::string& message, size_t bytesRemains) noexcept = 0;

    virtual void onConnect(ISessionInfoPtr) noexcept = 0;
    virtual void onDisconnect(SessionId) noexcept = 0;
    virtual void onError(SessionId, const std::string& errorMessage) noexcept = 0;
    virtual void onWarning(SessionId, const std::string& errorMessage) noexcept = 0;
};

} // namespace ews::srv
