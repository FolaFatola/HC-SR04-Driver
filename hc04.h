/*
 * hc04.h
 *
 *  Created on: Aug 30, 2023
 *      Author: folafatola
 */

#ifndef INC_HC04_H_
#define INC_HC04_H_

#include "main.h"


#define sound_speed_cmpms 0.034
#define timeout_value 1000000


typedef enum{
	ECHO_READ = 0,
	ECHO_PULSE_NOT_DETECTED = 1,
	ECHO_PULSE_NOT_COMPLETE = 2
} Hc04_StageTypeDef;

typedef enum{
	HC04_OK = 0,
	HC04_ERROR = 1
} Hc04_StatusTypeDef;

typedef struct{
	Hc04_StageTypeDef stage;
	Hc04_StatusTypeDef status;
	float unfiltered_distance;
	float distance;
} Hc04_TypeDef;

extern Hc04_TypeDef hc04;

void hc04_init(TIM_HandleTypeDef *htim, uint16_t tim_channel, Hc04_TypeDef *hc04_obj, uint8_t compare_val);

void send_pulse(TIM_HandleTypeDef *htim,
		        Hc04_TypeDef *hc04_obj,
			    uint16_t tim_channel,
			    GPIO_TypeDef* trig_pin_port,
				uint16_t trig_pin);

Hc04_StatusTypeDef read_trig(TIM_HandleTypeDef *htim,
							 Hc04_TypeDef *hc04_obj,
							 GPIO_TypeDef* echo_pin_port,
		                     uint16_t echo_pin);

Hc04_StatusTypeDef get_distance(TIM_HandleTypeDef *htim,
				   Hc04_TypeDef *hc04_obj,
				   uint16_t tim_channel,
				   uint8_t compare_val,
				   GPIO_TypeDef* echo_pin_port,
				   GPIO_TypeDef* trig_pin_port,
				   uint16_t echo_pin,
				   uint16_t trig_pin);



#endif /* INC_HC04_H_ */
