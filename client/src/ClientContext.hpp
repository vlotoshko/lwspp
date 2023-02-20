/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <string>

#include "IClientContext.hpp"

namespace wspp::cli
{

class ClientContext : public IClientContext
{
public:
    void accept(IContextVisitor&) const override;

    int port = 0;
    std::string behavior;
    int loglevel = 0;
};

} // namespace wspp::cli
