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
 * @brief The Client class is the wrapper for the interlnal LwsClient implementation
 * @note The client connects on construction and disconnects on destruction
 * @note The client spawns a separate thread to connect to a server
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
