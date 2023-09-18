#include "js_drive.h"
#include "esphome/core/log.h"

namespace esphome {
namespace js_drive {

void JSDrive::loop() {
    if(this->uart_desk != nullptr) {
        uint8_t lastByte;
        // Read data from Desk UART until a whole message is retrieved
        while(this->uart_desk.peek_byte(&lastByte) && lastByte != MessageType.MC) {
            this->uart_desk.read_byte(&lastByte);
            this->desk_command_buffer.push_back(lastByte);
        }

        
    }
    if(this->uart_controller != nullptr) {
        // Read data from Controller UART until a whole message is retrieved
    }
}

Message JSDrive::processDeskMessage(std::vector<uint8_t> message) {
    if(message.front().c != MessageType.MC) {
        return nullptr; // TODO: Create INVALID/INCOMPLETE message type and return it here
    }

    // TODO: Parse [message] into Message class

}

}
}
