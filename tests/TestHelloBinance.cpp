/**
 * @author Volodymyr Lotoshko (vlotoshko@gmail.com)
 * @date Jul, 2023
 */

#include <thread>

#include "catch2/catch.hpp"

#include "easywebsocket/client/ClientContextBuilder.hpp"
#include "easywebsocket/client/ClientFactory.hpp"
#include "easywebsocket/client/EventHandlerBase.hpp"
#include "easywebsocket/client/IMessageSender.hpp"

// NOLINTBEGIN (readability-function-cognitive-complexity)
namespace ews::tests
{
namespace
{

const cli::Port PORT = 9443;
const cli::Address ADDRESS = "stream.binance.com";
const std::string SUBSCRIBE = R"({"method": "SUBSCRIBE","params":["xrpusdt@trade"],"id": 3})";

class ClientEventHandler : public cli::EventHandlerBase
{
public:
    explicit ClientEventHandler(bool& connected, bool& messageReceived)
        : _connected(connected)
        , _messageReceived(messageReceived)
    {}

    void onConnect(cli::ISessionInfoPtr) noexcept override
    {
        _connected = true;
        _messageSender->sendMessage(SUBSCRIBE);
    }

    void onMessageReceive(const std::string&) noexcept override
    {
        _messageReceived = true;
    }

private:
    bool& _connected;
    bool& _messageReceived;
};

cli::IClientPtr setupClient(bool& connected, bool& messageReceived)
{
    auto clientEventHandler = std::make_shared<ClientEventHandler>(connected, messageReceived);
    auto clientContextBuilder = cli::ClientContextBuilder{};
    clientContextBuilder
        .setVersion(cli::ClientVersion::v1_Amsterdam)
        .setAddress(ADDRESS)
        .setPath("/ws")
        .setPort(PORT)
        .setEventHandler(clientEventHandler)
        .setMessageSenderAcceptor(clientEventHandler)
        .enableSsl()
        ;

    return cli::createClient(*clientContextBuilder.build());
}

} // namespace

SCENARIO( "Clients connect to the binance service", "[hello_binance]" )
{
    GIVEN( "Client" )
    {
        bool actualConnected = false;
        bool actualMessageReceived = false;
        auto client = setupClient(actualConnected, actualMessageReceived);

        WHEN( "Client sends message to server" )
        {
            THEN( "Server receives message from client and sends message back" )
            {
                // waiting for the client receive message from server
                const auto timeout = 5000U;
                std::this_thread::sleep_for(std::chrono::milliseconds(timeout));

                bool expectedConnected = true;
                bool expectedMessageReceived = true;
                REQUIRE(actualConnected == expectedConnected);
                REQUIRE(actualMessageReceived == expectedMessageReceived);

            }
        }
    } // GIVEN
} // SCENARIO

} // namespace ews::tests
// NOLINTEND (readability-function-cognitive-complexity)
