/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#include "Client.hpp"
#include "LwsAdapter/LwsClient.hpp"

namespace ews::cli
{

Client::Client(const ClientContext& context)
    : _lwsClient(std::make_shared<LwsClient>(context))
    , _clientStop(std::async(std::launch::async, [&]{ _lwsClient->connect(); }))
{}

Client::~Client()
{
    _lwsClient.reset();
    _clientStop.wait();
}


} // namespace ews::cli
