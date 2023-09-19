#include "js_drive.h"
#include "esphome/core/log.h"
#include <vector>

namespace esphome {
namespace js_drive {

    static const char *const TAG = "jsdrive";

    const void log_hex(const char *location, std::vector<uint8_t> bytes) {

        if(bytes.empty()) return;

        uint8_t separator = ' ';
        std::string res;
        size_t len = bytes.size();

        char buf[5];
        for (size_t i = 0; i < len; i++) {
            if (i > 0) {
                res += separator;
            }
            sprintf(buf, "%02X", bytes[i]);
            res += buf;
        }
        ESP_LOGI(TAG, "%s message(%u): %s", location, len, res.c_str());
        delay(10);
    }

    // This doesn't work yet, will fix later
    // const void readUARTBus(uart::UARTComponent *uart, std::vector<uint8_t> *buffer, uint8_t delimiter, void (*handler)(std::vector<uint8_t>*)) {
    //     uint8_t c;
    //     while(uart->available()) {
    //         if(!buffer->empty() && uart->peek_byte(&c) && c == delimiter) {
    //             handler(buffer);
    //             buffer->clear();
    //         }
    //         uart->read_byte(&c);
    //         buffer->push_back(c);
    //     }
    // }

    void JSDrive::setup() { }

    void JSDrive::loop() {
        readDeskBus();
        readCtrlBus();
    }
    
    void JSDrive::readDeskBus() {
        // readUARTBus(&this->uart_desk, &this->desk_buffer, 0x5a, &handleDeskBuffer);
        uint8_t c;
        if(this->uart_desk != nullptr) {
            while(this->uart_desk->available()) {
                if(!this->desk_buffer.empty() && this->uart_desk->peek_byte(&c) && c == 0x5a) {
                    this->handleDeskBuffer(this->desk_buffer);
                    this->desk_buffer.clear();
                }
                this->uart_desk->read_byte(&c);
                this->desk_buffer.push_back(c);
            }
        }
    }

    void JSDrive::readCtrlBus() {
        //readUARTBus(&this->uart_ctrl, &this->ctrl_buffer, 0xa5, &handleCtrlBuffer);
        uint8_t c;
        if(this->uart_ctrl != nullptr) {
            while(this->uart_ctrl->available()) {
                if(!this->ctrl_buffer.empty() && this->uart_ctrl->peek_byte(&c) && c == 0xa5) { 
                    this->handleCtrlBuffer(this->ctrl_buffer);
                    this->ctrl_buffer.clear();
                }
                this->uart_ctrl->read_byte(&c);
                this->ctrl_buffer.push_back(c);
            }
        }
    }

    void JSDrive::handleDeskBuffer(std::vector<uint8_t> buffer) {
        log_hex("DESK", buffer);
        this->uart_ctrl->write_array(buffer);
    }

    void JSDrive::handleCtrlBuffer(std::vector<uint8_t> buffer) {
        log_hex("CTRL", buffer);
        this->uart_desk->write_array(buffer);
    }

    void JSDrive::dump_config() {
        ESP_LOGCONFIG(TAG, "JSDrive Desk");
    }
}
}