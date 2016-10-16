/*
 C 2016 Marcel Ochsendorf
 marcel.ochsendorf@gmail.com
 https://github.com/RBEGamer
 24.07.2016
*/



#define PIN_CONFIG_LED_RELAIS 2
//#define PIN_CONFIG_LED_RELAIS_INVERT
#define pca_enable_fade
#define pca_fade_time_timetick 40
#define pca_fade_corretion_execute 50
#define pca_count_pcas 2
#define pca_channel_count 16
#define pca_pwm_max_value 4096
#define pca_pwm_user_steps 255
#define pca_allchip_channel_count (pca_channel_count*pca_count_pcas)
#define pca_pwm_multiplier (pca_pwm_max_value/pca_pwm_user_steps)
#define pca_max_channel_sum (pca_channel_count*pca_count_pcas*pca_pwm_user_steps)
#define pca_channel_start_index 0
#define pca_channel_end_index (pca_max_channel_sum-1)
PCA9685* drivers[pca_count_pcas];

unsigned int drivers_value_set[pca_allchip_channel_count];
unsigned int drivers_value_curr[pca_allchip_channel_count];
unsigned int channel_value_sum = 0; //holds a sum of all channel values

int led_relais_state = 0;
void setup_pcas(){

pinMode(PIN_CONFIG_LED_RELAIS, OUTPUT);
  
channel_value_sum = 0;



  for(int i = 0; i < pca_count_pcas; i++){
    drivers[i] = 0;
    drivers[i] = new PCA9685(0x0 + i, PCA9685_MODE_N_DRIVER, 800.0);
    if(drivers[i] == 0){
      Serial.println("pca setup failed is the pca conntected ?");
      }
    drivers[i]->setup();
  }


 for(int i = 0; i < pca_allchip_channel_count; i++){
drivers_value_set[i] = 0;
drivers_value_curr[i] = 0;

 int pca_offset = i / pca_channel_count;
    int channel_offset = i % pca_channel_count;
  drivers[pca_offset]->getPin(channel_offset).setValueAndWrite(0);
    
 }
disable_led_relais();



}

elapsedMillis timeElapsed_pca;

void loop_pcas(){


#ifdef pca_enable_fade
if(timeElapsed_pca > pca_fade_time_timetick){
  timeElapsed_pca = 0;
  check_led_relais_state();
}else{
  return;
  }
  for(int i = 0; i < pca_allchip_channel_count; i++){
    int pca_offset = i / pca_channel_count;
    int channel_offset = i % pca_channel_count;
     if(  drivers_value_curr[i] != drivers_value_set[i]){
      int ect_times = pca_fade_corretion_execute;
      ect_times = drivers_value_set[i] -  drivers_value_curr[i];
      
if(ect_times < 0){
  ect_times = -ect_times;
  }
      ect_times = pca_pwm_max_value - ect_times;
      ect_times++;
      ect_times = ect_times /10;
      if(ect_times < pca_fade_corretion_execute){
        ect_times = pca_fade_corretion_execute;
        }
       // Serial.println(ect_times);
      for(int j = 0; j < ect_times; j++){
          if( drivers_value_curr[i] > drivers_value_set[i]){drivers_value_curr[i]--;}
          else if( drivers_value_curr[i] < drivers_value_set[i]){drivers_value_curr[i]++;}
          else{break;}
      }
     drivers[pca_offset]->getPin(channel_offset).setValueAndWrite(drivers_value_curr[i]);
     }
  }
#endif
}





void set_all_on(){
   for(int i = 0; i < pca_allchip_channel_count; i++){
    pca_set_value(i,pca_pwm_user_steps);
    }
}

void set_all_off(){
    for(int i = 0; i < pca_allchip_channel_count; i++){
    pca_set_value(i,0);
    }
}

void set_all_value(int _value){
    for(int i = 0; i < pca_allchip_channel_count; i++){
    pca_set_value(i,_value);
    }
}



void enable_led_relais(){
  #ifdef PIN_CONFIG_LED_RELAIS_INVERT
  digitalWrite(PIN_CONFIG_LED_RELAIS, HIGH);
  #else
  digitalWrite(PIN_CONFIG_LED_RELAIS, LOW);
  #endif
  led_relais_state = 1;
}

void disable_led_relais(){
   #ifdef PIN_CONFIG_LED_RELAIS_INVERT 
  digitalWrite(PIN_CONFIG_LED_RELAIS, LOW);
  #else
  digitalWrite(PIN_CONFIG_LED_RELAIS, HIGH);
  #endif
led_relais_state = 0;
}






void check_led_relais_state(){
      channel_value_sum = 0;
      for(int i = 0; i < pca_allchip_channel_count; i++){
      channel_value_sum += drivers_value_curr[i];
      }
      if(channel_value_sum > 0){
      enable_led_relais();
      }else{
      disable_led_relais();
      }
  }

void pca_set_value(unsigned int _channel, int _value){
  if(_channel > pca_allchip_channel_count){
    return;
    }
    if(_value > pca_pwm_user_steps){
      _value = pca_pwm_user_steps;
      }

      if(_value < 0){_value = 0;}

drivers_value_set[_channel] = _value * pca_pwm_multiplier;      
#ifdef pca_enable_fade
     #else
     int pca_offset = _channel / pca_channel_count;
      int channel_offset = _channel % pca_channel_count;
      drivers_value_curr[_channel] = drivers_value_set[_channel];
     drivers[pca_offset]->getPin(channel_offset).setValueAndWrite(drivers_value_set[_channel]);
     #endif 


     


    check_led_relais_state();
      
  }



  void pca_set_value_current(unsigned int _channel, int _value){
    Serial.println("set lvc");
  if(_channel > pca_allchip_channel_count){
    return;
    }
    if(_value > pca_pwm_user_steps){
      _value = pca_pwm_user_steps;
      }

      if(_value < 0){_value = 0;}

drivers_value_curr[_channel] = _value * pca_pwm_multiplier;      
#ifdef pca_enable_fade
     #else
     int pca_offset = _channel / pca_channel_count;
      int channel_offset = _channel % pca_channel_count;
     // drivers_value_curr[_channel] = drivers_value_set[_channel];
     drivers[pca_offset]->getPin(channel_offset).setValueAndWrite(drivers_value_curr[_channel]);
     #endif 


     


    check_led_relais_state();
      
  }
