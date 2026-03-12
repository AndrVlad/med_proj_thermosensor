/*
 * protocol_parser.h
 *
 *  Created on: Mar 12, 2026
 *      Author: vlado
 */

#ifndef INC_PROTOCOL_PARSER_H_
#define INC_PROTOCOL_PARSER_H_

void parser(uint8_t* command_frame);
// открыто здесь для тестов
bool checkCRC32(uint8_t* command_frame, uint16_t length);
// открыто здесь для тестов
uint32_t calculateCRC32(uint8_t* arg,uint16_t length);

uint16_t sensorSelfCheck();

#endif /* INC_PROTOCOL_PARSER_H_ */
