#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>
#include "InputDebounce.h"

#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

static InputDebounce buttonTest1; // not enabled yet, setup has to be called later
static InputDebounce buttonTest2; // not enabled yet, setup has to be called later
static InputDebounce buttonTest3; // not enabled yet, setup has to be called later
static InputDebounce buttonTest4; // not enabled yet, setup has to be called later
static InputDebounce buttonTest5; // not enabled yet, setup has to be called later


#ifdef ESP32
  #include <Arduino.h>
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

// Set receive and transmit pin numbers (GDO0 and GDO2)
#ifdef ESP32 // for esp32! Receiver on GPIO pin 4. Transmit on GPIO pin 2.
  #define RX_PIN 4 
  #define TX_PIN 2
#elif ESP8266  // for esp8266! Receiver on pin 4 = D2. Transmit on pin 5 = D1.
  #define RX_PIN 4
  #define TX_PIN 5
#else // for Arduino! Receiver on interrupt 0 => that is pin #2. Transmit on pin 6.
  #define RX_PIN 0
  #define TX_PIN 6
#endif 

// Set CC1101 frequency
// Harbor Breeze Remote: Model TR214B FCC# KUJCE10712
#define FREQUENCY     304.25

// RC-switch settings
#define RF_PROTOCOL 6
#define RF_REPEATS  4

RCSwitch mySwitch = RCSwitch();


int count;
void setup() {
  Serial.begin(115200);
  count = 0;

  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  print_GPIO_wake_up();
  
  uint64_t mask = 1 << 12 | 1 << 13 | 1 << 14 | 1 << 26 | 1 << 15;
   
  esp_sleep_enable_ext1_wakeup(mask,ESP_EXT1_WAKEUP_ANY_HIGH);

  buttonTest1.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback, buttonTest_releasedDurationCallback);
  buttonTest2.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback, buttonTest_releasedDurationCallback);
  buttonTest3.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback, buttonTest_releasedDurationCallback);
  buttonTest4.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback, buttonTest_releasedDurationCallback);
  buttonTest5.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback, buttonTest_releasedDurationCallback);

  buttonTest1.setup(15, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_DOWN_RES, 0);
  buttonTest2.setup(14, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_DOWN_RES, 0);
  buttonTest3.setup(13, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_DOWN_RES, 0);
  buttonTest4.setup(12, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_DOWN_RES, 0);
  buttonTest5.setup(26, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_DOWN_RES, 0);

  ELECHOUSE_cc1101.setSpiPin(18, 19, 23, 17); // Set custom pins: (SCK, MISO, MOSI, CSN). Must be BEFORE Init()!
  ELECHOUSE_cc1101.setGDO(2, 4); // Set custom GDO pins: (GDO0, GDO2). Gdo0 (tx) and Gdo2 (rx). Must be BEFORE Init();
  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.setMHZ(FREQUENCY);

  ELECHOUSE_cc1101.SetTx();               // set Transmit on
  mySwitch.disableReceive();              // Receiver off
  mySwitch.enableTransmit(TX_PIN);        // Transmit on
  mySwitch.setRepeatTransmit(RF_REPEATS); // transmission repetitions.
  mySwitch.setPulseLength(447);
  mySwitch.setProtocol(RF_PROTOCOL);      // send Received Protocol

}

void buttonTest_pressedCallback(uint8_t pinIn)
{
  // handle pressed state
  Serial.print("HIGH (pin: ");
  Serial.print(pinIn);
  Serial.println(")");
}

void buttonTest_releasedCallback(uint8_t pinIn)
{
  // handle released state
  Serial.print("LOW (pin: ");
  Serial.print(pinIn);
  Serial.println(")");

}

void buttonTest_pressedDurationCallback(uint8_t pinIn, unsigned long duration)
{
  // handle still pressed state
  Serial.print("HIGH (pin: ");
  Serial.print(pinIn);
  Serial.print(") still pressed, duration ");
  Serial.print(duration);
  Serial.println("ms");


  // Light
  if (pinIn == 12){
    Serial.println("Light");
    //mySwitch.send("100011010101111110100000"); // FAN_A
    mySwitch.send("101011011101101110100000"); // FAN_B
  }
  // Fan Off
  if (pinIn == 26){
    Serial.println("Fan Off");
    //mySwitch.send("100011010101111100100000"); // FAN_A
    mySwitch.send("101011011101101100100000"); // FAN_B
  }
  // Fan Low
  if (pinIn == 13){
    Serial.println("Fan Low");
    //mySwitch.send("100011010101111101000000"); // FAN_A
    mySwitch.send("101011011101101101000000"); // FAN_B
  }
  // Fan Med
  if (pinIn == 14){
    Serial.println("Fan Med");
    //mySwitch.send("100011010101111110000000"); // FAN_A
    mySwitch.send("101011011101101110000000"); // FAN_B
  }
  // Fan High
  if (pinIn == 15){
    Serial.println("Fan High");
    //mySwitch.send("100011010101111111000000"); // FAN_A
    mySwitch.send("101011011101101111000000"); // FAN_B
  }
  
  //delay(55);
}

void buttonTest_releasedDurationCallback(uint8_t pinIn, unsigned long duration)
{
  // handle released state
  Serial.print("LOW (pin: ");
  Serial.print(pinIn);
  Serial.print("), duration ");
  Serial.print(duration);
  Serial.println("ms");
}

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

/*
Method to print the GPIO that triggered the wakeup
*/
void print_GPIO_wake_up(){
  uint64_t GPIO_reason = esp_sleep_get_ext1_wakeup_status();
  Serial.print("GPIO that triggered the wake up: GPIO ");
  Serial.println((log(GPIO_reason))/log(2), 0);
}

void loop() {
  count ++;
  unsigned long now = millis();

  buttonTest1.process(now); // Fan high
  buttonTest2.process(now);
  buttonTest3.process(now);
  buttonTest4.process(now);
  buttonTest5.process(now);
  delay(1);
  if (count > 10000){
    //Go to sleep now
    Serial.println("Going to sleep now");
    delay(10);
    esp_deep_sleep_start();
  }
}
