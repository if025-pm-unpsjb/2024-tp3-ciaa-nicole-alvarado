/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "FreeRTOS.h"
#include "task.h"
#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI
#include "semphr.h"	// Para la gestión de semáforos

/* Tasks periods for STR 1. */
#define TASK1_PERIOD 	4000
#define TASK2_PERIOD 	6000
#define TASK3_PERIOD 	8000
#define TASK4_PERIOD	13000

/* Tasks WCETs. */
#define TASK1_WCET		1000
#define TASK2_WCET		1000
#define TASK3_WCET		1000
#define TASK4_WCET		3000

void task(void *p);

// Declaración de un semáforo
SemaphoreHandle_t s;

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();
        // Configurar el módulo PWM (Pulse Width Modulation) en modo habilitado
        pwmConfig(0, PWM_ENABLE);
        // Habilitar la salida PWM en el canal PWM7
        pwmConfig(PWM7, PWM_ENABLE_OUTPUT);
        // Establecer un valor inicial del ciclo de trabajo para PWM7
        pwmWrite(PWM7, 10);
        // Crear un mutex (semáforo de exclusión mutua)
    	s = xSemaphoreCreateMutex();
    	// Crear una tarea
    	// - Nombre de la tarea: "T1".
    	// - Tamaño de pila: 256 bytes.
    	// - Parámetros: NULL.
    	// - Prioridad: `configMAX_PRIORITIES - 1`.
    	// - Puntero al identificador de la tarea: NULL.
        xTaskCreate( task, "T1", 256, NULL, configMAX_PRIORITIES - 1, NULL );
        // Iniciar el planificador de tareas de FreeRTOS
        vTaskStartScheduler();
        for(;;);
}

// Función de tarea que controla el ciclo de trabajo de PWM
void task(void *p)
{
	// Configurar un período de 10 ticks para la tarea
	TickType_t xPeriod = 10;

	// Iniciar el ciclo de trabajo (duty cycle) en 0
	int dutyCicle = 0;

	while( TRUE ) {
		// Escribir el valor actual del ciclo de trabajo en el canal PWM7
		pwmWrite(PWM7, dutyCicle);

		vTaskDelay( xPeriod );

		// Incrementar el ciclo de trabajo, reiniciándolo a 0 si alcanza 255
		dutyCicle = ( dutyCicle + 1 ) % 255;
	}
}
