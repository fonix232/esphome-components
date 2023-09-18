#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace js_drive {

static const char *const TAG = "jsdrive";

// Known messages

MOVE_UP = { 0xA5, 0x03, 0x12, CHECKSUM } // Move table up
MOVE_DOWN = { 0xA5, 0x03, 0x14, CHECKSUM } // Move table down
STOP = { 0xA5, 0x03, 0x10, CHECKSUM } // Stop moving
GET_HEIGHT ={ 0xA5, 0x03, 0x21, CHECKSUM } // Get table height information
GET_HC_INFO = { 0xA5, 0x04, 0x20, 0x01, CHECKSUM } // Get HC information

MOVE_TO = { 0xA5, 0x05, 0x31, HEIGHT_H, HEIGHT_L, CHECKSUM } // Move table to specific height

UNLOCK = { 0xA5, 0x04, 0x32, 0x00, CHECKSUM } // Unlock HC
LOCK = { 0xA5, 0x04, 0x32, 0x01, CHECKSUM } // Lock HC

SET_CM = { 0xA5, 0x04, 0x33, 0x00, CHECKSUM } // Set HC unit to cm
SET_IN = { 0xA5, 0x04, 0x33, 0x01, CHECKSUM } // Set HC unit to in

// Responses

// MOVE_UP, MOVE_DOWN, STOP
MOVE_RESPONSE = { 0x5A, 0x06, CU_SETTING, HEIGHT_H, HEIGHT_L, CHECKSUM } 
// CU_SETTING:
// Bit 0: Lock status (0x00 = unlocked, 0x01 = locked)
// Bit 1: Display unit (0x00 = cm, 0x01 = in)

// GET_HEIGHT
GET_HEIGHT_RESPONSE = { 0x5A, 0x09, 0x21, CURR_HEIGHT_H, CURR_HEIGHT_L, MIN_HEIGHT_H, MIN_HEIGHT_L, MAX_HEIGHT_H, MAX_HEIGHT_L, CHECKSUM}

// GET_HC_INFO
GET_HC_INFO_RESPONSE = { 0x5A, 0x04, 0x20, HC_INFO, CHECKSUM }
// HC_INFO:
// Bit 0: Lock status (0x00 = unlocked, 0x01 = locked)
// Bit 1: Display unit (0x00 = cm, 0x01 = in)
// Bit 2: Error Status (0x00 = no error, 0x01 = error)
// Bit 3: Init status (0x00 = not initialised, 0x01 = initialised)
// ...?

// MOVE_TO
MOVE_TO_RESPONSE = { 0x5A, 0x03, 0x31, CHECKSUM }

// UNLOCK, LOCK
UNLOCK_LOCK_RESPONSE = { 0x5A, 0x03, 0x32, CHECKSUM }

// SET_CM, SET_IN
SET_UNIT_RESPONSE = { 0x5A, 0x03, 0x33, CHECKSUM }

// During movement, the MC will send height updates every second
HEIGHT_RESPONSE = { 0x5A, 0x06, HC_INFO, CURR_HEIGHT_H, CURR_HEIGHT_L, ERROR_CODE, CHECKSUM }
// HC_INFO as above
// ERROR_CODE info:
// 0x01 - Main power too high
// 0x02 - Screw clearance over 1cm
// 0x03 - HC Not Connected
// 0x04 - HC Communication Error
// 0x05 - Blocked off Stop
// 0x06 - Main power start error
// 0x07 - Main power run protect
// 0x08 - Table is tilted while running
// 0x09 - Main power high temperature protect

// Motor error codes
// 0xN1 - Motor N not connected
// 0xN2 - Motor N current sampling error
// 0xN3 - Motor N lost phase line
// 0xN4 - Motor N hall effect sensor error
// 0xN5 - Motor N phase short
// 0xN6 - Motor N blocked
// 0xN7 - Motor N direction error
// 0xN8 - Motor N overload

// 0x40 - Tandem line drops
// 0x41 - Tandem signal error
// 0x42 - EEPROM error
// 0x43 - gyroscope error
// 0xFF - HC is locked


enum MessageType : uint8_t {
    MC = 0x5a,
    HC = 0xa5
}

class Message {

    public:
        bool isValid() {
            uint8_t csum = getChecksum;
            return csum == checksum;
        }

        uint8_t getChecksum() {
            uint8_t csum;
            for(int i = 0; i < message_data.size(); i++) {
                csum += d[i];
            }
            return csum;
        }

    protected:
        // Struct parts
        MessageType type;
        std::vector<uint8_t> message_data;
        uint8_t checksum;
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
        Message processControllerMessage(std::vector<uint8_t> message);

        void handleDeskMessage(Nessage *message);
        void handleControllerMessage(Message *message);
}       

}
}
