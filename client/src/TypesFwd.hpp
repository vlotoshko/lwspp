/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jan, 2023
 */

#pragma once

#include <memory>

namespace ews::cli
{

class ClientContext;

class LwsClient;
using LwsClientPtr = std::shared_ptr<LwsClient>;

} // namespace ews::cli
