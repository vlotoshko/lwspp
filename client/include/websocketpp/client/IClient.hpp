/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

namespace wspp::cli
{

class IClient
{
public:
    IClient() = default;
    IClient(IClient&&) = default;
    auto operator=(IClient&&) noexcept -> IClient& = default;

    IClient(const IClient&) = delete;
    auto operator=(const IClient&) noexcept -> IClient& = delete;

    virtual ~IClient() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual auto isConnected() const -> bool = 0;
};

} // namespace wspp::cli
