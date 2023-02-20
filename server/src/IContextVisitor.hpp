/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "TypesFwd.hpp"

namespace wspp::srv
{

class IContextVisitor
{
public:
    IContextVisitor() = default;
    virtual ~IContextVisitor() = default;

    IContextVisitor(const IContextVisitor&) = default;
    auto operator=(const IContextVisitor&) -> IContextVisitor& = default;

    IContextVisitor(IContextVisitor&&) noexcept = default;
    auto operator=(IContextVisitor&&) noexcept -> IContextVisitor& = default;

public:
    virtual void VisitServerContext(const ServerContext&) = 0;
};

} // namespace wspp::srv
