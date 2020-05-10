#include "AutomaticFan.h"

#define DEFAULT_UPDATE_INTERVAL_MILLIS 2000
#define DEFAULT_FAN_OFF_TEMP_C 30.0f
#define DEFAULT_FAN_MAX_TEMP_C 50.0f
#define DEFAULT_MANUAL_OVERRIDE false
#define DEFAULT_MANUAL_SPEED 255

/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2 (not used by Arduino core, doesn't affect millis()/delay()/Servo library)
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   https://forum.arduino.cc/index.php?topic=16612#msg121031
 */
void setPwmFrequency(int pin, int divisor)
{
	byte mode;
	if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
		switch(divisor) {
			case 1: mode = 0x01; break;
			case 8: mode = 0x02; break;
			case 64: mode = 0x03; break;
			case 256: mode = 0x04; break;
			case 1024: mode = 0x05; break;
			default: return;
		}
		if(pin == 5 || pin == 6) {
			TCCR0B = TCCR0B & 0b11111000 | mode;
		} else {
			TCCR1B = TCCR1B & 0b11111000 | mode;
		}
	} else if(pin == 3 || pin == 11) {
		switch(divisor) {
			case 1: mode = 0x01; break;
			case 8: mode = 0x02; break;
			case 32: mode = 0x03; break;
			case 64: mode = 0x04; break;
			case 128: mode = 0x05; break;
			case 256: mode = 0x06; break;
			case 1024: mode = 0x07; break;
			default: return;
		}
		TCCR2B = TCCR2B & 0b11111000 | mode;
	}
}

AutomaticFan::AutomaticFan(uint8_t pin, TemperatureSensor* sensor, uint16_t eepromOffset)
{
    this->pin = pin;
    this->sensor = sensor;
	this->eepromOffset = eepromOffset;

    started = false;
}

void AutomaticFan::begin(bool lowerPwmFrequency = false)
{
	if (started) return;
	started = true;

	EEPROM.get(eepromOffset, updateIntervalMillis);
	EEPROM.get(eepromOffset+2, fanOffTemp);
	EEPROM.get(eepromOffset+6, fanMaxTemp);
	EEPROM.get(eepromOffset+10, manualOverride);
	EEPROM.get(eepromOffset+11, manualSpeed);

	pinMode(pin, OUTPUT);

    if (lowerPwmFrequency)
        setPwmFrequency(pin, 1024);//Stuff could stop working if used with wrong pins!!! (see function description)

	updateManualSpeed();
}

void AutomaticFan::loop()
{
	if (manualOverride) return;

	if (sensor->isCurrentTempValid() && millis() >= nextUpdateMillis){
		
		float newSpeed = (sensor->getCurrentTempC() - fanOffTemp)/(fanMaxTemp - fanOffTemp);
		newSpeed = (uint8_t)(constrain(newSpeed, 0, 1) * 255);
		setFanSpeed(newSpeed);

		nextUpdateMillis = millis() + updateIntervalMillis;
	}
}

void AutomaticFan::resetEEPROM()
{
	setUpdateIntervalMillis(DEFAULT_UPDATE_INTERVAL_MILLIS);
	setFanOffTemp(DEFAULT_FAN_OFF_TEMP_C);
	setFanMaxTemp(DEFAULT_FAN_MAX_TEMP_C);
	setManualOverride(DEFAULT_MANUAL_OVERRIDE);
	setManualSpeed(DEFAULT_MANUAL_SPEED);
}

uint8_t AutomaticFan::getCurrentFanSpeed()
{
	return currentFanSpeed;
}

uint16_t AutomaticFan::getUpdateIntervalMillis()
{
	return updateIntervalMillis;
}

float AutomaticFan::getFanOffTemp()
{
	return fanOffTemp;
}

float AutomaticFan::getFanMaxTemp()
{
	return fanMaxTemp;
}

bool AutomaticFan::getManualOverride()
{
	return manualOverride;
}

uint8_t AutomaticFan::getManualSpeed()
{
	return manualSpeed;
}

void AutomaticFan::setUpdateIntervalMillis(uint16_t interval)
{
	updateIntervalMillis = interval;
	EEPROM.put(eepromOffset, interval);
}

void AutomaticFan::setFanOffTemp(float offTemp)
{
	fanOffTemp = offTemp;
	EEPROM.put(eepromOffset+2, offTemp);
}

void AutomaticFan::setFanMaxTemp(float maxTemp)
{
	fanMaxTemp = maxTemp;
	EEPROM.put(eepromOffset+6, maxTemp);
}

void AutomaticFan::setManualOverride(bool override)
{
	manualOverride = override;
	EEPROM.put(eepromOffset+10, manualOverride);

	updateManualSpeed();
}

void AutomaticFan::setManualSpeed(uint8_t speed)
{
	manualSpeed = speed;
	EEPROM.put(eepromOffset+11, manualSpeed);

	updateManualSpeed();
}


String AutomaticFan::getEEPROMKey(String baseKey)
{
	return ("f" + String(pin) + baseKey);
}

void AutomaticFan::setFanSpeed(uint8_t speed)
{
	analogWrite(pin, speed);
	currentFanSpeed = speed;
}

void AutomaticFan::updateManualSpeed()
{
	if (!manualOverride) return;

	setFanSpeed(manualSpeed);
}