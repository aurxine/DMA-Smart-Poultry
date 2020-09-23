#include "Farm.h"
#include<Arduino.h>

int writeByteInEEPROM(int init_addr, byte data)
{
    EEPROM.begin(eeprom_size);
    EEPROM.write(init_addr, data);
        if(EEPROM.commit())
        {
            Serial.println("EEPROM successfully committed");
        }
        else 
        {
            Serial.println("ERROR! EEPROM commit failed");
            return -1;
        }
    return ++init_addr; //new address
}

byte readByteInEEPRPOM(int address)
{
    EEPROM.begin(eeprom_size);
    byte data;
    data = EEPROM.read(address);
    //Serial.println("Data: "+String(data));
    return data;
}

Farm::Farm()
{
    this->set_Temperature();
    this->set_Humidity();
    this->set_Ammonia();
}

void Farm::getFarmType(String type)
{
    type.toLowerCase();
    if(type == "brooder")
    {
        this->Farm_type = 0;
    }
    else if(type == "grower")
    {
        this->Farm_type = 3;
    }
    else
    {
        this->Farm_type = 18;
    }

    writeByteInEEPROM(44, Farm_type);
    
}

void Farm::getInitAge(String age)
{
    this->Initial_age = age.toInt();
    writeByteInEEPROM(45, Initial_age);
}

void Farm::set_Temperature()
{
    Temperature_summer[0][0] = 32;
    Temperature_summer[0][1] = 35;
    Temperature_summer[0][2] = 35;
    Temperature_summer[0][3] = 36;

    Temperature_summer[1][0] = 30;
    Temperature_summer[1][1] = 32;
    Temperature_summer[1][2] = 32;
    Temperature_summer[1][3] = 32;

    Temperature_summer[2][0] = 28;
    Temperature_summer[2][1] = 29;
    Temperature_summer[2][2] = 29;
    Temperature_summer[2][3] = 29;

    Temperature_summer[3][0] = 18;
    Temperature_summer[3][1] = 28;
    Temperature_summer[3][2] = 30;
    Temperature_summer[3][3] = 35;

    Temperature_summer[4][0] = 28;
    Temperature_summer[4][1] = 28;
    Temperature_summer[4][2] = 30;
    Temperature_summer[4][3] = 35;

    Temperature_summer[5][0] = 28;
    Temperature_summer[5][1] = 28;
    Temperature_summer[5][2] = 30;
    Temperature_summer[5][3] = 35;

    Temperature_summer[6][0] = 16;
    Temperature_summer[6][1] = 26;
    Temperature_summer[6][2] = 30;
    Temperature_summer[6][3] = 35;

    Temperature_winter[0][0] = 32;
    Temperature_winter[0][1] = 35;
    Temperature_winter[0][2] = 35;
    Temperature_winter[0][3] = 36;

    Temperature_winter[1][0] = 32;
    Temperature_winter[1][1] = 32;
    Temperature_winter[1][2] = 32;
    Temperature_winter[1][3] = 32;

    Temperature_winter[2][0] = 29;
    Temperature_winter[2][1] = 29;
    Temperature_winter[2][2] = 29;
    Temperature_winter[2][3] = 29;

    Temperature_winter[3][0] = 27;
    Temperature_winter[3][1] = 27;
    Temperature_winter[3][2] = 27;
    Temperature_winter[3][3] = 27;

    Temperature_winter[4][0] = 24;
    Temperature_winter[4][1] = 24;
    Temperature_winter[4][2] = 24;
    Temperature_winter[4][3] = 24;

    Temperature_winter[5][0] = 21;
    Temperature_winter[5][1] = 21;
    Temperature_winter[5][2] = 21;
    Temperature_winter[5][3] = 21;

    Temperature_winter[6][0] = 16;
    Temperature_winter[6][1] = 18;
    Temperature_winter[6][2] = 21;
    Temperature_winter[6][3] = 32;

    for(int i = 7; i < max_Age_week; i++)
    {
        Temperature_summer[i][0] = 26;
        Temperature_summer[i][1] = 26;
        Temperature_summer[i][2] = 30;
        Temperature_summer[i][3] = 30;

        Temperature_winter[i][0] = 18;
        Temperature_winter[i][1] = 18;
        Temperature_winter[i][2] = 21;
        Temperature_winter[i][3] = 21;
    }
}

void Farm::show_Temperature(bool summer)
{
    Serial.println("Showing Temperature Data:");
    Serial.println("    Lowest   Optimal Low   Optimal High    Highest");
    if(summer)
    {
        for(int i = 0; i < max_Age_week; i++)
        {
            Serial.print(i); Serial.print("  :");
            Serial.print(Temperature_summer[i][0]); Serial.print("      ");
            Serial.print(Temperature_summer[i][1]); Serial.print("           ");
            Serial.print(Temperature_summer[i][2]); Serial.print("          ");
            Serial.print(Temperature_summer[i][3]); Serial.println();
            yield();
        }
    }
    else
    {
        for(int i = 0; i < max_Age_week; i++)
        {
            Serial.print(i); Serial.print("  :");
            Serial.print(Temperature_winter[i][0]); Serial.print("      ");
            Serial.print(Temperature_winter[i][1]); Serial.print("           ");
            Serial.print(Temperature_winter[i][2]); Serial.print("          ");
            Serial.print(Temperature_winter[i][3]); Serial.println();
            yield();
        }
    }
}

void Farm::set_Humidity()
{
    for(int i = 0; i < max_Age_week; i++)
    {
        if(i < 3)
        {
            Humidity[i][0] = 30;
            Humidity[i][1] = 50;
            Humidity[i][2] = 80;
            Humidity[i][3] = 85;
        }
        else
        {
            Humidity[i][0] = 30;
            Humidity[i][1] = 60;
            Humidity[i][2] = 80;
            Humidity[i][3] = 80;
        }
        
    }
}

void Farm::show_Humidity()
{
    Serial.println("Showing Humidity Data:");
    Serial.println("    Lowest   Optimal Low   Optimal High    Highest");

    for(int i = 0; i < max_Age_week; i++)
    {
        Serial.print(i); Serial.print("  :");
        Serial.print(Humidity[i][0]); Serial.print("      ");
        Serial.print(Humidity[i][1]); Serial.print("           ");
        Serial.print(Humidity[i][2]); Serial.print("          ");
        Serial.print(Humidity[i][3]); Serial.println();
        yield();
    }
}

void Farm::set_Ammonia()
{
    for(int i = 0; i < max_Age_week; i++)
    {
        if(i < 3)
        {
            Ammonia[i][0] = 10;
            Ammonia[i][1] = 12;
            Ammonia[i][2] = 25;
            Ammonia[i][3] = 33;
        }
        else
        {
            Ammonia[i][0] = 10;
            Ammonia[i][1] = 20;
            Ammonia[i][2] = 28;
            Ammonia[i][3] = 33;
        }
        
    }
}

void Farm::show_Ammonia()
{
    Serial.println("Showing Ammonia Data: ");
    Serial.println("    Lowest   Optimal Low   Optimal High    Highest");

    for(int i = 0; i < max_Age_week; i++)
    {
        Serial.print(i); Serial.print("  :");
        Serial.print(Ammonia[i][0]); Serial.print("      ");
        Serial.print(Ammonia[i][1]); Serial.print("           ");
        Serial.print(Ammonia[i][2]); Serial.print("          ");
        Serial.print(Ammonia[i][3]); Serial.println();
        yield();
    }
}


bool Farm::isSummer(int month)
{
    if(month >= 3 && month <= 10)
    return true;
    return false;
}

int Farm::check_Temperature(int month, int Age_week, float Current_Temperature)
{
    this->Farm_type = readByteInEEPRPOM(44);
    this->Initial_age = readByteInEEPRPOM(45);
    Month = month;
    this->Age_week = Age_week + Farm_type + Initial_age;
    this->Current_Temperature = Current_Temperature;

    if(this->isSummer(month))
    {
        if(Current_Temperature < (Temperature_summer[Age_week - 1][0] - Temperature_tolerance))
        return 2;

        else if(Current_Temperature > (Temperature_summer[Age_week - 1][3] + Temperature_tolerance))
        return 3;

        else
        {
            if(this->isOptimal_Temperature(month, Age_week, Current_Temperature))
            return 0;
            else
            return 1;
        }
        
        
    }
    else
    {
        if(Current_Temperature < (Temperature_winter[Age_week - 1][0] - Temperature_tolerance))
        return 2;

        else if(Current_Temperature > (Temperature_winter[Age_week - 1][3] + Temperature_tolerance))
        return 3;

        else
        {
            if(this->isOptimal_Temperature(month, Age_week, Current_Temperature))
            return 0;
            else
            return 1;
        }
    }
    
}

int Farm::check_Humidity(int Age_week, float Current_Humidity)
{
    this->Farm_type = readByteInEEPRPOM(44);
    this->Initial_age = readByteInEEPRPOM(45);
    this->Age_week = Age_week + Farm_type + Initial_age;
    this->Current_Humidity = Current_Humidity;
    if(Current_Humidity < (Humidity[Age_week - 1][0] - Humidity_tolerance))
        return 2;

        else if(Current_Humidity > (Humidity[Age_week - 1][3] + Humidity_tolerance))
        return 3;

        else
        {
            if(isOptimal_Humidity(Age_week, Current_Humidity))
            return 0;
            else
            return 1;
        }
}

int Farm::check_Ammonia(int Age_week, float Current_Ammonia)
{
    this->Farm_type = readByteInEEPRPOM(44);
    this->Initial_age = readByteInEEPRPOM(45);
    this->Age_week = Age_week + Farm_type + Initial_age;
    this->Current_Ammonia = Current_Ammonia;

    if(Current_Ammonia <= Ammonia[Age_week - 1][0])
    {
        //Serial.println("Optimal");
        return 0;
    }
    

    else if(Current_Ammonia >= Ammonia[Age_week - 1][0] && Current_Ammonia <= Ammonia[Age_week - 1][1])
    {
        //Serial.println("Optimal Low");
        return 1;
    }

    else if(Current_Ammonia >= Ammonia[Age_week - 1][1] && Current_Ammonia <= Ammonia[Age_week - 1][2])
    {
        //Serial.println("Optimal High");
        return 2;
    }

    else if(Current_Ammonia >= Ammonia[Age_week - 1][2] && Current_Ammonia <= Ammonia[Age_week - 1][3])
    {
        //Serial.println("High");
        return 3;
    }

    else
    {
        //Serial.println("Extreme");
        return 4;
    }
}

bool Farm::isOptimal_Temperature(int month, int Age_week, float Current_Temperature)
{
    this->Farm_type = readByteInEEPRPOM(44);
    this->Initial_age = readByteInEEPRPOM(45);
    Month = month;
    this->Age_week = Age_week + Farm_type + Initial_age;
    this->Current_Temperature = Current_Temperature;
    if(this->isSummer(month))
    {
        if(Current_Temperature >= Temperature_summer[Age_week - 1][1] && Current_Temperature <= Temperature_summer[Age_week - 1][2])
        return true;
        return false;
    }
    else
    {
        if(Current_Temperature >= Temperature_winter[Age_week - 1][1] && Current_Temperature <= Temperature_winter[Age_week - 1][2])
        return true;
        return false;
    }
    
}

bool Farm::isOptimal_Humidity(int Age_week, float Current_Humidity)
{
    this->Farm_type = readByteInEEPRPOM(44);
    this->Initial_age = readByteInEEPRPOM(45);
    this->Age_week = Age_week + Farm_type + Initial_age;
    this->Current_Humidity = Current_Humidity;
    if(Current_Humidity >= Humidity[Age_week - 1][1] && Current_Humidity <= Humidity[Age_week - 1][2])
    return true;
    return false;
}

bool Farm::isOptimal_Ammonia(int Age_week, float Current_Ammonia)
{
    this->Farm_type = readByteInEEPRPOM(44);
    this->Initial_age = readByteInEEPRPOM(45);
    this->Age_week = Age_week + Farm_type + Initial_age;
    if(Current_Ammonia <= Ammonia[Age_week - 1][0])
    return true;
    return false;
}

void Farm::show_Temperature_status()
{
    int status = this->check_Temperature(Month, Age_week, Current_Temperature);

    if(status == 2)
    {
        Serial.println("Temperature is low");
        Serial.print("Lowest acceptable temperature is: ");
        if(isSummer(Month))
            Serial.println(Temperature_summer[Age_week][0]);
        else
            Serial.println(Temperature_winter[Age_week][0]);
        Serial.print("Current temperature: ");
        Serial.println(Current_Temperature);
    }

    if(status == 3)
    {
        Serial.println("Temperature is high");
        Serial.print("Highest acceptable temperature is: ");
        if(isSummer(Month))
            Serial.println(Temperature_summer[Age_week][3]);
        else
            Serial.println(Temperature_winter[Age_week][3]);
        Serial.print("Current temperature: ");
        Serial.println(Current_Temperature);
    }

    if(status == 0)
    {

        Serial.println("Temperature is at optimal");
        Serial.print("Current temperature is: ");
        Serial.println(Current_Temperature);
    }
    else
    {
        Serial.println("Temperature is not at a risky level");
        Serial.print("Current temperature is: ");
        Serial.println(Current_Temperature);
    }
}

void Farm::show_Humidity_status()
{
    int status = this->check_Humidity(Age_week, Current_Humidity);

    if(status == 2)
    {
        Serial.println("Humidity is low");
        Serial.print("Lowest acceptable humidity is: ");
        Serial.println(Humidity[Age_week][0]);
        Serial.print("Current humidity: ");
        Serial.println(Current_Humidity);
    }

    if(status == 3)
    {
        Serial.println("Humidity is high");
        Serial.print("Highest acceptable humidity is: ");
        Serial.println(Humidity[Age_week][3]);
        Serial.print("Current humidity: ");
        Serial.println(Current_Humidity);
    }

    if(status == 0)
    {
        Serial.println("Humidity is at optimal");
        Serial.print("Current humidity is: ");
        Serial.println(Current_Humidity);
    }
    else
    {
        Serial.println("Humidity is not at a risky level");
        Serial.print("Current humidity is: ");
        Serial.println(Current_Humidity);
    }
}

void Farm::show_Ammonia_status()
{
    int status = this->check_Ammonia(Age_week, Current_Ammonia);

    if(status == 0)
    {
        Serial.println("Ammonia is at best level");
        Serial.print("Current ammonia level is: ");
        Serial.println(Current_Ammonia);
    }

    if(status == 1)
    {
        Serial.println("Ammonia is at acceptable level");
        Serial.print("Current ammonia level is: ");
        Serial.println(Current_Ammonia);
    }

    if(status == 2)
    {
        Serial.println("Ammonia is at risky level");
        Serial.print("Current ammonia level is: ");
        Serial.println(Current_Ammonia);
    }

    if(status == 3)
    {
        Serial.println("Ammonia is at critical level");
        Serial.print("Current ammonia level is: ");
        Serial.println(Current_Ammonia);
    }

    if(status == 4)
    {
        Serial.println("Ammonia is at extreme level");
        Serial.print("Current ammonia level is: ");
        Serial.println(Current_Ammonia);
    }
}


String Farm::Temperature_status()
{
    int status = this->check_Temperature(Month, Age_week, Current_Temperature);

    if(status == 2)
    return "Low";

    if(status == 3)
    return "High";

    if(status == 0)
    return "Optimal";

    return "Acceptable";
    
}

String Farm::Humidity_status()
{
    int status = this->check_Humidity(Age_week, Current_Humidity);

    if(status == 2)
    return "Low";

    if(status == 3)
    return "High";

    if(status == 0)
    return "Optimal";

    return "Acceptable";
}

String Farm::Ammonia_status()
{
    int status = this->check_Ammonia(Age_week, Current_Ammonia);

    if(status == 0)
    return "Optimal";

    if(status == 1)
    return "Acceptable";

    if(status == 2)
    return "Risky";

    if(status == 3)
    return "Critical";

    return "Extreme";
}


String Farm::getErrorMessageInParameters(String parameter)
{
    parameter.toLowerCase();
    if(parameter == "temperature")
    {
        return this->Temperature_status();
    }
    else if(parameter == "humidity")
    {
        return this->Humidity_status();
    }
    else if(parameter == "ammonia")
    {
        return this->Ammonia_status();
    }
    else
    {
        Serial.println("Invalid input");
        return "";
    }
    

}