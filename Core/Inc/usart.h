#ifndef _USART_
#define _USART_
#include "General.h"
#define DEBUG_LEVEL 1

#define ENABLE_PREFIX 1

#define ENABLE_LOG_INFO 1
#define ENABLE_LOG_ERROR 1
#define ENABLE_LOG_DEBUG 1


#if ENABLE_LOG_INFO
	#if ENABLE_PREFIX
		#define logInfo(x)  {  printToBufferWithoutEndUART2("INFO: ");  printToBufferUART2(x);}
		#define logInfoD(x,y,z)  {  printToBufferWithoutEndUART2("INFO: ");  printToBufferUART2D(x,y,z);}
	#else
		#define logInfo(x)  {printToBufferUART2(x);}
		#define logInfoD(x,y,z)  {printToBufferUART2D(x,y,z);}
	#endif
#else
#define logInfo(x)  // nothing
#define logInfoD(x,y,z) // nothing
#endif


#if ENABLE_LOG_ERROR
	#if ENABLE_PREFIX
		#define logError(x)  {  printToBufferWithoutEndUART2("ERROR: ");  printToBufferUART2(x);}
		#define logErrorD(x,y,z)  {  printToBufferWithoutEndUART2("ERROR: ");  printToBufferUART2D(x,y,z);}
	#else
		#define logError(x)  {printToBufferUART2(x);}
		#define logErrorD(x,y,z)  {printToBufferUART2D(x,y,z);}
	#endif
#else
#define logError(x)  // nothing
#define logErrorD(x,y,z) // nothing
#endif

#if ENABLE_LOG_DEBUG
	#if ENABLE_PREFIX
		#define logDebug(x)  {  printToBufferWithoutEndUART2("DEBUG: ");  printToBufferUART2(x);}
		#define logDebugD(x,y,z)  {  printToBufferWithoutEndUART2("DEBUG: ");  printToBufferUART2D(x,y,z);}
	#else
		#define logDebug(x)  {printToBufferUART2(x);}
		#define logDebugD(x,y,z)  {printToBufferUART2D(x,y,z);}
	#endif
#else
#define logDebug(x)  // nothing
#define logDebugD(x,y,z) // nothing
#endif


void Print_to_USART1(volatile char *data);
void Print_to_USART1_d(volatile int32_t value,volatile char *string,uint8_t koma);
void putCharInBufferUart1(uint8_t c);
void putCharDirectToUart1(uint8_t c);
void putStringInBufferUart1(volatile char *s);
void putStringDirectToUart1(volatile char *s);
void printToBufferUART1(volatile char* str);
void printToBufferWithoutEndUART1(volatile char* str);
void printToBufferUART1D(volatile char* str, volatile int32_t value, volatile uint8_t koma);
uint8_t getCharFromBufferUART1(void);
void putDataInBufferUart1(volatile char *s, uint8_t lenght);

void Print_to_USART2(volatile char *data);
void Print_to_USART2_d(volatile int32_t value,volatile char *string,uint8_t koma);
void putCharInBufferUart2(uint8_t c);
void putCharDirectToUart2(uint8_t c);
void putStringInBufferUart2(volatile char *s);
void putStringDirectToUart2(volatile char *s);
void printToBufferUART2(volatile char* str);
void printToBufferWithoutEndUART2(volatile char* str);
void printToBufferUART2D(volatile char* str, volatile int32_t value, volatile uint8_t koma);
void putDataInBufferUart2(volatile char *s, uint8_t lenght);


#endif
