/*
 * protocol_parser.c
 *
 *  Created on: Feb 18, 2026
 *      Author: AVA
 * Содержит функции по разбору команд, формированию ответов
 */

#include "protocol_common.h"
#include "protocol_parser.h"
#include "SPI_Connection.h"

/* раздел объявления переменных */

uint8_t response[FRAME_LEN] = {0};

uint8_t data_frame[264] = {0};											// буфер кадра ответа для измерения											// флаг готовности ответа (true - готов, false - не готов)
uint16_t meas_request_cnt = 0;											// флаг запроса на выполнение измерения
uint16_t sensor_state = STATE_NOT_READY;								// внутреннее состояние датчика (работоспособность)
uint16_t measurement_state = STATE_NOT_READY;							// статус готовности результата измерения
uint8_t FSM_state;														// текущее состояние FSM
bool meas_data_ready = false;											// признак готовности измеренных данных

// рассчитанная таблица CRC-32
const uint32_t crc32_table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
    0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
    0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
    0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
    0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
    0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
    0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
    0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
    0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
    0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
    0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
    0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
    0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
    0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
    0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
    0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
    0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
    0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
    0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
    0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
    0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
    0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};


/* раздел объявления функций */
uint32_t calculateCRC32(uint8_t* arg,uint16_t length);

/* реализация функций */

void setFSMProtocolState(uint8_t state) {
	FSM_state = state;
}
/* Заполняет кадр ответа и уведомляет модуль SPI_connection о готовности ответа к отправке
 * Параметр 1: код состояния датчика по протоколу
 * Параметр 2: код команды, на которую дается ответ */
void fillResponseFrame(uint16_t response_code, uint16_t command_code) {

	// формирование ответа
	memset(response,0,FRAME_LEN);
	response[0] = 0xFA;
	response[1] = 0xAF;
	response[2] = response_code;

	if (command_code == CMD_STATUS) {
		// установка признака наличия хотя бы одного готового результата измерения
		response[3] = meas_data_ready;
	} else {
		response[3] = 0;
	}

	// формирование CRC для кадра в порядке MSB
	uint32_t crc = calculateCRC32(response,FRAME_LEN);
	response[260] = (crc >> 24) & 0xFF;
	response[261] = (crc >> 16) & 0xFF;
	response[262] = (crc >> 8) & 0xFF;
	response[263] = crc & 0xFF;

	// устанавливаем указатель модуля приема/передачи на буфер с подготовленным ответом
	new_response_frame = response;

	// сигнализируем модулю приема/передачи SPI о том, что ответ готов
	response_ready = true;
	return;
};
/* Подготавливает к отправке предыдущий кадр ответа */
void sendPreviousResponse() {

	// устанавливаем указатель модуля приема/передачи на буфер с прошлым ответом
	new_response_frame = response;

	// сигнализируем модулю приема/передачи SPI о том, что ответ готов
	response_ready = true;
	return;
};

void parserFSM() {
	// проверка контрольной суммы
	if(!checkCRC32(safe_command_frame, FRAME_LEN)) {
		// формирование ответа - ошибка CRC
		fillResponseFrame(CRC_ERROR, 0);
		return;
	}
	// анализ состояния датчика
	switch(FSM_state) {
	case CONNECTED_STATE:
		// анализ полученной команды
		switch (safe_command_frame[2]) {
		case CMD_GET_MEASURE:
			if(meas_data_ready) { // данные измерения есть
				fillResponseFrame(STATE_READY, CMD_GET_MEASURE);
				FSM_state = EXCHANGE_STATE;
				//fillDataFrame();
			} else {
				fillResponseFrame(STATE_NOT_READY, CMD_GET_MEASURE);
			}
			break;
		case CMD_STATUS:
			// выполнение самопроверки
			if (sensorSelfCheck()) {
				FSM_state = READY_STATE;
				fillResponseFrame(SENSOR_ID, CMD_STATUS);
			} else {
				fillResponseFrame(STATE_ERROR, CMD_STATUS);
			}
			break;
		case CMD_RESET:
			//resetSensor();
			fillResponseFrame(STATE_RESET, CMD_RESET);
			break;
		case CMD_CRC_ANS_ERR:
			sendPreviousResponse();
			break;
		}
		break;
	case READY_STATE:
		// анализ полученной команды
		switch (safe_command_frame[2]) {
		case CMD_GET_MEASURE:

			break;
		case CMD_STATUS:
			break;
		case CMD_RESET:
			//resetSensor();
			fillResponseFrame(STATE_RESET, CMD_RESET);
			FSM_state = CONNECTED_STATE;
			break;
		case CMD_CRC_ANS_ERR:
			sendPreviousResponse();
			break;
		}
		break;
	case MEASUREMENT_STATE:
		// анализ полученной команды
		switch (safe_command_frame[2]) {
		case CMD_GET_MEASURE:

			break;
		case CMD_STATUS:
			break;
		case CMD_RESET:
			//resetSensor();
			fillResponseFrame(STATE_RESET, CMD_RESET);
			FSM_state = CONNECTED_STATE;
			break;
		case CMD_CRC_ANS_ERR:
			sendPreviousResponse();
			break;
		}
		break;
	case EXCHANGE_STATE:
		// анализ полученной команды
		switch (safe_command_frame[2]) {
		case CMD_GET_MEASURE:

			break;
		case CMD_STATUS:
			break;
		case CMD_RESET:
			//resetSensor();
			fillResponseFrame(STATE_RESET, CMD_RESET);
			FSM_state = CONNECTED_STATE;
			break;
		case CMD_CRC_ANS_ERR:
			sendPreviousResponse();
			break;
		}
		break;
	case MEASUREMENT_EXCHANGE_STATE:
		// анализ полученной команды
		switch (safe_command_frame[2]) {
		case CMD_GET_MEASURE:

			break;
		case CMD_STATUS:
			break;
		case CMD_RESET:
			//resetSensor();
			fillResponseFrame(STATE_RESET, CMD_RESET);
			FSM_state = CONNECTED_STATE;
			break;
		case CMD_CRC_ANS_ERR:
			sendPreviousResponse();
			break;
		}
		break;
	}
};
/*
void parser(uint8_t* command_frame) {
	memcpy(safe_command_frame, command_frame, CONTROL_FRAME_LEN);
	// проверка контрольной суммы
	if(!checkCRC32(safe_command_frame, CONTROL_FRAME_LEN)) { 		// ошибка CRC
		// формирование ответа - ошибка CRC
		fillResponseFrame(CRC_ERROR);
	} else {														// CRC верна
		switch(safe_command_frame[2]) {
			case CMD_GET_MEASURE:

				break;
			case CMD_STATUS:
				if(meas_request_cnt != 0) { // запрос на измерение уже был
					// сформировать ответ с состоянием готовности измерения
					fillResponseFrame(measurement_state);
				} else { // запроса на измерение не было
					// выполнение проверки работоспособности датчика и обновление его статуса
					sensor_state = sensorSelfCheck();
					// формирование ответа
					fillResponseFrame(sensor_state);
				}
				break;
			case CMD_RESET:
				sensor_state = STATE_RESET;
				fillResponseFrame(sensor_state);
				handleSensorReset();
				break;
			case CMD_START_MEASURE:
				// установка состояния датчика - измерение и отправка ответа
				sensor_state = STATE_START_MEASURE;
				fillResponseFrame(sensor_state);
				// увеличение счетчика запросов на измерение
				meas_request_cnt++;
				startMeasurement();
				break;
			case CRC_ERROR: // ведущим обнаружена ошибка CRC в ответе с результатом измерения
				// повторная отправка результата измерения, принятого с ошибкой
				sendLastDataResponse();
				break;
		}
	}
}
*/

bool checkCRC32(uint8_t* command_frame, uint16_t length) {
	// получение CRC полученного кадра
	uint32_t command_crc = (uint32_t)command_frame[263] |
							((uint32_t)command_frame[262] << 8) |
							((uint32_t)command_frame[261] << 16) |
							((uint32_t)command_frame[260] << 24);

	// расчет ожидаемой CRC для полученной посылки
	uint32_t calc_crc = calculateCRC32(command_frame, FRAME_LEN);
	// сравнение полученной и ожидаемой CRC
	if (command_crc != calc_crc) {
		return false;
	} else {
		return true;
	}
};

uint32_t calculateCRC32(uint8_t* arg,uint16_t length) {
  uint32_t crc_f = 0xFFFFFFFF; 	// Начальное значение CRC
    for (int i=0;i<length;i++)
    {crc_f=crc32_table[(crc_f ^ arg[i]) & 0xFF] ^ (crc_f >> 8);}
  return ~crc_f;
}

/* Описание: выполняет проверку работоспособности датчика */
uint16_t sensorSelfCheck() {

	// код для выполнения проверки датчика ...

	if(true) {	// проверка успешна
		return true;
	} else {
		return false;
	}
}

