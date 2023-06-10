/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <future>

#include "easywebsocket/server/IServer.hpp"
#include "TypesFwd.hpp"

namespace ews::srv
{

/**
 * @brief The Server class is the wrapper for the interlnal LwsServer implementation
 * @note The server starts listening on construction and stops listening on destruction
 * @note The server spawns a separate thread for the listening
 */
class Server : public IServer
{
public:
    explicit Server(const ServerContext&);
    ~Server() override;

    Server(Server&&) noexcept = default;
    auto operator=(Server&&) noexcept -> Server& = default;

    Server(const Server&) = delete;
    auto operator=(const Server&) -> Server& = delete;

private:
    LwsServerPtr _lwsServer;
    std::future<void> _serverStop;
};

} // namespace ews::srv
