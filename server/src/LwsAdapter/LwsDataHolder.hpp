/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Feb, 2023
 */

#pragma once

#include "websocketpp/server/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace wspp::srv
{

/**
 * @brief The LwsDataHolder class holds the set of data low level context points on
 */
struct LwsDataHolder
{
    explicit LwsDataHolder(const ServerContext&);

    Port port;
    LwsProtocols protocols;
};

} // namespace wspp::srv
