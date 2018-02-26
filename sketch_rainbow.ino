
#include <Adafruit_CircuitPlayground.h>
#include <math.h>
#include <stdbool.h>
#define PI 3.14159265358979323846

void setup() {
    CircuitPlayground.begin();
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
    bool timeout = true;
    while (true) {
        for (int i=0; i<360; i+=speed) {
            CircuitPlayground.clearPixels();
            for (int j=0; j<10; j++) {
                lightColorChanger(i, j);
            }
            // CircuitPlayground.setBrightness(i*255/360);

            delay(5);
            if (timeout) {
                if (CircuitPlayground.leftButton()) {
                    if (speed < 5) {
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
        }
        timeout = true;
    }
}
