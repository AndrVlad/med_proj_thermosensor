/*
 * SPI_connection.h
 *
 *  Created on: Mar 6, 2026
 *      Author: vlado
 */

#ifndef INC_SPI_CONNECTION_H_
#define INC_SPI_CONNECTION_H_

#include <stdio.h>
#include <stdbool.h>
#include "main.h"

#define SPI_MODE_RX 0
#define SPI_MODE_TX 1

#define FRAME_LEN 264

extern bool spi_rx_complete;

void initSPIConnection();

#endif /* INC_SPI_CONNECTION_H_ */
