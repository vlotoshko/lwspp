/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "Client.hpp"

namespace ews::cli
{

Client::Client(const ClientContext& context)
    : _lwsContext(context)
{}

void Client::connect()
{
    _lwsContext.connect();
}

} // namespace ews::cli
