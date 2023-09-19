#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace js_drive {

// TODO: Define static known values e.g. command beginnings

class JSDrive : public Component {
    public:
        void setup() override;
        void loop() override;
        void dump_config() override;
        void set_uart_desk(uart::UARTComponent *uart) { this->uart_desk = uart; }
        void set_uart_ctrl(uart::UARTComponent *uart) { this->uart_ctrl = uart; }

    protected:
        uart::UARTComponent *uart_desk{nullptr};
        uart::UARTComponent *uart_ctrl{nullptr};
    
        std::vector<uint8_t> desk_buffer;
        std::vector<uint8_t> ctrl_buffer;

        bool isDeskBufferValid() { return this->desk_buffer.data()[0] == 0x5a; }
        bool isCtrlBufferValid() { return this->ctrl_buffer.data()[0] == 0xa5; }

        void readDeskBus();
        void readCtrlBus();

        void handleDeskBuffer(std::vector<uint8_t> buffer);
        void handleCtrlBuffer(std::vector<uint8_t> buffer);
};

}
}
