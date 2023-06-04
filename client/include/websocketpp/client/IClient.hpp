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

public:
    // NOTE: the 'connect' method can be implemented as blocking thread method
    virtual void connect() = 0;
};

} // namespace wspp::cli
