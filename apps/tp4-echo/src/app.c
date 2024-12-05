/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */


#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI

#define mainMAX_MSG_LEN 25

int main(void) {
    // Inicializar y configurar la placa de desarrollo
    boardConfig();

    // Inicializar UART para comunicación serie
    uartConfig(UART_USB, 9600);

    // Buffer para almacenar los datos recibidos
    char buffer[mainMAX_MSG_LEN];
    uint8_t index = 0;

    while (true) {
        // Verificar si hay datos disponibles en la UART
        if (uartReadByte(UART_USB, (uint8_t*)&buffer[index])) {
            // Si se recibe un terminador '\r', enviar el mensaje de vuelta
            if (buffer[index] == '\r') {
            	// Agregar terminador de cadena
                buffer[index + 1] = '\0';
                // Enviar mensaje completo
                uartWriteString(UART_USB, buffer);
                index = 0;
            } else if (index < (mainMAX_MSG_LEN - 1)) {
                index++;
            } else {
                // Si el mensaje supera el tamaño máximo, reiniciar el buffer
                index = 0;
            }
        }
    }
    return 0;
}
