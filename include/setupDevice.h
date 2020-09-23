#ifndef setupDevice_H
#define setupDevice_H

#include<Arduino.h>
#include<EEPROM.h>
#define max_number_of_contacts 5
#define eeprom_size 128



class  setupDevice
{
    private:
        String device_id; // nodeMCU mac address
        String wifi_ssid; 
        String wifi_password;

        //byte farm_type = 0; //0 = incubator, 1 = parent_house. Could be more types
        byte Reset; //first time 1, rest of the time no assignment

        int number_of_contacts; //how many contacts to save
        String contacts[max_number_of_contacts];//string array to save the phone numbers
        int contact_count = 0; 

        int current_address = 0; //address to save byte data
        int reset_address = current_address;
        int ssid_length_address = current_address + 1;
        int password_length_address = current_address + 2;
        int number_of_contact_address = current_address + 3;
        int ssid_start_address = current_address + 4;
        int password_start_address = current_address + 20;
        int date_start_address = current_address + 36;
        int phone_number_start_address = current_address + 46; //44 and 45 is reserved for Farm_type and Initial age in Farm class

        int second, minute, hour, dayOfWeek, dayOfMonth, month, year; // initial setup date

    public:
        setupDevice();
        setupDevice(String id, String ssid, String password, String number);
        setupDevice(String id, String ssid, String password, String number[]);

        bool checkID(String id);
        void getID(String id);
        String ID();
        void showID();

        void getSSID(String ssid);
        String SSID();
        void showSSID();

        void getPassword(String password);
        String Password();
        void showPassword();

        void getNumberOfContacts(int num);
        int NumberOfContacts();
        void showNumberOfContacts();

        void getContact(String phone_number);
        String lastContact();
        String Contact(int n);
        void showContacts();

        void reset(bool state);
        bool resetState();

        bool setStartingDate(int second, int minute, int hour, int dayOfWeek, int dayOfMonth, int month, int year);
        void showStartingDate();
        int Second();
        int Minute();
        int Hour();
        int DayOfWeek();
        int DayOfMonth();
        int Month();
        int Year();

        int writeStringInEEPROM(int init_addr, int size, String data);
        int writeByteInEEPROM(int init_addr, byte data);
        byte readByteInEEPRPOM(int address);
        String readStringInEEPRPOM(int init_addr, int size);

        //~ setupDevice();
};


#endif