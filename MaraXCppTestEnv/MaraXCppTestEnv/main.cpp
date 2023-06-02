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

#define ESP_LOGD(x,y,z) ( printf(y,z) )

int pumpActive = 0;
uint32_t state_previousTimestamp = 0;
uint32_t state_timeElapsedSincePumpStarted = 0;
uint32_t state_timeElapsedSincePumpStopped = 0;
uint32_t state_lastPumpStoppedTimestamp = 0;
uint32_t state_lastTimeElapsedBetweenPumpStoppedAndStarted = 0;

uint32_t millis() {
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
    int senBoostCountdown=0;
    int senHeating=0;
    int senPumpActive=0;
    
    uint32_t previousTimestamp=0;
    uint32_t currentTimestamp=0;
    uint32_t timeElapsedSincePumpStarted=0;
    uint32_t timeElapsedSincePumpStopped=0;
    uint32_t lastPumpStoppedTimestamp = 0;
    uint32_t lastTimeElapsedBetweenPumpStoppedAndStarted = 0;
    int lastPumpActiveState=0;
    
    char stringBuffer[10];
    std::string sensorValueText;
    
    //TODOUNCOMMENTstd::string str(bytes.begin(), bytes.end());
    
    int elementCount = sscanf(str.c_str(), "%1c%4c,%d,%d,%d,%d,%d,%d", senType, senVersion, &senSteamActual, &senSteamTarget, &senHeatActual, &senBoostCountdown, &senHeating, &senPumpActive);
    
    if(debugMode) {
        ESP_LOGD("MaraX", "Element Count: %d \n", elementCount);
    }
    if(elementCount == 3) {
        if(debugMode) {
            ESP_LOGD("MaraX", "Water container is removed: %d\n", 1);
        }
        
        elementCount = sscanf(str.c_str(), "%1c%4c,%d,%3c,%d,%d,%d,%d", senType, senVersion, &senSteamActual, senSteamTargetStr, &senHeatActual, &senBoostCountdown, &senHeating, &senPumpActive);
    }
    
    if (elementCount >= 7 ) {
        
        snprintf(stringBuffer, sizeof(stringBuffer), "%.1s", senType);
        sensorValueText = stringBuffer;
        if(debugMode) {
            ESP_LOGD("MaraX", "Ops Mode: %s \n", sensorValueText.c_str());
        }
        //TODOUNCOMMENTid(opsType).publish_state(sensorValueText.c_str());
        
        snprintf(stringBuffer, sizeof(stringBuffer), "%.4s", senVersion);
        sensorValueText = stringBuffer;
        if(debugMode) {
            ESP_LOGD("MaraX", "MaraX Firmware: %s \n", sensorValueText.c_str());
        }
        //TODOUNCOMMENTid(coffeeVersion).publish_state(sensorValueText.c_str());
        
        if(debugMode) {
            ESP_LOGD("MaraX", "Steam Actual: %d \n", senSteamActual);
        }
        //TODOUNCOMMENTid(steam_actual).publish_state(senSteamActual);
        
        if(debugMode) {
            ESP_LOGD("MaraX", "Steam Target: %d \n", senSteamTarget);
        }
        //TODOUNCOMMENTid(steam_target).publish_state(senSteamTarget);
        
        if(debugMode) {
            ESP_LOGD("MaraX", "Heat Actual: %d \n", senHeatActual);
        }
        //TODOUNCOMMENTid(heat_actual).publish_state(senHeatActual);
        
        if(debugMode) {
            ESP_LOGD("MaraX", "Boost Countdown: %d \n", senBoostCountdown);
        }
        //TODOUNCOMMENTid(boost_countdown).publish_state(senBoostCountdown);
        
        if(debugMode) {
            ESP_LOGD("MaraX", "Is Heating: %d \n", senHeating);
        }
        //TODOUNCOMMENTid(heating).publish_state(senHeating);
        
        if(debugMode) {
            ESP_LOGD("MaraX", "Pump is active: %d \n", senPumpActive);
        }
        
        currentTimestamp = millis();
        
        // READ current global state values
        
        //TODOUNCOMMENTpreviousTimestamp = id(state_previousTimestamp);
        previousTimestamp = state_previousTimestamp; //TODOremove
        if (previousTimestamp == 0) {
            previousTimestamp = currentTimestamp;
        }
        
        //TODOUNCOMMENTlastPumpActiveState = round(id(pumpActive).state);
        lastPumpActiveState = pumpActive; //TODOremove
        
        //TODOUNCOMMENTtimeElapsedSincePumpStarted = id(state_timeElapsedSincePumpStarted);
        timeElapsedSincePumpStarted = state_timeElapsedSincePumpStarted; //TODOremove
        
        //TODOUNCOMMENTtimeElapsedSincePumpStopped = id(state_timeElapsedSincePumpStopped);
        timeElapsedSincePumpStopped = state_timeElapsedSincePumpStopped; //TODOremove
        
        //TODOUNCOMMENTlastPumpStoppedTimestamp = id(state_lastPumpStoppedTimestamp);
        lastPumpStoppedTimestamp = state_lastPumpStoppedTimestamp; //TODOremove
        
        //TODOUNCOMMENTlastTimeElapsedBetweenPumpStoppedAndStarted = id(state_lastTimeElapsedBetweenPumpStoppedAndStarted);
        lastTimeElapsedBetweenPumpStoppedAndStarted = state_lastTimeElapsedBetweenPumpStoppedAndStarted;
        
        
        // while pump is active
        if(senPumpActive == 1 && currentTimestamp != 0) {
            
            // if the pump just started to be active
            if (lastPumpActiveState == 0) {
                if(debugMode) {
                    ESP_LOGD("MaraX", "The pump just started to be active as last state was: %d \n", lastPumpActiveState);
                }
                // reset time elapsed since pump started counter
                timeElapsedSincePumpStarted = 0;
                
                // store time since pump stopped last time and this start of the pump for history keeping reasons
                lastTimeElapsedBetweenPumpStoppedAndStarted = timeElapsedSincePumpStopped;
                if(debugMode) {
                    ESP_LOGD("MaraX", "Store time elapsed between last stop and this new start of the pump: %d \n", lastTimeElapsedBetweenPumpStoppedAndStarted);
                }
                                
            } else {
                
                //add up the elapsed "pump is active"-time
                timeElapsedSincePumpStarted = timeElapsedSincePumpStarted + (currentTimestamp - previousTimestamp);
                if(debugMode) {
                    ESP_LOGD("MaraX", "Time elapsed since pump started to be active: %d \n", timeElapsedSincePumpStarted);
                }
            }
        }
        
        // while the pump is NOT active
        if(senPumpActive == 0 && currentTimestamp != 0) {
            
            // when the pump has JUST stopped, but was active before
            if(lastPumpActiveState == 1) {
                
                // save the timestamp when that Stop-Event happend
                lastPumpStoppedTimestamp = currentTimestamp;
                if(debugMode) {
                    ESP_LOGD("MaraX", "Pump has just stopped, but was active before. Stop event happend at timestamp: %d \n", lastPumpStoppedTimestamp);
                }
                // reset time elapsed since pump stopped counter
                timeElapsedSincePumpStopped = 0;
            }
            // if pump is stopped, but was already active before (using the lastPumpStoppedTimestamp as an indicator for that)
            else if (lastPumpStoppedTimestamp != 0) {
             
                //  increment the timeElapsedSincePumpStopped by the time elapsed meanwhile
                timeElapsedSincePumpStopped = timeElapsedSincePumpStopped + (currentTimestamp - previousTimestamp);
                if(debugMode) {
                    ESP_LOGD("MaraX", "Pump is stopped, but was active before at least once. Time elapsed since stop of the pump: %d \n", timeElapsedSincePumpStopped);
                }
            }
        }

        
        // finally publish everything
        
        //TODOUNCOMMENTid(state_timeElapsedSincePumpStarted) = timeElapsedSincePumpStarted;
        state_timeElapsedSincePumpStarted = timeElapsedSincePumpStarted; //TODOremove
        
        //TODOUNCOMMENTid(state_timeElapsedSincePumpStopped) = timeElapsedSincePumpStopped;
        state_timeElapsedSincePumpStopped = timeElapsedSincePumpStopped; //TODOremove
        
        //TODOUNCOMMENTid(state_lastPumpStoppedTimestamp) = lastPumpStoppedTimestamp;
        state_lastPumpStoppedTimestamp = lastPumpStoppedTimestamp; // TODOremove
        
        //TODOUNCOMMENTid(state_lastTimeElapsedBetweenPumpStoppedAndStarted) = lastTimeElapsedBetweenPumpStoppedAndStarted;
        state_lastTimeElapsedBetweenPumpStoppedAndStarted = lastTimeElapsedBetweenPumpStoppedAndStarted;
        
        //TODOUNCOMMENTid(state_previousTimestamp) = currentTimestamp;
        state_previousTimestamp = currentTimestamp; //TODOremove
        
        //TODOUNCOMMENTid(pumpActive).publish_state(senPumpActive);
        pumpActive = senPumpActive; //TODOremove
    }
    
}


int main(int argc, const char * argv[]) {
    
    std::string str = "C1.19,116,124,095,0560,0";
    /*
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
     */
    
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
    //Mit Tank, Modus 0, Temp 0, Pumpe immer noch AUS
    str = "+1.09,033,138,031,0000,1,0\r\n";
    readAndPublish(str);
    //Mit Tank, Modus 0, Temp 0, Pumpe WIEDER AN
    str = "+1.09,033,138,031,0000,1,1\r\n";
    readAndPublish(str);
    //Mit Tank, Modus 0, Temp 0, Pumpe an
    str = "+1.09,032,138,030,0000,1,1\r\n";
    readAndPublish(str);
    //Mit Tank, Modus 0, Temp 0, Pumpe AUS
    str = "+1.09,033,138,031,0000,1,0\r\n";
    readAndPublish(str);
    //Mit Tank, Modus 0, Temp 0, Pumpe immer noch AUS
    str = "+1.09,033,138,031,0000,1,0\r\n";
    readAndPublish(str);
    
    return(0);
}



void parseExperiments() {
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
}


// esphome-mara
//C123b,095,112,063,1095,1\r\n

// other
//C1.06,116,124,093,0840,1,0\n

// V2: https://github.com/SaibotFlow/marax-monitor
// Example Data: C1.06,116,124,093,0840,1,0\n
//C	Machine-Mode: C = CoffeeMode; V = Vapour/SteamMode
// 1.06	Firmware
// 116	Current Steam Temperature in Celsius
// 124	Target Steam Temperature in Celsius
// 093	Curent Hx Temperature in Celsius
// 0840	Countdown Boost-Mode
// 1	Heat state (0 = off; 1= on)
// 0	Pump state (0 = off; 1= on)

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
 
 # D][api:102]: Accepted 192.168.178.200
 # [17:28:17][D][api.connection:920]: Home Assistant 2023.3.5 (192.168.178.200): Connected successfully
 # [17:28:17][D][uart_debug:158]: <<< "C1.09,050,128,030,0000,1,0\r\n"
 # [17:28:17][D][text_sensor:067]: 'Betriebsmodus': Sending state 'C2'
 # [17:28:17][D][sensor:127]: 'Dampf Aktuelle Temperatur': Sending state 50.00000 °C with 1 decimals of accuracy
 # [17:28:17][D][sensor:127]: 'Dampf Zieltemperatur': Sending state 128.00000 °C with 1 decimals of accuracy
 # [17:28:17][D][sensor:127]: 'HX Aktuelle Temperatur': Sending state 30.00000 °C with 1 decimals of accuracy
 # [17:28:17][D][sensor:127]: 'Time Counter': Sending state 0.00000 s with 1 decimals of accuracy
 
 
 */
