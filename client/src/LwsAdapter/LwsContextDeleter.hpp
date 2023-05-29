/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date May, 2023
 */

#pragma once

#include <memory>

#include "libwebsockets.h"

namespace wspp::cli
{

class LwsContextDeleter
{
public:
    void operator()(lws_context* p)
    {
        lws_context_destroy(p);
    }
};

} // namespace wspp::cli
