#include "RC_1_main.h"
//
//
const uint8_t end1 = B11111110;
const uint8_t end2 = B11111101;
uint8_t input[20];               // сюда приходят байты из порта
int inputsize=0;
//boolean lifeled;
long int lifetm;
long int but[4];
bool set[15];
bool dd[4];
//
unsigned long dd1time;
unsigned long dd2time;
unsigned long light_time=45000;
//
dht11 sensor;
//
//
void initFunc() 
{    
  //включаем ватчдог
  wdt_enable(WDTO_500MS);
  //режим шины - прием - сразу
  D2_Out;//pinMode(2, OUTPUT);  
  D2_Low;//digitalWrite(2, LOW);
  //Serial.begin(115200);
  sei(); // ????????? ??????????
  init(); // ????????????? ???????? ???????
  UART_Init(115200);
  //пульс
  D13_Out;//pinMode(13, OUTPUT);
  //lifeled = true; 
  D13_High;//digitalWrite(13, HIGH);
  //реле
  D11_Out;//pinMode(11, OUTPUT);  
  D10_Out;//pinMode(10, OUTPUT);  
  D9_Out;//pinMode(9, OUTPUT);  
  D8_Out;//pinMode(8, OUTPUT);  
  //релейные кнопки "SET"
  D7_In;//pinMode(7, INPUT);  
  D6_In;//pinMode(6, INPUT);  
  D5_In;//pinMode(5, INPUT);  
  D4_In;//pinMode(4, INPUT);
  D7_High;//digitalWrite(7, HIGH);
  D6_High;//digitalWrite(6, HIGH);
  D5_High;//digitalWrite(5, HIGH);
  D4_High;//digitalWrite(4, HIGH);  
  //несвязанные с реле кнопки "BUT"
  D12_In;//pinMode(12, INPUT);  
  D3_In;//pinMode(3, INPUT);
  D12_High;//digitalWrite(12, HIGH);
  D3_High;//digitalWrite(3, HIGH);
  D16_Out;//pinMode(A4, INPUT);  
  D17_Out;
  D18_In;//pinMode(A4, INPUT);  
  D19_In;//pinMode(A5, INPUT);
  D18_High;//digitalWrite(A4, HIGH);
  D19_High;//digitalWrite(A5, HIGH);
  // инициализируем начальный массив настроек
  set[0]=false;
  set[1]=false;
  set[2]=false;
  set[3]=false;
  set[10]=false;
  set[11]=false;
  set[12]=false;
  set[13]=false;
  set[14]=false;
  
  but[0]=-1;
  but[1]=-1;
  but[2]=-1;
  but[3]=-1;
  //загружаем настройки
  loadSets();
  //настройка датчиков движения
  dd[0]=false;
  dd[1]=false;
  dd[2]=false;
  dd[3]=false;
  //подтягиваем к 5V RX
  D0_In;
  //serialStartPrintln("ok");
}
//
// ????????? ???????? ????????? ??????????
void changeValue(bool * val)
{
  if (*val==false) *val=true;
    else *val=false;
}
//
// ?????? ??????? ????????? ?? ????????????????? ??????
void saveSets()
{
  WriteEEPROM_Byte(0, set[4]);
  WriteEEPROM_Byte(1, set[5]);
  WriteEEPROM_Byte(2, set[6]);
  WriteEEPROM_Byte(3, set[7]);
  WriteEEPROM_Byte(4, set[8]);
  WriteEEPROM_Byte(5, set[9]);
}
//
// ???????? ??????? ????????? ? ????????????????? ??????
void loadSets()
{
  set[4]=ReadEEPROM_Byte(0);
  set[5]=ReadEEPROM_Byte(1);
  set[6]=ReadEEPROM_Byte(2);
  set[7]=ReadEEPROM_Byte(3);
  set[8]=ReadEEPROM_Byte(4);
  set[9]=ReadEEPROM_Byte(5);
}
//
// ?????? ??????? ????????? ?? ??????????? ???????
void readSets()
{
  if (input[1] & B10000000) set[0]=true; 
  else set[0]=false;
  if (input[1] & B01000000) set[1]=true; 
  else set[1]=false;
  if (input[1] & B00100000) set[2]=true;
  else set[2]=false;
  if (input[1] & B00010000) set[3]=true; 
  else  set[3]=false;
  if (input[1] & B00001000) set[4]=true; 
  else set[4]=false;
  if (input[1] & B00000100) set[5]=true; 
  else set[5]=false;
  if (input[1] & B00000010) set[6]=true; 
  else set[6]=false;
  if (input[1] & B00000001) set[7]=true; 
  else set[7]=false;
  if (input[2] & B01000000) set[8]=true; 
  else set[8]=false;
  if (input[2] & B00100000) set[9]=true; 
  else set[9]=false;
    if (input[2] & B00010000) set[10]=true; 
  else set[10]=false;
    if (input[2] & B00001000) set[11]=true; 
  else set[11]=false;
    if (input[2] & B00000100) set[12]=true; 
  else set[12]=false;
    if (input[2] & B00000010) set[13]=true; 
  else set[13]=false;
    if (input[2] & B00000001) set[14]=true; 
  else set[14]=false;
  saveSets();
}
//
// ????????? ????????? ??????? ? ???????????? ? ???????? ?????????
void applySets()
{
  if (set[0]==true) D8_High; else D8_Low;  
  if (set[1]==true) D9_High; else D9_Low;    
  if (set[2]==true) D10_High; else 
  {
    if ((set[8]==1)&&(dd[0]==1)) D10_High;
    else D10_Low;   
  }
  if (set[3]==true) D11_High; else 
  {
    if ((set[9]==1)&&(dd[1]==1)) D11_High;
    else D11_Low;  
  }
  if (set[10]==true) D16_High; else D16_Low;  
  if (set[11]==true) D17_High; else D17_Low;    
}
//
// ???? ??????? ??????, ? ?????????? ???????? ?????????
void seekButtPress()
{
  if (set[4]==1)
  {
    if (but[0]!=-1)
    {
      if (lifetm<but[0]) 
        {if (LIFETM_LEN+lifetm-but[0] > LIFETM_LEN/SEEK_BUTT_DEVISOR) but[0]=-1;}
      else if (lifetm-but[0] > LIFETM_LEN/SEEK_BUTT_DEVISOR) but[0]=-1;
    }
    if ((D4_Read==LOW)&&(but[0]==-1))
    {
      changeValue(&set[0]);
      but[0]=lifetm;
    }
  }
  if (set[5]==1)
  {
    if (but[1]!=-1)
    {
      if (lifetm<but[1]) 
        {if (LIFETM_LEN+lifetm-but[1] > LIFETM_LEN/SEEK_BUTT_DEVISOR) but[1]=-1;}
      else if (lifetm-but[1] > LIFETM_LEN/SEEK_BUTT_DEVISOR) but[1]=-1;
    }
    if ((D5_Read==LOW)&&(but[1]==-1))
    {
      changeValue(&set[1]);
      but[1]=lifetm;
    }
  }
  if ((set[6]==1)&&(set[8]==0))
  {
    if (but[2]!=-1)
    {
      if (lifetm<but[2]) 
        {if (LIFETM_LEN+lifetm-but[2] > LIFETM_LEN/SEEK_BUTT_DEVISOR) but[2]=-1;}
      else if (lifetm-but[2] > LIFETM_LEN/SEEK_BUTT_DEVISOR) but[2]=-1;
    }
    if ((D6_Read==LOW)&&(but[2]==-1))
    {
      changeValue(&set[2]);
      but[2]=lifetm;
    }
  }
  if ((set[7]==1)&&(set[9]==0))
  {
    if (but[3]!=-1)
    {
      if (lifetm<but[3]) 
        {if (LIFETM_LEN+lifetm-but[3] > LIFETM_LEN/SEEK_BUTT_DEVISOR) but[3]=-1;}
      else if (lifetm-but[3] > LIFETM_LEN/SEEK_BUTT_DEVISOR) but[3]=-1;
    }
    if ((D7_Read==LOW)&&(but[3]==-1))
    {
      changeValue(&set[3]);
      but[3]=lifetm;
    }
  }
}
// ???????, ????????? ?? ??????? ???????? - ???? ??, ?????????? ???????
//
void seekDD()
{
  if (set[8]==1)
  {
    if (dd[2]==true)
    {
      unsigned long a = (millis()-dd1time);
      if (a > 10)
      {
        dd[2]=false;
        dd[0]=false;
      }
    }
    if ((D6_Read==LOW))
    {
      dd[0]=true;
      dd[2]=true;
      dd1time=millis();
    }
  }
  if (set[9]==1)
  {
    if (dd[3]==true)
    {
      unsigned long a = (millis()-dd2time);
      if (a > light_time)
      {
        dd[3]=false;
        dd[1]=false;
      }
    }
    if ((D7_Read==HIGH))
    {
      dd[1]=true;
      dd[3]=true;
      dd2time=millis();
    }
  }
}
// ???????? ??????? ????????? ???????????
// 
void sendAll()
{
  uint8_t buf[12];
  buf[0] = B00000000; 
  buf[1] = B00000000; buf[2] = B00000000; buf[3] = B00000000; buf[4] = B00000000; buf[5] = B00000000;
  buf[6] = B00000000; buf[7] = B00000000; buf[8] = B00000000; buf[9] = B00000000; buf[10] = B00000000;
  buf[11] = B00000000; 
  D0_High;//digitalWrite(0, HIGH);
  D2_High;//digitalWrite(2, HIGH);
  buf[0] = (uint8_t)SN;
  if (set[0]==true) buf[1] |= B10000000;  
  if (set[1]==true) buf[1] |= B01000000;  
  if (set[2]==true) buf[1] |= B00100000;  
  if (set[3]==true) buf[1] |= B00010000;  
  if (set[4]==true) buf[1] |= B00001000;  
  if (set[5]==true) buf[1] |= B00000100;  
  if (set[6]==true) buf[1] |= B00000010;  
  if (set[7]==true) buf[1] |= B00000001;  
  if (set[8]==true) buf[2] |= B01000000;  
  if (set[9]==true) buf[2] |= B00100000;  
  if (set[10]==true) buf[2] |= B00010000;  
  if (set[11]==true) buf[2] |= B00001000;  
  if (set[12]==true) buf[2] |= B00000100;  
  if (set[13]==true) buf[2] |= B00000010;  
  if (set[14]==true) buf[2] |= B00000001;  
  int ibuf = A1_Read;
  buf[3] = (uint8_t)(ibuf >> 8);
  buf[4] = (uint8_t)(ibuf & B11111111);
  if (D4_Read==HIGH) buf[5] |= B10000000; 
  if (D5_Read==HIGH) buf[5] |= B01000000; 
  if (D6_Read==HIGH) buf[5] |= B00100000; 
  if (D7_Read==HIGH) buf[5] |= B00010000; 
  buf[6] = (uint8_t)sensor.temperature;
  buf[7] = (uint8_t)sensor.humidity;
  if (D3_Read==HIGH) buf[8] |= B10000000; 
  if (D12_Read==HIGH) buf[8] |= B01000000; 
  if (D18_Read==HIGH) buf[8] |= B00100000; 
  if (D19_Read==HIGH) buf[8] |= B00010000; 
  buf[9] = 0 - (buf[0]+buf[1]+buf[2]+buf[3]+buf[4]+buf[5]+buf[6]+buf[7]+buf[8]);
  buf[10] = end1;
  buf[11] = end2;
  UART_SendArray(buf, 12);
  delay(1);
  D2_Low;//digitalWrite(2, LOW);
  D0_Low;//digitalWrite(0, LOW);
}
//
//
int main()
{
  initFunc();
  while(1)
  {
	  //сброс таймера ватчдога
	  wdt_reset();
	  lifetm++;
	  //мигание пульсом
	  if (lifetm==LIFETM_LEN)
	  {
		D13_Inv;
		sensor.read();
		lifetm = 0;
		//disable_Int();
		//if(sensor.read()==DHTLIB_OK)
		//{
		//  tem=sensor.temperature;
		//  hum=sensor.humidity;
		//}
		//else
		//{  
		//  tem=-300;
		//  hum=-300;
		//}
	    //enable_Int();
	  }
	  //отлавливаем нажатия связанных кнопок
	  seekButtPress();
	  //обрабатываем срабатывания датчиков движения
	  seekDD();
	  //меняем положение релюшек по конфигурации
	  applySets();
	  //if (lifetm == LIFETM_LEN)
		//lifetm = 0;
	}
}
//
//
//вызывается, когда что нибудь попадает в последовательный порт, 
//выполняется параллельно с основным циклом
ISR(USART_RX_vect)
{
  while(UART_ReadByte(input[inputsize]))
  {
    inputsize++;
    if ((inputsize > 1)&&(input[inputsize-2]==end1)&&(input[inputsize-1]==end2))
    {
    
      if (inputsize == 6)
      {
        if ((input[0] == (uint8_t)SN) && (input[0] == input[1])&& (input[2] == input[1])&& (input[2] == input[3])) 
        {
          sendAll();
          //D13_Inv;
          inputsize = 0;
        }
        if ((input[0] == (uint8_t)SN) && ( (uint8_t)(0-(input[0]+input[1]+input[2]))==(uint8_t)input[3] ))
        {
          readSets();
          inputsize = 0;
        }
      }
      inputsize = 0;
    }
    if (inputsize > 18) {inputsize = 0;}
  }
}
//
//
//
