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
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

void RTC::readTime()
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    this->second = bcdToDec(Wire.read() & 0x7f);
    this->minute = bcdToDec(Wire.read());
    this->hour = bcdToDec(Wire.read() & 0x3f);
    this->dayOfWeek = bcdToDec(Wire.read());
    this->dayOfMonth = bcdToDec(Wire.read());
    this->month = bcdToDec(Wire.read());
    this->year = bcdToDec(Wire.read());
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
    case 1:
        Serial.println("Sunday");
        break;
    case 2:
        Serial.println("Monday");
        break;
    case 3:
        Serial.println("Tuesday");
        break;
    case 4:
        Serial.println("Wednesday");
        break;
    case 5:
        Serial.println("Thursday");
        break;
    case 6:
        Serial.println("Friday");
        break;
    case 7:
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

int RTC::countLeapYears(byte y, byte m) 
{
    int years = y + 2000;
    // Check if the current year needs to be considered 
    // for the count of leap years or not 
    if (m<= 2) 
        years--; 
    // An year is a leap year if it is a multiple of 4, 
    // multiple of 400 and not a multiple of 100. 
    return years / 4 - years / 100 + years / 400; 
} 

unsigned long int RTC::daysPassed() 
{ 
    this->readTime();
    // COUNT TOTAL NUMBER OF DAYS BEFORE FIRST DATE 'dt1' 
  
    // initialize count using years and day 
    long int n1 = this->init_year*365 + this->init_dayOfMonth; 
  
    // Add days for months in given date 
    for (int i=0; i<this->init_month - 1; i++) 
        n1 += monthDays[i]; 
  
    // Since every leap year is of 366 days, 
    // Add a day for every leap year 
    n1 += countLeapYears(this->init_year, this->init_month); 
  
    // SIMILARLY, COUNT TOTAL NUMBER OF DAYS BEFORE 'dt2' 
  
    long int n2 = year*365 + dayOfMonth; 
    for (int i=0; i<month - 1; i++) 
        n2 += monthDays[i]; 
    n2 += countLeapYears(year, month); 
  
    // return difference between two counts 
    return (n2 - n1); 
} 

void RTC::setInitialDate(byte second, byte minute, byte hour, byte dayOfWeek, 
                        byte dayOfMonth, byte month, byte year)
{
    this->init_second = second; 
    this->init_minute = minute; 
    this->init_hour = hour; 
    this->init_dayOfWeek = dayOfWeek; 
    this->init_dayOfMonth = dayOfMonth; 
    this->init_month = month; 
    this->init_year = year;
}

unsigned long int RTC::weeksPassed()
{
    return this->daysPassed() % 7;
}