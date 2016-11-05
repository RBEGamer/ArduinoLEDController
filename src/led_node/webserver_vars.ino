/*
 C 2016 Marcel Ochsendorf
 marcel.ochsendorf@gmail.com
 https://github.com/RBEGamer
 24.07.2016
*/


#ifdef ENABLE_WEBSERVER
    #define NAMELEN 32
#define VALUELEN 32
 //  #define ENABLE_HTML_FORM
    P(phead) = 
"<!DOCTYPE html><html><head>"
  "<title>RBN LED NODE CONTROL</title>"
"<meta http-equiv='content-type' content='text/html; charset=utf-8'>"
"<meta charset='utf-8'>" 
  "<link href='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/themes/base/jquery-ui.css' rel=stylesheet />"
  "<script src='http://ajax.googleapis.com/ajax/libs/jquery/1.6.4/jquery.min.js'></script>"
  "<script src='http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.16/jquery-ui.min.js'></script>"
"<style> body {  } #red, #green, #blue { margin: 10px; } #red { background: #f00; } #green { background: #0f0; } #blue { background: #00f; } </style>"
  "<script>"
    "function changeRGB(event, ui) { jQuery.ajaxSetup({timeout: 110}); /*not to DDoS the Arduino, you might have to change this to some threshold value that fits your setup*/ var id = $(this).attr('id'); if (id == 'red') $.post('/rgb', { red: ui.value } ); if (id == 'green') $.post('/rgb', { green: ui.value } ); if (id == 'blue') $.post('/rgb', { blue: ui.value } ); } "
    "$(document).ready(function(){ $('#red, #green, #blue').slider({min: 0, max:255, change:changeRGB, slide:changeRGB}); });"
    
    "function lv_send(lvid){"
    "var xmlHttp = new XMLHttpRequest();"
    "xmlHttp.onreadystatechange = function() {"
        "if (xmlHttp.readyState == 4 && xmlHttp.status == 200){"
        //"alert(xmlHttp.responseText);"
      "}"
    "};"
    "var get_cmd = '?lv_' + lvid + '=' + this.document.getElementById('lv_' + lvid).value;"
    "xmlHttp.open('GET', '/set_lv.html' + get_cmd, false);"
    "xmlHttp.send(null);"
"}"
  "</script>"
"<style>"
"html, body {"
"  background: #F2F2F2;"
" width: 100%;"
" height: 100%;"
" margin: 0px;"
" padding: 0px;"
" font-family: 'Verdana';"
" font-size: 16px;"
" color: #404040;"
" }"
"img {"
" border: 0px;"
"}"
"span.title {"
" display: block;"
" color: #000000;"
" font-size: 30px;"
"}"
"span.subtitle {"
" display: block;"
" color: #000000;"
" font-size: 20px;"
"}"
".sidebar {"
" background: #FFFFFF;"
" width: 250px;"
" min-height: 100%;"
" height: 100%;"
" height: auto;"
" position: fixed;"
" top: 0px;"
" left: 0px;"
" border-right: 1px solid #D8D8D8;"
"}"
".logo {"
" padding: 25px;"
" text-align: center;"
" border-bottom: 1px solid #D8D8D8;"
"}"
".menu {"
" padding: 25px 0px 25px 0px;"
" border-bottom: 1px solid #D8D8D8;"
"}"
".menu a {"
" padding: 15px 25px 15px 25px;"
" display: block;"
" color: #000000;"
" text-decoration: none;"
" transition: all 0.25s;"
"}"
".menu a:hover {"
" background: #0088CC;"
" color: #FFFFFF;"
"}"
".right {"
" margin-left: 250px;"
" padding: 50px;"
"}"
".content {"
" background: #FFFFFF;"
" padding: 25px;"
" border-radius: 5px;"
" border: 1px solid #D8D8D8;"
"}"
"</style>"
"</head>";

P(pstart)="<body style='font-size:62.5%;'>"
"<div class='sidebar'>"
"<div class='logo'>"
"<span class='title'>LED NODE INTERFACE</span>"
"<span class='subtitle'>- Backend -</span>"
"</div>"
"<div class='menu'>"
"<a href='index.html'>MAIN</a>"
//"<a href='settings.html'>SETTINGS</a>"
"</div>"
"</div>"
"<div class='right'>"
"<div class='content'>";
P(pend)="</div>"
"</div>"
"</body>"
"</html>";




// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
};
IPAddress ip(192, 168, 178, 199);
IPAddress myDns(192,168,178, 1);
IPAddress gateway(192, 168, 178, 1);
IPAddress subnet(255, 255, 255, 0);

#define PREFIX ""
WebServer webserver(PREFIX, 80);


void webserver_setup(){
   Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // initialize the Ethernet device not using DHCP:
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print(".");

#ifdef USE_I2C_DISPLAY
      enable_bg();
    lcd.print(ip[thisByte], DEC);
    lcd.print(".");
 #endif

    
  }
  Serial.println();
  // start listening for clients


  webserver.setDefaultCommand(&indexCmd);

  /* run the same command if you try to load /index.html, a common
   * default page name */
  webserver.addCommand("index.html", &indexCmd);
   webserver.addCommand("set_lv.html", &led_value_Cmd);
    webserver.addCommand("config.xml", &config_Cmd);


webserver.addCommand("api", &api_Cmd);
  /* start the webserver */
  webserver.begin();
  }


void api_Cmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
    server.httpSuccess();

if (type != WebServer::HEAD)
  {
    server.println("<html><head></head><body>USE WITH ?lv_0=245&lv_3=333</body></html>");
    }

if (type == WebServer::GET)
  {

 URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];


if (strlen(url_tail))
    {
    //server.printP(Parsed_tail_begin);
    //Serial.println("123");
    //Serial.println(url_tail);
    while (strlen(url_tail))
      {
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      Serial.println(name);
      if (rc == URLPARAM_EOS){
//        server.printP(Params_end);
      }else
        {
        //server.print(name);
       // server.printP(Parsed_item_separator);
       // server.print(value);
       // server.printP(Tail_end);
   
        if (strcmp(name, "all") == 0)
        {
          set_all_value(strtoul(value, NULL, 10));
         }


      for(int i = 0; i <pca_allchip_channel_count; i++){
        String search = "lv_" + String(i);
        if (strcmp(name, search.c_str()) == 0)
        {
          pca_set_value(i,strtoul(value, NULL, 10));


          #ifdef USE_I2C_DISPLAY
           lcd.setCursor(0,2); //Start at character 4 on line 0
          lcd.print("Level " + String(i) + " to " + value);
          enable_bg();
          #endif
          
        }//ende if
      }//ende for
      
        
        }
      }
    }

    

  }//ende if
  }


void config_Cmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  server.httpSuccess();

  if (type != WebServer::HEAD)
  {
    server.print("<xml version='1.0'>");
    server.print("<config>");
    server.print("<sum_channels value='"+ String(pca_allchip_channel_count) +"' />");
    server.print("<connected_pca value='"+ String(pca_count_pcas) +"' />");
    server.print("<pca_user_steps value='"+ String(pca_pwm_user_steps) +"' />");
     server.print("<led_relais_state value='"+ String(led_relais_state) +"' />");
     server.print("<channel_start_index value='"+ String(pca_channel_start_index) +"' />");
     server.print("<channel_end_index value='"+ String(pca_channel_end_index) +"' />");
     server.print("request_url value='set_lv.html' />");
     server.print("request_get_param_name value='lv_' />");
    server.println("</config>");
    }
    
}

void led_value_Cmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
    server.httpSuccess();

if (type != WebServer::HEAD)
  {
    server.println("<html><head></head><body>OK</body></html>");
    }

if (type == WebServer::GET)
  {

 URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];


if (strlen(url_tail))
    {
    //server.printP(Parsed_tail_begin);
    //Serial.println("123");
    //Serial.println(url_tail);
    while (strlen(url_tail))
      {
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      Serial.println(name);
      if (rc == URLPARAM_EOS){
//        server.printP(Params_end);
      }else
        {
        //server.print(name);
       // server.printP(Parsed_item_separator);
       // server.print(value);
       // server.printP(Tail_end);

      for(int i = 0; i <pca_allchip_channel_count; i++){

String searcha = "all";
        if (strcmp(name, searcha.c_str()) == 0)
        {
          pca_set_value(i,strtoul(value, NULL, 10));


          #ifdef USE_I2C_DISPLAY
           lcd.setCursor(0,2); //Start at character 4 on line 0
          lcd.print("Level ALL" + String(i) + " to " + String(value));
          enable_bg();
          #endif
          break;
        }//ende if


        
        String search = "lv_" + String(i);
        if (strcmp(name, search.c_str()) == 0)
        {
          pca_set_value(i,strtoul(value, NULL, 10));


          #ifdef USE_I2C_DISPLAY
           lcd.setCursor(0,2); //Start at character 4 on line 0
          lcd.print("Level LV" + String(i) + " to " + String(value));
          enable_bg();
          #endif
          
        }//ende if



                String searchc = "lvc_" + String(i);
        if (strcmp(name, searchc.c_str()) == 0)
        {
          pca_set_value_current(i,strtoul(value, NULL, 10));


          #ifdef USE_I2C_DISPLAY
           lcd.setCursor(0,2); //Start at character 4 on line 0
          lcd.print("Level LVC" + String(i) + " to " + String(value));
          enable_bg();
          #endif
          
        }//ende if
      }//ende for
      
        
        }
      }
    }

    

  }//ende if
  }


void indexCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();


if (type == WebServer::GET)
  {

 URLPARAM_RESULT rc;
  char name[NAMELEN];
  char value[VALUELEN];


if (strlen(url_tail))
    {
    //server.printP(Parsed_tail_begin);
    //Serial.println("123");
    //Serial.println(url_tail);
    while (strlen(url_tail))
      {
      rc = server.nextURLparam(&url_tail, name, NAMELEN, value, VALUELEN);
      Serial.println(name);
      if (rc == URLPARAM_EOS){
//        server.printP(Params_end);
      }else
        {
        //server.print(name);
       // server.printP(Parsed_item_separator);
       // server.print(value);
       // server.printP(Tail_end);

    



      for(int i = 0; i <pca_allchip_channel_count; i++){
        String search = "lv_" + String(i);
        if (strcmp(name, search.c_str()) == 0)
        {
          pca_set_value(i,strtoul(value, NULL, 10));


          #ifdef USE_I2C_DISPLAY
          lcd.setCursor(0,2); //Start at character 4 on line 0
          lcd.print("Level LV" + String(i) + " to " + String(value));
          
          enable_bg();
          #endif
          
        }//ende if



                String search_c = "lvc_" + String(i);
        if (strcmp(name, search_c.c_str()) == 0)
        {
          pca_set_value_current(i,strtoul(value, NULL, 10));


          #ifdef USE_I2C_DISPLAY
          lcd.setCursor(0,2); //Start at character 4 on line 0
          lcd.print("Level LVC" + String(i) + " to " + String(value));
          
          enable_bg();
          #endif
          
        }//ende if

        
      }//ende for
      
        
        }
      }
    }

    

  }//ende if


  
if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */
    
    /* this is a special form of print that outputs from PROGMEM */
    server.printP(phead);


  
     server.printP(pstart);
     //DRAW HERE FORM FOR ALL CHANNELS


server.println("<h2>CONFIG</h2><br><ul>");
server.println("<li>CONNECTED PCA CONTROLLER :" + String(pca_count_pcas) + "</li>");
server.println("<li>SUM PWM CHANNELS :" + String(pca_allchip_channel_count) + "</li>");
server.println("<li>PWM STEPS :" + String(pca_pwm_user_steps) + "</li>");
server.println("<li>PWM MULTIPLIER :" + String(pca_pwm_multiplier) + "</li>");
server.println("<li>API (NORMAL): /set_lv.html?lv_CHANNEL_ID=VALUE&lv_CHANNEL_ID=VALUE</li>");
server.println("<li>API (CURRENT): /set_lv.html?lvc_CHANNEL_ID=VALUE&lvc_CHANNEL_ID=VALUE</li>");
server.println("<li>API (ALL): /set_lv.html?all=VALUE</li>");
server.println("</ul><br><h2>Channels</h2>");
     //ADD FORM
for(int i = 0; i < pca_allchip_channel_count; i++){
  #ifdef ENABLE_HTML_FORM
  server.print("<form action='set_lv.html' method='get'>");
  #endif
  server.print("Channel " + String(i) +" : <input onchange='lv_send(" + String(i) +");' id='lv_" + String(i) +"' name='lv_" + String(i) +"' value='" + String(drivers_value_set[i]) +"' type='range' min='0' max='" + String(pca_pwm_user_steps) +"' step='1' />");
 #ifdef ENABLE_HTML_FORM
  server.print("<button type='sumbit'> OK </button>");
  server.println("</form>");
  #endif
  server.println("<br />");
}



     server.printP(pend);
  }
}


void webserver_loop(){
   char buff[64];
  int len = 64;

  /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len);
  
  }


  #endif
