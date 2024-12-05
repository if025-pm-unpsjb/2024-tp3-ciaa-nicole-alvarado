#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI

int main(void) {
    // Inicializar y configurar la placa de desarrollo
    boardConfig();

    // Variables para almacenar el estado anterior de los botones
    bool button2LastState = false;
    bool button3LastState = false;
    bool button4LastState = false;
    bool ciaaButtonLastState = false;

    // Variables para almacenar el estado actual de los LEDs
    bool ciaaLedState = false;
    bool led1State = false;
    bool led2State = false;
    bool led3State = false;

    while (true) {
        // Leer el estado actual de los botones
        bool ciaaButtonState = !gpioRead(CIAA_BOARD_BUTTON);
        bool button2State = !gpioRead(TEC2);
        bool button3State = !gpioRead(TEC3);
        bool button4State = !gpioRead(TEC4);

        if (ciaaButtonState && !ciaaButtonLastState) {
            ciaaLedState = !ciaaLedState;
            gpioWrite(CIAA_BOARD_LED, ciaaLedState);
        }

        if (button2State && !button2LastState) {
            led1State = !led1State;
            gpioWrite(LED1, led1State);
        }

        if (button3State && !button3LastState) {
            led2State = !led2State;
            gpioWrite(LED2, led2State);
        }

        if (button4State && !button4LastState) {
            led3State = !led3State;
            gpioWrite(LED3, led3State);
        }

        ciaaButtonLastState = ciaaButtonState;
        button2LastState = button2State;
        button3LastState = button3State;
        button4LastState = button4State;

        delay(50);
    }

    return 0;
}
