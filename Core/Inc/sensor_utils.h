/*
 * sensor_utils.h
 *
 *  Created on: 20 мар. 2026 г.
 *      Author: AVA
 */

#ifndef INC_SENSOR_UTILS_H_
#define INC_SENSOR_UTILS_H_

#include <stdbool.h>

bool sensorSelfCheck();
void resetSensor();
void startMeasurement();
void stopMeasurement();


#endif /* INC_SENSOR_UTILS_H_ */
