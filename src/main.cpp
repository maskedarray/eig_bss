/*
 * Copyright 2020, Energy Informatics Group, LUMS University. All rights reserved.
 * Developed by Abdur Rahman(rao.arrn@gmail.com), Wajahat Ali(s_ali@lums.edu.pk), Farasat(razi97@hotmail.com).
 * 
 * Main file for Battery Swapping Station code
 * 
 * This file is the property of Energy Informaics Group, Lums University. Its sole private
 * ownership remains with LUMS and it should not be shared with anyone without the consent 
 * of respective authority.
 */

//TODO: handle the problem when semaphore is not available within the defined time. and also define the blocking time
//TODO: OPTIMIZATION: convert String to c string.
//TODO: optimization of data that is to be saved and sent
#define DATA_ACQUISITION_TIME 1000      //perform action every 1000ms
#define DATA_MAX_LEN 1200   //bytes
#define LED_1   2

#include <Arduino.h>
#include <FreeRTOS.h>
#include <can.h>
#include <bluetooth.h>

String towrite, toread;
TaskHandle_t dataTask, blTask;
void vAcquireData( void *pvParameters );
void vBlTransfer( void *pvParameters );

SemaphoreHandle_t semaAqData1, seamAqData2, semaBlTx1, semaStorage1, seamStorage2, semaWifi1;
void addSlotsData(String B_Slot,String B_ID,String B_Auth, String B_Age,String B_Type ,String B_M_Cycles ,String B_U_Cycles , 
                    String B_Temp, String B_SoC, String B_SoH, String B_RoC,String B_Vol ,String B_Curr){
    towrite += B_Slot + "," + B_ID + "," + B_Auth + "," + B_Age + "," + B_Type + "," + B_M_Cycles + "," + 
            B_U_Cycles + "," + B_Temp + "," + B_SoC + "," + B_SoH + "," + B_RoC + "," + B_Vol + "," + B_Curr;
    return;
}


void setup() {
    Serial.begin(115200); //Start Serial monitor
    pinMode(LED_1, OUTPUT);
    bt.init();
    semaAqData1 = xSemaphoreCreateBinary();
    seamAqData2 = xSemaphoreCreateBinary();
    semaBlTx1 = xSemaphoreCreateBinary();
    xSemaphoreGive(semaAqData1);
    
    xTaskCreatePinnedToCore(vAcquireData, "Data Acquisition", 10000, NULL, 2, &dataTask, 1);
    xTaskCreatePinnedToCore(vBlTransfer, "Bluetooth Transfer", 10000, NULL, 1, &blTask, 0);
    log_d("created all tasks");
}
unsigned long lastMillis = 0;
String CloudData = "";
void loop() {
    
}
void vAcquireData( void *pvParameters ){
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = DATA_ACQUISITION_TIME;
    xLastWakeTime = xTaskGetTickCount();

    for(;;){    //infinite loop
        xSemaphoreTake(semaAqData1, portMAX_DELAY); //semaphore to check if sending of data over bluetooth and storage has returned
        {
            //Dummy acquisition of data
            //we need to place a valid CSV string in towrite string
            towrite = "";
            //towrite += getTime() + ",";                 //time
            towrite += String("BSS1715001") + ",";      //BSSID
            towrite += String("16") + ",";              //total slots
            towrite += String("20.273") + ",";              //BSS voltage
            towrite += String("55.781") + ",";              //BSS CURRENT
            towrite += String("7400") + ",";                  //BSS POWER
            towrite += String("0.8") + ",";                 //BSS power factor
            //addSlotsData(String B_Slot,String B_ID,String B_Auth, String B_Age,String B_Type ,String B_M_Cycles ,
            //             String B_U_Cycles , String B_Temp, String B_SoC, String B_SoH, String B_RoD,String B_Vol ,String B_Curr)
            addSlotsData("01", "1718953129", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("02", "1718953130", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("03", "1718953131", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("04", "1718953128", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "26.561");towrite += ",";
            addSlotsData("05", "1718953127", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("06", "1718953126", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("07", "1718953125", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("08", "1718953124", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "26.561");towrite += ",";
            addSlotsData("09", "1718953123", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("10", "1718953122", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("11", "1718953121", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("12", "1718953120", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "26.561");towrite += ",";
            addSlotsData("13", "1718953119", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("14", "2718953129", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("15", "1518953129", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "20.561");towrite += ",";
            addSlotsData("16", "1718253129", "BSS22", "22", "2211", "500", "200", "30", "80", "50", "22", "12.371", "26.561");
            //Now towrite string contains one valid string of CSV data chunk
        }
        xSemaphoreGive(semaBlTx1);      //signal to call bluetooth transfer function once
        vTaskDelayUntil(&xLastWakeTime, xPeriod);    //defines the data acquisition rate
    }   //end for
}   //end vAcquireData

void vBlTransfer( void *pvParameters ){ //synced by the acquire data function
    for(;;){    //infinite loop
        xSemaphoreTake(semaBlTx1,portMAX_DELAY);
        {
            bt.send(towrite);
        }
        xSemaphoreGive(semaAqData1);
    }   //end for
}   //end vBlTransfer task
