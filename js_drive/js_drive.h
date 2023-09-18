#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace js_drive {

static const char *const TAG = "jsdrive";

enum MessageType : uint8_t {
    MC = 0x5a,
    HC = 0xa5
}

class Message {

    public:
        bool isValid() {
            
        }

    protected:
        std::vector<uint8_t> message;
        uint8_t getByte(int position) {
            uint8_t *d = this->message.data();
            return d[position];
        }
}


class JSDrive : public Component {
    public:
        float get_setup_priority() const override { return setup_priority::LATE; }
        void loop() override;
        void dump_config() override;

        void set_uart_desk(uart::UARTComponent *uart) { this-> uart_desk = uart; }
        void set_uart_controller(uart::UARTComponent *uart) { this -> uart_controller = uart; }

    protected:
        uart::UARTComponent *uart_desk{nullptr};
        uart::UARTComponent *uart_controller{nullptr};
        std::vector<uint8_t> desk_command_buffer;
        std::vector<uint8_t> remote_command_buffer;

        Message processDeskMessage(std::vector<uint8_t> message);
        Message processRemoteMessage(std::vector<uint8_t> message);
}       


}
}
