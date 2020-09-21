#ifndef RTC_H
#define RTC_H
#include "Wire.h"
#include<RTClib.h>
#define DS3231_I2C_ADDRESS 0x68
#include "Arduino.h"

class RTC
{
    private:
        RTC_DS3231 rtc;
        DateTime init_date;
        byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
        byte init_second, init_minute, init_hour, init_dayOfWeek, init_dayOfMonth, init_month, init_year;
        const int monthDays[12] = {31, 28, 31, 30, 31, 30, 
                           31, 31, 30, 31, 30, 31}; 

    public:
        RTC();
        byte decToBcd(byte val);
        byte bcdToDec(byte val);

        void setTime(byte second, byte minute, byte hour, byte dayOfWeek, 
                        byte dayOfMonth, byte month, byte year);
        void setInitialDate(byte second, byte minute, byte hour, byte dayOfWeek, 
                        byte dayOfMonth, byte month, byte year);
        void readTime();
        void displayTime();
        byte Second();
        byte Minute();
        byte Hour();
        byte DayOfWeek();
        byte DayOfMonth();
        byte Month();
        byte Year();
        int countLeapYears(byte y, byte m);
        unsigned long int daysPassed(); 
        unsigned long int weeksPassed();

};

#endif 