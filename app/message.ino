#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

const int gasSensor = A0;

// static DHT dht(DHT_PIN, DHT_TYPE); # DHT borte
void initSensor()
{
    pinMode(gasSensor, INPUT);
}

float readGas() {
 float gasValue = analogRead (gasSensor);
 float returnValue = gasValue*0.004882814;
 return(returnValue);
}


bool readMessage(int messageId, char *payload)
{
    float gas = readGas();
    
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["deviceId"] = DEVICE_ID;
    root["messageId"] = messageId;
    bool gasAlert = false;

    // NAN is not the valid json, change it to NULL
    if (std::isnan(gas))
    {
        root["gas"] = NULL;
    }
    else
    {
        root["gas"] = gas;
        if (gas > GAS_ALERT)
        {
            gasAlert = true;
        }
    }

    root.printTo(payload, MESSAGE_MAX_LEN);
    return gasAlert;
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
