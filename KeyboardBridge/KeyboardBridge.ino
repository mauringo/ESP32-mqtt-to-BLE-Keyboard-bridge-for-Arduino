
#include <BleKeyboard.h>
#include "EspMQTTClient.h"



EspMQTTClient client(
  "MauroNet",
  "SSID!",
  "localip",  // MQTT Broker server ip
  "username",   // Can be omitted if not needed
  "password",   // Can be omitted if not needed
  "RemoteControl",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

BleKeyboard bleKeyboard("MauroDevice","SuperMauro",100);

void setup() {
  Serial.begin(115200);

  delay(1000);
  Serial.println("Starting BLE work!");


  bleKeyboard.begin();

   // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

void onConnectionEstablished()
{

  

  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("remote/volplus", [](const String & payload) {
     if(bleKeyboard.isConnected()) {
        Serial.println("Sending Volume UP...");
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);

     }
  });

    client.subscribe("remote/mute", [](const String & payload) {
     if(bleKeyboard.isConnected()) {
        Serial.println("Sending Volume UP...");
        bleKeyboard.write(KEY_MEDIA_MUTE);

     }
  });

  client.subscribe("remote/volminus", [](const String & payload) {
     if(bleKeyboard.isConnected()) {
      Serial.println("Sending Volume Down...");
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
     }
  });


  client.subscribe("remote/next", [](const String & payload) {
     if(bleKeyboard.isConnected()) {
        Serial.println("Sending KEY_MEDIA_NEXT_TRACK");
        bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);

     }
  });

    client.subscribe("remote/pause", [](const String & payload) {
     if(bleKeyboard.isConnected()) {
        Serial.println("Sending KEY_MEDIA_PLAY_PAUSE");
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);

     }
  });

  client.subscribe("remote/previous", [](const String & payload) {
     if(bleKeyboard.isConnected()) {
      Serial.println("Sending KEY_MEDIA_PREVIOUS_TRACK");
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
     }
  });



///communicaiton and reset 
  client.subscribe("remote/comunication", [](const String & payload) {
     String feedback="false";

     if (bleKeyboard.isConnected()){
      feedback="true";
     }
     String myjson = "{\"status\":" + feedback + "}";
     client.publish("remote/returndata", myjson);
      });

  client.subscribe("remote/reset", [](const String & payload) {
     ESP.restart(); // Trigger the software reset
      });

}

void loop() {
    client.loop();

}
