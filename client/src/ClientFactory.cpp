/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "websocketpp/client/ClientFactory.hpp"
#include "Client.hpp"

namespace wspp::cli
{

auto createClient(const IClientContext&) -> IClientPtr
{
    return std::make_shared<Client>();
}

} // namespace wspp::cli
