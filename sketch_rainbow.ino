#include <Adafruit_CircuitPlayground.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#define PI 3.14159265358979323846

void setup() {
    CircuitPlayground.begin();
    Serial.begin(9600);
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
    int speed = 1;
    int brightness = 64;
    bool timeout = true;
    while (true) {
        for (int i=0; i<360; i+=10) {
            CircuitPlayground.clearPixels();
            //CircuitPlayground.setBrightness((sinCalc(i)/4)+16);
            for (int j=0; j<10; j++) {
                lightColorChanger(i, j);
            }

            delay(5);
            if (timeout) {
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
                timeout = false;
            }

            uint16_t spectrum[32];
            int avg = 0;
            CircuitPlayground.mic.fft(spectrum);
            for(int k=0; k < 32; k++) {             // Add for an average
                if(spectrum[k] > 255) {
                    spectrum[k] = 255; // limit outlier data
                }
                avg += spectrum[k];
            }
            avg/=32;

            CircuitPlayground.setBrightness((avg-16)+16);
        }
        timeout = true;
    }
}
