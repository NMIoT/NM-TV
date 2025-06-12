// TouchButton.h
#ifndef _TOUCH_BUTTON_H
#define _TOUCH_BUTTON_H
#include <Arduino.h>

class TouchButton {
public:
    TouchButton(uint8_t touchPin, uint16_t threshold = 40);
    void tick();
    void attachClick(void (*func)());
    void attachDoubleClick(void (*func)());
    void attachLongPress(void (*func)());
private:
    uint8_t _pin;
    uint16_t _threshold;
    unsigned long _lastTouchTime = 0;
    unsigned long _startTime = 0;
    int _state = 0;
    int _clickCount = 0;
    void (*_clickFunc)() = nullptr;
    void (*_doubleClickFunc)() = nullptr;
    void (*_longPressFunc)() = nullptr;
};
#endif // _TOUCH_BUTTON_H