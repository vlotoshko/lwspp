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
{
    std::weak_ptr<LwsClient> weakLwsClient = _lwsClient;

    auto asyncConnect = [weakLwsClient]
    {
        if (auto lwsClient = weakLwsClient.lock())
        {
            lwsClient->connect();
        }
    };

    _clientStop = std::async(std::launch::async, asyncConnect);
}

Client::~Client()
{
    _lwsClient->disconnect();
    _clientStop.wait();
}

} // namespace ews::cli
