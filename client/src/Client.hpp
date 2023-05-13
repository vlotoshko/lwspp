/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "websocketpp/client/IClient.hpp"

namespace wspp::cli
{

class Client : public IClient
{
public:
    void connect() override;
    auto isConnected() const -> bool override;

private:
};

} // namespace wspp::cli
