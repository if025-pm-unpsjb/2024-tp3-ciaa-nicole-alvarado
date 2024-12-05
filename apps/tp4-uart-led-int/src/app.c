/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "sapi.h"

// Prototipo de la función callback
void onUartReceive(void* unused);

int main(void) {
    // Inicializar y configurar la placa de desarrollo
    boardConfig();

    // Inicializar UART para comunicación serie
    uartConfig(UART_USB, 9600);

    // Habilitar interrupciones UART
    uartInterrupt(UART_USB, true);

    // Configurar callback para evento UART_RECEIVE
    uartCallbackSet(UART_USB, UART_RECEIVE, onUartReceive, NULL);

    while (true) {

    }
    return 0;
}

// Función callback para manejar la recepción de datos UART
void onUartReceive(void* unused) {
    // Leer el byte recibido desde UART
    uint8_t receivedByte = uartRxRead(UART_USB);

    // Alternar el estado del LED según el valor recibido
    switch (receivedByte) {
        case '1':
            gpioToggle(LED1);
            uartWriteString(UART_USB, "LED1\n");
            break;
        case '2':
            gpioToggle(LED2);
            uartWriteString(UART_USB, "LED2\n");
            break;
        case '3':
            gpioToggle(LED3);
            uartWriteString(UART_USB, "LED3\n");
            break;
        case '4':
            gpioToggle(CIAA_BOARD_LED);
            uartWriteString(UART_USB, "LEDR\n");
            break;
        default:
            uartWriteString(UART_USB, "Led invalido\n");
            break;
    }
}


