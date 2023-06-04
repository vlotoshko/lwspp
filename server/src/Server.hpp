/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include "easywebsocket/server/IServer.hpp"
#include "LwsAdapter/LwsContext.hpp"
#include "TypesFwd.hpp"

namespace ews::srv
{

class Server : public IServer
{
public:
    explicit Server(const ServerContext&);
    ~Server() override;

    Server(Server&&) noexcept = default;
    auto operator=(Server&&) noexcept -> Server& = default;

    Server(const Server&) = delete;
    auto operator=(const Server&) -> Server& = delete;

public:
    // NOTE: the 'start' member function blocks the thread
    void start() override;

private:
    void stop_();

private:
    LwsContext _lwsContext;
};

} // namespace ews::srv
