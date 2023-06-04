/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "IContextVisitorAcceptor.hpp"

namespace ews::cli
{

class IClientContext : public IContextVisitorAcceptor
{
public:
    IClientContext() = default;
    ~IClientContext() override = default;

    IClientContext(const IClientContext&) = default;
    auto operator=(const IClientContext&) -> IClientContext& = default;

    IClientContext(IClientContext&&) noexcept = default;
    auto operator=(IClientContext&&) noexcept -> IClientContext& = default;
};

} // namespace ews::cli
