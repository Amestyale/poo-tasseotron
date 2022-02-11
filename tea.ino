/*
++++++++++++++++++++++++++++++++++++++++++++
+++++                                  +++++
++++    ####  ###  ##### ##### #####    ++++
+++      ##  ## ## ##    ##    ##        +++
++       ##  ##### ##### ##### ###        ++
+        ##  #   #    ##    ## ##          +
+        ##  #   # ##### ##### #####       +
+                                          +
+                 #####                    +
+                 ## ##                    +
+        -O0O-    ## ##    -O0O-           +
+                 ## ##                    +
+                 #####                    +
+                                          +
+         #### ##### ##### ##  #           +
+          ##  ##  # ## ## ##  #           +
++         ##  ####  ## ## # # #          ++
+++        ##  ##  # ## ## #  ##         +++
++++       ##  ##  # ##### #  ##        ++++
+++++                                  +++++
++++++++++++++++++++++++++++++++++++++++++++
*/


#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <string> 
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define DHTPIN 33
#define DHTTYPE DHT11
#define USE_SERIAL Serial

DHT dht(DHTPIN, DHTTYPE);
WiFiMulti wifiMulti;

// IDENTIFIANTS WIFI
const char* WIFI_LOGIN = "##################";
const char* WIFI_PASSWORD = "##################";

// NOMBRE ET PINS DES LEDS DE LA JAUGE A LED
const int NB_JAUGE_LEDS =10;
const double JAUGE_LEDS[NB_JAUGE_LEDS] = {15,32,25, 26,27,14,16,17,18,19};

// ANALYSE DE LA TEMPERATURE
const float MIN_CELSIUS = 0;
const float MAX_CELSIUS = 50;
float max_temp_memory = 0.0;
float max_temp = 0.0;
float real_temp = false;

// NOMBRE DE LIGNES ET COLONNES DE L'ECRAN
const int NB_LCD_COLUMNS = 16;
const int NB_LCD_ROWS = 2;
const int PIN_LCD = 0x27;
// INSTANCIATION DE L'ECRAN
LiquidCrystal_I2C lcd(PIN_LCD, NB_LCD_COLUMNS, NB_LCD_ROWS);  

// CAPTEUR DE LUMIERE
const int PIN_LUM = 35;

// POTENTIOMETRE
const int PIN_BUTTON = 34;

// NOTIFICATION
const char* MSG_URL = "https://maker.ifttt.com/trigger/temp_goal/with/key/##################?value1=";
boolean msgSent = 0;

void setup(){
  Serial.begin(9600);
  init_pin();
  init_dht();
  init_lcd();
  verif_leds();
  init_wifi();  
  delay(3000);
  
  reset_screen();
}

void loop(){
  int goal = goaltemp();
  int lum = analogRead(PIN_LUM);
  float t = dht.readTemperature();
  Serial.print("t : ");
  Serial.println(t);
  screen(goal, lum);
  histo_temp(t);
  light_jauge(calc_percent_celsius(t));
  need_to_notify(lum, goal, t);
}

// !----- FONCTIONS DE L'ECRAN -----!
// Réinitialiser l'écran
void reset_screen(){
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

// Afficher la première ligne de l'écran
void identity_screen(){
  lcd.setCursor(0, 0);
  // print message ( ^^)_旦~~
  lcd.print("##Tasse-O-Tron##");
}

// Afficher la seconde ligne de l'écran (temperature souhaitée)
void screen(int goal, int lum){
  identity_screen();
  lcd.setCursor(4, 1);
  lcd.print(goal);
  lcd.setCursor(6, 1);
  lcd.print("C");

  lcd.setCursor(9, 1);
  lcd.print( (lum < 10) ? " ON" : "OFF" );
}

// Afficher la seconde ligne de l'écran (wifi connecté)
void screen_wifi_ok(){
  identity_screen();
  lcd.setCursor(2, 1);
  lcd.print("WIFI : OK !");
}
// !----- FIN DES FONCTIONS DE L'ECRAN -----!


// !----- FONCTIONS D'INITIALISATION -----!
void init_dht()
{
  dht.begin();
}

void init_wifi()
{
  wifiMulti.addAP(WIFI_LOGIN, WIFI_PASSWORD);
  while ( wifiMulti.run() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  screen_wifi_ok();
  Serial.println("");
  Serial.println("WiFi connecté");
  Serial.print("MAC : ");
  Serial.println(WiFi.macAddress());
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void init_lcd()
{
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}
void init_pin()
{
  for (int i = 0; i < NB_JAUGE_LEDS; i++ ){
    pinMode(JAUGE_LEDS[i], OUTPUT);
  }
}
// !----- FIN DES FONCTIONS D'INITIALISATION -----!


// !----- FONCTIONS DE VERIFICATION -----!
// VERIFICATION DE LA JAUGE DE LEDS
void verif_leds()
{
  for (int i = 0; i < NB_JAUGE_LEDS; i++ ){
    digitalWrite(JAUGE_LEDS[i], HIGH);
    delay(1200/NB_JAUGE_LEDS);
  }
  
  flash_jauge(5);
}

void need_to_notify(int lum, int goal, float temperature)
{
  if(lum<100){
    if(real_temp){
      max_temp_memory = temperature;
      if(temperature <= goal && msgSent == 0){
        Serial.println("!----------------------------------------!");
        Serial.println("Sending message");
        Serial.println("!----------------------------------------!");
        sendSlackMsg(goal); 
      }
    }
  } else {
    max_temp = false;
    real_temp = false;
  }
}
// !----- FIN DES FONCTIONS DE VERIFICATION -----!


// !----- FONCTIONS DE LECTURE & CALCUL -----!
// CALCUL DE POURCENTAGE, D'APRES TENSION BOUTON
float calc_percent(float value)
{
    float p = (value/4095.0)*100;
    return p;
}

// CALCUL DE POURCENTAGE, D'APRES TEMPERATURE CELSIUS
float calc_percent_celsius(float temperature)
{
    float v = temperature - MIN_CELSIUS;
    float d = MAX_CELSIUS - MIN_CELSIUS;
   float p = v * 100 / d;
   return p; 
}

// CALCUL DE LA TEMPERATURE SOUHAITEE
int goaltemp(){
  int val = analogRead(PIN_BUTTON);
  int goal = val / 100;
  return goal + 10;
}

// SAUVEGARDE DE LA TEMPERATURE
int histo_temp(float t){
  if(t>max_temp){
    max_temp = t;
  } else if(t < max_temp - 0.10 && !real_temp){
    if(t >= max_temp_memory + 1) msgSent = 0;
    max_temp_memory = t;
    real_temp = true;
        Serial.println("!----------------------------------------!");
        Serial.println("PIC ATTEINT");
        Serial.println(max_temp);
        Serial.println(t);
        Serial.println("!----------------------------------------!");
  }
}
// !----- FIN DES FONCTIONS DE CALCUL -----!


// !----- FONCTIONS D'AFFICHAGE -----!
// ALLULMAGE DE LA JAUGE D'APRES POURCENTAGE
void light_jauge(float percent)
{
  for (int i = 0; i < NB_JAUGE_LEDS; i++ ){
    double p = ((i+1.0)/NB_JAUGE_LEDS)*100.0;
    if(p < percent){
      digitalWrite(JAUGE_LEDS[i], HIGH);
    } else {
      digitalWrite(JAUGE_LEDS[i], LOW);
    }
  }
}

// OUTPUT DE L'ENTIERETE DE LA JAUGE
void full_jauge(int state)
{
  for (int i = 0; i < NB_JAUGE_LEDS; i++ ){
    digitalWrite(JAUGE_LEDS[i], state);
  } 
}

// FAIRE CLIGNOTER L'ENTIERETE DE LA JAUGE
void flash_jauge(int nb)
{
  for (int i = 0; i < nb; i++ ){
    full_jauge(HIGH);
    delay(500);
    full_jauge(LOW);
    delay(500);
  } 
}
// !----- FIN DES FONCTIONS D'AFFICHAGE -----!


// !----- FONCTIONS DE CONTACT AVEC L'EXTERIEUR -----!
void sendSlackMsg(int goal) {
    String value = String(goal);
    if((wifiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;
        http.begin(MSG_URL + value); //HTTP
        int httpCode = http.GET();

        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.print("(∩｀-´)⊃━☆ﾟ.*･｡ﾟ  ");
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    } else {
      USE_SERIAL.println("WIFI NOT CONNECTED");
    }
    msgSent = 1;
}
// !----- FIN DES FONCTIONS DE CONTACT AVEC L'EXTERIEUR -----!