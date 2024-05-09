#include <ParallaxJoystick.hpp>

const uint8_t xInput = A0;
const uint8_t yInput = A1;
const uint8_t btnSelect = 8;
const uint8_t lBtn = 9;
const uint8_t  mBtn = 10;
const uint8_t rBtn = 11;
const int MAX_ANALOG_VAL = 1023;
const unsigned long debounceDelay = 40;

ParallaxJoystick _analogJoystick(yInput, xInput, MAX_ANALOG_VAL, LEFT);

class Button
{
  public:
    int _pin;
    int _lastButtonState;
    long _lastDebounceTime;
    Button(int pin) {
      _pin = pin;
      _lastButtonState = HIGH;
      _lastDebounceTime = 0;
    }

    bool debounce(long t, uint8_t readVal) {
      bool btnPressed = false;  // read button state
      // record switch change
      if (readVal != _lastButtonState) {
        _lastDebounceTime = t;
      }
      // if the switch has remained stable long enough
      if (t - _lastDebounceTime >= debounceDelay) {
        if (readVal != _buttonState) {
          _buttonState = readVal;
          if (_buttonState == LOW) {
            // pressed down
            btnPressed = true;
          }
        }
      }
      _lastButtonState = readVal;
      return btnPressed;
    }
  private:
    int _buttonState;
};


Button buttons[4] = {Button(btnSelect), Button(lBtn), Button(mBtn), Button(rBtn)};

void setup() {
  // put your setup code here, to run once:
  
  //Serial.begin(115200);
  Serial.begin(9600);
  pinMode(btnSelect, INPUT_PULLUP);
  pinMode(lBtn, INPUT_PULLUP);
  pinMode(mBtn, INPUT_PULLUP);
  pinMode(rBtn, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() % 400 == 0) {
      _analogJoystick.read();
    int x = _analogJoystick.getLeftRightVal();
    int xSquare = map(x, 0, _analogJoystick.getMaxAnalogValue() + 1, -2, 3);
    int y = _analogJoystick.getUpDownVal();
    int ySquare =  map(y, 0, _analogJoystick.getMaxAnalogValue() + 1, -2, 3);
    Serial.print(xSquare);
    Serial.print(" ");
    Serial.println(ySquare);
  }
  checkBtns();
  // if (checkBtns()) {
  //   Serial.println("press");
  // }
}

// const uint8_t btnSelect = 8;
// const uint8_t lBtn = 9;
// const uint8_t  mBtn = 10;
// const uint9_t rBtn = 11;

void checkBtns() {
  long t = millis();
  for (int i = 0; i < 4; i++) {
    uint8_t pin = buttons[i]._pin;
    uint8_t readVal = digitalRead(pin);
    bool press = buttons[i].debounce(t, readVal);
    if (press) {
      switch(pin) {
        case btnSelect:
          Serial.println("plot");
          break;
        case lBtn:
          Serial.println("l");
          break;
        case mBtn:
          Serial.println("m");
          break;
        case rBtn:
          Serial.println("r");
          break;
      }
    }
  }
}

//ARDUINO  EXAMPLE button debounce (https://docs.arduino.cc/built-in-examples/digital/Debounce/):

