#include "RTC.h"
#include<Arduino.h>
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68

RTC::RTC()
{
    Wire.begin();
}

byte RTC::decToBcd(byte val)
{
    return( (val/10*16) + (val%10));
}

byte RTC::bcdToDec(byte val)
{
    return( (val/16*10) + (val%16));
}

void RTC::setTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
    this->init_second = second; 
    this->init_minute = minute; 
    this->init_hour = hour; 
    this->init_dayOfWeek = dayOfWeek; 
    this->init_dayOfMonth = dayOfMonth; 
    this->init_month = month; 
    this->init_year = year;
    rtc.adjust(DateTime(year, month, dayOfMonth, hour, minute, second));

}

void RTC::readTime()
{
    DateTime now = rtc.now();
    // Wire.beginTransmission(DS3231_I2C_ADDRESS);
    // Wire.write(0); // set DS3231 register pointer to 00h
    // Wire.endTransmission();
    // Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // // request seven bytes of data from DS3231 starting from register 00h
    this->second = now.second();
    this->minute = now.minute();
    this->hour = now.hour();
    this->dayOfWeek = now.dayOfTheWeek();
    this->dayOfMonth = now.day();
    this->month = now.month();
    this->year = now.year();
}

void RTC::displayTime()
{
    this->readTime();
    // send it to the serial monitor
    Serial.print(this->hour, DEC);
    // convert the byte variable to a decimal number when displayed
    Serial.print(":");
    if (this->minute<10){
        Serial.print("0");
    }
    Serial.print(this->minute, DEC);
    Serial.print(":");
    if (this->second<10){
        Serial.print("0");
    }
    Serial.print(this->second, DEC);
    Serial.print(" ");
    Serial.print(this->dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(this->month, DEC);
    Serial.print("/");
    Serial.print(this->year, DEC);
    Serial.print(" Day of week: ");
    switch(this->dayOfWeek){
    case 0:
        Serial.println("Sunday");
        break;
    case 1:
        Serial.println("Monday");
        break;
    case 2:
        Serial.println("Tuesday");
        break;
    case 3:
        Serial.println("Wednesday");
        break;
    case 4:
        Serial.println("Thursday");
        break;
    case 5:
        Serial.println("Friday");
        break;
    case 6:
        Serial.println("Saturday");
        break;
    }
}

byte RTC::Second()
{
    return this->second;
}

byte RTC::Minute()
{
    return this->minute;
}

byte RTC::Hour()
{
    return this->hour;
}

byte RTC::DayOfWeek()
{
    return dayOfWeek;
}

byte RTC::DayOfMonth()
{
    return dayOfMonth;
}

byte RTC::Month()
{
    return month;
}

byte RTC::Year()
{
    return year;
}

unsigned long int RTC::daysPassed() 
{ 
    DateTime now = rtc.now();
    Serial.println(now.year());
    TimeSpan span = now - init_date;
    return span.days();
} 

void RTC::setInitialDate(byte second, byte minute, byte hour, byte dayOfWeek, 
                        byte dayOfMonth, byte month, byte year)
{
    init_date = DateTime(year, month, dayOfMonth, hour, minute, second);
    Serial.println(init_date.year());
}

// unsigned long int RTC::weeksPassed()
// {
//     return this->daysPassed(DateTime init_date) % 7;
// }