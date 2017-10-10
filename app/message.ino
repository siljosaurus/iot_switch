#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

int switchPin = 5;
int yellowPin = 4;

// static DHT dht(DHT_PIN, DHT_TYPE); # DHT borte
void initSensor()
{
   pinMode(switchPin, INPUT);
}

int readSwitchForGreenTime() {
    int switchValue = digitalRead (switchPin);
    return(switchValue);
}
int readSwitchForYellowTime() {
    int switchValue = digitalRead (yellowPin);
    return switchValue;
}


bool readMessage(int messageId, float usedTime, char *payload)
{
    
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["deviceId"] = DEVICE_ID;
    root["messageId"] = messageId;
    bool knappAlert = false;


    //// PUSHER: tidsbruk
        if (std::isnan(usedTime))
    {
        root["Tidsbruk"] = NULL;
    }
    else
    {
        root["Tidsbruk"] = usedTime;
    }

    root.printTo(payload, MESSAGE_MAX_LEN);
    return knappAlert;
}

void parseTwinMessage(char *message)
{
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(message);
    if (!root.success())
    {
        Serial.printf("Parse %s failed.\r\n", message);
        return;
    }

    if (root["desired"]["interval"].success())
    {
        interval = root["desired"]["interval"];
    }
    else if (root.containsKey("interval"))
    {
        interval = root["interval"];
    }
}
