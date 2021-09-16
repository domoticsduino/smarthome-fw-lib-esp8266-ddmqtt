/* 1.0.0 VERSION */

#include "ddmqtt.h"
#include "ddcommon.h"

DDMqtt::DDMqtt(const char *deviceName, const char *mqttHost, int mqttPort, const char *mqttUser, const char *mqttPwd, const char *subscribeTopic, int subscribeQOS, int led)
{

	this->_deviceName = deviceName;
	this->_mqttHost = mqttHost;
	this->_mqttPort = mqttPort;
	this->_mqttUser = mqttUser;
	this->_mqttPwd = mqttPwd;
	this->_subscribeTopic = subscribeTopic;
	this->_subscribeQOS = subscribeQOS;
	this->_ledStatusPin = led;
	this->_clientMqtt.setBufferSize(this->mqtt_max_packet_size);
}

void DDMqtt::reconnectMQTT()
{

	writeToSerial("Reconnecting...", true);

	while (!this->_clientMqtt.connected())
	{

		writeToSerial("Attempting MQTT connection to ", false);
		writeToSerial(this->_mqttHost, false);
		writeToSerial(":", false);
		writeToSerial(this->_mqttPort, false);
		writeToSerial("...", true);

		this->_clientMqtt.setServer(this->_mqttHost, this->_mqttPort);

		// Attempt to connect

		if (this->_clientMqtt.connect(this->_deviceName, this->_mqttUser, this->_mqttPwd))
		{

			writeToSerial("connected", true);

			if (this->callback != nullptr)
			{

				this->_clientMqtt.setCallback(this->callback);
				if (this->_clientMqtt.subscribe(this->_subscribeTopic, this->_subscribeQOS))
				{

					writeToSerial("subscribed to topic ", false);
					writeToSerial(this->_subscribeTopic, true);
				}
				else
				{

					writeToSerial("failed to subscribe to topic ", false);
					writeToSerial(this->_subscribeTopic, true);
				}
			}
		}

		else
		{

			writeToSerial("failed, rc=", false);
			writeToSerial(this->_clientMqtt.state(), false);
			writeToSerial(" try again", true);
			digitalWrite(this->_ledStatusPin, LOW);
		}

		delay(5000);
	}

	digitalWrite(this->_ledStatusPin, HIGH);
}

void DDMqtt::loop()
{

	this->_clientMqtt.loop();
}

void DDMqtt::sendMessage(const char *topic, String val)
{

	//String outBuf = String(val);

	writeToSerial("Sending ", false);
	writeToSerial(val, false);
	writeToSerial(" to topic ", false);
	writeToSerial(topic, true);

	if (!this->_clientMqtt.connected())
	{
		writeToSerial("#2 MQTT not connected. Retrying... ", true);
		this->reconnectMQTT();
	}

	this->_clientMqtt.loop();

	if (!this->_clientMqtt.publish(topic, val.c_str(), true))
	{

		writeToSerial("MQTT publishing error", true);
		digitalWrite(this->_ledStatusPin, LOW);
	}
	else
	{

		digitalWrite(this->_ledStatusPin, LOW);
		delay(100);
		digitalWrite(this->_ledStatusPin, HIGH);
		delay(100);
		digitalWrite(this->_ledStatusPin, LOW);
		delay(100);
		digitalWrite(this->_ledStatusPin, HIGH);
		delay(100);
		digitalWrite(this->_ledStatusPin, LOW);
		delay(100);
		digitalWrite(this->_ledStatusPin, HIGH);
	}
}

void DDMqtt::setSubscribeCallback(MQTT_CALLBACK_SIGNATURE)
{

	this->callback = callback;
}
