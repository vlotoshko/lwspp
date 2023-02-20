/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "TypesFwd.hpp"

namespace wspp::srv
{

class IContextVisitorAcceptor
{
public:
    IContextVisitorAcceptor() = default;
    virtual ~IContextVisitorAcceptor() = default;

    IContextVisitorAcceptor(const IContextVisitorAcceptor&) = default;
    auto operator=(const IContextVisitorAcceptor&) -> IContextVisitorAcceptor& = default;

    IContextVisitorAcceptor(IContextVisitorAcceptor&&) noexcept = default;
    auto operator=(IContextVisitorAcceptor&&) noexcept -> IContextVisitorAcceptor& = default;

public:
    virtual void accept(IContextVisitor&) const = 0;
};

} // namespace wspp::srv
