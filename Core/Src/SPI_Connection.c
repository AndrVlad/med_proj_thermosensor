/*
 * SPI_Connection.c
 *
 *  Created on: Mar 6, 2026
 *      Author: AVA
 */

#include "SPI_Connection.h"

bool spi_rx_complete = false;
bool spi_state;

uint8_t response_frame[264] = {};		// буфер для хранения команды от мастера
uint8_t safe_response_frame[264] = {};
uint8_t dummy_frame[264] = {};			// буфер-заглушка для ответа мастеру
uint8_t trash_frame[264] = {};			// буфер для приема заглушки от мастера
uint8_t *spi_tx_ptr, *spi_rx_ptr;		// указатели на буфера для передачи по SPI
bool cs_selected = false;

void switchBuffer(bool spi_state);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // Мастер опустил CS
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET) {
	// Запускаем прием/передачу
		HAL_SPI_TransmitReceive_IT(&hspi2, spi_tx_ptr, spi_rx_ptr, FRAME_LEN);
		cs_selected = true;
    }
}

void fillBuffer(uint8_t* buf, uint8_t fill_symbol) {
	for(int i = 0; i < FRAME_LEN; i++) {
		buf[i] = fill_symbol;
	}
}
void initSPIConnection() {
	spi_state = SPI_MODE_RX;
	switchBuffer(spi_state);
	fillBuffer(dummy_frame, 0xA);
};

void switchBuffer(bool spi_state) {
	if(spi_state == SPI_MODE_TX) {
		spi_tx_ptr = &response_frame[0];
		spi_rx_ptr = &trash_frame[0];
	} else {
		spi_tx_ptr = &dummy_frame[0];
		spi_rx_ptr = &response_frame[0];
	}
};
/* Обработчик прерывания по окончанию передачи/приема всего кадра */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2)
    {
        if (spi_state == SPI_MODE_TX) {
        	// сделать датчик приемником в следующем сеансе
        	spi_state = SPI_MODE_RX;
        } else {
        	// сделать датчик передатчиком в следующем сеансе
        	spi_state = SPI_MODE_TX;
        	memcpy(safe_response_frame,response_frame,FRAME_LEN);
        	spi_rx_complete = true;
        }
        switchBuffer(spi_state);
    }
}

