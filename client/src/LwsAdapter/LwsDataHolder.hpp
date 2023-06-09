/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include "easywebsocket/client/Types.hpp"
#include "LwsAdapter/LwsTypesFwd.hpp"
#include "TypesFwd.hpp"

namespace ews::cli
{

/**
 * @brief The LwsDataHolder class holds the set of data low level context points on
 */
struct LwsDataHolder
{
    explicit LwsDataHolder(const ClientContext&);

    Address address;
    Port port;
    Path path;
    std::string protocolName;
    LwsProtocols protocols;
};

} // namespace ews::cli
