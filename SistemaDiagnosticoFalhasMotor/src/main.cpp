#include <Arduino.h>
#include <WiFi.h>
#include "FirebaseESP32.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

//Configurações do wifi
#define WIFI_SSID "****"
#define WIFI_PASSWORD "******"

//Configuração Firebase
#define FIREBASE_HOST "****"
#define FIREBASE_AUTH "****"

//Objeto de dados do firebase
FirebaseData firebaseData;

//Objeto do mpu
Adafruit_MPU6050 mpu;

//Porta do microfone
const int portMicro = 35;

int valorMicro = 0;

//Rota dos dados
const String route = "/data";

void setup() {
  pinMode(35, INPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.begin(9600);
  Serial.print("Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(300);
  }

   if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
   }
  Serial.println("MPU6050 Found!");

   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);

  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
}

void loop() {
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  valorMicro = analogRead(portMicro);

  valorMicro = map(valorMicro, 0, 4095, 22,120);
  
  
  Firebase.pushInt(firebaseData,route + "/acelerometro X",a.acceleration.x);
  
  Firebase.pushInt(firebaseData,route + "/acelerometro  Y",a.acceleration.y);
  
  Firebase.pushInt(firebaseData,route + "/acelerometro  Z",a.acceleration.z);
  

  
  Firebase.pushInt(firebaseData,route + "/giroscopio X",g.gyro.x);
  
  Firebase.pushInt(firebaseData,route + "/giroscopio  Y",g.gyro.y);
  
  Firebase.pushInt(firebaseData,route + "/giroscopio  Z",g.gyro.z);
  

  Firebase.pushInt(firebaseData,route + "/microfone", valorMicro);

  delay(500);
}