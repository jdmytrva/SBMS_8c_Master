#include "usart.h"





#define INTERUPT 0// 1 - yes, 0 - no



//UART1==========================


void printToBufferUART1(volatile char* str)//10us for one char 24Mgz
{
	//while (bufferUart1.tx_counter >= TX_BUFFER_SIZE-30); //если буфер переполнен, ждем

	//LL_USART_DisableIT_TXE(USART1); //запрещаем прерывание, чтобы оно не мешало менять переменную
	USART1->CR1 &= ~USART_CR1_TXEIE;  // Interrupt Disable

	while (*str != 0)
	{
		bufferUart1.tx_buffer[bufferUart1.tx_wr_index++]=*str++; //то кладем данные в буфер
		if (bufferUart1.tx_wr_index == TX_BUFFER_SIZE) bufferUart1.tx_wr_index=0; //идем по кругу
		++bufferUart1.tx_counter; //увеличиваем счетчик количества данных в буфере
		#if DEBUG_LEVEL
			if (bufferUart1.tx_buffer_overflow < bufferUart1.tx_counter)
				bufferUart1.tx_buffer_overflow = bufferUart1.tx_counter;
		#endif
	}
	bufferUart1.tx_buffer[bufferUart1.tx_wr_index++]='\n'; //то кладем данные в буфер
	if (bufferUart1.tx_wr_index == TX_BUFFER_SIZE) bufferUart1.tx_wr_index=0; //идем по кругу
	++bufferUart1.tx_counter; //увеличиваем счетчик количества данных в буфере
	#if DEBUG_LEVEL
		if (bufferUart1.tx_buffer_overflow < bufferUart1.tx_counter)
			bufferUart1.tx_buffer_overflow = bufferUart1.tx_counter;
	#endif

	//LL_USART_EnableIT_TXE(USART1); //разрешаем прерывание
	USART1->CR1 |= USART_CR1_TXEIE;// Interrupt enable
}


void printToBufferWithoutEndUART1(volatile char* str)//10us for one char 24Mgz
{
	//while (bufferUart1.tx_counter >= TX_BUFFER_SIZE-30); //если буфер переполнен, ждем

	//LL_USART_DisableIT_TXE(USART1); //запрещаем прерывание, чтобы оно не мешало менять переменную
	USART1->CR1 &= ~USART_CR1_TXEIE;  // Interrupt Disable

	while (*str != 0)
	{
		bufferUart1.tx_buffer[bufferUart1.tx_wr_index++]=*str++; //то кладем данные в буфер
		if (bufferUart1.tx_wr_index == TX_BUFFER_SIZE) bufferUart1.tx_wr_index=0; //идем по кругу
		++bufferUart1.tx_counter; //увеличиваем счетчик количества данных в буфере
		#if DEBUG_LEVEL
			if (bufferUart1.tx_buffer_overflow < bufferUart1.tx_counter)
				bufferUart1.tx_buffer_overflow = bufferUart1.tx_counter;
		#endif

	}
	USART1->CR1 |= USART_CR1_TXEIE;// Interrupt enable
}

void printToBufferUART1D(volatile char* str, volatile int32_t value, volatile uint8_t koma) //10us for one char + 6us for one digit 24Mgz
{
	char *str1;
	if (koma == 0)
		str1 = itoa(value);
	else
		str1 = itoa_koma(value,koma);

	//while (bufferUart1.tx_counter >= TX_BUFFER_SIZE-30); //если буфер переполнен, ждем

	//LL_USART_DisableIT_TXE(USART1); //запрещаем прерывание, чтобы оно не мешало менять переменную
	USART1->CR1 &= ~USART_CR1_TXEIE;  // Interrupt Disable

	while (*str != 0)
	{
		bufferUart1.tx_buffer[bufferUart1.tx_wr_index++]=*str++; //то кладем данные в буфер
		if (bufferUart1.tx_wr_index == TX_BUFFER_SIZE) bufferUart1.tx_wr_index=0; //идем по кругу
		++bufferUart1.tx_counter; //увеличиваем счетчик количества данных в буфере
		#if DEBUG_LEVEL
			if (bufferUart1.tx_buffer_overflow < bufferUart1.tx_counter)
				bufferUart1.tx_buffer_overflow = bufferUart1.tx_counter;
		#endif

	}

	while (*str1 != 0)
	{
		bufferUart1.tx_buffer[bufferUart1.tx_wr_index++]=*str1++; //то кладем данные в буфер
		if (bufferUart1.tx_wr_index == TX_BUFFER_SIZE) bufferUart1.tx_wr_index=0; //идем по кругу
		++bufferUart1.tx_counter; //увеличиваем счетчик количества данных в буфере
		#if DEBUG_LEVEL
			if (bufferUart1.tx_buffer_overflow < bufferUart1.tx_counter)
				bufferUart1.tx_buffer_overflow = bufferUart1.tx_counter;
		#endif

	}

	bufferUart1.tx_buffer[bufferUart1.tx_wr_index++]='\n'; //то кладем данные в буфер
	if (bufferUart1.tx_wr_index == TX_BUFFER_SIZE) bufferUart1.tx_wr_index=0; //идем по кругу
	++bufferUart1.tx_counter; //увеличиваем счетчик количества данных в буфере
	#if DEBUG_LEVEL
		if (bufferUart1.tx_buffer_overflow < bufferUart1.tx_counter)
			bufferUart1.tx_buffer_overflow = bufferUart1.tx_counter;
	#endif

	//LL_USART_EnableIT_TXE(USART1); //разрешаем прерывание
	USART1->CR1 |= USART_CR1_TXEIE;// Interrupt enable
}


void Print_to_USART1(volatile char *data)
{
#if INTERUPT
	putStringInBufferUart1(data);
	putCharInBufferUart1('\n');
#else
	putStringDirectToUart1(data);
	putCharDirectToUart1('\n');
#endif
}


void Print_to_USART1_d(volatile int32_t value,volatile char *string,uint8_t koma)
{
	char *str;

	if (koma == 0)
	{
		str = itoa(value);
	}
	else
		str = itoa_koma(value,koma);

#if INTERUPT
	putStringInBufferUart1(string);
	putStringInBufferUart1 (str);
	putCharInBufferUart1('\n');
#else
	putStringDirectToUart1(string);
	putStringDirectToUart1(str);
	putCharDirectToUart1('\n');
#endif
}


void putCharInBufferUart1(uint8_t c) //вывод данных
{
	//while (bufferUart1.tx_counter >= TX_BUFFER_SIZE); //если буфер переполнен, ждем
	LL_USART_DisableIT_TXE(USART1); //запрещаем прерывание, чтобы оно не мешало менять переменную
	bufferUart1.tx_buffer[bufferUart1.tx_wr_index++]=c; //то кладем данные в буфер
	if (bufferUart1.tx_wr_index == TX_BUFFER_SIZE)
		bufferUart1.tx_wr_index=0; //идем по кругу
	++bufferUart1.tx_counter; //увеличиваем счетчик количества данных в буфере
	#if DEBUG_LEVEL
		if (bufferUart1.tx_buffer_overflow < bufferUart1.tx_counter)
			bufferUart1.tx_buffer_overflow = bufferUart1.tx_counter;
	#endif

	LL_USART_EnableIT_TXE(USART1); //разрешаем прерывание

}
void putStringInBufferUart1(volatile char *s)
{
  while (*s != 0)
	  putCharInBufferUart1(*s++);
}

void putDataInBufferUart1(volatile char *s, uint8_t lenght)
{
	uint8_t i;
    for (i = 0; i < lenght; i++ )
    {
	   putCharInBufferUart1(s[i]);
    }
    putCharInBufferUart1('\n');
}



void putCharDirectToUart1(uint8_t c)
{
	while (!LL_USART_IsActiveFlag_TXE(USART1));
	LL_USART_TransmitData8(USART1, c);
	c++;
}

void putStringDirectToUart1(volatile char *s)
{
  while (*s != 0)
	  putCharDirectToUart1(*s++);
}

uint8_t getCharFromBufferUART1(void) //прием данных
{
	uint8_t data; //переменная для данных
	while (bufferUart1.rx_counter==0) return;  //если данных нет, ждем
	data=bufferUart1.rx_buffer[bufferUart1.rx_rd_index++]; //берем данные из буфера
	if (bufferUart1.rx_rd_index == RX_BUFFER_SIZE) bufferUart1.rx_rd_index=0; //идем по кругу
	USART1->CR1 &= ~USART_CR1_RXNEIE;  // Interrupt Disable
	--bufferUart1.rx_counter; //чтобы оно не помешало изменить переменную
	USART1->CR1 |= USART_CR1_RXNEIE;// Interrupt enable
	return data;

}










//UART3===========================================================


void printToBufferUART2(volatile char* str)//10us for one char 24Mgz
{
	//while (bufferUart2.tx_counter >= TX_BUFFER_SIZE-30); //если буфер переполнен, ждем

	//LL_USART_DisableIT_TXE(USART2); //запрещаем прерывание, чтобы оно не мешало менять переменную
	USART3->CR1 &= ~USART_CR1_TXEIE;  // Interrupt Disable

	while (*str != 0)
	{
		bufferUart2.tx_buffer[bufferUart2.tx_wr_index++]=*str++; //то кладем данные в буфер
		if (bufferUart2.tx_wr_index == TX_BUFFER_SIZE) bufferUart2.tx_wr_index=0; //идем по кругу
		++bufferUart2.tx_counter; //увеличиваем счетчик количества данных в буфере
		#if DEBUG_LEVEL
			if (bufferUart2.tx_buffer_overflow < bufferUart2.tx_counter)
				bufferUart2.tx_buffer_overflow = bufferUart2.tx_counter;
		#endif
	}
	bufferUart2.tx_buffer[bufferUart2.tx_wr_index++]='\n'; //то кладем данные в буфер
	if (bufferUart2.tx_wr_index == TX_BUFFER_SIZE) bufferUart2.tx_wr_index=0; //идем по кругу
	++bufferUart2.tx_counter; //увеличиваем счетчик количества данных в буфере
	#if DEBUG_LEVEL
		if (bufferUart2.tx_buffer_overflow < bufferUart2.tx_counter)
			bufferUart2.tx_buffer_overflow = bufferUart2.tx_counter;
	#endif

	//LL_USART_EnableIT_TXE(USART2); //разрешаем прерывание
	USART3->CR1 |= USART_CR1_TXEIE;// Interrupt enable
}


void printToBufferWithoutEndUART2(volatile char* str)//10us for one char 24Mgz
{
	//while (bufferUart2.tx_counter >= TX_BUFFER_SIZE-30); //если буфер переполнен, ждем

	//LL_USART_DisableIT_TXE(USART2); //запрещаем прерывание, чтобы оно не мешало менять переменную
	USART3->CR1 &= ~USART_CR1_TXEIE;  // Interrupt Disable

	while (*str != 0)
	{
		bufferUart2.tx_buffer[bufferUart2.tx_wr_index++]=*str++; //то кладем данные в буфер
		if (bufferUart2.tx_wr_index == TX_BUFFER_SIZE) bufferUart2.tx_wr_index=0; //идем по кругу
		++bufferUart2.tx_counter; //увеличиваем счетчик количества данных в буфере
		#if DEBUG_LEVEL
			if (bufferUart2.tx_buffer_overflow < bufferUart2.tx_counter)
				bufferUart2.tx_buffer_overflow = bufferUart2.tx_counter;
		#endif

	}
	USART3->CR1 |= USART_CR1_TXEIE;// Interrupt enable
}

void printToBufferUART2D(volatile char* str, volatile int32_t value, volatile uint8_t koma) //10us for one char + 6us for one digit 24Mgz
{
	char *str1;
	if (koma == 0)
		str1 = itoa(value);
	else
		str1 = itoa_koma(value,koma);

	//while (bufferUart2.tx_counter >= TX_BUFFER_SIZE-30); //если буфер переполнен, ждем

	//LL_USART_DisableIT_TXE(USART2); //запрещаем прерывание, чтобы оно не мешало менять переменную
	USART3->CR1 &= ~USART_CR1_TXEIE;  // Interrupt Disable

	while (*str != 0)
	{
		bufferUart2.tx_buffer[bufferUart2.tx_wr_index++]=*str++; //то кладем данные в буфер
		if (bufferUart2.tx_wr_index == TX_BUFFER_SIZE) bufferUart2.tx_wr_index=0; //идем по кругу
		++bufferUart2.tx_counter; //увеличиваем счетчик количества данных в буфере
		#if DEBUG_LEVEL
			if (bufferUart2.tx_buffer_overflow < bufferUart2.tx_counter)
				bufferUart2.tx_buffer_overflow = bufferUart2.tx_counter;
		#endif

	}

	while (*str1 != 0)
	{
		bufferUart2.tx_buffer[bufferUart2.tx_wr_index++]=*str1++; //то кладем данные в буфер
		if (bufferUart2.tx_wr_index == TX_BUFFER_SIZE) bufferUart2.tx_wr_index=0; //идем по кругу
		++bufferUart2.tx_counter; //увеличиваем счетчик количества данных в буфере
		#if DEBUG_LEVEL
			if (bufferUart2.tx_buffer_overflow < bufferUart2.tx_counter)
				bufferUart2.tx_buffer_overflow = bufferUart2.tx_counter;
		#endif

	}

	bufferUart2.tx_buffer[bufferUart2.tx_wr_index++]='\n'; //то кладем данные в буфер
	if (bufferUart2.tx_wr_index == TX_BUFFER_SIZE) bufferUart2.tx_wr_index=0; //идем по кругу
	++bufferUart2.tx_counter; //увеличиваем счетчик количества данных в буфере
	#if DEBUG_LEVEL
		if (bufferUart2.tx_buffer_overflow < bufferUart2.tx_counter)
			bufferUart2.tx_buffer_overflow = bufferUart2.tx_counter;
	#endif

	//LL_USART_EnableIT_TXE(USART2); //разрешаем прерывание
	USART3->CR1 |= USART_CR1_TXEIE;// Interrupt enable
}


void Print_to_USART2(volatile char *data)
{
#if INTERUPT
	putStringInBufferUart2(data);
	putCharInBufferUart2('\n');
#else
	putStringDirectToUart2(data);
	putCharDirectToUart2('\n');
#endif
}


void Print_to_USART2_d(volatile int32_t value,volatile char *string,uint8_t koma)
{
	char *str;

	if (koma == 0)
	{
		str = itoa(value);
	}
	else
		str = itoa_koma(value,koma);

#if INTERUPT
	putStringInBufferUart2(string);
	putStringInBufferUart2 (str);
	putCharInBufferUart2('\n');
#else
	putStringDirectToUart2(string);
	putStringDirectToUart2(str);
	putCharDirectToUart2('\n');
#endif
}


void putCharInBufferUart2(uint8_t c) //вывод данных
{
	//while (bufferUart2.tx_counter >= TX_BUFFER_SIZE); //если буфер переполнен, ждем
	LL_USART_DisableIT_TXE(USART3); //запрещаем прерывание, чтобы оно не мешало менять переменную
	bufferUart2.tx_buffer[bufferUart2.tx_wr_index++]=c; //то кладем данные в буфер
	if (bufferUart2.tx_wr_index == TX_BUFFER_SIZE)
		bufferUart2.tx_wr_index=0; //идем по кругу
	++bufferUart2.tx_counter; //увеличиваем счетчик количества данных в буфере
	#if DEBUG_LEVEL
		if (bufferUart2.tx_buffer_overflow < bufferUart2.tx_counter)
			bufferUart2.tx_buffer_overflow = bufferUart2.tx_counter;
	#endif

	LL_USART_EnableIT_TXE(USART3); //разрешаем прерывание

}
void putStringInBufferUart2(volatile char *s)
{
  while (*s != 0)
	  putCharInBufferUart2(*s++);
}

void putCharDirectToUart2(uint8_t c)
{
	while (!LL_USART_IsActiveFlag_TXE(USART3));
	LL_USART_TransmitData8(USART3, c);
	c++;
}

void putStringDirectToUart2(volatile char *s)
{
  while (*s != 0)
	  putCharDirectToUart2(*s++);
}


