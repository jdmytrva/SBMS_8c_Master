/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define THRESHOLD2 1500 //Long pressing for buttons
#define THRESHOLD 20 //Debounce for buttons


#define TIMEOUT_OFF 3600 //Sec timeout when device shutdown if no press any button and current < 30mA
#define MIN_CURRENT_OFF 20  //mA

#define CELLS_MIN_VOLTAGE_TIMER_OFF_SEC 5 //sec

#define NUMBER_OF_CELLS 8 //Number of cells
#define MAX_NUMBERS_OF_LEVELS 5// 5 or 10 ..
#define LIFEPO4 1
#define LI_ION 0

#if LIFEPO4
#define MIN_VOLTAGE 280 //270
#define MAX_VOLTAGE 360//3.6v
#define RESISTANCE 10//mOm
#define VOLTAGE_95_PERCENT 335//3.30v
#define VOLTAGE_10_PERCENT 285 //2.85v
#define VOLTAGE_CHARGE_100_PERCENT 360//3.30v
#define VOLTAGE_CHARGE_10_PERCENT 325 //2.85v

#elif LI_ION
#define MIN_VOLTAGE 320//2.8v
#define MAX_VOLTAGE 420//3.6v
#define RESISTANCE 10//mOm
#define VOLTAGE_95_PERCENT 410//3.35v
#define VOLTAGE_10_PERCENT 330 //3.00v
#else
#define MIN_VOLTAGE 280//2.8v
#define MAX_VOLTAGE 365//3.6v
#define VOLTAGE_95_PERCENT 335//3.35v
#define VOLTAGE_10_PERCENT 300 //3.00v

#endif

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char Version[] = "SBMS 8c M V1.03";



volatile uint16_t Voltage95Percent;
volatile uint16_t Voltage10Percent;

volatile uint32_t  time_sec = 0;
volatile uint32_t  Timer_Sec = 0;
volatile uint32_t  TimerForReadyMeasurement_ms = 0;
volatile uint8_t  Status_Timer_Sec = 0;
volatile uint32_t  ChargeTimeSec = 0;
volatile uint32_t  DischargeTimeSec = 0;
volatile uint32_t  DischargeTimeSec_Previous = 0;
volatile uint32_t  PowerOffTimesec = 0;
volatile uint32_t  sec = 0;
volatile uint32_t  time_min = 0;
volatile uint32_t  time_hour = 0;
volatile uint32_t BatteryCapacityDischargeCurrent = 0;
volatile uint32_t BatteryCapacityDischargeCurrentAfterPOwerUp = 0;
volatile uint32_t BatteryCapacityCharge = 0;
int16_t i_LogItems=0;

volatile int32_t SumI1 = 0;
volatile int32_t SumI2 = 0;
volatile int32_t SumI3 = 0;
volatile int32_t SumU1 = 0;
volatile int32_t SumU2 = 0;
volatile int32_t SumU3 = 0;
volatile int32_t SumU4 = 0;
volatile int32_t SumU5 = 0;
volatile int32_t SumU6 = 0;
volatile int32_t SumU7 = 0;
volatile int32_t SumU8 = 0;

volatile int16_t SumI1Counter = 0;
volatile int16_t SumI2Counter = 0;
volatile int16_t SumI3Counter = 0;
volatile int16_t SumU1Counter = 0;
volatile int16_t SumU2Counter = 0;
volatile int16_t SumU3Counter = 0;
volatile int16_t SumU4Counter = 0;
volatile int16_t SumU5Counter = 0;
volatile int16_t SumU6Counter = 0;
volatile int16_t SumU7Counter = 0;
volatile int16_t SumU8Counter = 0;
volatile int32_t U_OUTtmp = 0;

void BUT_Debrief(void);
void init_timer6();
uint8_t BUT_GetKey(void);
void Output_OFF();
void Output_ON();
void adc_func();

volatile uint8_t On_off = 0;
volatile uint8_t ChargeStatusForTimer = 0;
volatile uint8_t DisChargeStatusForTimer = 0;
uint32_t ChargeDurationSec;
uint32_t SelectedOptionValue;
uint32_t SelectedOptionValue1;

typedef enum Key_Pressed {
    KEY_BACK=1,
    KEY_NEXT,
    KEY_OK,
    KEY_UP,
	KEY_ACTION
}Key_Pressed_t ;

Key_Pressed_t pressedKey = 0;


void OFF_Itself()
{
	if (EEpromSaveStatus == 0)
	{
		if (BatteryCapacityDischargeCurrent/3600 > 200)
		{
			//SaveData.BatteryCapacityDischargePreviousValue = BatteryCapacityDischargeCurrent;
			//EEpromWrite();
			logInfo("Data saved ");
		}
	}
	EEpromSaveStatus = 1;
	logInfo("OFF All System");
	delay_ms(100);
	GPIOA->BSRR =  GPIO_BSRR_BR11;//OFF CPU

}

void AllBalansirON()
{
	GPIOC->BSRR = GPIO_BSRR_BS7;//Battery 1 Balansir ON
	GPIOC->BSRR = GPIO_BSRR_BS6;//Battery 2 Balansir ON
	GPIOB->BSRR = GPIO_BSRR_BS15;//Battery 3 Balansir ON
	GPIOB->BSRR = GPIO_BSRR_BS14;//Battery 4 Balansir ON
	GPIOB->BSRR = GPIO_BSRR_BS13;//Battery 5 Balansir ON
	GPIOB->BSRR = GPIO_BSRR_BS12;//Battery 6 Balansir ON
	GPIOB->BSRR = GPIO_BSRR_BS8;//Battery 7 Balansir ON
	GPIOB->BSRR = GPIO_BSRR_BS5;//Battery 8 Balansir ON

}
void AllBalansirOFF()
{
	GPIOC->BSRR = GPIO_BSRR_BR7;//Battery 1 Balansir OFF
	GPIOC->BSRR = GPIO_BSRR_BR6;//Battery 2 Balansir OFF
	GPIOB->BSRR = GPIO_BSRR_BR15;//Battery 3 Balansir OFF
	GPIOB->BSRR = GPIO_BSRR_BR14;//Battery 4 Balansir OFF
	GPIOB->BSRR = GPIO_BSRR_BR13;//Battery 5 Balansir OFF
	GPIOB->BSRR = GPIO_BSRR_BR12;//Battery 6 Balansir OFF
	GPIOB->BSRR = GPIO_BSRR_BR8;//Battery 7 Balansir OFF
	GPIOB->BSRR = GPIO_BSRR_BR5;//Battery 8 Balansir OFF
}



#define DEBAUNCE 3  //3 - 30mV
#define DEBAUNCE_FAIL 2 //1 - 10mV
void Balansir_handler()
{
	if(CellsDatabase[0].Voltage>(MAX_VOLTAGE+DEBAUNCE)) GPIOC->BSRR = GPIO_BSRR_BS7;//Battery 1 Balansir ON
	if(CellsDatabase[0].Voltage<(MAX_VOLTAGE-1)) GPIOC->BSRR = GPIO_BSRR_BR7;//Battery 1 Balansir OFF

	if(CellsDatabase[1].Voltage>(MAX_VOLTAGE+DEBAUNCE)) GPIOC->BSRR = GPIO_BSRR_BS6;//Battery 2 Balansir ON
	if(CellsDatabase[1].Voltage<(MAX_VOLTAGE-1)) GPIOC->BSRR = GPIO_BSRR_BR6;//Battery 2 Balansir OFF

	if(CellsDatabase[2].Voltage>(MAX_VOLTAGE+DEBAUNCE)) GPIOB->BSRR = GPIO_BSRR_BS15;//Battery 3 Balansir ON
	if(CellsDatabase[2].Voltage<(MAX_VOLTAGE-1)) GPIOB->BSRR = GPIO_BSRR_BR15;//Battery 3 Balansir OFF

	if(CellsDatabase[3].Voltage>(MAX_VOLTAGE+DEBAUNCE)) GPIOB->BSRR = GPIO_BSRR_BS14;//Battery 4 Balansir ON
	if(CellsDatabase[3].Voltage<(MAX_VOLTAGE-1)) GPIOB->BSRR = GPIO_BSRR_BR14;//Battery 4 Balansir OFF

	if(CellsDatabase[4].Voltage>(MAX_VOLTAGE+DEBAUNCE)) GPIOB->BSRR = GPIO_BSRR_BS13;//Battery 5 Balansir ON
	if(CellsDatabase[4].Voltage<(MAX_VOLTAGE-1)) GPIOB->BSRR = GPIO_BSRR_BR13;//Battery 5 Balansir OFF

	if(CellsDatabase[5].Voltage>(MAX_VOLTAGE+DEBAUNCE)) GPIOB->BSRR = GPIO_BSRR_BS12;//Battery 6 Balansir ON
	if(CellsDatabase[5].Voltage<(MAX_VOLTAGE-1)) GPIOB->BSRR = GPIO_BSRR_BR12;//Battery 6 Balansir OFF

	if(CellsDatabase[6].Voltage>(MAX_VOLTAGE+DEBAUNCE)) GPIOB->BSRR = GPIO_BSRR_BS8;//Battery 7 Balansir ON
	if(CellsDatabase[6].Voltage<(MAX_VOLTAGE-1)) GPIOB->BSRR = GPIO_BSRR_BR8;//Battery 7 Balansir OFF

	if(CellsDatabase[7].Voltage>(MAX_VOLTAGE+DEBAUNCE)) GPIOB->BSRR = GPIO_BSRR_BS5;//Battery 8 Balansir ON
	if(CellsDatabase[7].Voltage<(MAX_VOLTAGE-1)) GPIOB->BSRR = GPIO_BSRR_BR5;//Battery 8 Balansir OFF





	volatile uint8_t i = 0;
	volatile uint8_t RestoreVoltageBeforeUmax  = 0;
	for(i=0;i<ID_MAX_COUNT;i++)
	{
		if(CellsDatabase[i].Voltage>(MAX_VOLTAGE+DEBAUNCE+DEBAUNCE_FAIL))
		{
			Output_OFF();
			OverChargeStatus=1;
			logDebugD("OverCharge B",i+1,0);
		}
		if(CellsDatabase[i].Voltage <= (MAX_VOLTAGE))
		{
			RestoreVoltageBeforeUmax++;
		}

	}

	if((OverChargeStatus==1)&&(RestoreVoltageBeforeUmax == i)&&(OverDisChargeStatus==0)&&(CurrentShortStatus==0))
	{
		Output_ON();
		OverChargeStatus=0;
		logDebug("No OverCharge - Output_ON");
	}

}
void MinVoltage_handler()
{
	volatile uint8_t i = 0;
	volatile uint8_t RestoreVoltageonCells = 0;
	for(i=0;i<ID_MAX_COUNT;i++)
	{
		//logDebugD("i ",i+1,0);
		//logDebugD("CellsDatabase[i].Voltage ",CellsDatabase[i].Voltage,2);
		//logDebugD("Res ",(RESISTANCE*Module32(Battery.Current))/1000,2);
		//logDebugD("xxx ",(MIN_VOLTAGE - (RESISTANCE*Module32(Battery.Current))/1000),2);
		if(CellsDatabase[i].Voltage < (MIN_VOLTAGE - (RESISTANCE*Module32(Battery.Current))/1000))
		{
			if(CellsDatabase[i].BatVoltLowerMin_Status==0)
			{
				CellsDatabase[i].TimeSec = time_sec;
				CellsDatabase[i].BatVoltLowerMin_Status=1;
			}
		}
		else
		{
			CellsDatabase[i].BatVoltLowerMin_Status=0;
			RestoreVoltageonCells++;
		}

		if (CellsDatabase[i].BatVoltLowerMin_Status==1)
			if((time_sec-CellsDatabase[i].TimeSec)>CELLS_MIN_VOLTAGE_TIMER_OFF_SEC)
			{
				OverDisChargeStatus = 1;
				Output_OFF();
				Battery.BatteryLevel = LOW_BATTERY;
				logDebugD("OFF. Min voltage ",i+1,0);
			}
	}

	if((OverDisChargeStatus==1)&&(RestoreVoltageonCells==i)&&(OverChargeStatus==0)&&(CurrentShortStatus==0))
	{
		Output_ON();
		OverDisChargeStatus=0;
		Battery.BatteryLevel = BATTERY_OK;
		logDebugD("ON. B V > min V ",i+1,0);
	}
}
void OffByTimeOutIfNoAction_handler()
{
	if(PowerOffTimesec>TIMEOUT_OFF)
	{
		OFF_Itself();
	}
}

void RestoreAfterCurrentShort()
{
	if (CurrentShortStatus==1)
		CurrentShortTimer++;
	if ((CurrentShortTimer>5)&&(OverChargeStatus==0)&&(OverDisChargeStatus==0))
	{
		Output_ON();
		CurrentShortTimer=0;
		CurrentShortStatus = 0;
	}

}

uint8_t flash = 1;
uint32_t FlashVoltage =0 ;
int32_t step;
void VoltageLevelByLEDFlash()
{

   if (Battery.Voltage <= ID_MAX_COUNT*MIN_VOLTAGE) GPIOB->BSRR =  GPIO_BSRR_BR3;//LED OFF always
   else
   {
    	step =(10*ID_MAX_COUNT*(Voltage95Percent - Voltage10Percent))/100;//~30
		if (Battery.Voltage*10>FlashVoltage)
		{
			GPIOB->BSRR =  GPIO_BSRR_BS3;
		}else
		{
			GPIOB->BSRR =  GPIO_BSRR_BR3;
		}
		FlashVoltage = FlashVoltage+step;
		if (FlashVoltage>Voltage95Percent*ID_MAX_COUNT*10) FlashVoltage=Voltage10Percent*ID_MAX_COUNT*10;
   }

}


int16_t comp = 0;
void BUT_Debrief(void)
{
	Key_Pressed_t key;

	if (!LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_10))
		key = KEY_ACTION;
	//else if (!LL_GPIO_IsInputPinSet(GPIOB,LL_GPIO_PIN_5))
	//	key = KEY_NEXT;
	//else if (!LL_GPIO_IsInputPinSet(GPIOB,LL_GPIO_PIN_6))
	//	key = KEY_BACK;

	else
	{
		key = 0;
	}

	if (key)
	{
		if (comp > THRESHOLD2)
		{
			comp = THRESHOLD2 - 40;
			pressedKey = key;
			return;
		}
		else comp++;

		if (comp == THRESHOLD)
			{
			pressedKey = key;
			return;
		}
	}
	else comp=0;
}

Key_Pressed_t BUT_GetKey(void)
{
	if (pressedKey) PowerOffTimesec=0;
	Key_Pressed_t key = pressedKey;
	pressedKey = 0;
	return key;
}

void Start_Timer_sec()
{
	Timer_Sec = 0;
	Status_Timer_Sec = 1;
}
void ReStart_Timer_sec()
{
	Timer_Sec = 0;
	Status_Timer_Sec = 1;
}
void Stop_Timer_sec()
{

	Status_Timer_Sec = 0;

}



void SysTick_Callback()//1 mc
{
	BUT_Debrief();
	TimerForReadyMeasurement_ms++;

	//5mc
	if (Count5mSecond >= 5)
	{
		Count5mSecond = 0;

		adc_func();

	}
	//10mc
	if (Count10mSecond >= 10)
	{
		Count10mSecond = 0;
		VoltageLevelByLEDFlash();

	}
	//100mc
	if (Count100mSecond >= 100)
	{
		Count100mSecond = 0;

		if (Module16( Battery.Current) > 0  || OverChargeStatus == 1 )
			PowerOffTimesec = 0;
	}
	//1000mc  1c
	if (Count1000mSecond >= 1000)
	{
		Count1000mSecond = 0;

		OffByTimeOutIfNoAction_handler();
		PowerOffTimesec++;

		if (time_sec < 4)
			AllBalansirON();
		else
			Balansir_handler();

		MinVoltage_handler();

		if (Current < 2)
			BatteryCapacityDischargeCurrent = BatteryCapacityDischargeCurrent + Module16(Current);
		if (Current > 2)
			BatteryCapacityCharge = BatteryCapacityCharge + Module16(Current);
		if (ChargeStatusForTimer == 1)
			ChargeTimeSec++;
		if (DisChargeStatusForTimer == 1)
			DischargeTimeSec++;
		//Global timer
		if (Status_Timer_Sec == 1)
			Timer_Sec++;
		time_sec++;

		//if (time_sec%2==0) GPIOB->BSRR =  GPIO_BSRR_BS4;// Battery level
		//else GPIOB->BSRR =  GPIO_BSRR_BR4;

		if (Battery.BatteryLevel == LOW_BATTERY)
		{
			//if (time_sec%2==0) GPIOB->BSRR =  GPIO_BSRR_BS6;//Fault
			//else GPIOB->BSRR =  GPIO_BSRR_BR6;

			GPIOD->BSRR =  GPIO_BSRR_BS2;//Fault
		}
		else
		{
			GPIOD->BSRR =  GPIO_BSRR_BR2;
		}

		if (OverChargeStatus == 1)
		{
			GPIOD->BSRR =  GPIO_BSRR_BS2;//Fault
		}else
		{
			GPIOD->BSRR =  GPIO_BSRR_BR2;
		}

		//if (time_sec%2==0) GPIOB->BSRR =  GPIO_BSRR_BS7;//Charge/ Discharge
		//else GPIOB->BSRR =  GPIO_BSRR_BR7;

		//if (time_sec%2==0) GPIOB->BSRR =  GPIO_BSRR_BS8;
		//else GPIOB->BSRR =  GPIO_BSRR_BR8;




		//if (time_sec%10==0) Output_ON();
		//if (time_sec%15==0) Output_OFF();

		if (Battery.Current>2)
		{
			Voltage95Percent = VOLTAGE_CHARGE_100_PERCENT;
			Voltage10Percent = VOLTAGE_CHARGE_10_PERCENT;
		}
		else
		{
			Voltage95Percent = VOLTAGE_95_PERCENT;
			Voltage10Percent = VOLTAGE_10_PERCENT;
		}
	}
	Count5mSecond++;
	Count10mSecond++;
	Count100mSecond++;
	Count1000mSecond++;
}


void adc_func()
{
	  /**ADC1 GPIO Configuration
	  PC0   ------> ADC1_IN10 b8
	  PC1   ------> ADC1_IN11 b7
	  PC2   ------> ADC1_IN12 b6
	  PC3   ------> ADC1_IN13 b5
	  PA0-WKUP  --> ADC1_IN0 b4
	  PA1   ------> ADC1_IN1 b3
	  PA2   ------> ADC1_IN2 Rtb [2]
	  PA3   ------> ADC1_IN3 b2
	  PA4   ------> ADC1_IN4 b1
	  PA5   ------> ADC1_IN5 I
	  PA6   ------> ADC1_IN6 I
	  PA7   ------> ADC1_IN7 M1 [7]
	  PC4   ------> ADC1_IN14 M2out [13]
	  PC5   ------> ADC1_IN15 Rtm  [14]
	  PB1   ------> ADC1_IN9
	  */


	//4  [4] B1
	//3  [3] B2
	//1  [1] B3
	//0  [0] B4
	//13 [12] B5
	//12 [11] B6
	//11 [10] B7
	//10 [9] B8
	//6  [6] I
	//5  [5] I
//	Vref [15]


	volatile int32_t Ut = 0;
	volatile int32_t It = 0;
	volatile int32_t Ut_m = 0;
	volatile int32_t It_m = 0;
	#define NUM_READ 32
	//LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);


	U_Controller = 491520 / RegularConvData[15];// Uref V/10;  1200 * 4096/ChVref

	It = (RegularConvData[6] * CalibrationData.CalibrationValueForCurrent1) / RegularConvData[15] ;//  Current
	It_m = It;//middle_of_3Imax1(It);
	SumI1 =SumI1 + RunningAverageI1(It_m);
	SumI1Counter ++;
	if (SumI1Counter >= NUM_READ)
	{
		Battery.Current_Load = SumI1/NUM_READ;
		SumI1Counter = 0;
		SumI1 = 0;
	}

	It= (RegularConvData[5] * CalibrationData.CalibrationValueForCurrent2) / RegularConvData[15] ;//  Current
	It_m =It;// middle_of_3Imax2(It);
	SumI2 =SumI2 + RunningAverageI2(It_m);
	SumI2Counter ++;
	if (SumI2Counter >= NUM_READ)
	{
		Battery.Current_Charge = SumI2/NUM_READ;
		SumI2Counter = 0;
		SumI2 = 0;
	}

	Ut= (RegularConvData[4] * CalibrationData.CalibrationValueForVoltage1) / RegularConvData[15];
	Ut_m = Ut;//middle_of_3Umax1(Ut);
	SumU1 =SumU1 + RunningAverageU1(Ut_m);
	SumU1Counter ++;
	if (SumU1Counter >= NUM_READ)
	{
		CellsDatabase[0].Voltage = SumU1/NUM_READ;
		SumU1Counter = 0;
		SumU1 = 0;
	}

	Ut = (RegularConvData[3] * CalibrationData.CalibrationValueForVoltage2) / RegularConvData[15];
	Ut_m = Ut;//middle_of_3Umax2(Ut);
	SumU2 = SumU2 + RunningAverageU2(Ut_m);
	SumU2Counter ++;
	if (SumU2Counter >= NUM_READ)
	{
		CellsDatabase[1].Voltage = SumU2/NUM_READ;
		SumU2Counter = 0;
		SumU2 = 0;
	}


	Ut = (RegularConvData[1] * CalibrationData.CalibrationValueForVoltage3) / RegularConvData[15];//
	Ut_m = Ut;//middle_of_3Umax3(Ut);
	SumU3 =SumU3 + RunningAverageU3(Ut_m);
	SumU3Counter ++;
	if (SumU3Counter >=NUM_READ)
	{
		CellsDatabase[2].Voltage = SumU3/NUM_READ;
		SumU3Counter = 0;
		SumU3 = 0;
	}

	Ut = (RegularConvData[0] * CalibrationData.CalibrationValueForVoltage4) / RegularConvData[15];
	Ut_m = Ut;
	SumU4 =SumU4 + RunningAverageU4(Ut_m);
	SumU4Counter ++;
	if (SumU4Counter >=NUM_READ)
	{
		CellsDatabase[3].Voltage = SumU4/NUM_READ;
		SumU4Counter = 0;
		SumU4 = 0;
	}

	Ut = (RegularConvData[12] * CalibrationData.CalibrationValueForVoltage5) / RegularConvData[15];
	Ut_m = Ut;
	SumU5 =SumU5 + RunningAverageU5(Ut_m);
	SumU5Counter ++;
	if (SumU5Counter >=NUM_READ)
	{
		CellsDatabase[4].Voltage = SumU5/NUM_READ;
		SumU5Counter = 0;
		SumU5 = 0;
	}

	Ut = (RegularConvData[11] * CalibrationData.CalibrationValueForVoltage6) / RegularConvData[15];//
	Ut_m = Ut;
	SumU6 =SumU6 + RunningAverageU6(Ut_m);
	SumU6Counter ++;
	if (SumU6Counter >=NUM_READ)
	{
		CellsDatabase[5].Voltage = SumU6/NUM_READ;
		SumU6Counter = 0;
		SumU6 = 0;
	}

	Ut = (RegularConvData[10] * CalibrationData.CalibrationValueForVoltage7) / RegularConvData[15];
	Ut_m = Ut;
	SumU7 =SumU7 + RunningAverageU7(Ut_m);
	SumU7Counter ++;
	if (SumU7Counter >=NUM_READ)
	{
		CellsDatabase[6].Voltage = SumU7/NUM_READ;
		SumU7Counter = 0;
		SumU7 = 0;
	}

	Ut = (RegularConvData[9] * CalibrationData.CalibrationValueForVoltage7) / RegularConvData[15];
	Ut_m = Ut;
	SumU8 =SumU8 + RunningAverageU8(Ut_m);
	SumU8Counter ++;
	if (SumU8Counter >=NUM_READ)
	{
		CellsDatabase[7].Voltage = SumU8/NUM_READ;
		SumU8Counter = 0;
		SumU8 = 0;
	}



	uint8_t i = 0;
	uint16_t Volt = 0;
	for(i=0;i<ID_MAX_COUNT;i++)
	{
		Volt = Volt + CellsDatabase[i].Voltage;
	}
	Battery.Voltage = Volt;
	Battery.Current = 0;
	if (Battery.Current_Charge > 2) Battery.Current = Battery.Current_Charge;
	if (Battery.Current_Load > 2) Battery.Current = Battery.Current_Load * (-1);

	Battery.BalansirTemperature = RegularConvData[2];
	Battery.MosfetsTemperature = RegularConvData[14];
	Battery.BatteryTemperature = 4096;

	Battery.Gate1Voltage_NearShunt = 1400 * RegularConvData[7]/RegularConvData[15];
	Battery.Gate2Voltage_NearOUT = 1400 * RegularConvData[13]/RegularConvData[15];

	 //LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_1);
}


//PACKAGE_BEGIN
//[0]LEN -  Number of data bytes · Not including LEN byte · Including CS and EOM
//[1]Command - Read Write Exec
//[2]Screen
//[] Data - several bytes
//[] CS · Checksum is a sum up of all bytes within the message frame · Sum is computed in an 8-bit counter · CS and EOM are not included in the checksum
//[] EOM End of Message 1 0x0A

#define READ_DATA 0xFA
#define WRITE_DATA 0xFB
#define COMMAND_EXEC 0xFC

//READ_DATA
#define SYSTEM_INFO 8
#define SETTINGS_SCREEN 10
#define MAIN_SCREEN 20
#define CHARGE_SCREEN 30
#define DISCHARGE_SCREEN 40
#define COMMANDS_SCREEN 50
#define DIAGNOSTIC_SCREEN 60
#define LOG_SCREEN 70
#define CALIBRATION_SCREEN 80
#define FACTORY_SCREEN 90

#define DATALENGTH_MAX 30
struct MasterSlave_struct
{
	volatile uint8_t Package[DATALENGTH_MAX];
	volatile uint8_t Data_length;
	volatile uint8_t DataCRC;
	volatile uint8_t DataCRC_Calc;
	volatile uint8_t ReadWriteCommand;
};

struct MasterSlave_struct Slave;
struct MasterSlave_struct Master;

struct RequestFromClientToSrv_struct
{
	volatile uint8_t Data_length;
	volatile uint8_t DataCRC;
	volatile uint8_t DataCRC_Calc;
	volatile uint8_t Data[DATALENGTH_MAX];
};

struct AnswerFromSrvToClient_struct
{
	volatile uint8_t Package[DATALENGTH_MAX+4];
	volatile uint8_t Data_length;
	volatile uint8_t DataCRC;
	volatile uint8_t Data[DATALENGTH_MAX];
};
struct RequestFromClientToSrv_struct RequestFromClientToSrv;
struct AnswerFromSrvToClient_struct  AnswerFromSrvToClient;

void MainScreenAnswer()
{
	uint16_t var_16=0;
	uint8_t *arrayPointer_16 = (uint8_t*) &var_16;

	uint16_t var_u16=0;
	uint8_t *arrayPointer_u16 = (uint8_t*) &var_u16;
	arrayPointer_u16 = (uint8_t*) &Battery.Voltage;
	Master.Package[4]=arrayPointer_u16[0];
	Master.Package[5]=arrayPointer_u16[1];

	arrayPointer_16 = (uint8_t*) &Battery.Current;
	Master.Package[6]=arrayPointer_16[0];
	Master.Package[7]=arrayPointer_16[1];

	arrayPointer_u16 = (uint8_t*) &CellsDatabase[0].Voltage;
	Master.Package[8]=arrayPointer_u16[0];
	Master.Package[9]=arrayPointer_u16[1];

	arrayPointer_u16 = (uint8_t*) &CellsDatabase[1].Voltage;
	Master.Package[10]=arrayPointer_u16[0];
	Master.Package[11]=arrayPointer_u16[1];

	arrayPointer_u16 = (uint8_t*) &CellsDatabase[2].Voltage;
	Master.Package[12]=arrayPointer_u16[0];
	Master.Package[13]=arrayPointer_u16[1];

	arrayPointer_u16 = (uint8_t*) &CellsDatabase[3].Voltage;
	Master.Package[14]=arrayPointer_u16[0];
	Master.Package[15]=arrayPointer_u16[1];

	if (NUMBER_OF_CELLS == 4)
	{
		Master.Package[16]=95;
		Master.Package[17]=Battery.Power;
		Master.Data_length = 19-1;
		Master.DataCRC = calcCRC(Master.Package, Master.Data_length, 0);
		Master.Package[18] = Master.DataCRC;
		Master.Package[19] = 0x0A;
		putDataInBufferUart2(Master.Package,Master.Data_length+2);
	}

	if (NUMBER_OF_CELLS == 8)
	{
		arrayPointer_u16 = (uint8_t*) &CellsDatabase[4].Voltage;
		Master.Package[16]=arrayPointer_u16[0];
		Master.Package[17]=arrayPointer_u16[1];

		arrayPointer_u16 = (uint8_t*) &CellsDatabase[5].Voltage;
		Master.Package[18]=arrayPointer_u16[0];
		Master.Package[19]=arrayPointer_u16[1];

		arrayPointer_u16 = (uint8_t*) &CellsDatabase[6].Voltage;
		Master.Package[20]=arrayPointer_u16[0];
		Master.Package[21]=arrayPointer_u16[1];

		arrayPointer_u16 = (uint8_t*) &CellsDatabase[7].Voltage;
		Master.Package[22]=arrayPointer_u16[0];
		Master.Package[23]=arrayPointer_u16[1];

		Master.Package[24]=95;
		Master.Package[25]=Battery.Power;
		Master.Data_length = 27-1;

		Master.DataCRC = calcCRC(Master.Package, Master.Data_length, 0);
		Master.Package[26] = Master.DataCRC;
		Master.Package[27] = 0x0A;
		putDataInBufferUart2(Master.Package,Master.Data_length+2);

	}


	Master.Package[0] = PACKAGE_BEGIN;
	Master.Package[1] = Master.Data_length;
	Master.Package[2] = READ_DATA;
	Master.Package[3] = MAIN_SCREEN;

}

void SystemInfoAnswer()
{
	Master.Package[4]=1;//version before point
	Master.Package[5]=0;//version after point
	Master.Package[6]=1;// 1- LIFEPO4, 2 - LIon
	Master.Package[7]=NUMBER_OF_CELLS;//Number of cells
	Master.Package[8]=0;
	Master.Package[9]=0;
	Master.Package[10]=0;
	Master.Package[11]=0;
	Master.Package[12]=0;
	Master.Package[13]=0;

	Master.Data_length = 15-1;

	Master.Package[0] = PACKAGE_BEGIN;
	Master.Package[1] = Master.Data_length;
	Master.Package[2] = READ_DATA;
	Master.Package[3] = SYSTEM_INFO;
	Master.DataCRC = calcCRC(Master.Package, Master.Data_length, 0);
	Master.Package[14] = Master.DataCRC;
	Master.Package[15] = 0x0A;
	putDataInBufferUart2(Master.Package,Master.Data_length+2);
}

void TIM7_Callback()
{

	//logDebugD("l=",bufferUart1.rx_counter,0);
	//logDebugD("Le=",ParsingData.IsPassedPackageLengthFlag,0);
	//logDebugD("PB=",ParsingData.IsPassedPackageBeginFlag,0);
	//logDebugD("RE=",ParsingData.IsDataReadyReadFromBuffer,0)
	if (ParsingData.IsDataReadyReadFromBuffer == 1)
	{
		if ( (getCharFromBufferUART2() == PACKAGE_BEGIN) )
		{
			ParsingData.IsDataReadyReadFromBuffer = 0;
			//logDebug("PACKAGE_BEGIN");
			//logDebugD("l=",bufferUart1.rx_counter,0);

			Slave.Data_length = getCharFromBufferUART2();
			Slave.Package[0] = PACKAGE_BEGIN;
			Slave.Package[1] = Slave.Data_length;
			//logDebugD("Slave.Data_length  ",Slave.Data_length,0);
			if (Slave.Data_length <= DATALENGTH_MAX)
			{
				uint8_t i=0;
				for(i = 2; i<=Slave.Data_length+1;i++)
				{
					Slave.Package[i] = getCharFromBufferUART2();
					//logDebugD("pack ",Slave.Package[i],0)
				}
				Slave.DataCRC = Slave.Package[i-2];
				//logDebugD("CRC1",Slave.Package[i-2],0);
				//logDebugD("CRC2",Slave.Package[i-1],0);
				Slave.DataCRC_Calc = calcCRC(Slave.Package, Slave.Data_length, 0);
				if (Slave.DataCRC_Calc == Slave.DataCRC)
				{
					//logDebug("CRC OK");
					if (Slave.Package[2] == READ_DATA)
					{
						//logDebug("READ Command");
						if(Slave.Package[3] == MAIN_SCREEN)
						{
							//logDebug("MAIN_SCREEN");
							MainScreenAnswer();
						}
						if(Slave.Package[3] == SYSTEM_INFO)
						{
							logDebug("SYSTEM_INFO");
							SystemInfoAnswer();
						}

					}
				}//CRC
			}//DATALENGTH_MAX
		}//PACKAGE_BEGIN
	}//ParsingData.IsDataReadyReadFromBuffer
}//f

void Output_ON()
{
    GPIOA->BSRR =  GPIO_BSRR_BS8;//ON OUT
    logDebug("out on");
    Battery.Power = ON;
}
void Output_OFF()
{
    GPIOA->BSRR =  GPIO_BSRR_BR8;//ON OUT
    logDebug("out off");
    Battery.Power = OFF;
}
void InverseOUT()
{
	if (Battery.Power == OFF)
	{
		Output_ON();
	}
	else
	{
		Output_OFF();
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);//SystemCoreClock/1000 - 1mc

	LoggingData.RecordsQuantity= 0;
	uint8_t EEpromReadStatus;

	delay_ms(100);
   GPIOA->BSRR =  GPIO_BSRR_BS11;//ON CPU
   GPIOA->BSRR =  GPIO_BSRR_BS12;//12 V for
   GPIOB->BSRR =  GPIO_BSRR_BS4;//12 V for
   Battery.BatteryLevel = BATTERY_OK;
   Battery.Power = OFF;
   logDebug("System ON");
	FlashVoltage = Voltage10Percent*ID_MAX_COUNT*10;

  FactoryWriteToFlash_CRC();
	delay_ms(1000);

	flash_read_block();



	if (LoggingData.RecordsQuantity>=MAX_LOG_ITEMS) LoggingData.RecordsQuantity = 0;
	EEpromReadStatus = ReadFromFlash();
	if (EEpromReadStatus==0)
	{
		delay_ms(1000);
		EEpromReadStatus = ReadFromFlash();
		logInfo("Read from EEprom - FAIL");
	}
	if (EEpromReadStatus == 0)
	{

		delay_ms(4000);
		WriteInLOG("EEprmReadFAIL");
		logInfo("Read from EEprom - FAIL");
	}
	if (EEpromReadStatus == 1)
		logInfo("Read from EEprom - SUCCESS");



  BatteryCapacityDischargeCurrentAfterPOwerUp = SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent;
  ChargeDurationSec = SettingsData.ChargeTime*3600;
  SelectedOptionValue = SettingsData.Option1;
  SelectedOptionValue1 = SettingsData.Option2;
 // Generation_Coefficients_R_A_B();
  InfoToUARTBeforeStart();
  logInfo(Version);
  delay_ms(1000);

  //Output_ON();
  //logDebug("OUTPUT ON");
  //printToBufferUART1("Hello");

//	  GPIOC->BSRR =  GPIO_BSRR_BS15;

  Key_Pressed_t Key;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Key = BUT_GetKey();
	  if (Key == KEY_ACTION)
		  InverseOUT();
/*
	  if (LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_14))
	  {
		   logDebug("Short");
		  delay_ms(1000);
		  GPIOC->BSRR =  GPIO_BSRR_BR15;//restore current short
		  delay_ms(10);
		  GPIOC->BSRR =  GPIO_BSRR_BS15;
	  }
	  */
	  /*
		f1 = SysTick->VAL;
		  logDebugD("f1 ",f1,0);
		  logDebugD("f2 ",f2,0);
		  logDebugD("f1-f2 ",f1-f2,0);
*/


		//4  [4] B1
		//3  [3] B2
		//1  [1] B3
		//0  [0] B4
		//13 [12] B5
		//12 [11] B6
		//11 [10] B7
		//10 [9] B8
		//6  [6] I
		//5  [5] I
	//	Vref [15]


	  logDebugD("sec ",time_sec,0);
	  logDebugD("PowerOffTimesec ",PowerOffTimesec,0);
	  logDebugD("[6] ", RegularConvData[6],0);
	  logDebugD("I charge ", Battery.Current_Load,2);
	  logDebugD("[5] ", RegularConvData[5],0);
	  logDebugD("I Discharge ", Battery.Current_Charge,2);

	  /*
	  logDebugD("[4] ", RegularConvData[4],0);
	  logDebugD("B1 ", CellsDatabase[0].Voltage,2);
	  logDebugD("[3] ", RegularConvData[3],0);
	  logDebugD("B2 ", CellsDatabase[1].Voltage,2);
	  logDebugD("[1] ", RegularConvData[1],0);
	  logDebugD("B3 ", CellsDatabase[2].Voltage,2);
	  logDebugD("[0] ", RegularConvData[0],0);
	  logDebugD("B4 ", CellsDatabase[3].Voltage,2);
	  logDebugD("[12] ", RegularConvData[12],0);
	  logDebugD("B5 ", CellsDatabase[4].Voltage,2);
	  logDebugD("[11] ", RegularConvData[11],0);
	  logDebugD("B6 ", CellsDatabase[5].Voltage,2);
	  logDebugD("[10] ", RegularConvData[10],0);
	  logDebugD("B7 ", CellsDatabase[6].Voltage,2);
	  logDebugD("[9] ", RegularConvData[9],0);
	  logDebugD("B8 ", CellsDatabase[7].Voltage,2);

	  logDebugD("Ref ", RegularConvData[15],0);
	  logDebugD("Ucontroller:", U_Controller,2);
	  logDebugD("Battery.Voltage:", Battery.Voltage,2);

*/
	  logDebugD("T Mosfet: ",Battery.MosfetsTemperature,0);
	  logDebugD("T Balans: ",Battery.BalansirTemperature,0);
	  logDebugD("Gate1Voltage_NearShunt: ",Battery.Gate1Voltage_NearShunt,2);
	  logDebugD("Gate2Voltage_NearOUT: ",Battery.Gate2Voltage_NearOUT,2);

	 // logDebugD("Rec ", vard1,0);
	 // logDebugD("snd ", vard2,0);
	 // logDebugD("int ", vard3,0);

	  //printToBufferUART2("hello");

	  //printToBufferUART1(getCharFromBufferUART1());
	 // logDebugD("charge W ",RegularConvData[1],0 );
	 // logDebugD("charge   ",Battery.Current_Charge,0 );
	 // logDebugD("load W   ",RegularConvData[2],0 );
	 // logDebugD("load     ",Battery.Current_Load,0 );
	 // logDebugD("PowerOffTimesec = ",PowerOffTimesec, 0);




	  delay_ms(1000);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_3);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(24000000);
  LL_SetSystemCoreClock(24000000);
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_2);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  /**ADC1 GPIO Configuration
  PC0   ------> ADC1_IN10
  PC1   ------> ADC1_IN11
  PC2   ------> ADC1_IN12
  PC3   ------> ADC1_IN13
  PA0-WKUP   ------> ADC1_IN0
  PA1   ------> ADC1_IN1
  PA2   ------> ADC1_IN2
  PA3   ------> ADC1_IN3
  PA4   ------> ADC1_IN4
  PA5   ------> ADC1_IN5
  PA6   ------> ADC1_IN6
  PA7   ------> ADC1_IN7
  PC4   ------> ADC1_IN14
  PC5   ------> ADC1_IN15
  PB1   ------> ADC1_IN9
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3
                          |LL_GPIO_PIN_4|LL_GPIO_PIN_5;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3
                          |LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* ADC1 DMA Init */

  /* ADC1 Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);

  /* USER CODE BEGIN ADC1_Init 1 */
  LL_DMA_ConfigAddresses(DMA1,
                           LL_DMA_CHANNEL_1,
                           LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                           (uint32_t)RegularConvData,
                           LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

   //LL_DMA_SetPeriphAddress(DMA1,LL_DMA_CHANNEL_1,LL_ADC_DMA_GetRegAddr(ADC1,LL_ADC_DMA_REG_REGULAR_DATA));
   //LL_DMA_SetMemoryAddress(DMA1,LL_DMA_CHANNEL_1,RegularConvData);
   LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, NUMBER_OF_CHANNELS);
   //LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
   //LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);
   LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_16RANKS;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_1);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_2);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_2, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_4, LL_ADC_CHANNEL_3);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_3, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_5, LL_ADC_CHANNEL_4);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_6, LL_ADC_CHANNEL_5);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_5, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_7, LL_ADC_CHANNEL_6);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_6, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_8, LL_ADC_CHANNEL_7);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_7, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_9, LL_ADC_CHANNEL_9);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_10, LL_ADC_CHANNEL_10);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_10, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_11, LL_ADC_CHANNEL_11);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_11, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_12, LL_ADC_CHANNEL_12);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_12, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_13, LL_ADC_CHANNEL_13);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_13, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_14, LL_ADC_CHANNEL_14);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_14, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_15, LL_ADC_CHANNEL_15);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_15, LL_ADC_SAMPLINGTIME_239CYCLES_5);

  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_16, LL_ADC_CHANNEL_VREFINT);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_239CYCLES_5);
  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
  /* USER CODE BEGIN ADC1_Init 2 */
	 LL_ADC_Enable(ADC1);
	  delay_ms(100);
	  LL_ADC_StartCalibration(ADC1);
	  while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0) {}
	  LL_ADC_REG_StartConversionSWStart(ADC1);
  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7);

  /* TIM7 interrupt Init */
  NVIC_SetPriority(TIM7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM7_IRQn);

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  TIM_InitStruct.Prescaler = 24000;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 10;
  LL_TIM_Init(TIM7, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM7);
  LL_TIM_SetTriggerOutput(TIM7, LL_TIM_TRGO_UPDATE);
  LL_TIM_DisableMasterSlaveMode(TIM7);
  /* USER CODE BEGIN TIM7_Init 2 */
  LL_TIM_EnableCounter(TIM7);
  LL_TIM_EnableIT_UPDATE(TIM7);
  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 38400;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */
  LL_USART_EnableIT_TXE(USART1);
  // LL_USART_EnableIT_RXNE(USART1);
  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  /**USART3 GPIO Configuration
  PB10   ------> USART3_TX
  PB11   ------> USART3_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USART3 interrupt Init */
  NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART3_IRQn);

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  USART_InitStruct.BaudRate = 38400;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART3, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART3);
  LL_USART_Enable(USART3);
  /* USER CODE BEGIN USART3_Init 2 */
  //LL_USART_EnableIT_TXE(USART3);


  LL_USART_EnableIT_TXE(USART3);
    LL_USART_EnableIT_RXNE(USART3);
  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15
                          |LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_8);

  /**/
  LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_9|LL_GPIO_PIN_11
                          |LL_GPIO_PIN_12);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8|LL_GPIO_PIN_11|LL_GPIO_PIN_12);

  /**/
  LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_2);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15
                          |LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7|LL_GPIO_PIN_9|LL_GPIO_PIN_11
                          |LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
