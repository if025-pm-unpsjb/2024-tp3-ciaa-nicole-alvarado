/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "sapi.h"  // Biblioteca sAPI para la plataforma

int main(void) {
    // Inicializar y configurar la placa de desarrollo
    boardConfig();

    // Inicializar UART para comunicación serie
    uartConfig(UART_USB, 9600);  // Configurar UART_USB a 115200 bps

    while (true) {
        // Variable para almacenar el dato recibido
        uint8_t receivedByte;

        // Verificar si hay datos disponibles en la UART
        if (uartReadByte(UART_USB, &receivedByte)) {
            // Convertir el valor ASCII recibido al número del LED
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
                    uartWriteString(UART_USB, "CIAA\n");
                    break;
                default:
                    uartWriteString(UART_USB, "Led invalido\n");
                    break;
            }
        }
    }

    return 0;
}
