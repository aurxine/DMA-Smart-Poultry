#ifndef Farm_H
#define Farm_H

#include<Arduino.h>

#define max_Age_week 72

class Farm
{
    private:
        int Age_week = 0;
        int Initial_age = 0;
        int Month = 1;

        float Current_Temperature;
        float Current_Humidity;
        float Current_Ammonia;

        float Temperature_tolerance = 0.5;
        float Humidity_tolerance = 2;
        float Ammonia_tolerance = 1; // did not use

        float Temperature_summer[max_Age_week][4];
        float Temperature_winter[max_Age_week][4];
        float Humidity[max_Age_week][4];
        float Ammonia[max_Age_week][4];


        /*
        [Age][0] = Lowest
        [Age][1] = Optimum Lowest
        [Age][2] = Optimum Highest
        [Age][3] = Highest
        */

       int Farm_type;
       /*
       0 = Brooder (0-3 weeks)
       3 = Grower (3-18 weeks)
       18 = Parents (18-72 weeks)
       */
      int Brooder_period_start = 0;
      int Brooder_period_end = 3;
      int Grower_period_start = 3;
      int Grower_period_end = 18;
      int Parents_period_start = 18;
      int Parents_period_end = 72;
      

    public:

        Farm();

        bool isSummer(int month);
        /* returns:
        0 = winter
        1 = summer
        */

        void getFarmType(String type);
        void getInitAge(String age);

        void set_Temperature(); // sets initial temperature data provided by Manufarm
        void show_Temperature(bool summer); // shows temperature data

        void set_Humidity(); // sets initial humidity data provided by Manufarm
        void show_Humidity(); // shows humidity data

        void set_Ammonia(); // sets initial ammonia data provided by Manufarm
        void show_Ammonia(); // shows ammonia data

        /*
        These functions only check if there is any risks or not.
        Input:
        month = month number, recquired for checking summer or winter
        Age_week  = Bird age in weeks

        Returns (temperature and humidity):
        0  = The parameter is within optimal value
        1  = The parameter is within acceptable range
        2  = The parameter has exceeded minimum acceptable value
        3  = The parameter has exceeded maximum acceptable value

        Returns (ammonia):
        0 = Best level
        1 = Acceptable level
        2 = Risky level
        3 = Critical level
        4 = Extreme level
        */

        int check_Temperature(int month, int Age_week, float Temp);
        int check_Humidity(int Age_week, float Hum);
        int check_Ammonia(int Age_week, float NH3);
        
        /*
        These functions check if the parameters are optimal (returns true) or not (returns false)
       */
        bool isOptimal_Temperature(int month, int Age_week, float Temp);
        bool isOptimal_Humidity(int Age_week, float Hum);
        bool isOptimal_Ammonia(int Age_week, float NH3);
        
 
        String getErrorMessageInParameters(String parameter);
        /*
        input: temperature/humidity/ammonia
        output: String returns from <parameter>_status()
        */

        /*
        These functions show the status of the parameters.
        Should be used after calling check_<Parameters> or isOptimal_<Parameters>
        */
        void show_Temperature_status();
        void show_Humidity_status();
        void show_Ammonia_status();

        /*
        These functions return the status of the parameters as Strings.
        Returns (Temperature, Humidity):
        Optimal     = The parameter is within optimal value
        Acceptable  = The parameter is within acceptable range
        Low         = The parameter has exceeded minimum acceptable value
        High        = The parameter has exceeded maximum acceptable value 

        Returns (Ammonia):
        Optimal    = Best level
        Acceptable = Acceptable level
        Risky      = Risky level
        Critical   = Critical level
        Extreme    = Extreme level
        */
        String Temperature_status();
        String Humidity_status();
        String Ammonia_status();
      


};

#endif