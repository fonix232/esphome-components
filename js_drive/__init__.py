import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ['uart']

js_drive_ns = cg.esphome_ns.namespace('js_drive')

JSDrive = js_drive_ns.class_('JSDrive', cg.Component)

CONF_UART_DESK = "uart_desk"
CONF_UART_CTRL = "uart_ctrl"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(JSDrive),
    cv.Optional(CONF_UART_DESK): cv.use_id(uart.UARTComponent),
    cv.Optional(CONF_UART_CTRL): cv.use_id(uart.UARTComponent),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_UART_DESK in config:
        uart_desk = await cg.get_variable(config[CONF_UART_DESK])
        cg.add(var.set_uart_desk(uart_desk))
    if CONF_UART_CTRL in config:
        uart_ctrl = await cg.get_variable(config[CONF_UART_CTRL])
        cg.add(var.set_uart_ctrl(uart_ctrl))
