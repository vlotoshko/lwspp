/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "types/TypesFwd.hpp"

namespace wspp::cli
{

class ClientContext;

class ClientContextBuilder
{
public:
    explicit ClientContextBuilder(int port);
    ~ClientContextBuilder();

    ClientContextBuilder(ClientContextBuilder&&) noexcept = default;
    auto operator=(ClientContextBuilder&&) noexcept -> ClientContextBuilder& = default;

    ClientContextBuilder(const ClientContextBuilder&) = delete;
    auto operator=(const ClientContextBuilder&) -> ClientContextBuilder& = delete;

public:
    auto build() const -> IClientContextPtr;

    auto setBehavior(std::string) -> ClientContextBuilder&;
    auto setLogging(int) -> ClientContextBuilder&;

private:
    std::unique_ptr<ClientContext> _clientContext;
};

} // namespace wspp::cli
