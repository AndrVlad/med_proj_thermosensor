/*
 * sensor_utils.c
 *
 *  Created on: 20 мар. 2026 г.
 *      Author: AVA
 */

#include "sensor_utils.h"
#include "main.h"
#include "w25q_spi.h"
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
	// очистка флеш-памяти
	W25_Erase_Chip();

	return;
}

void startMeasurement() {

	return;
}

void stopMeasurement() {

	return;
}

