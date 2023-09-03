/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include <queue>
#include <string>

#include "LwsAdapter/LwsTypes.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief The ILwsSession class represents the connection to the server.
 */
class ILwsSession
{
public:
    ILwsSession() = default;
    virtual ~ILwsSession() = default;

    ILwsSession(const ILwsSession&) = default;
    auto operator=(const ILwsSession&) -> ILwsSession& = default;

    ILwsSession(ILwsSession&&) noexcept = default;
    auto operator=(ILwsSession&&) noexcept -> ILwsSession& = default;

public:
    virtual auto getLwsInstance() -> LwsInstanceRawPtr = 0;

    virtual void addBinaryDataToSend(const std::vector<char>&) = 0;
    virtual void addTextDataToSend(const std::string&) = 0;
    virtual auto getPendingData() -> std::queue<Message>& = 0;
};

} // namespace ews::cli
