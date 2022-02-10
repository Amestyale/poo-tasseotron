
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
const char* WIFI_LOGIN = "ADE-WIFI-ETUDIANTS";
const char* WIFI_PASSWORD = "wifi@ade2018";

// NOMBRE ET PINS DES LEDS DE LA JAUGE A LED
const int NB_JAUGE_LEDS =10;
const double JAUGE_LEDS[NB_JAUGE_LEDS] = {15,32,25, 26,27,14,16,17,18,19};

// ANALYSE DE LA TEMPERATURE
const float MIN_CELSIUS = 10;
const float MAX_CELSIUS = 50;
const int HISTORIC_CELSIUS_SIZE = 10;
float historic_celsius[HISTORIC_CELSIUS_SIZE] = {0,0,0,0,0,0,0,0,0,0};

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

// NOMBRE D'ITERATIONS DURANT LESQUELS LA TASSE EST POSEE
int scdPosed = 0;

// EST-CE QUE LE MESSAGE A DEJA ETE ENVOYE
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
  
  screen(goal, lum);
  histo_temp(t);
  light_jauge(calc_percent_celsius(t));
  need_to_notify(lum, goal, t);
  delay(1000);
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
  lcd.print( (lum < 100) ? " ON" : "OFF" );
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
    delay(500);
  }
  
  flash_jauge(5);
}

void need_to_notify(int lum, int goal, float temperature)
{
  if(lum<100){
    scdPosed += 1;
    Serial.println("TASS-O-TRON RECOUVERT ʕ•3•ʔ");
    if(scdPosed > 10 &&  temperature <= goal && msgSent == 0){
      Serial.println("!----------------------------------------!");
      Serial.println("Sending message");
      Serial.println("!----------------------------------------!");
      sendSlackMsg(goal); 
    }
  } else {
    if(scdPosed > 0)Serial.println("TASS-O-TRON PRIVÉ DE SA TASSE ʕ•`3•´ʔ ");
    scdPosed = 0;
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
  Serial.println();
  for(int i = 1; i < HISTORIC_CELSIUS_SIZE; i++){
    historic_celsius[i-1] = historic_celsius[i];
    Serial.print(historic_celsius[i-1]);
    Serial.print(" | ");
  }
  historic_celsius[HISTORIC_CELSIUS_SIZE-1] = t;
  Serial.println();
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
        http.begin("https://maker.ifttt.com/trigger/temp_goal/with/key/Na-nqM2lU1A6ZTH8QZq42?value1=" + value); //HTTP
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