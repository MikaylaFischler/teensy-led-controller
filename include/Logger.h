#ifndef CONTROLLER_LOGGER_H_
#define CONTROLLER_LOGGER_H_

#include <Arduino.h>
#include <string>

#define TLC_MAX_PRINTF_LEN 256

#if defined(ARDUINO_TEENSY41)
#define MAX_HW_SERIAL 8
#else
#define MAX_HW_SERIAL 7
#endif

class Logger {
public:
    static void init(usb_serial_class* usb_serial);
    static void add_output(HardwareSerial* hwserial);
    static void print(const char* string);
    static void println(const char* string);
private:
    static usb_serial_class* serial;
    static HardwareSerial*   hw_mirrors[MAX_HW_SERIAL];
    static uint8_t           num_hw_mirrors;
};

#define tlc_print(str) Logger::print(str)
#define tlc_println(str) Logger::println(str)

void tlc_printf(const char* str, ...);

#endif // CONTROLLER_LOGGER_H_
