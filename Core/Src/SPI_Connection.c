/*
 * SPI_Connection.c
 *
 *  Created on: Mar 6, 2026
 *      Author: vlado
 */
bool spi_tx_rx_complete = false;
bool spi_state = SPI_MODE_RX;

uint8_t response_frame[264] = {0}, data_frame[264] = {0};
uint8_t dummy_frame[264] = {0xA};
uint8_t spi_tx_buffer[264], spi_rx_buffer[264];

void initSPIConnection() {

}

void switchBuffer(bool spi_state) {
	if(spi_state == SPI_MODE_TX) {
		spi_tx_buffer = response_frame;
		spi_rx_buffer = dummy_frame;
	} else {
		spi_tx_buffer = dummy_frame;
		spi_rx_buffer = response_buffer;
	}
};

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI2)
    {
        spi_tx_rx_complete = true;
        if (spi_state == SPI_MODE_TX) {
        	spi_state = SPI_MODE_RX;
        } else {
        	spi_state = SPI_MODE_TX;
        }
        switchBuffer(spi_state);
        HAL_SPI_TransmitReceive_IT(&hspi2, spi_tx_buffer, spi_rx_buffer, DATA_LEN);
    }
}

