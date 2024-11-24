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
#define TASK1_WCET		900
#define TASK2_WCET		900
#define TASK3_WCET		900
#define TASK4_WCET		2900

#define LED_INTENSIDAD_MAX 255
#define LED_INTENSIDAD_MEDIA 32
#define LED_INTENSIDAD_MIN 0

/* Declaración de funciones de las tareas. */
static void vBusyWait(TickType_t ticks);
void task1(void *pvParameters);
void task2(void *pvParameters);
void task3(void *pvParameters);
void task4(void *pvParameters);

// Declaración de un manejador de semáforo
SemaphoreHandle_t s;

int main( void )
{
        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        // Configuración de módulo PWM (Modulación por Ancho de Pulso)
        pwmConfig(0, PWM_ENABLE);          // Habilita el módulo PWM general


        pwmConfig(PWM7, PWM_ENABLE_OUTPUT);// Habilita la salida específica PWM7
        pwmConfig(PWM8, PWM_ENABLE_OUTPUT);
        pwmConfig(PWM9, PWM_ENABLE_OUTPUT);

        // Establecer un valor inicial del ciclo de trabajo para PWM7 (10%)
        //pwmWrite(PWM7, 10);

        // Crear un mutex (semáforo de exclusión mutua)
    	s = xSemaphoreCreateMutex();

        xTaskCreate( task1, "T1", 256, NULL, configMAX_PRIORITIES - 1, NULL );
        xTaskCreate( task2, "T2", 256, NULL, configMAX_PRIORITIES - 2, NULL );
        xTaskCreate( task3, "T3", 256, NULL, configMAX_PRIORITIES - 3, NULL );
        xTaskCreate( task4, "T4", 256, NULL, configMAX_PRIORITIES - 4, NULL );

        // Iniciar el planificador de tareas de FreeRTOS
        vTaskStartScheduler();
        for(;;);
}


/* Simular trabajo de la tarea */
static void vBusyWait(TickType_t ticks)
{
    TickType_t elapsedTicks = 0;
    TickType_t currentTick = 0;
    while (elapsedTicks < ticks) {
        currentTick = xTaskGetTickCount();
        while (currentTick == xTaskGetTickCount()) {
            __asm__("nop");
        }
        elapsedTicks++;
    }
}


// Función de tarea que controla el ciclo de trabajo de PWM
/*void task(void *p)
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
}*/

void task1(void *pvParameters)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    unsigned int uxReleaseCount = 0;
    for (;;) {
        // Reducir intensidad de otros LEDs
        if (pwmRead(PWM7) == LED_INTENSIDAD_MAX) pwmWrite(PWM7, LED_INTENSIDAD_MEDIA);
    	if (pwmRead(PWM8) == LED_INTENSIDAD_MAX) pwmWrite(PWM8, LED_INTENSIDAD_MEDIA);
        if (pwmRead(PWM9) == LED_INTENSIDAD_MAX) pwmWrite(PWM9, LED_INTENSIDAD_MEDIA);

        // Encender LED
        gpioWrite(CIAA_BOARD_LED, ON);

        // Simular trabajo
        vBusyWait(TASK1_WCET);

        // Apagar LED
        gpioWrite(CIAA_BOARD_LED, OFF);

        // Restaurar intensidad de otros LEDs
        if (pwmRead(PWM7) == LED_INTENSIDAD_MEDIA) pwmWrite(PWM7, LED_INTENSIDAD_MAX);
        if (pwmRead(PWM8) == LED_INTENSIDAD_MEDIA) pwmWrite(PWM8, LED_INTENSIDAD_MAX);
        if (pwmRead(PWM9) == LED_INTENSIDAD_MEDIA) pwmWrite(PWM9, LED_INTENSIDAD_MAX);

        vTaskDelayUntil(&pxPreviousWakeTime, pdMS_TO_TICKS(TASK1_PERIOD));
        uxReleaseCount++;
    }
}

void task2(void *pvParameters)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    unsigned int uxReleaseCount = 0;
    for (;;) {
        if (gpioRead(CIAA_BOARD_LED)) gpioWrite(CIAA_BOARD_LED, OFF);
        if (pwmRead(PWM8) == LED_INTENSIDAD_MAX) pwmWrite(PWM8, LED_INTENSIDAD_MEDIA);
        if (pwmRead(PWM9) == LED_INTENSIDAD_MAX) pwmWrite(PWM9, LED_INTENSIDAD_MEDIA);

        pwmWrite(PWM7, LED_INTENSIDAD_MAX);
        vBusyWait(TASK2_WCET);
        pwmWrite(PWM7, LED_INTENSIDAD_MIN);

        if (gpioRead(CIAA_BOARD_LED) == 0) gpioWrite(CIAA_BOARD_LED, ON);
        if (pwmRead(PWM8) == LED_INTENSIDAD_MEDIA) pwmWrite(PWM8, LED_INTENSIDAD_MAX);
        if (pwmRead(PWM9) == LED_INTENSIDAD_MEDIA) pwmWrite(PWM9, LED_INTENSIDAD_MAX);

        vTaskDelayUntil(&pxPreviousWakeTime, pdMS_TO_TICKS(TASK2_PERIOD));
        uxReleaseCount++;
    }
}

void task3(void *pvParameters)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    unsigned int uxReleaseCount = 0;
    for (;;) {
        if (gpioRead(CIAA_BOARD_LED)) gpioWrite(CIAA_BOARD_LED, OFF);
        if (pwmRead(PWM7) == LED_INTENSIDAD_MAX) pwmWrite(PWM7, LED_INTENSIDAD_MEDIA);
        if (pwmRead(PWM9) == LED_INTENSIDAD_MAX) pwmWrite(PWM9, LED_INTENSIDAD_MEDIA);

        pwmWrite(PWM8, LED_INTENSIDAD_MAX);
        vBusyWait(TASK3_WCET);
        pwmWrite(PWM8, LED_INTENSIDAD_MIN);

        if (gpioRead(CIAA_BOARD_LED) == 0) gpioWrite(CIAA_BOARD_LED, ON);
        if (pwmRead(PWM7) == LED_INTENSIDAD_MEDIA) pwmWrite(PWM7, LED_INTENSIDAD_MAX);
        if (pwmRead(PWM9) == LED_INTENSIDAD_MEDIA) pwmWrite(PWM9, LED_INTENSIDAD_MAX);

        vTaskDelayUntil(&pxPreviousWakeTime, pdMS_TO_TICKS(TASK3_PERIOD));
        uxReleaseCount++;
    }
}

void task4(void *pvParameters)
{
    TickType_t pxPreviousWakeTime = xTaskGetTickCount();
    unsigned int uxReleaseCount = 0;
    for (;;) {
        if (gpioRead(CIAA_BOARD_LED)) gpioWrite(CIAA_BOARD_LED, OFF);
        if (pwmRead(PWM7) == LED_INTENSIDAD_MAX) pwmWrite(PWM7, LED_INTENSIDAD_MEDIA);
        if (pwmRead(PWM8) == LED_INTENSIDAD_MAX) pwmWrite(PWM8, LED_INTENSIDAD_MEDIA);

        pwmWrite(PWM9, LED_INTENSIDAD_MAX);
        vBusyWait(TASK4_WCET);
        pwmWrite(PWM9, LED_INTENSIDAD_MIN);

        if (gpioRead(CIAA_BOARD_LED) == 0) gpioWrite(CIAA_BOARD_LED, ON);
        if (pwmRead(PWM7) == LED_INTENSIDAD_MEDIA) pwmWrite(PWM7, LED_INTENSIDAD_MAX);
        if (pwmRead(PWM8) == LED_INTENSIDAD_MEDIA) pwmWrite(PWM8, LED_INTENSIDAD_MAX);

        vTaskDelayUntil(&pxPreviousWakeTime, pdMS_TO_TICKS(TASK4_PERIOD));
        uxReleaseCount++;
    }
}
