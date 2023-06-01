/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "websocketpp/client/IClient.hpp"
#include "LwsAdapter/LwsContext.hpp"
#include "TypesFwd.hpp"

namespace wspp::cli
{

class Client : public IClient
{
public:
    explicit Client(const ClientContext&);

    void connect() override;
    void disconnect() override;
    auto isConnected() const -> bool override;

private:
    LwsContext _lwsContext;
};

} // namespace wspp::cli
