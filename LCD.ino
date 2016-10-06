#include <LiquidCrystal.h>
#include <AD9850.h>  

double freq = 7.050;
long frequency = 1000000 * freq;
int phase = 0;
char buff[16];
long ch1=100;
// ~ char
byte customChar[8] = {
	0b00100,
	0b00100,
	0b11111,
	0b00100,
	0b00100,
	0b00000,
	0b11111,
	0b00000
};
//+- char
byte customChar2[8] = {
	0b00000,
	0b00000,
	0b01000,
	0b10100,
	0b10101,
	0b00101,
	0b00010,
	0b00000
};


// select the pins used on the LCD panel
LiquidCrystal lcd(7, 8, 3, 4, 5, 6);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 //Serial.println(adc_key_in);  //Debug
 if (adc_key_in > 800) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 
 if (adc_key_in < 20)   return btnRIGHT;  
 if (adc_key_in < 150)  return btnUP; 
 if (adc_key_in < 300)  return btnDOWN; 
 if (adc_key_in < 500)  return btnLEFT; 
 if (adc_key_in < 700)  return btnSELECT;  


 return btnNONE;  // when all others fail, return this...
}

void setup()
{
 Serial.begin(9600);  // control
  
  lcd.createChar(0, customChar);
  lcd.createChar(1, customChar2);
  
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("-=-  SV2RCK  -=-"); // print a simple message
 lcd.setCursor(0,1);
 lcd.print("SIGNAL GENERATOR");
 //lcd.blink();
 
 //DDS.begin(w_clk, fq_ud, data, reset);
 DDS.begin(9, 10, 11, 12);
 DDS.calibrate(125001600);
 
 delay(4000);
 lcd.clear();
}
 
void loop()
{
 delay(200);
 DDS.setfreq(frequency, phase);
 
 lcd.setCursor(0,0);            // Frequency
 lcd.write((uint8_t)1);
 lcd.print("  ");
 sprintf(buff, "%8ld", frequency);
 lcd.print(buff);      
 lcd.print(" Hz ");

 lcd.setCursor(0,1); // Step
 lcd.write((uint8_t)0);
 lcd.print("  ");
 sprintf(buff, "%8ld", ch1);
 lcd.print(buff);
 lcd.print(" Hz ");
 
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     //lcd.print("RIGHT ");
     if (ch1>1) {
     ch1=ch1/10;
     }
     break;
     }
   case btnLEFT:
     {
     //lcd.print("LEFT   ");
     if (ch1<10000000) {
       ch1=ch1*10;
     }
     break;
     }
   case btnUP:
     {
     //lcd.print("UP    ");
     if (frequency+ch1<125000000) {
     frequency=frequency+ch1;
     }
     break;
     }
   case btnDOWN:
     {
     //lcd.print("DOWN  ");
     if (frequency-ch1>100) {
     frequency=frequency-ch1;
     }
     break;
     }
   case btnSELECT:
     {
     //lcd.print("SELECT");
     frequency=1000000 * freq;
     break;
     }
     case btnNONE:
     {
     //lcd.print("NONE  ");
     break;
     }
 }

}
