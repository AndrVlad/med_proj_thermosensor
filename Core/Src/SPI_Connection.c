/*
 * SPI_Connection.c
 *
 *  Created on: Mar 6, 2026
 *      Author: AVA
 */

#include "SPI_Connection.h"

bool spi_rx_complete = false;
bool spi_state;

uint8_t response_frame[264] = {0xB};
uint8_t dummy_frame[264] = {0xA};
uint8_t *spi_tx_ptr, *spi_rx_ptr;
uint8_t trash[264] = {0};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
        // Мастер опустил CS - начинаем подготовку
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET) {
            // Запускаем прием/передачу
        	HAL_SPI_TransmitReceive_IT(&hspi2, spi_tx_ptr, spi_rx_ptr, FRAME_LEN);
    }
}

void switchBuffer(bool spi_state);
void initBuffer() {
	for(int i = 0; i < FRAME_LEN; i++) {
		dummy_frame[i] = 0xA;
		response_frame[i] = 0xB;
	}
}
void initSPIConnection() {
	spi_state = SPI_MODE_RX;
	switchBuffer(spi_state);
	//HAL_SPI_TransmitReceive_IT(&hspi2, spi_tx_ptr, spi_rx_ptr, FRAME_LEN);
	initBuffer();
};

void switchBuffer(bool spi_state) {
	if(spi_state == SPI_MODE_TX) {
		spi_tx_ptr = &response_frame[0];
		spi_rx_ptr = &dummy_frame[0];
	} else {
		spi_tx_ptr = &dummy_frame[0];
		spi_rx_ptr = &response_frame[0];
	}
};

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2)
    {
        if (spi_state == SPI_MODE_TX) {
        	spi_state = SPI_MODE_RX;
        } else {
        	spi_state = SPI_MODE_TX;
        	spi_rx_complete = true;
        }
        memcpy(trash,spi_rx_ptr,264);
        memcpy(trash,spi_tx_ptr,264);
        switchBuffer(spi_state);
        //HAL_SPI_TransmitReceive_IT(&hspi2, spi_tx_ptr, spi_rx_ptr, FRAME_LEN);
    }
}

