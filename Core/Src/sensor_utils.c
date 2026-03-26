/*
 * sensor_utils.c
 *
 *  Created on: 20 мар. 2026 г.
 *      Author: AVA
 */

#include "sensor_utils.h"
#include "main.h"
#include "w25q_spi.h"
#include "protocol_parser.h"
#include "SPI_Connection.h"
#include <stdint.h>

/* Выполняет проверку работоспособности датчика
 * Возврат: 1 - датчик работоспособен
 * 			0 - датчик не работоспособен */
bool sensorSelfCheck() {

	// код для выполнения проверки датчика ...

	if(true) {	// проверка успешна
		return 1;
	} else {
		return 0;
	}
}

/* Выполняет сброс датчика */
void resetSensor() {

	stopMeasurement();

    // сброс внутреннего состояния датчика
    resetFSMProtocol();

    // сброс переменных
    page_pos_ptr = 0;
    page_ptr = 0;

	// очистка флеш-памяти
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    W25_Erase_Chip();

    reset_ready = true;

    // сброс SPI-соединения с мастером
    //resetSPIConnection();


	return;
}
/* Запуск измерения */
void startMeasurement() {
	dt1[0] = '1';
	return;
}

/* Остановка измерения */
void stopMeasurement() {
	dt1[0] = '2';
	return;
}

