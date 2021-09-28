/* 1.1.0 VERSION */

#ifndef ddmqtt_h
#define ddmqtt_h

#include <WiFiClient.h>
#include <PubSubClient.h>

class DDMqtt
{

private:
	const char *_deviceName;
	const char *_mqttUser;
	const char *_mqttPwd;
	const char *_mqttHost;
	const char *_subscribeTopic;
	int _subscribeQOS;
	int _mqttPort;
	MQTT_CALLBACK_SIGNATURE;
	WiFiClient client;
	PubSubClient _clientMqtt = PubSubClient(client);
	int _ledStatusPin = -1;
	uint16_t mqtt_max_packet_size = 512;

public:
	DDMqtt(const char *, const char *, int, const char *, const char *, const char *, int, int);

	void loop();
	void reconnectMQTT();
	void sendMessage(const char *, String);
	void setSubscribeCallback(MQTT_CALLBACK_SIGNATURE);
};

#endif