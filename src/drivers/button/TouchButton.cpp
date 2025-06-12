#include "TouchButton.h"

// TouchButton.cpp
TouchButton::TouchButton(uint8_t touchPin, uint16_t threshold)
    : _pin(touchPin), _threshold(threshold) {}

void TouchButton::attachClick(void (*func)()) { _clickFunc = func; }
void TouchButton::attachDoubleClick(void (*func)()) { _doubleClickFunc = func; }
void TouchButton::attachLongPress(void (*func)()) { _longPressFunc = func; }

void TouchButton::tick() {
    bool pressed = touchRead(_pin) < _threshold;
    unsigned long now = millis();
    static bool lastPressed = false;
    static unsigned long lastRelease = 0;

    switch (_state) {
        case 0: // idle
            if (pressed) {
                _startTime = now;
                _state = 1;
            }
            break;
        case 1: // pressed
            if (!pressed) {
                if (now - _startTime > 800) { // 长按
                    if (_longPressFunc) _longPressFunc();
                    _state = 0;
                } else {
                    _clickCount++;
                    lastRelease = now;
                    _state = 2;
                }
            }
            break;
        case 2: // wait for double click
            if (pressed) {
                _startTime = now;
                _state = 1;
            } else if (now - lastRelease > 300) {
                if (_clickCount == 1 && _clickFunc) _clickFunc();
                else if (_clickCount == 2 && _doubleClickFunc) _doubleClickFunc();
                _clickCount = 0;
                _state = 0;
            }
            break;
    }
}