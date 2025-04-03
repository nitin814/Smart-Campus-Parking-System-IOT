#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_COUNT 4
#define I2C_ADDR 0x27

const char* SSID = "Wokwi-GUEST";  
const char* PASSWORD = "";    

// ThingSpeak API Details
const char* THINGSPEAK_API_KEY = "Insert your key";
const char* THINGSPEAK_URL = "https://api.thingspeak.com/update";

#define FIREBASE_HOST "https://iot-project-d423a-default-rtdb.firebaseio.com/"  
#define FIREBASE_AUTH "Interset api key"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


// ESP32 GPIO Mapping
const int trigPins[SENSOR_COUNT] = {12, 13, 14, 15};
const int echoPins[SENSOR_COUNT] = {32, 33, 25, 26};
const int ledPins[SENSOR_COUNT] = {2, 0, 4, 16};

const int displaySDA = 22;
const int displaySCL = 21;
LiquidCrystal_I2C lcd(I2C_ADDR, 20, 4);  // 20x4 LCD

// WiFi Connection Function
void connectWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected!");
}

void sendToThingSpeak(String parkingStatus) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(THINGSPEAK_URL) + "?api_key=" + THINGSPEAK_API_KEY;

        // Append parking status as ThingSpeak fields
        for (int i = 0; i < SENSOR_COUNT; i++) {
            url += "&field" + String(i + 1) + "=" + parkingStatus[i];
        }

        Serial.println("Sending data to ThingSpeak...");
        Serial.println(url);

        http.begin(url);
        int httpCode = http.GET();  // Send request

        if (httpCode > 0) {
            Serial.println("ThingSpeak Response: " + http.getString());
        } else {
            Serial.println("Error sending data: " + String(httpCode));
        }
        http.end();
    } else {
        Serial.println("WiFi Disconnected!");
    }
}

void sendToFirebase(int parkingStatus[]) {
    if (Firebase.ready()) {
        StaticJsonDocument<200> jsonDoc;
        JsonArray spots = jsonDoc.createNestedArray("parkingSpots");

        for (int i = 0; i < SENSOR_COUNT; i++) {
            JsonObject obj = spots.createNestedObject();
            obj["id"] = i + 1;  
            obj["status"] = parkingStatus[i]; 
        }

        String jsonString;
        serializeJson(jsonDoc, jsonString);

        String path = "/parkingSpots";  // Node path in Firebase
        Firebase.setJSON(fbdo, path, jsonDoc);
        
        Serial.println("Data sent to Firebase: ");
        Serial.println(jsonString);
    } else {
        Serial.println("Firebase not ready!");
    }
}

void LED_LCD_behaviour (int status[], float distances[]) {
    // Blink LED if any sensor detects an object < 30cm away 
    lcd.clear();
    for (int i=0; i<SENSOR_COUNT; ++i) {
      if (status[i] & distances[i] <= 30) {
        digitalWrite(ledPins[i], HIGH);
        Serial.println("||| DETECTED |||");
      } else {
        digitalWrite(ledPins[i], LOW);
      }

      // display info on LCD regardless of distance
      lcd.setCursor(0, i);
      lcd.print("S");
      lcd.print(i + 1);
      lcd.print(" distance: ");
      lcd.print(distances[i]);
      lcd.print(" cm");
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 Parking System with ThingSpeak");

    // Connect to WiFi
    connectWiFi();

    Initialize Firebase
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    // Configure GPIOs
    for (int i = 0; i < SENSOR_COUNT; i++) {
        pinMode(trigPins[i], OUTPUT);
        pinMode(echoPins[i], INPUT);
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }

    Wire.begin(displaySDA, displaySCL);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Hello, Wokwi!");
}

void loop() {
    String parkingStatus = "";
    int status[SENSOR_COUNT] = {0};
    float distances[SENSOR_COUNT] = {0};

    for (int i = 0; i < SENSOR_COUNT; i++) {
        // Trigger pulse
        digitalWrite(trigPins[i], LOW);
        delayMicroseconds(2);
        digitalWrite(trigPins[i], HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPins[i], LOW);

        // Measure echo duration
        long duration = pulseIn(echoPins[i], HIGH);
        float distance = (duration * 0.0343) / 2;  // Convert to cm
        distances[i] = distance;

        Serial.print("Sensor ");
        Serial.print(i + 1);
        Serial.print(" Distance: ");
        Serial.print(distance);
        Serial.println(" cm");


        if (distance > 0 && distance < 30) {
            parkingStatus += "1"; 
            status[i] = 1;
        } else {
            parkingStatus += "0";  
        }
    }
    
    LED_LCD_behaviour(status, distances);
    Serial.println("Parking Status: " + parkingStatus);
    sendToThingSpeak(parkingStatus);
    sendToFirebase(parkingStatus);

    delay(15000);  // ThingSpeak free-tier requires ~15 seconds delay
}
