/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "Client.hpp"

namespace wspp::cli
{

Client::Client(const ClientContext& context)
    : _lwsContext(context)
{
}

void Client::connect()
{
    _lwsContext.connect();
}

void Client::disconnect()
{
    _lwsContext.disconnect();
}

auto Client::isConnected() const -> bool
{
    return false;
}

} // namespace wspp::cli
