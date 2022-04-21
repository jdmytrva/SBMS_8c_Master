#ifndef _GENERAL_
#define _GENERAL_
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"
#include "lib.h"
#include "usart.h"
#include "Lib.h"
#include "Eeprom.h"
#define PACKAGE_BEGIN 0xFE

#define NUMBER_OF_CHANNELS 16
__IO uint16_t RegularConvData[NUMBER_OF_CHANNELS] ;


volatile uint16_t U_Controller;
volatile int16_t Current;

struct StructCalibrationValuetoSaveInFlash
{
	uint32_t CRC_data;
	uint32_t CRC_AddresInEEprom;

	uint32_t CalibrationValueForCurrent1;
	uint32_t CalibrationValueForCurrent1_AddresInEEprom;

	uint32_t CalibrationValueForCurrent2;
	uint32_t CalibrationValueForCurrent2_AddresInEEprom;

	uint32_t CalibrationValueForVoltage1;
	uint32_t CalibrationValueForVoltage1_AddresInEEprom;

	uint32_t CalibrationValueForVoltage2;
	uint32_t CalibrationValueForVoltage2_AddresInEEprom;

	uint32_t CalibrationValueForVoltage3;
	uint32_t CalibrationValueForVoltage3_AddresInEEprom;

	uint32_t CalibrationValueForVoltage4;
	uint32_t CalibrationValueForVoltage4_AddresInEEprom;

	uint32_t CalibrationValueForVoltage5;
	uint32_t CalibrationValueForVoltage5_AddresInEEprom;

	uint32_t CalibrationValueForVoltage6;
	uint32_t CalibrationValueForVoltage6_AddresInEEprom;

	uint32_t CalibrationValueForVoltage7;
	uint32_t CalibrationValueForVoltage7_AddresInEEprom;
};

struct StructSettingsValuetoSaveInFlash
{
	uint32_t CRC_data;
	uint32_t CRC_AddresInEEprom;

	uint32_t Option1;
	uint32_t Option1_AddresInEEprom;
//
	uint32_t ChargeTime;
	uint32_t ChargeTime_AddresInEEprom;

	uint32_t LowVoltage;
	uint32_t LowVoltage_AddresInEEprom;

	uint32_t MaxVoltage;
	uint32_t MaxVoltage_AddresInEEprom;

	uint32_t Swing_Chrg_time;
	uint32_t Swing_Chrg_time_AddresInEEprom;

	uint32_t Swing_DChrg_time;
	uint32_t Swing_DChrg_time_AddresInEEprom;

	uint32_t ChargeAdapt;
	uint32_t ChargeAdapt_AddresInEEprom;

	uint32_t Option2;
	uint32_t Option2_AddresInEEprom;

};
struct StructValuetoSaveInFlashWhenPowerOFF
{
	uint32_t CRC_data;
	uint32_t CRC_AddresInEEprom;

	uint32_t BatteryCapacityDischargePreviousValue;
	uint32_t BatteryCapacityDischargePrevious_AddresInEEprom;

	uint32_t BatteryCapacityDischargeCurrent;
	uint32_t BatteryCapacityDischargeCurrent_AddresInEEprom;

	uint32_t OutState;
	uint32_t OutState_AddresInEEprom;
};


struct StructCalibrationValuetoSaveInFlash CalibrationData;
struct StructCalibrationValuetoSaveInFlash CalibrationDataForVerify;
struct StructCalibrationValuetoSaveInFlash CalibrationDataFactory;

struct StructSettingsValuetoSaveInFlash SettingsData;
struct StructSettingsValuetoSaveInFlash SettingsDataForVerify;
struct StructSettingsValuetoSaveInFlash SettingsDataFactory;

struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOff;
struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffForVerify;
struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffFactory;


volatile uint16_t  Count100mSecond;
volatile uint16_t  Count10mSecond;
volatile uint16_t  Count5mSecond;
volatile uint16_t  Count1000mSecond;
volatile uint16_t  CountAnymSecond;


struct StructTemperatureLinearTable
{
	int16_t T;
	int16_t R;
};
struct StructTemperature_R_A_B
{
	int32_t R;
	int32_t A10000;
	int32_t B;
};
#define TLT 8
struct StructTemperatureLinearTable T_Table[TLT];
struct StructTemperature_R_A_B Temp_R_A_B[TLT];

#define MAX_LOG_ITEMS 24

struct StructLoggingData
{
	uint32_t RecordsQuantity;
	char Records[MAX_LOG_ITEMS][17];
};
struct StructLoggingData LoggingData;
void WriteInLOG(char  str[17]);
void WriteInLOGc(char  str [17],char c);


void InfoToUARTBeforeStart(void);

volatile uint32_t f1;
volatile uint32_t f2;
volatile uint32_t f3;
volatile uint32_t f4;
volatile uint32_t vard1;
volatile uint32_t vard2;
volatile uint32_t vard3;
void delay_ms(volatile uint32_t value);
void delay_us(uint32_t us);

#define RX_BUFFER_SIZE 100 // for receive
#define TX_BUFFER_SIZE 1000 // for send

struct StructRX_TX_Buffer
{
	// Буфер на прием
	volatile uint8_t    rx_buffer[RX_BUFFER_SIZE];
	volatile uint16_t   rx_wr_index, //индекс хвоста буфера (куда писать данные)
	                    rx_rd_index, //индекс начала буфера (откуда читать данные)
	                    rx_counter; //количество данных в буфере
	volatile uint16_t    rx_buffer_overflow; //информация о переполнении буфера


	// Буфер на передачу
	volatile uint8_t   tx_buffer[TX_BUFFER_SIZE];
	volatile uint16_t  tx_wr_index, //индекс хвоста буфера (куда писать данные)
	                   tx_rd_index, //индекс начала буфера (откуда читать данные)
	                   tx_counter; //количество данных в буфере
	volatile uint16_t   tx_buffer_overflow; //информация о переполнении буфера
};
struct StructRX_TX_Buffer bufferUart1;
struct StructRX_TX_Buffer bufferUart2;



struct StructDataFromBatteryCells
{
	uint16_t Voltage;//mv*10
	uint8_t TemperatureBattery;
	uint8_t TemperatureController;
	uint32_t SuccessGetData;
	uint16_t FailsGetData;
	uint8_t IDs;
	uint8_t FailStatus; //0-OK, 1-Fail
	uint32_t TimeSec;
	uint8_t VoltageLevel;//0-8
	uint8_t TemperatureLevel;
	uint8_t FailsGetDataLevel;
	//debounce
	uint32_t BatVoltLowerMin_Timer;
	uint8_t  BatVoltLowerMin_Status;

};
#define 	ID_MAX_COUNT 8
struct StructDataFromBatteryCells CellsDatabase[ID_MAX_COUNT];

typedef enum Battery_Level_Enum
{
	LOW_BATTERY=1,
	BATTERY_OK
}Battery_Level_t;

typedef enum Battery_OutputState_Enum
{
	ON=1,
	OFF
}Battery_OutputState_t;

struct BatteryStruct
{
	volatile uint16_t Voltage;
	volatile int16_t  Current;
	volatile uint16_t Current_Charge;
	volatile uint16_t Current_Load;
	volatile Battery_Level_t BatteryLevel;
	volatile uint8_t MosfetsTemperature;
	volatile uint8_t BalansirTemperature;
	volatile uint8_t BatteryTemperature;
	volatile Battery_OutputState_t Out_On_Off;
	volatile uint16_t Gate1Voltage_NearShunt;
	volatile uint16_t Gate2Voltage_NearOUT;


};
struct BatteryStruct Battery;
volatile uint8_t OverChargeStatus;
volatile uint8_t OverDisChargeStatus;
volatile uint8_t CurrentShortStatus;
volatile uint16_t CurrentShortTimer;

uint16_t EEpromSaveStatus;
char calcCRC(char *Ptr, char Num, char CRC1);
struct ParsingData_struct
{
	volatile uint8_t IsPassedPackageBeginFlag;
	volatile uint8_t IsPassedPackageLengthFlag;
	volatile uint8_t IsDataReadyReadFromBuffer;
	//volatile uint8_t IsStartingReadingData;
	volatile uint16_t Length;
	volatile uint16_t i;
};

struct  ParsingData_struct ParsingData;

#endif
