#include "Logger.h"

usb_serial_class* Logger::serial = nullptr;
HardwareSerial*   Logger::hw_mirrors[MAX_HW_SERIAL];
uint8_t           Logger::num_hw_mirrors = 0;

/**
 * @brief Initialize the logger with the given USB serial object.
 * @param usb_serial USB serial output
 */
void Logger::init(usb_serial_class* usb_serial) {
    serial = usb_serial;
    serial->begin(115200);
}

/**
 * @brief Add a hardware serial output to mirror the USB serial output.
 * @param hwserial Pointer to hardware serial device
 */
void Logger::add_output(HardwareSerial* hwserial) {
    hw_mirrors[num_hw_mirrors++] = hwserial;
}

/**
 * @brief Print to the USB serial output and all hardware serial mirrors.
 * @param string String to print
 */
void Logger::print(const char* string) {
    serial->print(string);
    for (uint8_t i = 0; i < num_hw_mirrors; i++) { hw_mirrors[i]->print(string); }
}

/**
 * @brief Print a line to the USB serial output and all hardware serial mirrors.
 * @param string String to print
 */
void Logger::println(const char* string) {
    serial->println(string);
    for (uint8_t i = 0; i < num_hw_mirrors; i++) { hw_mirrors[i]->println(string); }
}

/**
 * @brief Teensy LED Controller printf wrapper. Utilizes sprintf, with a maximum length of TLC_MAX_PRINTF_LEN bytes.
 * @param str Format string
 */
void tlc_printf(const char* str, ...) {
    static char buffer[TLC_MAX_PRINTF_LEN];
    va_list args;

    // format output string
    va_start(args, str);
    vsprintf(buffer, str, args);
    va_end(args);

    // log to serial output
    Logger::print(buffer);
}
