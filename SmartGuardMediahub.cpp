#include <LiquidCrystal.h> 
#include "Arduino.h" 
#include "SoftwareSerial.h" 
#include "DFRobotDFPlayerMini.h" 
#include <SoftwareSerial.h> 
#include "VoiceRecognitionV3.h" 
#include <DS3231.h> 
DS3231 rtc(SDA, SCL); 
Time t; 
int incomingByte = 0; 
const int rs = 12, en = 13, d4 = 4, d5 = 5, d6 = 6, d7 = 7; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 
VR myVR(2,3); // 2:RX 3:TX, you can choose your favourite pins. 
uint8_t records[7]; // save record 
uint8_t buf[64]; 
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX 
DFRobotDFPlayerMini myDFPlayer; 
void printDetail(uint8_t type, int value); 
int led = 13; 
#define playRecord (0) 
#define stopRecord (1) 
#define temparaturRecord (2) 
#define timeRecord (3) 
#define ziraRecord (4) 48 

void printSignature(uint8_t *buf, int len) 
{ 
int i; 
for(i=0; i<len; i++){ 
if(buf[i]>0x19 && buf[i]<0x7F){ 
Serial.write(buf[i]); 
} 
else{ 
Serial.print("["); 
Serial.print(buf[i], HEX); 
Serial.print("]"); 
} 
} 
} 
void printVR(uint8_t *buf) 
{ 
Serial.println("VR Index\tGroup\tRecordNum\tSignature"); 
Serial.print(buf[2], DEC); 
Serial.print("\t\t"); 
if(buf[0] == 0xFF){ 
Serial.print("NONE"); 
} 
else if(buf[0]&0x80){ 
Serial.print("UG "); 
Serial.print(buf[0]&(~0x80), DEC); 49 

} 
else{ 
Serial.print("SG "); 
Serial.print(buf[0], DEC); 
} 
Serial.print("\t"); 
Serial.print(buf[1], DEC); 
Serial.print("\t\t"); 
if(buf[3]>0){ 
printSignature(buf+4, buf[3]); 
} 
else{ 
Serial.print("NONE"); 
} 
Serial.println("\r\n"); 
} 
void setup() 
{ 
mySoftwareSerial.begin(9600); 
Serial.begin(115200); 
Serial.println("hello@"); 
Serial.println(F("DFRobot DFPlayer Mini Demo")); 
Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)")); 
if (!myDFPlayer.begin(mySoftwareSerial)) { //Use softwareSerial to communicate with mp3. 
Serial.println(F("Unable to begin:")); 50 

Serial.println(F("1.Please recheck the connection!")); 
Serial.println(F("2.Please insert the SD card!")); 
while(true); 
} 
Serial.println(F("DFPlayer Mini online.")); 
myDFPlayer.volume(25); //Set volume value. From 0 to 30 
myVR.begin(9600); 
pinMode(led, OUTPUT); 
if(myVR.clear() == 0){ 
Serial.println("Recognizer cleared."); 
}else{ 
Serial.println("Not find VoiceRecognitionModule."); 
Serial.println("Please check connection and restart Arduino."); 
while(1); 
} 
if(myVR.load((uint8_t)playRecord) >= 0){ 
Serial.println("onRecord loaded"); 
} 
if(myVR.load((uint8_t)stopRecord) >= 0){ 
Serial.println("offRecord loaded"); 
} 
if(myVR.load((uint8_t)temparaturRecord) >= 0){ 
Serial.println("temparaturRecord loaded"); 
} 
if(myVR.load((uint8_t)timeRecord) >= 0){ 51 

Serial.println("timeRecord loaded"); 
} 
if(myVR.load((uint8_t)ziraRecord) >= 0){ 
Serial.println("ziraRecord loaded"); 
} 
Serial.println("Ready"); 
Serial.begin(115200); 
rtc.begin(); 
rtc.setTime(9, 20, 0); // Set the time to 12:00:00 (24hr format) 
rtc.setDate(21, 4, 2018); // Set the date to January 1st, 2014 
lcd.begin(16, 2); 
Serial.begin(115200); 
} 
void loop() 
{ 
int ret; 
t = rtc.getTime(); 
int t1 = rtc.getTemp(); 
ret = myVR.recognize(buf, 50); 
if(ret>0){ 
switch(buf[1]){ 
case ziraRecord: 
lcd.begin(16,1); 
lcd.print("HELLO DEAR"); 
myDFPlayer.play(111); 52 

digitalWrite(led, HIGH) 
break; 
case playRecord: 
myDFPlayer.play(113); 
lcd.begin(16,1); 
lcd.print("Track no:113"); 
digitalWrite(led, HIGH); 
break; 
case stopRecord: 
myDFPlayer.stop(); 
digitalWrite(led, LOW); 
break; 
case temparaturRecord: 
lcd.begin(16,1); 
Serial.println(" Temparature "); 
//int t = rtc.getTemp(); 
Serial.print(t1); 
Serial.println(" C"); 
myDFPlayer.volume(50); 
if ( t1 == 26) 
{ 
myDFPlayer.play(54); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 53 

if ( t1 == 27) 
{ 
myDFPlayer.play(56); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
if ( t1 == 28) 
{ 
myDFPlayer.play(58); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
else if ( t1 == 29) 
{ 
myDFPlayer.play(60); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
else if ( t1 == 30) 
{ 
myDFPlayer.play(62); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
else if ( t1 == 31) 54 

{ 
myDFPlayer.play(64); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
else if ( t1 == 32) 
{ 
myDFPlayer.play(66); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
else if ( t1 == 33) 
{ 
myDFPlayer.play(68); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
else if ( t1 == 34) 
{ 
myDFPlayer.play(70); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
else if ( t1 == 35) 
{ 55 

myDFPlayer.play(72); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
else if ( t1 == 36) 
{ 
myDFPlayer.play(74); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
else if ( t1 == 37) 
{ 
myDFPlayer.play(78); 
delay(500); 
myDFPlayer.play(110); 
lcd.print("Temp: "); 
lcd.print(t1); 
} 
digitalWrite(led, HIGH); 
break; 
case timeRecord: 
lcd.begin(16, 2); 
lcd.setCursor(0,0); 
lcd.print("Time: "); 
lcd.print(rtc.getTimeStr()); 56 

lcd.setCursor(0,1); 
lcd.print("Date: "); 
lcd.print(rtc.getDateStr()); 
digitalWrite(led, LOW); 
break; 
default: 
Serial.println("Record function undefined"); 
break; } 
/** voice recognized */ 
printVR(buf); 
} 
}
