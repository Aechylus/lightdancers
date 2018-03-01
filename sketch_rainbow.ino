#include <Adafruit_CircuitPlayground.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#define PI 3.14159265358979323846
#define TIME_STEP 5

void setup() {
    CircuitPlayground.begin();
    //Serial.begin(9600);
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

void loop() {
    int speed = 3;
    bool timeout = true;
    int count = 0;
    unsigned long previousMillisLED = 0;
    unsigned long previousMillisSpeed = 0;
    unsigned long currentMillis = 0;
    while (true) {
        // for (int i=0; i<360; i+=3) {
        currentMillis = millis();
        if (currentMillis - previousMillisLED >= TIME_STEP) {
            previousMillisLED = currentMillis;

            CircuitPlayground.clearPixels();
            for (int j=0; j<10; j++) {
                lightColorChanger(i, j);
            }
            count += speed;
            if (count >= 360) {
                count = 0;
            }
        }

        if (currentMillis - previousMillisSpeed >= 500) {
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

        uint16_t spectrum[32];
        int avg = 0;
        CircuitPlayground.mic.fft(spectrum);
        //creating rolling average


        // for(int k=0; k < 32; k++) {// Add for an average
        //     if(spectrum[k] > 255) {
        //         spectrum[k] = 255; // limit outlier data
        //     }
        //     avg += spectrum[k];
        // }
        // avg/=32;

        CircuitPlayground.setBrightness(abs(avg-16)+8);
    }
}
