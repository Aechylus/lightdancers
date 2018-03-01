#include <Adafruit_CircuitPlayground.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#define PI 3.14159265358979323846
#define TIME_STEP 5
#define BUTTON_DELAY 250
#define BUFFER_LENGTH 100
#define FFT_DELAY 10

int speed = 3;
int countLED = 0;
int countFFT = 0;
unsigned long previousMillisLED = 0;
unsigned long previousMillisSpeed = 0;
unsigned long previousMillisFFT = 0;
unsigned long currentMillis = 0;
int buffer[BUFFER_LENGTH];
int avg = 0;

void setup() {
    CircuitPlayground.setBrightness(64);
    for (int i=0; i<BUFFER_LENGTH; i++) {
        buffer[i] = soundLevelAverage();
        delay(FFT_DELAY);
    }
    CircuitPlayground.begin();
}

int soundLevelAverage() {
    uint16_t spectrum[32];
    CircuitPlayground.mic.fft(spectrum);
    avg = 0;
    for (int i=0; i<32; i++) {
        avg += (spectrum[i]*(32-i));
    }
    avg /= 32;
    return avg;
}

int sinCalc(double x) {
    double arg;
    double res;
    double scaled;

    arg = (x / 360) * 2 * PI;
    res = sin(arg);
    scaled = (res*127.5) + 127,5;

    return (int)scaled;
}

void lightColorChanger(int i, int n) {
    int offset = 120;
    int shift = n*36;
    int red = sinCalc(i+shift);
    int green = sinCalc(i+shift+offset);
    int blue = sinCalc(i+shift+2*offset);

    CircuitPlayground.setPixelColor(n, red, green, blue);
}

void revolutionSpeedController() {
    if (CircuitPlayground.leftButton()) {
        if (speed < 10) {
            speed++;
        }
    }
    if (CircuitPlayground.rightButton()) {
        if (speed > 1) {
            speed--;
        }
    }
}

void loop() {
    currentMillis = millis();
    if (currentMillis - previousMillisLED >= TIME_STEP) {
        previousMillisLED = currentMillis;

        CircuitPlayground.clearPixels();
        for (int n=0; n<10; n++) {
            lightColorChanger(countLED, n);
        }
        countLED += speed;
        if (countLED >= 360) {
            countLED = 0;
        }
    }

    if (currentMillis - previousMillisSpeed >= BUTTON_DELAY) {
        previousMillisSpeed = currentMillis;
        revolutionSpeedController();
    }

    if (currentMillis - previousMillisFFT >= FFT_DELAY) {
        buffer[countFFT] = soundLevelAverage();
        countFFT ++;
        if (countFFT >= BUFFER_LENGTH) {
            countFFT = 0;
        }
    }

    
}
