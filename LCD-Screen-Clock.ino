

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 7, 5, 4, 3, 2);
String TIME_STRING = "2021.12.31 - 23:59:53";                         //Given time and date value
int TIME_SCREEN_INFO = 1000;                                          //The variable required to show the information screen for 1 second
int TIME_SCREEN_CLOCK = 2000;                                         //The variable required to show the clock screen for 2 second
int buton_id = 13;                                                    //the port the button is connected to
long time = 0;                                                        //variable where milliseconds are kept instantaneously(for seconds calculation)
long time_main = 0;                                                   //variable where milliseconds are kept instantaneously(to display the clock for 2 seconds each in loop function)
bool flagSecond = true;                                               //Bool variable used to increase the minutes when 60 seconds are up
bool flagMinute = true;                                               //Bool variable used to increase the hours when 60 minutes are up
bool days = false;                                                    //Bool variable that increases one day after 24 hours
bool butonFlag = false;                                               //'buttonFlag' is false unless the button is pressed
bool first, secondt;                                                  //variables that hold the value of the button
bool TR_EN = true;                                                    //The variable that determines whether the time zone is displayed in Turkish or English. Its value changes when the button is pressed
int hour_12;                                                          //Variable that allows the clock to be kept for 12 periods
int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //An array that holds how many days the months contain
String daysTr[7] = {"PZT", "SAL", "CAR", "PER", "CUM", "CMT", "PZR"}; //Turkish naming of the days in 1 week
String daysEn[7] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"}; //English naming of the days in 1 week
int FirstDay2021 = 4;                                                 //The variable that holds the first day of 2021
int FirstDay2020 = 2;                                                 //The variable that holds the first day of 2020
int FirstDay2022 = 5;                                                 //The variable that holds the first day of 2022
String date = TIME_STRING.substring(0, TIME_STRING.indexOf(" "));     //String holding the date portion of the given TIME_STRING
String clck = TIME_STRING.substring(TIME_STRING.indexOf("-") + 2);    //String holding the clock portion of the given TIME_STRING
int hour = clck.substring(0, 2).toInt();                              //getting the hour and converting it to an integer
int minute = clck.substring(3, 5).toInt();                            //getting the minute and converting it to an integer
int second = clck.substring(6, 8).toInt();                            //getting the second and converting it to an integer
int year = date.substring(0, 4).toInt();                              //getting the year and converting it to an integer
int month = date.substring(5, 7).toInt();                             //getting the month and converting it to an integer
int day = date.substring(8, 10).toInt();                              //getting the day and converting it to an integer
void setup()// Setup Function
{
   
    lcd.begin(16, 2); //The range to display texts on the LCD display
    pinMode(buton_id, INPUT); //pinmode for button
    time = millis();//Keeping the milliseconds to start for second calculation
    time_main = millis();//Keeping the milliseconds to start for loop function
    lcd.setCursor(0, 0);
    lcd.print("00:00:00");//Time appears as 00:00:00 when no time is entered
}

//function that allows TIME_STRING to appear on the screen when the circuit is running and the clock to appear on the screen every time the clock cycle runs
void firstClock(int type) //The type variable required for the function provides a different representation of the hour if it is 12-period or 24-period.
{
  //':' sign appears on the screen
    lcd.setCursor(2, 0);
    lcd.print(":");
    lcd.setCursor(5, 0);
    lcd.print(":");

    if (type < 10) //Since the hour is single digit if the clock is less than 10, it allows one unit to start from the right on the LCD display.
    {
        lcd.setCursor(0, 0);
        lcd.print("0");
        lcd.setCursor(1, 0);
        lcd.print(type);
    }

    else //If the hour is greater than 10, it will be double-digit, so one unit on the LCD screen allows it to start from the left.
    {
        lcd.setCursor(0, 0);
        lcd.print(type);
    }
    if (minute < 10)//Since the minute is single digit if the clock is less than 10, it allows one unit to start from the right on the LCD display.
    {
        lcd.setCursor(3, 0);
        lcd.print("0");
        lcd.setCursor(4, 0);
        lcd.print(minute);
    }

    else //If the minute is greater than 10, it will be double-digit, so one unit on the LCD screen allows it to start from the left.
    {
        lcd.setCursor(3, 0);
        lcd.print(minute);
    }

    if (second < 10)//Since the second is single digit if the clock is less than 10, it allows one unit to start from the right on the LCD display.
    {
        lcd.setCursor(6, 0);
        lcd.print("0");
        lcd.setCursor(7, 0);
        lcd.print(second);
    }

    else//If the second is greater than 10, it will be double-digit, so one unit on the LCD screen allows it to start from the left.
    {
        lcd.setCursor(6, 0);
        lcd.print(second);
    }
}
//required function for time display
void clock(int hourType) //The hour Type variable required for the function determines whether the clock is 12 or 24 periods.
{

    if (hourType == 12)//For 12-period clock display, when the clock is greater than 12, it is subtracted from 12 and displayed as p.m. If it is less than 12 o'clock, it is shown as a.m.
    {
        if ((hour - 12) >= 0)
        {
            if ((hour - 12) == 0) //It is exactly 12 noon and is shown as p.m However, since we subtract it from 12, it takes the value '0'. In this case, the hour_12 variable is set to 12.
            {
                hour_12 = 12;
                lcd.setCursor(9, 0);
                lcd.print("PM  ENG");
            }
            else
            {
                hour_12 = hour - 12;

                lcd.setCursor(9, 0);
                lcd.print("PM  ENG");
            }
        }
        else
        {
            if (hour == 0) //Night 12 is expressed as '0' in the 24-hour clock. This is expressed as 12:00 a.m in the 12-hour period
            {
                hour_12 = 12;
            }
            else
            {
                hour_12 = hour;
            }

            lcd.setCursor(9, 0);
            lcd.print("AM  ENG");
        }

        firstClock(hour_12);
    }

    else //If hourType is 24 hours, this else block works.
    {

        lcd.setCursor(9, 0);
        lcd.print("    TUR");
        firstClock(hour);
    }

//The 'if block' where seconds are changed and shown. It increases by one second every 1000 milliseconds.
    if ((millis() - time) >= 1000)
    {
        if (second < 10)//Since the second is single digit if the clock is less than 10, it allows one unit to start from the right on the LCD display.
        {
            lcd.setCursor(7, 0);
        }

        else//If the second is greater than 10, it will be double-digit, so one unit on the LCD screen allows it to start from the left.
        {
            lcd.setCursor(6, 0);
        }
        second = second + 1; //increases by one second every 1000 milliseconds.
        //It resets seconds every 60 seconds and flagSecond becomes false for one minute increment
        if (second >= 60)
        {
            flagSecond = false;
            second = 0;

            lcd.setCursor(7, 0);
            lcd.print(second);
            lcd.setCursor(6, 0);
            lcd.print(second);
        }
        lcd.print(second);//Display every second on LCD screen

        if (flagSecond == false) //If falgSecond is false, the 'if block' will run and increment by 1 minute.
        {

            if (minute < 10)//Since the minute is single digit if the clock is less than 10, it allows one unit to start from the right on the LCD display.
            {
                lcd.setCursor(4, 0);
            }

            else//If the minute is greater than 10, it will be double-digit, so one unit on the LCD screen allows it to start from the left.
            {
                lcd.setCursor(3, 0);
            }
            minute = minute + 1;//increases by one minute every 60 second.
            //It resets seconds every 60 minutes and flagMinute becomes false for one hour increment
            if (minute >= 60)
            {
                flagMinute = false;
                minute = 0;
                lcd.setCursor(4, 0);
                lcd.print(minute);
                lcd.setCursor(3, 0);
                lcd.print(minute);
            }
            lcd.print(minute);//Display every minute on LCD screen
            flagSecond = true; //flagSecond is true until the next 60 seconds as it will increase by one minute every 60 seconds.
        }

        if (flagMinute == false)//If falgMinute is false, the 'if block' will run and increment by 1 hour.
        {

            if (hour < 10)//Since the hour is single digit if the clock is less than 10, it allows one unit to start from the right on the LCD display.
            {
                lcd.setCursor(1, 0);
            }

            else//If the hour is greater than 10, it will be double-digit, so one unit on the LCD screen allows it to start from the left.
            {
                lcd.setCursor(0, 0);
            }

            hour = hour + 1;//increases by one hour every 60 minute.(For the 24 hour period )
            hour_12 = hour_12 + 1;//increases by one hour every 60 minute.(For the 12 hour period )
            if (hour == 24) //The hour is '0' every 24 hours and the days variable is true for one increment of the day.
                            //The hour_12 variable in the 12-hour display is also common to the two time zones because it is dependent on the hour variable.
            {
                days = true;
                hour = 0;

                if (hourType == 24)//allows the time to be displayed as 00:00:00
                {
                    lcd.setCursor(1, 0);
                    lcd.print(hour);
                    lcd.setCursor(0, 0);
                    lcd.print(hour);
                }
            }

            if (hourType == 12)//the clock display that will be shown on the screen if the clock is desired to be displayed in 12 periods.
            {

                lcd.print(hour_12);
                flagMinute = true;
            }
            else //the clock display that will be shown on the screen if the clock is desired to be displayed in 24 periods.
            {
                lcd.print(hour);
                flagMinute = true;
            }
        }
        time = millis();//Keeping the milliseconds for second calculation
    }
}
//The function by which the date is calculated
void Date(bool flagDay)//If the flagDay variable is true (after 24 hours) it increases by 1 day.
{
    if (flagDay == true)
    {
        if (year == 2020 && month == 2)//Since February is 29 days in 2020, a separate check is required.
        {

            if (day >= monthDays[month - 1] + 1)//When February becomes 29 days in 2020, the month variable increases by 1 and the day variable becomes 1.
            {

                day = 1;
                month = month + 1;
            }

            else//If 29 days have not passed, the day variable will increase by 1
            {

                day = day + 1;
            }
        }

        else if (day >= monthDays[month - 1])//Except for February in 2020, the months of the year 2020 and 2021 have an equal number of days. Whether the 1 month is over or not is checked by the monthDays array.
        {

            day = 1;
            month = month + 1;
        }
        else
        {
            day = day + 1;
        }

        days = false; //the days variable will be false until another 24 hours have passed
    }

    if (month >= 13)//When the month variable exceeds 12, the year variable increases by 1. and the month variable is set to 1
    {
        month = 1;
        year = year + 1;
    }
    //In date display, between month, year, day '.' placed
    lcd.setCursor(4, 1);
    lcd.print(".");
    lcd.setCursor(7, 1);
    lcd.print(".");

    lcd.setCursor(0, 1);
    lcd.print(year);//The year is displayed at the (0, 1) position on the LCD.
    if (month < 10)//Since the month is single digit if the clock is less than 10, it allows one unit to start from the right on the LCD display.
    {
        lcd.setCursor(5, 1);
        lcd.print("0");
        lcd.setCursor(6, 1);
        lcd.print(month);
    }

    else//If the month is greater than 10, it will be double-digit, so one unit on the LCD screen allows it to start from the left.
    {
        lcd.setCursor(5, 1);
        lcd.print(month);
    }

    if (day < 10)//Since the day is single digit if the clock is less than 10, it allows one unit to start from the right on the LCD display.
    {
        lcd.setCursor(8, 1);
        lcd.print("0");
        lcd.setCursor(9, 1);
        lcd.print(day);
    }

    else//If the day is greater than 10, it will be double-digit, so one unit on the LCD screen allows it to start from the left.
    {
        lcd.setCursor(8, 1);
        lcd.print(day);
    }
}

//function that allows display of the given date and the day names of the changed date on the screen
void DaysName(String tr_en) //tr_en variable determines whether day names will be in Turkish or English.
{
  //The days from the beginning of the year to that date are summed to find out which day the entered date falls on. 
  //The mode of the total number of days to seven (1 week)
  //According to the mode result, the first day of the year (FirstDay2021 or FirstDay2020) is added to it and the day of the entered date is found.
    int total = 0;
    for (int i = 0; i < (month - 1); i++)
    {
        total = total + monthDays[i];
    };
    total = total + (day - 1);
    Serial.println(total);
    total = total % 7;
    int present_day;
    if (year == 2021)
    {
        present_day = FirstDay2021 + total;
    }
    else if (year == 2020)
    {
        present_day = FirstDay2020 + total + 1;
    }
    else if (year == 2022)
    {
        present_day = FirstDay2022 + total ;
    }

    if (present_day >= 7)
    {
        present_day = present_day - 7;
    }
    else
    {
        present_day = present_day;
    }

    Serial.println(present_day);
    lcd.setCursor(13, 1);
    //Writing in Turkish
    if (tr_en == "tr")
    {
        lcd.print(daysTr[present_day]);
    }
    //Writing in English
    else
    {
        lcd.print(daysEn[present_day]);
    }
}

void info(int hourType)//Displaying the information screen, displaying in Turkish or English. (depending on the hourType variable)
{
    if (hourType == 12)
    {
        lcd.setCursor(0, 0);
        lcd.print("HH:MM:SS 12H LNG");
        lcd.setCursor(0, 1);
        lcd.print("DD:MM:YYYY   DOW");
    }
    else
    {
        lcd.setCursor(0, 0);
        lcd.print("SA:DA:SB 24H DIL");
        lcd.setCursor(0, 1);
        lcd.print("GG:AA:YYYY   GUN");
    }
}

void btn()
{                                  //function that checks whether the button is pressed or not
    first = digitalRead(buton_id); //the value of the button is read
    butonFlag = false;             //'buttonFlag' is false unless the button is pressed
    if (secondt != first)          //The equality of the value read from the button with the previous value is checked.
    {
        if (first == HIGH) //If the value read from the button is high, the 'butonFlag' variable will be 'true'
        {
            butonFlag = true;
        }
    }
    secondt = first; //In order to compare it with the previous value, the value of the button is saved in the second variable.
}

void loop()
{

    while (((millis() - time_main) < TIME_SCREEN_CLOCK) && TR_EN == true) //Turkish display of the clock for 2 seconds. Since the TR_EN variable will change when the button is pressed, it displays in English.
    {
        clock(24);      //24-hour period display
        DaysName("tr"); //Turkish display of the days
        Date(days);     //Date display

        lcd.setCursor(8, 0);
        lcd.print(" ");

        btn(); //If the button is pressed, it will be displayed in English.
        if (first == HIGH && butonFlag)
        {
            TR_EN = false;
            break;
        }
    }

    while (((millis() - time_main) < TIME_SCREEN_CLOCK) && TR_EN == false) //English display of the clock for 2 seconds. Since the TR_EN variable will change when the button is pressed, it displays in Turkish.
    {
        clock(12);      //12-hour period display
        DaysName("en"); //English display of the days
        Date(days);     //Date display

        lcd.setCursor(8, 0);
        lcd.print(" ");

        btn(); //If the button is pressed, it will be displayed in Turkish.
        if (first == HIGH && butonFlag)
        {
            TR_EN = true;
            break;
        }
    }
    time_main = millis();
    while (((millis() - time_main) < TIME_SCREEN_INFO))//Information screen showing for 1 second
        {
            if (TR_EN == true) //Displaying the Information screen in Turkish
            {
                info(24);
                btn(); //If the button is pressed, it will be displayed in English.
                if (first == HIGH && butonFlag)
                {
                    TR_EN = false;
                    break;
                }
            }
            else //Displaying the Information screen in English
            {
                info(12);
                btn(); //If the button is pressed, it will be displayed in Turkish.
                if (first == HIGH && butonFlag)
                {
                    TR_EN = true;
                    break;
                }
            }
        }

    
    second = second + (TIME_SCREEN_INFO/1000);//After waiting 1 second on the information screen, the seconds are increased so that the time can continue to progress.
    time_main = millis();
    lcd.setCursor(0, 0);
}

