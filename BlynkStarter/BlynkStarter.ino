/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example runs directly on ESP32 chip.
  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32
  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!
  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define LED 2

const int freq = 5000;
const int ledChannel =0;
const int resolution =10;
int time_count = 0; // timer counter global variable
String content = "";  // null string constant ( an empty string )
BlynkTimer timer;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "RUiww1-tyaG_0IulqUwThoJeEpHJxk7z";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "FYDEHot";
char pass[32] = "Password123";
int value;

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(10L, myTimerEvent);

  ledcSetup(ledChannel, freq, resolution);

  ledcAttachPin(LED, ledChannel);

}

  BLYNK_WRITE(V2){
  int pinValue = param.asInt();

  if (pinValue == 0) {
  ledcWrite(ledChannel, 0);
  }
  else {
  ledcWrite(ledChannel, 255);
  }
  }

BLYNK_WRITE(V0)
{
   // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int val = param.asInt(); // assigning incoming value from pin V1 to a variable
    ledcWrite(ledChannel, val); // Note that this takes ledChannel as an argument, NOT the pin! Set duty = val.
}

void myTimerEvent() // Every 10 ms
{
    if (time_count % 25 == 0) { // every 250 ms
        // Do thing that needs to happen every 0.25 seconds
    }
    if (time_count == 100){
        Blynk.virtualWrite(V3, millis() / 1000);
        time_count = 0; // reset time counter
    }
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            content.concat(character);
        }
        if (content != "") {
            Blynk.virtualWrite(V1, content);
            content = ""; // Clear String
        }  
    }
    time_count += 1; // Increment on every tick
}



// You need to start your timer in the loop.
void loop()
{
    Blynk.run();
    timer.run(); // Initiates BlynkTimer
}
