import struct

from smbus2 import SMBus
from enum import Enum

I2C_ADDRESS = 4

class ModeOfOperation(Enum):
    ConfirmSwitch = 0
    Standby = 1
    RC = 2
    AI = 3

class RCController():

    def __init__(self, i2c_bus:int):
        self.bus = SMBus(i2c_bus)

    def getModeOfOperation(self):
        mode = self.bus.read_byte_data(I2C_ADDRESS, 0b00000000)
        try:
            return ModeOfOperation(mode)
        except ValueError:
            print("RCController returned invalid mode-of-operation:", mode)
            raise

    def getRCReceiverInput(self):
        data = self.bus.read_i2c_block_data(I2C_ADDRESS, 0b00000001, 4)
        result = {}
        result['throttle'] = struct.unpack("<H", bytes(data[0:2]))[0]
        result['steering'] = struct.unpack("<H", bytes(data[2:4]))[0]
        return result

    def getMotorFanInfo(self):
        data = self.bus.read_i2c_block_data(I2C_ADDRESS, 0b00000010, 9)
        result = {}
        result['fan_speed']                 = data[0]
        result['update_interval_millis']    = struct.unpack("<H", bytes(data[1:3]))[0]
        result['fan_off_temp']              = float(struct.unpack("<h", bytes(data[3:5]))[0])/100
        result['fan_max_temp']              = float(struct.unpack("<h", bytes(data[5:7]))[0])/100
        result['manual_override']           = data[7]
        result['manual_speed']              = data[8]
        return result

    def getMotorTemp(self):
        data = self.bus.read_i2c_block_data(I2C_ADDRESS, 0b00000011, 2)
        return float(struct.unpack("<h", bytes(data))[0])/100

    def requestModeOfOperation(self, mode:ModeOfOperation):
        if mode == ModeOfOperation.ConfirmSwitch:
            print("Warning: Invalid mode to request!", "(" + str(mode) + ")")
            return
        self.bus.write_byte_data(I2C_ADDRESS, 0b10000000, mode.value)

    def setRCControlSignals(self, throttlePulseMicros, steeringPulseMicros):
        data = []
        data.extend(struct.pack("<H", throttlePulseMicros))
        data.extend(struct.pack("<H", steeringPulseMicros))
        self.bus.write_i2c_block_data(I2C_ADDRESS, 0b10000001, data)

    def setMotorFanSettings(self, updateIntervalMillis, fanOffTemp, fanMaxTemp, manualOverride, manualSpeed):
        data = []
        data.extend(struct.pack("<H", updateIntervalMillis))
        data.extend(struct.pack("<h", int(fanOffTemp * 100)))
        data.extend(struct.pack("<h", int(fanMaxTemp * 100)))
        data.append(1 if manualOverride else 0)
        data.append(manualSpeed)
        self.bus.write_i2c_block_data(I2C_ADDRESS, 0b10000010, data)