#include "dht11.h"

// общение с термометром
// Возвращаемые значения:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
int dht11::read()
{
        // буфер приема
        uint8_t bits[5];
        uint8_t cnt = 7;
        uint8_t idx = 0;
        // чистим буфер
        for (int i=0; i< 5; i++) bits[i] = 0;
        // сам запрос - дело не хитрое
        D14_Out;
        D14_Low; //digitalWrite(pin, LOW);
		delay(20);
        D14_High; //digitalWrite(pin, HIGH);
        delayMicroseconds(80);
        D14_In; //pinMode(pin, INPUT);
        
        
        humidity    = -300; 
        temperature = -300; 
        // ответ или ругаемся!
        unsigned int loopCnt = 10000;
        while(D14_Read == LOW)
                if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
        loopCnt = 10000;
        while(D14_Read == HIGH)
                if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
        // читаем входящие данные - 5 байт, если нету - ругаемся
        for (int i=0; i<40; i++)
        {
                loopCnt = 10000;
                while(D14_Read == LOW)
                        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

                unsigned long t = micros();

                loopCnt = 10000;
                while(D14_Read == HIGH)
                        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

                if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
                if (cnt == 0)   // next byte?
                {
                        cnt = 7;    // restart at MSB
                        idx++;      // next byte!
                }
                else cnt--;
        }

        // если таки правильные - записываем 
        // а bits[1] и bits[3] всегда нули.
        humidity    = bits[0]; 
        temperature = bits[2]; 

        uint8_t sum = bits[0] + bits[2];  

        if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
        return DHTLIB_OK;
}