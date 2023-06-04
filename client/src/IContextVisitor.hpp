/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "TypesFwd.hpp"

namespace ews::cli
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
    virtual void VisitClientContext(const ClientContext&) = 0;
};

} // namespace ews::cli
