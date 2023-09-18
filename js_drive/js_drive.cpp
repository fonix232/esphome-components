#include "js_drive.h"
#include "esphome/core/log.h"

namespace esphome {
namespace js_drive {

void JSDrive::loop() {
    uint8_t lastByte;
    Message message;

    if(this->uart_desk != nullptr) {
        // Read data from Desk UART until a whole message is retrieved
        while(this->uart_desk.peek_byte(&lastByte) && lastByte != MessageType.MC) {
            this->uart_desk.read_byte(&lastByte);
            this->desk_command_buffer.push_back(lastByte);

            message = this.processDeskMessage(&desk_command_buffer);
            handleDeskMessage(&message);
        }
    }

    if(this->uart_controller != nullptr) {
        // Read data from Controller UART until a whole message is retrieved
        while(this->uart_controller.peek_byte(&lastByte) && lastByte != MessageType.MC) {
            this->uart_controller.read_byte(&lastByte);
            this->controller_command_buffer.push_back(lastByte);

            message = this.processControllerMessage(&controller_command_buffer);
            handleControllerMessage(&message);
        }
    }
}

Message JSDrive::processDeskMessage(std::vector<uint8_t> messageData) {
    if(messageData.front().c != MessageType.MC) {
        return nullptr; // TODO: Create INVALID/INCOMPLETE message type and return it here
    }

    // TODO: Parse [message] into Message class
}



Message JSDrive::processControllerMessage(std::vector<uint8_t> messageData) {
    if(messageData.front().c != MessageType.HC) {
        return nullptr; // TODO: Create INVALID/INCOMPLETE message type and return it here
    }

    // TODO: Parse [message] into Message class
}

}
}
