#include "General.h"
#define ADDRESS_FLASH_CALIBRATTION FLASH_BASE+2048*124
#define ADDRESS_FLASH_SETTINGS     FLASH_BASE+2048*123
#define ADDRESS_FLASH_WHEN_OFF     FLASH_BASE+2048*122


struct StructCalibrationValuetoSaveInFlash CalibrationData=
{
		0,
		ADDRESS_FLASH_CALIBRATTION,
		0,
		ADDRESS_FLASH_CALIBRATTION+4,
		0,
		ADDRESS_FLASH_CALIBRATTION+8,
		0,
		ADDRESS_FLASH_CALIBRATTION+12,
		0,
		ADDRESS_FLASH_CALIBRATTION+16,
		0,
		ADDRESS_FLASH_CALIBRATTION+20,
		0,
		ADDRESS_FLASH_CALIBRATTION+24,
		0,
		ADDRESS_FLASH_CALIBRATTION+28,
		0,
		ADDRESS_FLASH_CALIBRATTION+32,
		0,
		ADDRESS_FLASH_CALIBRATTION+36

};
struct StructCalibrationValuetoSaveInFlash CalibrationDataForVerify=
{
		0,
		ADDRESS_FLASH_CALIBRATTION,
		0,
		ADDRESS_FLASH_CALIBRATTION+4,
		0,
		ADDRESS_FLASH_CALIBRATTION+8,
		0,
		ADDRESS_FLASH_CALIBRATTION+12,
		0,
		ADDRESS_FLASH_CALIBRATTION+16,
		0,
		ADDRESS_FLASH_CALIBRATTION+20,
		0,
		ADDRESS_FLASH_CALIBRATTION+24,
		0,
		ADDRESS_FLASH_CALIBRATTION+28,
		0,
		ADDRESS_FLASH_CALIBRATTION+32,
		0,
		ADDRESS_FLASH_CALIBRATTION+36

};
struct StructCalibrationValuetoSaveInFlash CalibrationDataFactory=
{
		0,//CRC
		ADDRESS_FLASH_CALIBRATTION,
		118,//CalibrationValueForCurrent1
		ADDRESS_FLASH_CALIBRATTION+4,
		118,//CalibrationValueForCurrent2
		ADDRESS_FLASH_CALIBRATTION+8,
		256,//CalibrationValueForVoltage1
		ADDRESS_FLASH_CALIBRATTION+12,
		256,//CalibrationValueForVoltage2
		ADDRESS_FLASH_CALIBRATTION+16,
		256,//CalibrationValueForVoltage3
		ADDRESS_FLASH_CALIBRATTION+20,
		257,//CalibrationValueForVoltage4
		ADDRESS_FLASH_CALIBRATTION+24,
		266,//CalibrationValueForVoltage5
		ADDRESS_FLASH_CALIBRATTION+28,
		250,//CalibrationValueForVoltage6
		ADDRESS_FLASH_CALIBRATTION+32,
		256,//CalibrationValueForVoltage7
		ADDRESS_FLASH_CALIBRATTION+36
};
//=================================================

struct StructSettingsValuetoSaveInFlash SettingsData=
{
		0,
		ADDRESS_FLASH_SETTINGS,
		0,
		ADDRESS_FLASH_SETTINGS+4,
		0,
		ADDRESS_FLASH_SETTINGS+8,
		0,
		ADDRESS_FLASH_SETTINGS+12,
		0,
		ADDRESS_FLASH_SETTINGS+16,
		0,
		ADDRESS_FLASH_SETTINGS+20,
		0,
		ADDRESS_FLASH_SETTINGS+24,
		0,
		ADDRESS_FLASH_SETTINGS+28,
		0,
		ADDRESS_FLASH_SETTINGS+32
};
struct StructSettingsValuetoSaveInFlash SettingsDataForVerify=
{
		0,
		ADDRESS_FLASH_SETTINGS,
		0,
		ADDRESS_FLASH_SETTINGS+4,
		0,
		ADDRESS_FLASH_SETTINGS+8,
		0,
		ADDRESS_FLASH_SETTINGS+12,
		0,
		ADDRESS_FLASH_SETTINGS+16,
		0,
		ADDRESS_FLASH_SETTINGS+20,
		0,
		ADDRESS_FLASH_SETTINGS+24,
		0,
		ADDRESS_FLASH_SETTINGS+28,
		0,
		ADDRESS_FLASH_SETTINGS+32
};

struct StructSettingsValuetoSaveInFlash SettingsDataFactory=
{
		0,//CRC
		ADDRESS_FLASH_SETTINGS,
		1,//option1 -  Menu Position
		ADDRESS_FLASH_SETTINGS+4,
		15,//Charge time
		ADDRESS_FLASH_SETTINGS+8,
		1050,//LowVoltage
		ADDRESS_FLASH_SETTINGS+12,
		1450,//MaxVoltage
		ADDRESS_FLASH_SETTINGS+16,
		10,//Swing_Chrg_time
		ADDRESS_FLASH_SETTINGS+20,
		10,//Swing_DChrg_time
		ADDRESS_FLASH_SETTINGS+24,
		6,//ChargeAdapt %
		ADDRESS_FLASH_SETTINGS+28,
		1,//Out after powerUp option. 1 - OFF
		ADDRESS_FLASH_SETTINGS+32

};
//=====================================
struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOff=
{
		0,
		ADDRESS_FLASH_WHEN_OFF,
		0,
		ADDRESS_FLASH_WHEN_OFF+4,
		0,
		ADDRESS_FLASH_WHEN_OFF+8,
		0,
		ADDRESS_FLASH_WHEN_OFF+12
};

struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffForVerify=
{
		0,
		ADDRESS_FLASH_WHEN_OFF,
		0,
		ADDRESS_FLASH_WHEN_OFF+4,
		0,
		ADDRESS_FLASH_WHEN_OFF+8,
		0,
		ADDRESS_FLASH_WHEN_OFF+12
};

struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffFactory=
{
		0,//CRC
		ADDRESS_FLASH_WHEN_OFF,
		0,//BatteryCapacityDischargePreviousValue
		ADDRESS_FLASH_WHEN_OFF+4,
		0,//BatteryCapacityDischargeCurrent
		ADDRESS_FLASH_WHEN_OFF+8,
		0,//Out State. 0 - OFF, 1 - ON
		ADDRESS_FLASH_WHEN_OFF+12
};

struct StructTemperatureLinearTable T_Table []=
{
		{1,31000},
		{10,20000},
		{25,10000},//Temperature, Resistance
		{40,4300},
		{60,3000},
		{80,1950},
		{100,900},
		{150,500}
};
void Generation_Coefficients_R_A_B()
{
	int8_t i;
	for (i=0;i<(TLT-1);i++)
	{
		Temp_R_A_B[i].R = T_Table[i].R;
		Temp_R_A_B[i].A10000 = (T_Table[i+1].T - T_Table[i].T)*10000/(T_Table[i+1].R - T_Table[i].R);
		Temp_R_A_B[i].B = T_Table[i].T - Temp_R_A_B[i].A10000 * T_Table[i].R/10000;
	}
	Temp_R_A_B[i].R = T_Table[i].R;
/*
	for (i=0; i<(TLT-1);i++)
	{
		Print_to_USART1_d(Temp_R_A_B[i].R,"R:",0);
	    Print_to_USART1_d(Temp_R_A_B[i].A10000,"A10000:",0);
	    Print_to_USART1_d(Temp_R_A_B[i].B,"B:",0);
	    Print_to_USART1_d(Temp_R_A_B[i].A10000*Temp_R_A_B[i].R/10000+Temp_R_A_B[i].B,"T0:",0);
	    int32_t c;
	    c=(Temp_R_A_B[i].R-Temp_R_A_B[i+1].R)/2+Temp_R_A_B[i+1].R;
	    Print_to_USART1_d(c,"c= ",0);
	    Print_to_USART1_d(Temp_R_A_B[i].A10000*c/10000+Temp_R_A_B[i].B,"T:",0);
	}
*/
}
int32_t GetTemperature(int32_t Rtemp)
{
	if (Rtemp>Temp_R_A_B[0].R) return -10;

	int8_t i;
	for (i=0; i<(TLT-1);i++)
	{
		if (Rtemp <= Temp_R_A_B[i].R  && Rtemp > Temp_R_A_B[i+1].R)
		{
			return Rtemp*Temp_R_A_B[i].A10000/10000+Temp_R_A_B[i].B;

		}
	}
	return 200;
}

void WriteInLOG(char  str [17])
{
	uint8_t i;
	uint8_t j;

	char *number;
	number = itoa(LoggingData.RecordsQuantity);


	for(i=0;number[i]!='\0';i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = number[i];
	}
	LoggingData.Records[LoggingData.RecordsQuantity][i] = ':';
	i++;

	for(j=0;str[j]!='\0';i++,j++)
	{
		if (i<16) LoggingData.Records[LoggingData.RecordsQuantity][i] = str[j];
		else
		{
			LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';
			break;
		}
	}
	for (;i<16;i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = ' ';
	}
	logDebugD("ii: ",i,0);
	LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';

	LoggingData.RecordsQuantity++;
	if (LoggingData.RecordsQuantity>=(MAX_LOG_ITEMS))
	{
		for (i=0;i<MAX_LOG_ITEMS-10;i++)
		{
			number = itoa(i);
			uint8_t k;
			for(k=0;number[k]!='\0';k++)
			{
				LoggingData.Records[i][k] = number[k];
			}
			LoggingData.Records[i][k] = ':';
			for (j=k+1;LoggingData.Records[i][j]!='\0';j++)
			{
				if (i>=10)
					LoggingData.Records[i][j] = LoggingData.Records[i+10][j];
				else
					LoggingData.Records[i][j] = LoggingData.Records[i+10][j+1];
			}
			j--;
			for (;j<16;j++)
			{
				LoggingData.Records[i][j] = ' ';
			}
			LoggingData.Records[i][j] = '\0';
		}

		LoggingData.RecordsQuantity=MAX_LOG_ITEMS-10;
	}
	flash_write_block();
	logDebugD("Q: ",LoggingData.RecordsQuantity,0);
}

void WriteInLOGc(char  str [17],char c)
{
	uint8_t i;
	uint8_t j;

	char *number;
	number = itoa(LoggingData.RecordsQuantity);


	for(i=0;number[i]!='\0';i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = number[i];
	}
	LoggingData.Records[LoggingData.RecordsQuantity][i] = c;
	i++;

	for(j=0;str[j]!='\0';i++,j++)
	{
		if (i<16) LoggingData.Records[LoggingData.RecordsQuantity][i] = str[j];
		else
		{
			LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';
			break;
		}
	}
	for (;i<16;i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = ' ';
	}
	logDebugD("ii: ",i,0);
	LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';

	LoggingData.RecordsQuantity++;
	if (LoggingData.RecordsQuantity>=(MAX_LOG_ITEMS))
	{
		for (i=0;i<MAX_LOG_ITEMS-10;i++)
		{
			number = itoa(i);
			uint8_t k;
			for(k=0;number[k]!='\0';k++)
			{
				LoggingData.Records[i][k] = number[k];
			}
			LoggingData.Records[i][k] = c;
			for (j=k+1;LoggingData.Records[i][j]!='\0';j++)
			{
				if (i>=10)
					LoggingData.Records[i][j] = LoggingData.Records[i+10][j];
				else
					LoggingData.Records[i][j] = LoggingData.Records[i+10][j+1];
			}
			j--;
			for (;j<16;j++)
			{
				LoggingData.Records[i][j] = ' ';
			}
			LoggingData.Records[i][j] = '\0';
		}

		LoggingData.RecordsQuantity=MAX_LOG_ITEMS-10;
	}
	flash_write_block();
	logDebugD("Q: ",LoggingData.RecordsQuantity,0);
}



void InfoToUARTBeforeStart(void)
{

	logInfoD("CRC(Calibration) =",CalibrationData.CRC_data,0);
	logInfoD("Calibration0ValueForCurrent =",CalibrationData.CalibrationValueForCurrent1 ,0);
	logInfoD("CalibrationValueForCurrent1 =",CalibrationData.CalibrationValueForCurrent2 ,0);
	logInfoD("CalibrationValueForCurrent2 =",CalibrationData.CalibrationValueForVoltage1 ,0);
	logInfoD("CalibrationValueForCurrent3 =",CalibrationData.CalibrationValueForVoltage2 ,0);
	logInfoD("CalibrationValueForVoltage1 =",CalibrationData.CalibrationValueForVoltage3 ,0);
	logInfoD("CalibrationValueForVoltage2 =",CalibrationData.CalibrationValueForVoltage4 ,0);
	logInfoD("CalibrationValueForVoltage3 =",CalibrationData.CalibrationValueForVoltage5 ,0);
	logInfoD("CalibrationValueForVoltage4 =",CalibrationData.CalibrationValueForVoltage6 ,0) ;
	logInfoD("ResistanceComp_MOSFET =",CalibrationData.CalibrationValueForVoltage7 ,0) ;
	delay_ms(50);
	logInfoD("CRC(SettingsData) =",SettingsData.CRC_data,0);
	logInfoD("Option1(menu position) =",SettingsData.Option1 ,0);
	logInfoD("ChargeTime =",SettingsData.ChargeTime ,0);
	logInfoD("LowVoltage =",SettingsData.LowVoltage ,0);
	logInfoD("MaxVoltage =",SettingsData.MaxVoltage ,0);
	logInfoD("ChargeAdapt =",SettingsData.ChargeAdapt ,0);
	logInfoD("Swing_Chrg_time =",SettingsData.Swing_Chrg_time ,0);
	logInfoD("Swing_DChrg_time =",SettingsData.Swing_DChrg_time ,0);
	logInfoD("Option2(OUT after powerUp) =",SettingsData.Option2 ,0);
	delay_ms(50);

	logInfoD("CRC(SaveDataWhenPowerOff) =",SaveDataWhenPowerOff.CRC_data,0);
	logInfoD("BatteryCapacityDischargePreviousValue =",SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue ,0);
	logInfoD("BatteryCapacityDischargeCurrent =",SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent ,0);
	logInfoD("OutState = ",SaveDataWhenPowerOff.OutState ,0);

	logInfoD("SystemCoreClock: ", SystemCoreClock, 0);
	delay_ms(50);
}
void delay_us(uint32_t us)
{
	volatile uint32_t n = 0;
	if (us==1) n=1;//в тактах процесора
	if (us==2) n=2;//в тактах процесора
	if (us==3) n=18;//в тактах процесора
	if (us > 3)n=(SystemCoreClock/1000000)*(us-3);//в тактах процесора
	//uint32_t n=(SystemCoreClock/1000000)*us;//в тактах процесора
    volatile  uint32_t n0=SysTick->VAL;// Начальное значение счётчика SysTick.
    volatile uint32_t np=n0;  // Предыдущее значение счётчика.
    volatile int32_t nc;// Текущее значение счётчика SysTick.

    // Выполняем цикл до тех пор, пока не пройдёт заданное количество тактов процессора.
    do{
    	nc=SysTick->VAL;
        // Проверка на переполнение, корректировка на модуль пересчёта в случае переполнения.
        if(nc>=np)
            n0+=SysTick->LOAD+1;
        np=nc;
    }while(n0-nc<n);
}
// t1=SysTick->VAL;
// t2=SysTick->VAL;

void delay_ms(volatile uint32_t value)
{
	while(value>0)
	{
		delay_us(1000);
		value--;
	}
}

//For
volatile uint32_t f1=0;
volatile uint32_t f2=0;
volatile uint32_t f3=0;
volatile uint32_t f4=0;
volatile uint32_t vard1=0;
volatile uint32_t vard2=0;
volatile uint32_t vard3=0;


volatile uint16_t  Count100mSecond = 0;
volatile uint16_t  Count10mSecond = 0;
volatile uint16_t  Count5mSecond = 0;
volatile uint16_t  Count1000mSecond = 0;
volatile uint16_t  CountAnymSecond = 0;

struct StructRX_TX_Buffer bufferUart1 =
{
	{},
	0,
	0,
	0,
	0,
	{},
	0,
	0,
	0,
	0
};

struct StructRX_TX_Buffer bufferUart2 =
{
	{},
	0,
	0,
	0,
	0,
	{},
	0,
	0,
	0,
	0
};

struct  ParsingData_struct ParsingData =
{
	0,
	0,
	0,
	0,
	0
};

void CellsDatabase_Init()
{
	volatile uint8_t i = 0;
	for(i=0;i<(ID_MAX_COUNT);i++)
	{
		CellsDatabase[i].Voltage=0;
		CellsDatabase[i].TemperatureBattery=0;
		CellsDatabase[i].TemperatureController=0;
		CellsDatabase[i].SuccessGetData=0;
		CellsDatabase[i].FailsGetData=0;
		CellsDatabase[i].IDs=(i+1)*10;
		CellsDatabase[i].FailStatus=0; //0-OK, 1-Fail
		CellsDatabase[i].TimeSec=0;
		CellsDatabase[i].VoltageLevel=0;//0-8
		CellsDatabase[i].TemperatureLevel=0;
		CellsDatabase[i].FailsGetDataLevel=0;
		CellsDatabase[i].BatVoltLowerMin_Timer=0;
		CellsDatabase[i].BatVoltLowerMin_Status=0;
	}
CellsDatabase[ID_MAX_COUNT-1].IDs = 10;
CellsDatabase[ID_MAX_COUNT-2].IDs = 11;
CellsDatabase[ID_MAX_COUNT-3].IDs = 12;
CellsDatabase[ID_MAX_COUNT-4].IDs = 13;
}

volatile uint8_t OverChargeStatus=0;
volatile uint8_t OverDisChargeStatus=0;
volatile uint8_t CurrentShortStatus=0;
volatile uint16_t CurrentShortTimer=0;

uint16_t EEpromSaveStatus=0;


/*
***********************************************************************
* Расчет циклического избыточного кода CRC для массива сообщения
* Используется полином x^8 + x^2 + x + 1 (CRC-8-Dallas/Maxim)
* Задается указатель на начало рассчитываемых байтов в массиве,
* количество байтов, участвующих в расчете (ровно)
* начальный код CRC (если считается не с начала массива),
* Для ускорения расчета используется таблица
***********************************************************************/
char calcCRC(char *Ptr, char Num, char CRC1)
{
   char CrcTable[256] = {
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, // 00
    0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d, // 08
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, // 10
    0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, // 18
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, // 20
    0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD, // 28
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,  // 30
    0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,  // 38
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,  // 40
    0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,  // 48
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,  // 50
    0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,  // 58
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,  // 60
    0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,  // 68
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,  // 70
    0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,  // 78
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,  // 80
    0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,  // 88
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,  // 90
    0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,  // 98
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,  // a0
    0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,  // a8
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,  // b0
    0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,  // b8
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,  // c0
    0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,  // c8
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,  // d0
    0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, // d8
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, // e0
    0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83, // e8
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, // f0
    0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3 // f8
  };
  do  CRC1 = CrcTable[CRC1 ^ *Ptr++]; while (--Num);
  return CRC1;
}
