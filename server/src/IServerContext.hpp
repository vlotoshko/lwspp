/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "IContextVisitorAcceptor.hpp"

namespace wspp::srv
{

class IServerContext : public IContextVisitorAcceptor
{
public:
    IServerContext() = default;
    ~IServerContext() override = default;

    IServerContext(const IServerContext&) = default;
    auto operator=(const IServerContext&) -> IServerContext& = default;

    IServerContext(IServerContext&&) noexcept = default;
    auto operator=(IServerContext&&) noexcept -> IServerContext& = default;
};

} // namespace wspp::srv
