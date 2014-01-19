#include "CyberLib.h"
// непосредственная работа с передатчиком, без буффера, хреначим прямо в регистры
//**********Small UART****************************
#define UART_DOUBLESPEED
#define UART_TXREADY UDRE0
#define UART_RXREADY RXC0
#define UART_DOUBLE	U2X0
#define UDR  UDR0
#define UCRA UCSR0A
#define UCRB UCSR0B
#define UCRC UCSR0C
#define UCRC_VALUE ((1<<UCSZ01) | (1<<UCSZ00))
#define RXCIE RXCIE0
#define TXCIE TXCIE0
#define RXEN RXEN0
#define TXEN TXEN0
#define UBRRL UBRR0L
#define UBRRH UBRR0H
#define SIG_UART_TRANS SIG_USART_TRANS
#define SIG_UART_RECV  SIG_USART_RECV
#define SIG_UART_DATA  SIG_USART_DATA
#define UART_CALC_BAUDRATE(baudRate) ((uint32_t)((F_CPU) + ((uint32_t)baudRate * 4UL)) / ((uint32_t)(baudRate) * 8UL) - 1)

void enable_Int()
{
	UCRB |= (1<<RXCIE);
}

void disable_Int()
{
	UCRB &= B11111111 ^ (1<<RXCIE);
}

void UART_Init(uint32_t UART_BAUD_RATE)
{
    UBRRH = (UART_CALC_BAUDRATE(UART_BAUD_RATE)>>8) & 0xFF;
	UBRRL = (UART_CALC_BAUDRATE(UART_BAUD_RATE) & 0xFF);
    UCSR0A = ( 1<<UART_DOUBLE );
    UCRB = ((1<<TXEN) | (1<<RXEN));
    UCRB |= (1<<RXCIE);
    UCRC = UCRC_VALUE;
}

void UART_SendByte(uint8_t data)
{
    while (!(UCRA & (1<<UART_TXREADY)));
	UDR = data;
}

bool UART_ReadByte(uint8_t& data)
{
    if (UCRA & (1<<UART_RXREADY))
	{
		data = UDR;
		return true;
	} else return false;
}

void UART_SendArray(uint8_t *buffer, uint16_t bufferSize)
{
	for(uint16_t i=0; i<bufferSize; i++)
		UART_SendByte(buffer[i]);
    //while (!(UCRA & (1<<UART_TXREADY)));
}
// стыренная из инета, хз почему - но быстрее
//**********AnalogRead***************************
uint16_t AnRead(uint8_t An_pin)
{
  ADMUX=An_pin;   
  delayMicroseconds(10);	  
  ADCSRA=B11000110;	//B11000111-125kHz B11000110-250kHz 
  while (ADCSRA & (1 << ADSC));
  An_pin = ADCL;
  uint16_t An = ADCH; 
  return (An<<8) + An_pin;
}
//******************EEPROM*******************************
void WriteEEPROM_Byte(uint8_t addr, uint8_t data)  //сохранить в EEPROM
{
		eeprom_write_byte((uint8_t*)addr, data);
}

void WriteEEPROM_Word(uint16_t addr, uint16_t data)
{
		eeprom_write_word((uint16_t*)addr, data);
}

void WriteEEPROM_Long(uint8_t addr, uint32_t data)  //сохранить в EEPROM
{           
  addr *= 4;
        eeprom_write_byte((uint8_t*)addr, data & 0xFF);
        eeprom_write_byte((uint8_t*)addr+1, (data & 0xFF00) >> 8);
        eeprom_write_byte((uint8_t*)addr+2, (data & 0xFF0000) >> 16);
        eeprom_write_byte((uint8_t*)addr+3, (data & 0xFF000000) >> 24);
		
	  // addr *= 2;
        // eeprom_write_word((uint16_t*)addr, data & 0xFFFF);
        // eeprom_write_word((uint16_t*)addr+1, (data & 0xFFFF0000) >> 16);
}

uint8_t ReadEEPROM_Byte(uint8_t addr)
{
		return eeprom_read_byte((uint8_t*)addr);
}

uint16_t ReadEEPROM_Word(uint16_t addr)
{
		return eeprom_read_word((uint16_t*)addr);
}

uint32_t ReadEEPROM_Long(uint8_t addr)  // считываем значение из EEPROM
{
  addr *= 4; 
        uint32_t ir_code = eeprom_read_byte((uint8_t*)addr+3); 
        ir_code = (ir_code << 8) | eeprom_read_byte((uint8_t*)addr+2);
        ir_code = (ir_code << 8) | eeprom_read_byte((uint8_t*)addr+1);
        ir_code = (ir_code << 8) | eeprom_read_byte((uint8_t*)addr);
		//eeprom_read_word((uint16_t*) addr)
  return ir_code;
}
