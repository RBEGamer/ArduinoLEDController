

#ifdef USE_I2C_DISPLAY

#define I2C_DISPLAY_ROW 4
#define I2C_DISPLAY_COL 20
LiquidCrystal_I2C lcd(0x27, I2C_DISPLAY_COL, I2C_DISPLAY_ROW);
elapsedMillis timeElapsed; //declare global if you don't want it reset every time loop runs
// delay in milliseconds between blinks of the LED
unsigned int interval = 10000;
 bool bgs;
String line_content[4];


void print_i2c_disp(String _string, int _row, int _col);

 void i2c_display_setup(){
  lcd.begin();
 lcd.clear();

print_i2c_disp("LED NODE", 0, 0);




  }

void print_i2c_disp(String _string, int _row = 2, int _col = 0){
//hardcore clear line xD

line_content[_row] = _string;

 lcd.setCursor(0,_row);
lcd.println("                    "); //20x space

  //set curor to dest line
    lcd.setCursor(_col,_row);
    //print shit
  lcd.print(_string);
  enable_bg();
  //reset cursor
 lcd.setCursor(0,_row);
  }

void clear_i2c_display(){
    enable_bg();
   lcd.clear();
   lcd.setCursor(0,0);

   for(int i = 0; i < 4; i++){
    line_content[i] = "";
    }
  }

void enable_bg(){
      lcd.backlight();
        bgs = true;
         timeElapsed = 0; 
  }

  void i2c_display_loop(){
    
     if (timeElapsed > interval && bgs == true) 
  {       
    timeElapsed = 0;       // reset the counter to 0 so the counting starts over...
    bgs = false;
       lcd.noBacklight();
  }


#endif
    }
