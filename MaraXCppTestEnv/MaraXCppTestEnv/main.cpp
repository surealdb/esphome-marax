//
//  main.cpp
//  MaraXCppTestEnv
//
//  Created by Matthias Neubert on 23.03.23.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>


int pumpActive = 0;
uint32_t state_previousTimestamp = 0;
uint32_t state_timeElapsedSincePumpStarted = 0;

uint32_t getCurrentTimestamp() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


void readAndPublish(std::string str) {
    
    printf("READING:\n%s\n", str.c_str());
    
    bool debugMode = true;
    
    char senType[1];
    char senVersion[4];
    int senSteamActual=0;
    int senSteamTarget=0;
    char senSteamTargetStr[3];
    int senHeatActual=0;
    int senCounter=0;
    int senHeating=0;
    int senPumpActive=0;
    
    uint32_t previousTimestamp=0;
    uint32_t currentTimestamp=0;
    uint32_t timeElapsed=0;
    int lastPumpActiveState=0;
    
    char buff[10];
    std::string buffAsStdStr;
    
    //std::string str(bytes.begin(), bytes.end());
    
    int elementCount = sscanf(str.c_str(), "%1c%4c,%d,%d,%d,%d,%d,%d", senType, senVersion, &senSteamActual, &senSteamTarget, &senHeatActual, &senCounter, &senHeating, &senPumpActive);
    
    if(debugMode) printf("Element Count: %d \n", elementCount);
    
    if(elementCount == 3) {
        if(debugMode) {
            printf("Wasserbehaelter entfernt\n");
            //ESP_LOGD("MaraX", "Wasserbehaelter entfernt\n");
        }
        
        elementCount = sscanf(str.c_str(), "%1c%4c,%d,%3c,%d,%d,%d,%d", senType, senVersion, &senSteamActual, senSteamTargetStr, &senHeatActual, &senCounter, &senHeating, &senPumpActive);
    }
    
    if (elementCount >= 7 ) {
        
        snprintf(buff, sizeof(buff), "%.1s", senType);
        buffAsStdStr = buff;
        if(debugMode) {
            printf("Ops Mode: %s \n", buffAsStdStr.c_str());
            //ESP_LOGD("MaraX", "Ops Mode: %s \n", buffAsStdStr.c_str());
        }
        //id(opsType).publish_state(buffAsStdStr.c_str());
        
        snprintf(buff, sizeof(buff), "%.4s", senVersion);
        buffAsStdStr = buff;
        if(debugMode) {
            printf("MaraX Firmware: %s \n", buffAsStdStr.c_str());
            //ESP_LOGD("MaraX", "MaraX Firmware: %s \n", buffAsStdStr.c_str());
        }
        //id(coffeeVersion).publish_state(buffAsStdStr.c_str());
        
        if(debugMode) {
            printf("Steam Actual: %d \n", senSteamActual);
            ////ESP_LOGD("MaraX", "Steam Actual: %d \n", senSteamActual);
        }
        //id(steam_actual).publish_state(senSteamActual);
        
        if(debugMode) {
            printf("Steam Target: %d \n", senSteamTarget);
            //ESP_LOGD("MaraX", "Steam Target: %d \n", senSteamTarget);
        }
        //id(steam_target).publish_state(senSteamTarget);
        
        if(debugMode) {
            printf("Heat Actual: %d \n", senHeatActual);
            //ESP_LOGD("MaraX", "Heat Actual: %d \n", senHeatActual);
        }
        //id(heat_actual).publish_state(senHeatActual);
        
        if(debugMode) {
            printf("Time Counter: %d \n", senCounter);
            //ESP_LOGD("MaraX", "Time Counter: %d \n", senCounter);
        }
        //id(time_counter).publish_state(senCounter);
        
        if(debugMode) {
            printf("Is Heating: %d \n", senHeating);
            //ESP_LOGD("MaraX", "Is Heating: %d \n", senHeating);
        }
        //id(heating).publish_state(senHeating);
        
        if(debugMode) {
            printf("Pumpe an: %d \n", senPumpActive);
            //ESP_LOGD("MaraX", "Pumpe an: %d \n", senPumpActive);
        }
        
        //currentTimestamp = millis(); // TODO
        currentTimestamp = getCurrentTimestamp(); // TODO remove
        
        //previousTimestamp = round(id(state_previousTimestamp).raw_state); // TODO
        previousTimestamp = state_previousTimestamp; // TODO remove
        // init previous timestamp if needed
        if (previousTimestamp == 0) {
            previousTimestamp = currentTimestamp;
        }
        
        if(senPumpActive == 1 && currentTimestamp != 0) {
            
            //lastPumpActiveState = round(id(pumpActive).raw_state); // TODO
            lastPumpActiveState = pumpActive; // TODO remove
            
            //timeElapsed = round(id(state_timeElapsedSincePumpStarted).raw_state); // TODO
            timeElapsed = state_timeElapsedSincePumpStarted; // TODO remove
            // if pump just started
            if (lastPumpActiveState == 0) {
                // reset time elapsed counter
                timeElapsed = 0;
            }
            timeElapsed = timeElapsed + (currentTimestamp - previousTimestamp);
            
            //id(state_timeElapsedSincePumpStarted).publish_state(timeElapsed); // TODO
            state_timeElapsedSincePumpStarted = timeElapsed; // TODO remove
        }
        
        //id(state_previousTimestamp).publish_state(currentTimestamp); // TODO
        state_previousTimestamp = currentTimestamp; // TODO remove
        
        //id(pumpActive).publish_state(senPumpActive);
        pumpActive = senPumpActive; // TODO remove
    }
    
}



// esphome-mara
//C123b,095,112,063,1095,1\r\n

// other
//C1.06,116,124,093,0840,1,0\n

int main(int argc, const char * argv[]) {
    
    std::string str = "C1.19,116,124,095,0560,0";
    readAndPublish(str);
    str = "C1.19,116,124,095,0560,0\r\n";
    readAndPublish(str);
    str = "C123b,095,112,063,1095,1\r\n";
    readAndPublish(str);
    str = "C1.06,116,124,093,0840,1,0\n";
    readAndPublish(str);
    //meine: ohne tank drin, modus Schalter auf I, Temp Schalter 0
    str = "C1.09,031,\x1F+/,029,0000,0,0\r\n";
    readAndPublish(str);
    //meine: ohne tank drin, modus Schalter auf 0, Temp Schalter 0
    str = "+1.09,031,\x1F+/,028,0000,0,0\r\n";
    readAndPublish(str);
    //temp schalter hat keine Auswirkung, wenn Tank fehlt
    //Mit Tank, Modus I, Temp 0
    str = "C1.09,030,138,028,0000,1,0\r\n";
    readAndPublish(str);
    //Mit Tank, Modus 0, Temp 0
    str = "+1.09,030,138,028,0000,1,0\r\n";
    readAndPublish(str);
    
    //Mit Tank, Modus 0, Temp 0, Pumpe an
    str = "+1.09,030,138,028,0000,1,1\r\n";
    readAndPublish(str);
    //Mit Tank, Modus 0, Temp 0, Pumpe an
    str = "+1.09,031,138,029,0000,1,1\r\n";
    readAndPublish(str);
    //Mit Tank, Modus 0, Temp 0, Pumpe an
    str = "+1.09,032,138,030,0000,1,1\r\n";
    readAndPublish(str);
    //Mit Tank, Modus 0, Temp 0, Pumpe AUS
    str = "+1.09,033,138,031,0000,1,0\r\n";
    readAndPublish(str);
    //Mit Tank, Modus 0, Temp 0, Pumpe WIEDER AN
    str = "+1.09,033,138,031,0000,1,1\r\n";
    readAndPublish(str);
    
    return(0);
}



int main3(int argc, const char * argv[]) {
    std::string s = "C123b,095,112,063,1095,1\r\n";
    std::string delimiter = ",";
    
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        std::cout << token << std::endl;
        s.erase(0, pos + delimiter.length());
    }
    std::cout << s << std::endl;
    return(0);
}

/*
 From: https://github.com/SaibotFlow/marax-monitor/blob/main/marax_monitor/marax_monitor.ino
 Example Data: C1.06,116,124,093,0840,1,0\n every ~400-500ms
 Length: 26
 [Pos] [Data] [Describtion]
 0)      C     Coffee Mode (C) or SteamMode (V)
 -        1.06  Software Version
 1)      116   current steam temperature (Celsisus)
 2)      124   target steam temperature (Celsisus)
 3)      093   current hx temperature (Celsisus)
 4)      0840  countdown for 'boost-mode'
 5)      1     heating element on or off
 6)      0     pump on or off
 */

/*
 #format of: "C1.19,116,124,095,0560,0", every 400ms or so.
 # C1.19 - C for Coffee, 1.19 is the software version. This can also be V for Vapour (Steam) if the machine is in Steam priority mode.
 # 116 - This is the real/actual steam temperature in Celsius.
 # 124 - This is the target steam temperature in Celsius.
 # 096 - This is the real/actual heat exchanger temperature in Celsius.
 # 0560 - This is a countdown used to track if the machine is in "fast heating" mode, it seems to go anywhere from 1500-0. 0 means it's no longer boosting.
 # 0 - This represents if the heating element is on, 0 is Off, 1is On.
 
 # C123b,120,112,095,0000,0
 # Im Vergleich dazu, das was SaibotFlow als Beispiel angibt, bzw. was auch hier weiter oben gepostet wurde:
 # C1.06,116,124,093,0840,1,0\n
 # Ich habe also eine spaetere Software Version und die aktuelle Temperatur und die Zieltemperatur scheinen en Platz getauscht zu haben. Und ich habe eben nur sechs Werte, keine sieben. D
 
 */
