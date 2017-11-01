#ifndef DISPLAY_7SEG_H
#define DISPLAY_7SEG_H

#include <cstddef>

#include "i2c_bus.h"

class Display7Seg {
public:
    Display7Seg(I2CBus& i2c_bus,
                uint8_t device_address,
                uint8_t first_digit_register);

    ~Display7Seg() = default;
    
    virtual void Init() = 0;
    virtual void SetBlinkRate(uint8_t rate) = 0;
    virtual void SetBrightness(uint8_t brightness) = 0;

    void Clear();
    void ToggleColon(bool on);
    void SetNumber(uint8_t pos, uint8_t number, bool dot);
    void SetNumber(uint16_t number);
    void SetChar(uint8_t pos, char ch);
    void SetSegment(uint8_t pos, uint8_t segment, bool on, bool dot);
    void WriteDisplay();

protected:
    I2CBus& i2c_bus_;
    uint8_t device_address_;
    uint8_t first_digit_register_;
    
    uint16_t display_buffer_[4];
};

#endif
