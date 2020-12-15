#include <rtc.h>
#include <RTClib.h>

RTC_DS3231 rtc;

/*
 * Function initRTC initializes RTC and adjusts date and time in case of power loss
 * TODO: Handle error in case RTC initialization fails
 */
void initRTC(){
    if(rtc.begin()){
        if ( rtc.lostPower()) {
            Serial.println("initRTC -> rtc.cpp -> Readjusting RTC date and time");
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // sets the clock to time when code was burned 
        }
        Serial.println("initRTC -> rtc.cpp -> RTC initialization successful");
    }
    else{
        Serial.println("initRTC -> rtc.cpp -> RTC initialization failed");
    }
}


/*
 * Function getTime returns time from RTC hardware in form of string. 
 * The format is : "YYYY-MM-DD HH:MM:SS"
 * TODO: Optimize the function by replacing individual strings in the return statement.
 *       This will reduce the temporary ram usage
 */
String getTime(){
    DateTime now = rtc.now();
    String YYYY = String(now.year(), DEC);
    String mm = String(now.month(), DEC);
    if (mm.length() == 1){mm = "0" + mm; }
    String dd = String(now.day(), DEC);
    if (dd.length() == 1){dd = "0" + dd; }
    String HH = String(now.hour(), DEC);
    if (HH.length() == 1){HH = "0" + HH; }
    String MM = String(now.minute(), DEC);
    if (MM.length() == 1){MM = "0" + MM; }
    String SS = String(now.second(), DEC);
    if (SS.length() == 1){SS = "0" + SS; }
    return  (YYYY + "-" + mm + "-" + dd + " " + HH + ":" + MM + ":" + SS) ; 
  }
