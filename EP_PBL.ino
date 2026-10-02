// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64

// #define OLED_SDA 21
// #define OLED_SCL 22

// #define IR_SENSOR 18
// #define BUZZER 27

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// const int TOTAL_DROPS = 10000;
// const unsigned long BLOCK_TIME = 15000;

// int dropCount = 0;
// bool lastState = HIGH;

// unsigned long startTime;
// unsigned long lastDropTime;

// float dripRate = 0;
// float remainingMinutes = 0;

// void showDisplay()
// {
//   display.clearDisplay();

//   display.setTextColor(SSD1306_WHITE);

//   display.setTextSize(2);
//   display.setCursor(0,0);
//   display.println("IV Guard");

//   display.drawLine(0,18,127,18,SSD1306_WHITE);

//   display.setTextSize(1);

//   display.setCursor(0,24);
//   display.print("Drops:");
//   display.println(dropCount);

//   display.setCursor(0,36);
//   display.print("Rate:");
//   display.print((int)dripRate);
//   display.println(" dpm");

//   int hr = (int)remainingMinutes / 60;
//   int min = (int)remainingMinutes % 60;

//   display.setCursor(0,48);
//   display.print("Time:");
//   display.print(hr);
//   display.print("h ");
//   display.print(min);
//   display.print("m");

//   display.display();
// }

// void showBlocked()
// {
//   display.clearDisplay();

//   display.setTextColor(SSD1306_WHITE);

//   display.setTextSize(2);
//   display.setCursor(0,0);
//   display.println("WARNING");

//   display.drawLine(0,20,127,20,SSD1306_WHITE);

//   display.setCursor(0,40);
//   display.println("BLOCKED");

//   display.display();
// }

// void setup()
// {
//   Serial.begin(115200);

//   pinMode(IR_SENSOR, INPUT);
//   pinMode(BUZZER, OUTPUT);

//   Wire.begin(OLED_SDA, OLED_SCL);

//   if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
//   {
//     Serial.println("OLED Failed");
//     while(true);
//   }

//   display.clearDisplay();
//   display.setTextColor(SSD1306_WHITE);

//   display.setTextSize(2);
//   display.setCursor(0,20);
//   display.println("IV Guardian");
//   display.display();

//   delay(2000);

//   startTime = millis();
//   lastDropTime = millis();

//   showDisplay();
// }

// void loop()
// {
//   bool state = digitalRead(IR_SENSOR);

//   if(lastState==HIGH && state==LOW)
//   {
//     dropCount++;

//     lastDropTime=millis();

//     tone(BUZZER,1000);
//     delay(50);
//     noTone(BUZZER);

//     unsigned long elapsed=millis()-startTime;

//     if(elapsed>0)
//     {
//       dripRate=(dropCount*60000.0)/elapsed;

//       if(dripRate<1)
//         dripRate=1;

//       remainingMinutes=(TOTAL_DROPS-dropCount)/dripRate;
//     }

//     showDisplay();

//     delay(150);
//   }

//   lastState=state;

//   if(millis()-lastDropTime>BLOCK_TIME)
//   {
//     tone(BUZZER,1500);
//     showBlocked();
//   }
//   else
//   {
//     noTone(BUZZER);
//   }
// }





#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

#define IR_SENSOR 19
#define BUZZER 27

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------------- SETTINGS ----------------

const int TOTAL_DROPS = 10000;

const int MIN_DRIP_RATE = 20;
const int MAX_DRIP_RATE = 60;

const unsigned long BLOCK_TIME = 15000;

// ------------------------------------------

int dropCount = 0;

bool lastState = HIGH;

unsigned long startTime;
unsigned long lastDropTime;

float dripRate = 0;
float remainingMinutes = 0;


// =================================================
// NORMAL DASHBOARD
// =================================================

void showDisplay()
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("IV Guard");

  display.drawLine(0, 18, 127, 18, SSD1306_WHITE);

  display.setTextSize(1);

  // Drop count
  display.setCursor(0, 24);
  display.print("Drops:");
  display.println(dropCount);

  // Drip rate
  display.setCursor(0, 36);
  display.print("Rate:");
  display.print((int)dripRate);
  display.println(" dpm");

  // Remaining time
  display.setCursor(0, 48);
  display.print("Time:");

  if (dripRate > 0)
  {
    int hours = (int)remainingMinutes / 60;
    int minutes = (int)remainingMinutes % 60;

    display.print(hours);
    display.print("h ");
    display.print(minutes);
    display.print("m");
  }
  else
  {
    display.print("--");
  }

  display.display();
}


// =================================================
// SLOW WARNING
// =================================================

void showSlowWarning()
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("WARNING");

  display.drawLine(0, 20, 127, 20, SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 30);
  display.println("FLOW TOO SLOW");

  display.setCursor(0, 45);
  display.print("Rate:");
  display.print((int)dripRate);
  display.println(" dpm");

  display.display();
}


// =================================================
// FAST WARNING
// =================================================

void showFastWarning()
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("WARNING");

  display.drawLine(0, 20, 127, 20, SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 30);
  display.println("FLOW TOO FAST");

  display.setCursor(0, 45);
  display.print("Rate:");
  display.print((int)dripRate);
  display.println(" dpm");

  display.display();
}


// =================================================
// BLOCKED
// =================================================

void showBlocked()
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("WARNING");

  display.drawLine(0, 20, 127, 20, SSD1306_WHITE);

  display.setCursor(0, 40);
  display.println("BLOCKED");

  display.display();
}


// =================================================
// SETUP
// =================================================

void setup()
{
  Serial.begin(115200);

  pinMode(IR_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED Failed");

    while (true);
  }

  // Startup screen
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println("IV Guardian");

  display.display();

  delay(2000);

  startTime = millis();
  lastDropTime = millis();

  showDisplay();
}


// =================================================
// LOOP
// =================================================

void loop()
{
  bool state = digitalRead(IR_SENSOR);


  // =================================================
  // DROP DETECTED
  // =================================================

  if (lastState == HIGH && state == LOW)
  {
    dropCount++;

    lastDropTime = millis();


    // -----------------------------------------------
    // CALCULATE RATE
    // -----------------------------------------------

    unsigned long elapsed =
        millis() - startTime;

    if (elapsed > 0)
    {
      // Average drip rate
      dripRate =
          (dropCount * 60000.0) / elapsed;

      if (dripRate < 1)
      {
        dripRate = 1;
      }

      // Remaining time
      remainingMinutes =
          (TOTAL_DROPS - dropCount) / dripRate;
    }


    // -----------------------------------------------
    // CHECK THE SAME RATE SHOWN ON OLED
    // -----------------------------------------------

    Serial.print("Drops: ");
    Serial.print(dropCount);

    Serial.print(" | Rate: ");
    Serial.print((int)dripRate);

    Serial.println(" dpm");


    // RATE BELOW 20
    if (dripRate < MIN_DRIP_RATE)
    {
      Serial.println("FLOW TOO SLOW");

      tone(BUZZER, 1800);

      showSlowWarning();
    }


    // RATE ABOVE 60
    else if (dripRate > MAX_DRIP_RATE)
    {
      Serial.println("FLOW TOO FAST");

      tone(BUZZER, 1800);

      showFastWarning();
    }


    // RATE BETWEEN 20 AND 60
    else
    {
      Serial.println("FLOW NORMAL");

      noTone(BUZZER);

      showDisplay();
    }


    delay(150);
  }


  lastState = state;


  // =================================================
  // CHECK BLOCKAGE
  // =================================================

  if (millis() - lastDropTime > BLOCK_TIME)
  {
    tone(BUZZER, 1500);

    showBlocked();
  }
}

