#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

int switchPin = 5;

// static DHT dht(DHT_PIN, DHT_TYPE); # DHT borte
void initSensor()
{
   pinMode(switchPin, INPUT);
}

int readSwitch() {
 int switchValue = digitalRead (switchPin);
 return(switchValue);
}


bool readMessage(int messageId, char *payload)
{
    int knapp = readSwitch();
    
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["deviceId"] = DEVICE_ID;
    root["messageId"] = messageId;
    bool knappAlert = false;

    // NAN is not the valid json, change it to NULL
    if (std::isnan(knapp))
    {
        root["lyspaere"] = NULL;
    }
    else
    {
        root["lyspaere"] = knapp;
        if (knapp == KNAPP_ALERT)
        {
            knappAlert = true;
        }
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
