
#include <U8g2lib.h>

//This project used dual OLED displays. An I2C 1.3" OLED Display and an SPI 1.5" OLED Display.You can use a single display with smaller font to display all values
//1.3" I2C OLED display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//1.5" SPI OLED Display
U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI u8g2_2(U8G2_R0, /* cs=*/ 05, /* dc=*/ 2, /* reset=*/ 16);  



int voltsensorPin = 15;    
int currentsensorPin = 4;      
int voltsensorValue = 0;  // variable to store the value coming from the sensor
int currentsensorValue = 0;  // variable to store the value coming from the sensor
float voltagefactor=10.42/0.96; //use actual values as measured by multimeter to avoid error from resistor tolerance
float currentfactor=0.02;//20mA/volt for ACS 758 100ECB
float nominalvoltage=0;  //nominal voltage measured at zero current from ACS 758. This value will be subtracted from measured voltage to calculate current
float voltage=0;  //voltage measured at ACS758



void setup() {
  Serial.begin(115200);
  int sum=0;
  for(int x=0;x<500;x++)
  
  {
    sum=sum+analogRead(currentsensorPin);
  }
  sum=sum/500; //Current sensor out put has high variance. To reduce this variance, average of a lot of readings is used
 nominalvoltage = 3.3*(float(sum)/4095);//ESP32 has 12 bit ADC thus 4095 max value and this is corresponding to 3.3v
  
    u8g2.setBusClock(100000); //important to set bus clock at 1MHz otherwise other devices on I2C bus may not work
  u8g2.begin();
  u8g2_2.begin();

   u8g2.setFont(u8g2_font_9x15_mr);  // choose a suitable font
   u8g2_2.setFont(u8g2_font_9x15_mr);  // choose a suitable font
   //u8g2_2.setFont(u8g2_font_9x15_mr);  // choose a suitable font
 // u8g2.setFont(u8g2_font_6x12_mr);  // choose a suitable font
 }

void loop() {
  // read the value from the sensor:
   voltsensorValue = analogRead(voltsensorPin);

   int sum=0;
  for(int x=0;x<500;x++)
  
  {
    sum=sum+analogRead(currentsensorPin);
  }
 currentsensorValue=sum/500;
  
voltage=3.3*float(currentsensorValue)/4095;
  
  u8g2.clearBuffer();  // clear the internal memory
 u8g2.drawStr(2,22,"Current(A)"); 

    u8g2.setCursor(02,44);
  u8g2.print(String((voltage-nominalvoltage)/currentfactor));
   u8g2.setCursor(02,63);
  u8g2.print(String("Nom Volt= "));
  u8g2.setCursor(84,63);
  u8g2.print(String(nominalvoltage));
  u8g2.sendBuffer();          // transfer internal memory to the display
   u8g2_2.clearBuffer();  // clear the internal memory
  u8g2_2.drawStr(2,22,"Voltage"); // write something to the internal memory

    u8g2_2.setCursor(02,44);
  u8g2_2.print(String(voltagefactor*3.3*(float(voltsensorValue)/4095)));
  u8g2_2.sendBuffer();          // transfer internal memory to the display
  
  delay(100);
 
}
