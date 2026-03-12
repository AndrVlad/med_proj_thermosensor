/*
 * protocol_common.h
 *
 *  Created on: Mar 12, 2026
 *      Author: vlado
 */

#ifndef INC_PROTOCOL_COMMON_H_
#define INC_PROTOCOL_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Идентификатор датчика */
#define SENSOR_ID 0x2C

/* Длина кадра ответа */
#define CONTROL_FRAME_LEN 10

/* Состояние датчика */
#define STATE_READY 		0x01 		// устройство готово
#define STATE_NOT_READY 	0x02		// устройство занято
#define STATE_RESET 		0x05		// устройство сброшено
#define STATE_START_MEASURE 0x06		// измерение начато
#define STATE_ERROR 		0x07		// ошибка

/* Ответы датчика (введенные для удобства) */
#define CRC_ERROR			0xFF

/* Команды от ведущего устройства */
#define CMD_GET_MEASURE 	0xA1 		// запрос результата измерения
#define CMD_STATUS 			0xA2		// запрос статуса
#define CMD_RESET 			0xA3		// сброс датчика
#define CMD_START_MEASURE 	0xA4		// запрос начала измерения

#endif /* INC_PROTOCOL_COMMON_H_ */
