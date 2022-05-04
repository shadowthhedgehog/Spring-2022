
#include <dht.h>
#include <arduino-timer.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// FAHREN; 1 = print everything in Fahrenheit
//         0 = print everything in Celsius
#define FAHREN 1

// PRINTSERIAL 1 = print everything to the serial monitor
//             0 = Don't setup or print to serial monitor
#define PRINTSERIAL 0

// milliseconds in 24 hours
#define MAX_TIME 86400000

//amount of time between temp/humidity sampling and displaying in ms
#define REFRESH_TIME 45000

// mapping for I/O pins and systems
#define fans 2
#define lights 3
#define heater 4
#define humidifier 5
#define dht_module 6
#define cooler 7

// mapping for knobs to analog pins
#define HumKnob A0
#define TempKnob A1
#define LightKnob A2
#define AirKnob A3
// Pin A4 = SDA by default
// Pin A5 = SCL by default

// values to help interface with OLED 1306
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//------------function prototypes-----------------------------------------------------------
void DisplayInit(void);
void PinInit(void);
void SerialInit(void);


bool ClimateControl(void *);
bool ClimateControl(void);
bool DisplayDHT(void);
bool printSerial(void);

bool fanOn(void *);
bool fanOff(void *);
bool lightOn(void *);
bool lightOff(void *);

bool lightTimerHelper(void *);
bool airTimerHelper(void *);





//-----------global variables----------------------------------------------------------------
dht DHT; // instance of dht module

int temp = 0;
int hum = 0;

int tempin = 0;
int humin = 0;
int lightIn = 0;
int lightinScaled = 0;
int airIn = 0;
int airinScaled = 0;

int humScaled = 0;
int tempScaled = 0;
long lighttime = 0;
long fantime = 0;


Timer<3> timer;





//-------------setup--------------------------------------------------------------------------
void setup() {
  PinInit();  //setup all digital I/O pins


  // get initial chamber reading
   DHT.read11(dht_module);
   temp = DHT.temperature;
   hum = DHT.humidity;

  //read in knob values from user
   lightIn = analogRead(LightKnob);
   airIn = analogRead(AirKnob);
   tempin = analogRead(TempKnob);
   humin = analogRead(HumKnob);


   //scale the input values from raw analog levels to desired numbers
   tempScaled = 10 + ceil((tempin*30)/1023);  //becomes # from 10 to 40
   humScaled = 40 + ((humin*25)/1023) + ((humin*30)/1023);    //becomes # from 40 to 95
   lightinScaled = ceil((lightIn*24)/1023);  //becomes # from 0 to 24
   airinScaled = ceil((airIn*24)/1023);      //becomes # from 0 to 24

// setup and print to serial monitor
#if PRINTSERIAL
  SerialInit();
  printSerial();
#endif

// setup the displayy
  DisplayInit();



  lighttime = lightinScaled*3600000;
  fantime = airinScaled*3600000;

if(lightinScaled != 0){
  digitalWrite(lights, HIGH);
 // timer.in(lighttime, lightOff);
}

if(airinScaled !=0){
  digitalWrite(fans, HIGH);
 // timer.in(fantime, fanOff);
}

 //set up timer for temp/humidity
 timer.every(REFRESH_TIME, ClimateControl);


 ClimateControl();
//  //set up timers for light
//  if(lightinScaled == 0){
//    digitalWrite(lights, LOW);
//  }
//  else{
//    lighttime = floor((lightIn*MAX_TIME)/1023);
//    digitalWrite(lights, HIGH);
//    timer.every(MAX_TIME, lightOn);
//    timer.in(lighttime, lightTimerHelper);
//  }
//  
//  //set up timers for fan
//  if(airinScaled == 0){
//    digitalWrite(fans, LOW);
//  }
//  else{
//    fantime = floor((airIn*MAX_TIME)/1023);
//    digitalWrite(fans, HIGH);
//    timer.every(MAX_TIME, fanOn);
//    timer.in(fantime, airTimerHelper);
//  }
}




//------------main loop------------------------------------------------------------------------
void loop() {
timer.tick();
//  delay(REFRESH_TIME);
//  ClimateControl();
}





//----------------functions---------------------------------------------------------------------
void SerialInit(void){
  Serial.begin(9600);

  delay(500);
  Serial.println("Project GreenBox \n\n");
}





// initialize display and print all input values in 3 second increments
void DisplayInit(void){
// initialize display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 8);
  // Display static text
  display.println("GreenBox");
  display.display(); 

  delay(2000);  // Wait

  // display inputs
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Input Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(humScaled);
  display.print(" %"); 
  display.display(); 

  delay(2000);  // Wait

  // display inputs
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Input Air: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(airinScaled);
  display.print(" hrs."); 
  display.display(); 

  delay(2000);  // Wait

  // display inputs
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Input Light: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(lightinScaled);
  display.print(" hrs."); 
  display.display(); 

  delay(2000);  // Wait

    // display inputs
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Input Temperature: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
#if FAHREN
  display.print((tempScaled * 9/5) + 32);
  display.print(" F"); 
#else
  display.print(tempScaled);
  display.print(" C"); 
#endif
  display.display(); 

  delay(2000);
}




void PinInit(void){
  pinMode(fans, OUTPUT);
  pinMode(dht_module, INPUT);
  pinMode(lights, OUTPUT);
  pinMode(heater, OUTPUT);
  pinMode(humidifier, OUTPUT);
  pinMode(cooler, OUTPUT);
}





bool DisplayDHT(void){
  //clear display
  display.clearDisplay();

  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temp: ");
  display.setTextSize(2);
  display.setCursor(0,10);
#if FAHREN
  display.print((temp * 9/5) + 32);
#else
  display.print(temp);
#endif
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
#if FAHREN
  display.print("F");
#else
  display.print("C");
#endif

  // display humidity
  display.setTextSize(1);
  display.setCursor(64, 0);
  display.print("Hu: ");
  display.setTextSize(2);
  display.setCursor(64, 10);
  display.print(hum);
  display.print("%"); 
  display.display(); 
}



bool printSerial(void){
   Serial.print("input temp = ");
#if FAHREN
   Serial.print((tempScaled * 9/5) + 32);
#else
   Serial.print(tempScaled);
#endif
   Serial.print("\n");

   Serial.print("input humidity = ");
   Serial.print(humScaled);
   Serial.print("\n");

   Serial.print("input light time = ");
   Serial.print(lightinScaled);
   Serial.print("\n");

   Serial.print("input air-flow time = ");
   Serial.print(airinScaled);
   Serial.print("\n\n");
   
   // print current conditions to terminal
   Serial.print("Current Humidity = ");
   Serial.print(hum);
   Serial.print("% ");
   Serial.print("Current Temperature = ");
#if FAHREN
   Serial.print((temp * 9/5) + 32);
   Serial.print(" F \n\n");
#else
   Serial.print(temp);
   Serial.print(" C \n\n");
#endif

  Serial.print("lighttime = ");
  Serial.print(lighttime);

  Serial.print("  fantime = ");
  Serial.print(fantime);
  Serial.print("\n\n");

}



bool ClimateControl(void *){
  ClimateControl();
  return 1;
}
  
bool ClimateControl(void){

  // read chamber
   DHT.read11(dht_module);
   temp = DHT.temperature;
   hum = DHT.humidity;
   
  // display readings
   DisplayDHT();

  // print to serial if option turned on
#if PRINTSERIAL
   printSerial();
#endif



  //temperature if statement (from 10 to 40 degrees C)
  if(temp < .9*tempScaled){  //if temp too low
    digitalWrite(cooler, LOW);
    delay(500);
    digitalWrite(heater, HIGH);
  }
  else if(temp > 1.2*tempScaled){  //if temp too high
    digitalWrite(heater, LOW);
    delay(500);
    digitalWrite(cooler, HIGH);
  }
  else{  //if temp in range
    digitalWrite(heater, LOW);
    delay(500);
    digitalWrite(cooler, LOW);
  }


  
  //humidity if statement (from 40 to 95 percent)
  if(hum < .9*humScaled){
    digitalWrite(humidifier, HIGH);
  }
  else if(hum > 1.2*humScaled){
    digitalWrite(humidifier, LOW);
    delay(500);
    digitalWrite(fans, HIGH);
  }
  else{
    digitalWrite(humidifier, LOW);
    delay(500);
  //  digitalWrite(fans, LOW);
  }

  return 1;
}



bool fanOn(void *){
  digitalWrite(fans, HIGH);
}
bool fanOff(void *){
  digitalWrite(fans, LOW);
}
bool lightOn(void *){
  digitalWrite(lights, HIGH);
}
bool lightOff(void *){
  digitalWrite(lights, LOW);
}

//bool lightTimerHelper(void *){
//  timer.every(MAX_TIME, lightOff);
//}
//bool airTimerHelper(void *){
//  timer.every(MAX_TIME, fanOff);
//}
