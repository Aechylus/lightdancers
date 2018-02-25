
#include <Adafruit_CircuitPlayground.h>
#include <math.h>
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

void lightColorChanger(int n, int i) {
    int offset = 120;
    int shift = n*36;
    int red = sinCalc(i+shift);
    int green = sinCalc(i+shift+offset);
    int blue = sinCalc(i+shift+2*offset);

    CircuitPlayground.setPixelColor(n, red, green, blue);
}

void loop() {
    for (int i=0; i<360; i+=4) {
        CircuitPlayground.clearPixels();
        // CircuitPlayground.setBrightness(i*255/360);

        lightColorChanger(0, i);
        lightColorChanger(1, i);
        lightColorChanger(2, i);
        lightColorChanger(3, i);
        lightColorChanger(4, i);
        lightColorChanger(5, i);
        lightColorChanger(6, i);
        lightColorChanger(7, i);
        lightColorChanger(8, i);
        lightColorChanger(9, i);

        delay(5);
    }
}
