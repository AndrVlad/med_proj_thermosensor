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
#include <stdbool.h>
#include <stdint.h>

volatile uint16_t ADC_data_safe;
bool need_selfcheck = 0;

static float Krt_r25[10] = {4.232, 3.265, 2.539, 1.99, 1.571,
                     1.249, 1.0, 0.8057, 0.6531, 0.5327};
float temp_val = 0;
float getTempVal() {

	float RT_R25 = (float)(ADC_data_safe)/(float)(4095-ADC_data_safe); //R_t/R25
	float t = 0;
	for ( int i = 9; i >= 0; i-- )
	{
		if ( Krt_r25[i] >= RT_R25 ) //we are in true band
		{
		float tga = 5/(Krt_r25[i+1] - Krt_r25[i]); //k = calc td(a)
		float b = (i-1)*5 - tga*Krt_r25[i]; // calc b = offset
		t = tga * RT_R25 + b;
		break;
		}
	}

	return t;
}

/* Выполняет проверку работоспособности датчика
 * Возврат: 1 - датчик работоспособен
 * 			0 - датчик не работоспособен */
bool sensorSelfCheck() {

#ifdef TEST_VERSION
	return true;
#endif

	// устанавливаем блокировку от перезаписи последнего полученного значения АЦП
	need_selfcheck = 1;

	// проверка на работоспособность АЦП
	if (ADC_data_safe == 4095) {
		need_selfcheck = 0;
		return false;
	}
	temp_val = getTempVal();
	if (temp_val < 30.0f || temp_val > 50.0f) {
		need_selfcheck = 0;
		return false;
	}
	need_selfcheck = 0;
	return true;
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

