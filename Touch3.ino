
/*************** Avi Hayun 27/10/18 ***************/

#include "TFT9341Touch.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <LM75.h>

#define PIN 5
tft9341touch LcdTouch(10, 9, 7, 2); //cs, dc ,tcs, tirq
// הגדרת אובייקט המייצג את הלדים, הפרמטרים שמקבל: 
// מספר הלדים, הרגל שמחוברת לרצועה (5), סדר הצבעים ותדר התקשורת
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
int trig=8, echo=4; // הגדרת הפין 8 כטריגר ו 4 כאקו
LM75 sensor; // הגדרת אובייקט מסוג LM75 לצורך  תקורת עם החיישן
void setup() {
  LcdTouch.begin();
  LcdTouch.clearButton();
  LcdTouch.setRotation(0);//0,1,2,3
  LcdTouch.setTextSize (2);
  LcdTouch.setCursor (40, 40);
  LcdTouch.set(3780, 372, 489, 3811); //Calibration
  strip.begin();  // אתחול הרצועה
  strip.show(); // מראה את המצב הנוכחי- כל הלדים מכובים
  pinMode(trig,OUTPUT); // הדק 8 כדי לתת "דרבון" לחיישן לפעול
  pinMode(echo,INPUT); // הדק 4 כדי לתת מידע על ההד
 
 Wire.begin();
 Serial.begin(9600);
  screenMain();
}
int time;
int distance;
 uint16_t x, y;
  String str;
void loop() 
{
  
 while (!LcdTouch.touched());
  if (LcdTouch.touched())
  {
    LcdTouch.readTouch();
    x = LcdTouch.xTouch;
    y = LcdTouch.yTouch;

    int ButtonNum = LcdTouch.ButtonTouch(x, y);
   

    if (ButtonNum == 1) 
    {
      instruction();
      // screenMain();
    }
    else if (ButtonNum == 2) 
    {
      amountWater();
    }
    else if (ButtonNum == 3) 
    {
      amountWater();
    }
  }
}


void screenMain() {
  LcdTouch.fillScreen (WHITE);
  LcdTouch.print(20, 20, "welcome to smart buttle", 2, BLACK);
  LcdTouch.drawButton(1, 120,  100, 90, 50, 10, RED, BLACK, "start", 2); // NumButton, x, y, width, height, r, Color, textcolor, label, textsize);--
  
}
// פונקציה זו צובעת את הלדים בצבע שקיבלה ומחכה בין צביעה אחת לשנייה את הזמן שקיבלה
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


void instruction() {
  LcdTouch.fillScreen (WHITE);
  LcdTouch.setTextColor(BLACK);
  LcdTouch.setTextSize (3);
  LcdTouch.setCursor (25, 30);
  LcdTouch.println("instructions");
  LcdTouch.setTextSize (1);
  LcdTouch.println("  In the smart bottle you can see");
  LcdTouch.println("  the temperature of the water according to");
  LcdTouch.println("  the color of the bottle,");
  LcdTouch.println("  you can also know the amount of water");
  LcdTouch.println("  in the bottle on the screen");
  LcdTouch.println("  and when the water ran out");
  LcdTouch.drawButton(2, 90,  180, 150, 50, 10, BLACK, RED, "to continue", 2);
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  while (!LcdTouch.touched());
}


int f=0;
void amountWater() 
{
    LcdTouch.fillScreen (BLACK);
    LcdTouch.setTextColor(WHITE);
    LcdTouch.setTextSize (3);
    Serial.println ("The bottle isnot empty");
    LcdTouch.setCursor (25, 30);
    LcdTouch.println ("Amount of water");
    f=0;
    while(f==0)
    {
      int h=distanceB();
      int temp=temper();

      if(temp>70)
          colorWipe(strip.Color(255, 0, 0), 50); // Red
      if(temp<30)
          colorWipe(strip.Color(0, 0, 255), 50); // Red
      else
          colorWipe(strip.Color(0, 255, 0), 50); // Red
      double s=((135/20)+0.75)*h+90;
      // LcdTouch.println(temp);
      // LcdTouch.println(h);
      LcdTouch.fillRect(30, 80, 50, 50, RED);

  // הגדרת צבע הטקסט, גודל הטקסט ומיקום הריבוע
      LcdTouch.setCursor(43, 100);
      LcdTouch.setTextColor(ILI9341_WHITE); 
      LcdTouch.setTextSize(2);
      LcdTouch.print(temp);
      LcdTouch.fillCircle(70, 103, 2, WHITE);
      LcdTouch.fillRect(120, 90, 80, 135, WHITE); // x, y,width,height
      if(h>=19)
          emptyBottle();
      for(int i=s; i<220; i++)
      {
        LcdTouch.drawHLine (125,i, 70, BLUE) ; 
      }
      delay(1000);
    }
    // play();
  
  while (!LcdTouch.touched());
}

void emptyBottle()
{
  f=1;
  LcdTouch.fillScreen (WHITE);
  LcdTouch.setTextColor(BLACK);
  LcdTouch.setTextSize (2);
  LcdTouch.setCursor (25, 30);
  LcdTouch.println ("The bottle is empty");
  LcdTouch.setCursor (25, 50);
  LcdTouch.println ("you need to fill it");
  LcdTouch.drawButton(3, 90,  180, 150, 50, 10, BLACK, RED, "I filled", 2);
  rainbow(20);
  while (!LcdTouch.touched());
}
// הפונקציה מגדירה את הצבע של הלד לפי הצבע שהתקבל מהפונקציה WHEEL
void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// היא מקבלת מיקום על גלגל הצבעים
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0); // בונה את הצבע לפי אדום, ירוק וכחול
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
int distanceB()
{
  digitalWrite(trig,HIGH); // מעלים את הדק 8 ל "1" כדי לתת טריגר
  delayMicroseconds(10); // למשך 10 מיקרו שנייה
  digitalWrite(trig,LOW); // מורידים את הדק 8 ל "0"
  while(digitalRead(echo)==0); // מחכה עד שהאות האקו יהיה גבוה
  time=micros(); // מתעדת את הזמן עד שהאקו הפך לגבוה
  while(digitalRead(echo)==1); // מחכה עד שהאות יהפוך לנמוך-יחזור
  time=micros()-time; // נוריד מהזמן ששמרנו את הזמן שהוא שמר כדי לחשב את הזמן בהלוך וחזור
  distance= time/58; // חישוב המרחק בס"מ
  Serial.println(int(distance));
  return distance;
}

int temper()
{
  Serial.print("Current temp: ");
  Serial.print(sensor.temp()); // קריאה לפונקציה temp של האובייקט SENSOR שמחזירה את הפטמפרטורה הנוכחית
  Serial.println(" C");
  return sensor.temp();
}