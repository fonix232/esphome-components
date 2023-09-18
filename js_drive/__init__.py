import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor', 'binary_sensor']

js_drive_ns = cg.esphome_ns.namespace('js_drive')

JSDRIVE = js_drive_ns.class_('JSDrive', cg.Component)

CONF_UART_DESK = "uart_desk"
CONF_UART_CONTROLLER = "uart_controller"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(JSDRIVE),
    cv.Required(CONF_UART_DESK): cv.use_id(uart.UARTComponent),
    cv.Required(CONF_UART_CONTROLLER): cv.use_id(uart.UARTComponent),
})


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    uart_desk = await cg.get_variable(config[CONF_UART_DESK])
    cg.add(var.set_uart_desk(uart_desk))
    uart_controller = await cg.get_variable(config[CONF_UART_CONTROLLER])
    cg.add(var.set_uart_controller(uart_controller))