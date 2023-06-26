#include <DHT.h>;
#include <Servo.h>


//Constants
#define SoilMPin A1
#define LDRPIN = A2
#define DHTPIN 8     
#define DHTTYPE DHT22   
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
#define MQ2pin A3
#define Threshold 150
#define Threshold2 200
#define Thresholdmin 700
#define Thresholdmax 800

//servo_motors
Servo servo1;
Servo servo2;
int angle = 180;

//Variables
int LDRsensor1 = 0; //Stores light level
int LDRsensor2 = 0; //Stores light level
int diffLDR = 0;
float hum;  //Stores humidity value
float temp; //Stores temperature value
const int LMPin = A0; //Stores temperature value

float LMsensor;
float voltageOut;
float temperatureK;
float temperatureC;
float temperatureF;

float MQValue;

//solenoid_valve
int solenoidPin = 4;


void setup()
{
  pinMode(A0, INPUT);
  servo1.attach(9);
  servo2.attach(10);
  Serial.begin(9600);
  dht.begin();
  pinMode(solenoidPin, OUTPUT);
  Serial.println("MQ2 warming up!");
  servo1.write(angle);
  servo2.write(angle);
	delay(20000); // allow the MQ2 to warm up
}

void loop()
{
    //{MQ_2}
    MQValue = analogRead(MQ2pin);

    //{capacitive_sensor}
    int soil_moisture = analogRead(SoilMPin);//
    soil_moisture = map(soil_moisture , Thresholdmin, Thresholdmax, 0, 100 );

    //{LDR}
    LDRsensor1 = analogRead(A2);
    LDRsensor2 = analogRead(A4);
    
    //{LM335}
    LMsensor = analogRead(A0);
    voltageOut = ((LMsensor * 5000) / 1024) ;
    temperatureK = (voltageOut / 10) ;
    temperatureC = temperatureK - 273; 
    temperatureF = (temperatureC * 1.8) + 32;

    //{DHT22}
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    
    //{LM335}
    Serial.print("Temperature(ºC): "); 
    Serial.print(temperatureC);  
    Serial.print("voltage out): "); 
    Serial.print(voltageOut);  

    //{DHT22}
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");

    //{capacitive_sensor}
    Serial.print("Soil Moisture: ");
    Serial.println(soil_moisture);

    //{LDR}
    Serial.print("Light(sensor1): ");
    Serial.print(LDRsensor1);
    Serial.print("Light(sensor2): ");
    Serial.print(LDRsensor2);
    Serial.print("diff");
    diffLDR = LDRsensor2 - LDRsensor1;
    Serial.print(diffLDR);

    Serial.print("Smoke level: ");
    Serial.print(MQValue);

    if(MQValue > Threshold)
    {
     Serial.print(" | Smoke detected!");
    }
  
    Serial.println("");

    if(soil_moisture < 25)
    {
      digitalWrite(solenoidPin, LOW);
      Serial.println("valve closed");
    }

    if(soil_moisture > 100)
    {
      digitalWrite(solenoidPin, HIGH);
      Serial.println("valve open");
    }


    for(;diffLDR > 10 ; angle+=5)
    {
      servo1.write(angle);
      delay(15);
      servo2.write(angle);
      delay(15);
      LDRsensor1 = analogRead(A2);
      LDRsensor2 = analogRead(A4);
      diffLDR = LDRsensor2 - LDRsensor1;
      Serial.println("^");
      
    }

    for(;diffLDR < -10 ; angle-=5)
    {
      servo1.write(angle);
      delay(15);
      servo2.write(angle);
      delay(15);
      LDRsensor1 = analogRead(A2);
      LDRsensor2 = analogRead(A4);
      diffLDR = LDRsensor2 - LDRsensor1;
      Serial.println(".");
    }

    delay(5000); //Delay 2 sec.
}

   
