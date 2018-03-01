#include <Adafruit_CircuitPlayground.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#define PI 3.14159265358979323846
#define TIME_STEP 15
#define BUTTON_DELAY 250
#define BUFFER_LENGTH 10
#define FFT_DELAY 4
#define MAX_BRIGHTNESS 128

int speed = 3;
int countLED = 0;
int countFFT = 0;
unsigned long previousMillisLED = 0;
unsigned long previousMillisSpeed = 0;
unsigned long previousMillisFFT = 0;
unsigned long currentMillis = 0;
int buffer[BUFFER_LENGTH];
int avg = 0;

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

int soundLevelAverage() {
    uint16_t spectrum[32];
    CircuitPlayground.mic.fft(spectrum);
    avg = 0;
    avg += spectrum[0] * 2;
    avg += spectrum[1] * 1.5;
    avg += spectrum[2];
    avg /= 3;
    return avg;
}

int bufferAverage() {
    avg = 0;
    for (int i=0; i<BUFFER_LENGTH; i++) {
        avg += buffer[i];
    }
    avg = avg / BUFFER_LENGTH;
    return avg;
}

void setup() {
    CircuitPlayground.setBrightness(64);
    for (int i=0; i<BUFFER_LENGTH; i++) {
        buffer[i] = soundLevelAverage();
        delay(FFT_DELAY);
    }
    CircuitPlayground.begin();
    Serial.begin(9600);
}

void loop() {
    currentMillis = millis();
    //Light color
    if (currentMillis - previousMillisLED >= TIME_STEP) {
        previousMillisLED = currentMillis;

        CircuitPlayground.clearPixels();
        for (int n=0; n<10; n++) {
            lightColorChanger(countLED, n);
        }
        int brightness = 4*(bufferAverage()-16) > 0 ? 3*(bufferAverage()-16) : 0;
        Serial.print(brightness);
        Serial.println();
        CircuitPlayground.setBrightness(brightness);
        lightColorChanger(countLED, 9);
        countLED += speed;
        if (countLED >= 360) {
            countLED = 0;
        }
    }

    //Revolution speed
    if (currentMillis - previousMillisSpeed >= BUTTON_DELAY) {
        previousMillisSpeed = currentMillis;

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

    //FFT
    if (currentMillis - previousMillisFFT >= FFT_DELAY) {
        previousMillisFFT = currentMillis;

        buffer[countFFT] = soundLevelAverage();
        countFFT ++;
        if (countFFT >= BUFFER_LENGTH) {
            countFFT = 0;
        }
    }
}
