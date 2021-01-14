#include"setupDevice.h"
#include<Arduino.h>
#include<EEPROM.h>
#define max_number_of_contacts 5
#define eeprom_size 128

setupDevice::setupDevice()
{
    device_id = "BCDDC29AC77B";
    if(this->readByteInEEPRPOM(reset_address) == 0)
    {
        wifi_ssid = this->readStringInEEPRPOM(ssid_start_address, this->readByteInEEPRPOM(ssid_length_address));
        wifi_password = this->readStringInEEPRPOM(password_start_address, this->readByteInEEPRPOM(password_length_address));
        number_of_contacts = this->readByteInEEPRPOM(number_of_contact_address);
        for(int i = 0; i<number_of_contacts; i++)
        {
            contacts[i] = this->readStringInEEPRPOM(phone_number_start_address+11*i, 11);
        }
    }
    else
    {
        for(int i = 1; i<eeprom_size; i++)
        {
            this->writeByteInEEPROM(i, 0);
        }
    }
    
}

setupDevice::setupDevice(String id, String ssid, String password, String number)
{
    device_id = id;
    wifi_ssid = ssid;
    wifi_password = password;
    number_of_contacts = 1;
    contacts[0] = number;
    int ssid_length = ssid.length();
    int pass_length = password.length();
    this->writeByteInEEPROM(reset_address, 0);
    this->writeByteInEEPROM(ssid_length_address, ssid_length);
    this->writeByteInEEPROM(password_length_address, pass_length);
    this->writeByteInEEPROM(number_of_contact_address, number_of_contacts);
    current_address = writeStringInEEPROM(ssid_start_address, ssid_length, ssid);
    if(current_address == -1)
    {
        Serial.println("Error saving ssid");
    }
    current_address = writeStringInEEPROM(password_start_address, pass_length, password);
    if(current_address == -1)
    {
        Serial.println("Error saving password");
    }
    current_address = writeStringInEEPROM(phone_number_start_address, number_of_contacts*11, contacts[0]);
    if(current_address == -1)
    {
        Serial.println("Error saving contacts");
    }
    contact_count++;
}

setupDevice::setupDevice(String id, String ssid, String password, String number[])
{
    device_id = id;
    wifi_ssid = ssid;
    wifi_password = password;
    number_of_contacts = sizeof(number)/sizeof(number[0]);
    for(int i = 0; i<number_of_contacts; i++)
    {
        contacts[i] = number[i];
    }
    int ssid_length = ssid.length();
    int pass_length = password.length();
    this->writeByteInEEPROM(reset_address, 0);
    this->writeByteInEEPROM(ssid_length_address, ssid_length);
    this->writeByteInEEPROM(password_length_address, pass_length);
    this->writeByteInEEPROM(number_of_contact_address, number_of_contacts);
    current_address = writeStringInEEPROM(ssid_start_address, ssid_length, ssid);
    if(current_address == -1)
    {
        Serial.println("Error saving ssid");
    }
    current_address = writeStringInEEPROM(password_start_address, pass_length, password);
    if(current_address == -1)
    {
        Serial.println("Error saving password");
    }
    for(int i = 0; i<number_of_contacts; i++)
    {
        current_address = writeStringInEEPROM(phone_number_start_address + 11*i, 11, contacts[i]);
        if(current_address == -1)
        {
            Serial.println("Error saving contacts");
        }
    }
    
}

int setupDevice::writeStringInEEPROM(int init_addr, int size, String data)
{
    EEPROM.begin(eeprom_size);
    int i, j=0;
    //Serial.println("size"+String(size));
    for(i = init_addr; i<(size + init_addr); i++,j++)
    {
        EEPROM.write(i, data[j]);
        //Serial.println(data[j]);
        if(EEPROM.commit())
        {
            Serial.println("EEPROM successfully committed");
        }
        else 
        {
            Serial.println("ERROR! EEPROM commit failed");
            return -1;
        }
    }
    return i; //new address
}

int setupDevice::writeByteInEEPROM(int init_addr, byte data)
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

byte setupDevice::readByteInEEPRPOM(int address)
{
    EEPROM.begin(eeprom_size);
    byte data;
    data = EEPROM.read(address);
    //Serial.println("Data: "+String(data));
    return data;
}

String setupDevice::readStringInEEPRPOM(int init_addr, int size)
{
    EEPROM.begin(eeprom_size);
    String str;
    byte data;
    for(int i=init_addr; i<(size + init_addr); i++)
    {
        data = EEPROM.read(i);
        //Serial.println((char)data);
        str+=(char)data;
    }
    //Serial.println("String: " + str);
    return str;
}

bool setupDevice::checkID(String id)
{
    return id.compareTo(device_id)==0?true:false;
}

void setupDevice::getID(String id)
{
    device_id = id;
}

String setupDevice::ID()
{
    return device_id;
}

void setupDevice::showID()
{
    Serial.print("Device ID: ");
    Serial.println(device_id);
}


void setupDevice::getSSID(String ssid)
{
    wifi_ssid = ssid;
    this->writeByteInEEPROM(ssid_length_address, ssid.length());
    this->writeStringInEEPROM(ssid_start_address, ssid.length(), ssid);
}

String setupDevice::SSID()
{
    return wifi_ssid;
}

void setupDevice::showSSID()
{
    int ssid_length = this->readByteInEEPRPOM(ssid_length_address);
    String ssid = this->readStringInEEPRPOM(ssid_start_address, ssid_length);
    Serial.print("SSID: ");
    Serial.println(ssid);
}

void setupDevice::getPassword(String password)
{
    wifi_password = password;
    this->writeByteInEEPROM(password_length_address, password.length());
    this->writeStringInEEPROM(password_start_address, password.length(), password);
}

String setupDevice::Password()
{
    return wifi_password;
}

void setupDevice::showPassword()
{
    int pass_length = this->readByteInEEPRPOM(password_length_address);
    String password = this->readStringInEEPRPOM(password_start_address, pass_length);
    Serial.print("Password: ");
    Serial.println(password);
}

void setupDevice::getNumberOfContacts(int num)
{
    number_of_contacts = num;
    this->writeByteInEEPROM(number_of_contact_address, num);
}

int setupDevice::NumberOfContacts()
{
    return number_of_contacts;
}

void setupDevice::showNumberOfContacts()
{
    Serial.print("Total number of contacts: ");
    Serial.println(number_of_contacts);
}

void setupDevice::getContact(String phone_number)
{
    if(contact_count<max_number_of_contacts)
    {
        contacts[contact_count] = phone_number;
        this->writeStringInEEPROM(phone_number_start_address + 11*contact_count, 11, phone_number);
        contact_count++;
    }
    else
    {
        Serial.println("Reached maximum phone number");
    }
}

String setupDevice::lastContact()
{
    return contacts[contact_count];
}

String setupDevice::Contact(int n)
{
    if(n<max_number_of_contacts)
    return contacts[n];
    return "";
}

void setupDevice::showContacts()
{
    int n = readByteInEEPRPOM(number_of_contact_address);
    String contacts;
    for(int i = 0; i<n; i++)
    {
        contacts = this->readStringInEEPRPOM(phone_number_start_address+11*i, 11);
        Serial.print("Contact number ");
        Serial.print(i+1);
        Serial.print(": ");
        Serial.println(contacts);
    }
}

void setupDevice::reset(bool state)
{
    Reset = state;
    this->writeByteInEEPROM(reset_address, state);
}

bool setupDevice::resetState()
{
    return this->readByteInEEPRPOM(reset_address);
}

bool setupDevice::setStartingDate(int second, int minute, int hour, int dayOfWeek, int dayOfMonth, int month, int year)
{
    this->second = second;
    this->minute = minute;
    this->hour = hour;
    this-> dayOfWeek = dayOfWeek;
    this->dayOfMonth = dayOfMonth;
    this->month = month;
    this->year = year;

    int byte_count = date_start_address;
    this->writeByteInEEPROM(byte_count++, second);
    this->writeByteInEEPROM(byte_count++, minute);
    this->writeByteInEEPROM(byte_count++, hour);
    this->writeByteInEEPROM(byte_count++, dayOfWeek);
    this->writeByteInEEPROM(byte_count++, dayOfMonth);
    this->writeByteInEEPROM(byte_count++, month);
    this->writeByteInEEPROM(byte_count++, year%1000);

    return true;
}

void setupDevice::showStartingDate()
{
    //int byte_counter = date_start_address;
    for(int i = date_start_address; i < date_start_address + 7; i++)
    {
        Serial.println(this->readByteInEEPRPOM(i));
    }
}

int setupDevice::Second()
{
    return this->readByteInEEPRPOM(date_start_address);
}

int setupDevice::Minute()
{
    return this->readByteInEEPRPOM(date_start_address + 1);
}

int setupDevice::Hour()
{
    return this->readByteInEEPRPOM(date_start_address + 2);
}

int setupDevice::DayOfWeek()
{
    return readByteInEEPRPOM(date_start_address + 3);
}

int setupDevice::DayOfMonth()
{
    return readByteInEEPRPOM(date_start_address + 4);
}

int setupDevice::Month()
{
    return readByteInEEPRPOM(date_start_address + 5);
}

int setupDevice::Year()
{
    return readByteInEEPRPOM(date_start_address + 6) + 2000;
}

// setupDevice::~setupDevice()
// {
//     for(int i = 0; i < eeprom_size; i++)
//         this->writeByteInEEPROM(i, 0);
// }