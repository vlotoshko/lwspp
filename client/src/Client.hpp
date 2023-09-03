/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <future>

#include "easywebsocket/client/IClient.hpp"
#include "TypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief The Client class wraps the internal LwsClient implementation.
 *
 * The client automatically connects upon construction and disconnects upon destruction.
 * It operates within a separate thread to establish and maintain the connection with the server.
 */
class Client : public IClient
{
public:
    explicit Client(const ClientContext&);
    ~Client() override;

    Client(Client&&) noexcept = default;
    auto operator=(Client&&) noexcept -> Client& = default;

    Client(const Client&) = delete;
    auto operator=(const Client&) -> Client& = delete;

private:
    LwsClientPtr _lwsClient;
    std::future<void> _clientStop;
};

} // namespace ews::cli
