/*
 * hc04.c
 *
 *  Created on: Aug 30, 2023
 *      Author: folafatola
 */

#include "hc04.h"


//first, we could set this up based on the timing requirements.
//first of all, we need the unit, and we need the centimeter.

Hc04_TypeDef hc04;

//hc04.stage = PULSE_NOT_SENT;
//hc04.status = HC04_NOT_OK;
//hc04.unfiltered_distance = 0;
//hc04.distance = 0;

void hc04_init(TIM_HandleTypeDef *htim, uint16_t tim_channel, Hc04_TypeDef *hc04_obj, uint8_t compare_val){

	HAL_TIM_PWM_Start_IT(htim, tim_channel);
	hc04_obj->stage = ECHO_PULSE_NOT_DETECTED;
	hc04_obj->status = HC04_ERROR;
	hc04_obj->unfiltered_distance = 0;
	hc04_obj->distance = 0;
	HAL_TIM_PeriodElapsedCallback(htim);
}

void send_pulse(TIM_HandleTypeDef *htim,
		  Hc04_TypeDef *hc04_obj,
		  uint16_t tim_channel,
		  GPIO_TypeDef* trig_pin_port,
		  uint16_t trig_pin)
{
	__HAL_TIM_SET_COMPARE(htim, tim_channel, 10);
	__HAL_TIM_SET_COUNTER(htim, 0);
	hc04_obj->status = HC04_OK;
}

Hc04_StatusTypeDef read_trig(TIM_HandleTypeDef *htim,
						     Hc04_TypeDef *hc04_obj,
						     GPIO_TypeDef* echo_pin_port,
				             uint16_t echo_pin){

	uint32_t echo_pin_high_tim_value = 0;

	hc04_obj->stage = ECHO_PULSE_NOT_DETECTED;

	HAL_GPIO_WritePin(echo_pin_port, echo_pin, GPIO_PIN_RESET);

	//here you want a buffer period and so you need a timeout value.

	while(HAL_GPIO_ReadPin(echo_pin_port, echo_pin) == 0 && __HAL_TIM_GET_COUNTER(htim) < timeout_value);
	__HAL_TIM_SET_COUNTER(htim, 0);


	if(__HAL_TIM_GET_COUNTER(htim) >= timeout_value){
		hc04_obj->status = HC04_ERROR;
		HAL_TIM_PeriodElapsedCallback(htim);
		return hc04_obj->status;
	}




	while(HAL_GPIO_ReadPin(echo_pin_port, echo_pin) == 1 && __HAL_TIM_GET_COUNTER(htim) < timeout_value);

		//probably not going to happen.
	if(__HAL_TIM_GET_COUNTER(htim) >= timeout_value){
		hc04_obj->stage = ECHO_PULSE_NOT_COMPLETE;
		hc04_obj->status = HC04_ERROR;
		HAL_TIM_PeriodElapsedCallback(htim);
		return hc04_obj->status;
	}

	echo_pin_high_tim_value = __HAL_TIM_GET_COUNTER(htim);


	hc04_obj->unfiltered_distance = echo_pin_high_tim_value;
	hc04_obj->stage = ECHO_READ;

	HAL_TIM_PeriodElapsedCallback(htim);


	return hc04_obj->status;
}

Hc04_StatusTypeDef get_distance(TIM_HandleTypeDef *htim,
				   Hc04_TypeDef *hc04_obj,
				   uint16_t tim_channel,
				   uint8_t compare_val,
				   GPIO_TypeDef* echo_pin_port,
				   GPIO_TypeDef* trig_pin_port,
				   uint16_t echo_pin,
				   uint16_t trig_pin)
{


	send_pulse(htim, hc04_obj, tim_channel, trig_pin_port, trig_pin);


	Hc04_StatusTypeDef my_status = read_trig(htim, hc04_obj, echo_pin_port, echo_pin);
	if(my_status == HC04_OK){
		hc04_obj->distance = (hc04_obj->unfiltered_distance * sound_speed_cmpms) / 2;
		return my_status;
	}else{
		return my_status;
	}

}



