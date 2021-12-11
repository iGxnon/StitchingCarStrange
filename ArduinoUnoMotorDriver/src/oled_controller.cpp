#include "Arduino.h"
#include <Wire.h>
#include "logo_data.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // 设置OLED宽度,单位:像素
#define SCREEN_HEIGHT 64 // 设置OLED高度,单位:像素
#define ROW_SIZE 5.5
#define TEXT_SIZE 1.5
#define ROW_CNT 8
#define ROW_CHAR_CNT 22

#define OLED_RESET 14

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int row = 0;

void OLED_INIT()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    display.clearDisplay();
    delay(2000);
    display.drawBitmap(0, 0, logo, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    display.display();
    delay(1500);
    display.clearDisplay();
    display.display();
}

void PrintSerial(String str)
{
    display.setTextColor(WHITE);

    display.setTextSize(TEXT_SIZE);

    display.setCursor(0, row * ROW_SIZE * TEXT_SIZE);
    display.print(str);

    Serial.println(str.length());
    row += (str.length() / ROW_CHAR_CNT + 1);
    if (row >= ROW_CNT)
    {
        row = 0;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print(str);
        row += (str.length() / ROW_CHAR_CNT + 1);
    }

    display.display();
}

void ShowIP(String ip)
{
    display.clearDisplay();

    display.setTextColor(WHITE);

    display.setTextSize(1.5);

    //设置光标位置
    display.setCursor(0, 0);
    display.print("WIFI Connected!");

    display.setCursor(0, 10);
    display.print("IP Address: " + ip);

    display.display();
}