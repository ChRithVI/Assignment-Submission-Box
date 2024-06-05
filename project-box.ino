#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM900A(12, 13); // SoftSerial( RX , TX );

#define IR_SENSOR_PIN 11
#define BUTTON_PIN 3
#define TRIG_PIN 7
#define ECHO_PIN 6
#define SERVO_PIN 5

int count_value = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
int buttonState = HIGH;  // current state of the button
int lastButtonState = HIGH;  // previous state of the button
Servo myservo;

void setup()
{
  myservo.attach(SERVO_PIN);
  Serial.begin(9600);
  SIM900A.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // use INPUT_PULLUP to enable internal pull-up resistor
}


void loop() {

  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != lastButtonState) {
    delay(50);  // debounce delay
    if (buttonState == LOW) {  // Button is pressed
      count_value = 0;  // Reset the count
      lcd.clear();  // Clear the LCD display
      SIM900A.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode
      SIM900A.println("AT+CMGS=\"+94766238671\"\r"); //Type Your Mobile number to send message
      SIM900A.println("submitted count = " + String(count_value));// Messsage content
      count_value = 0;  // Reset the count

      myservo.write(0);              // tell servo to go to position in variable 'pos'


    }
  }
  lastButtonState = buttonState;




  long duration, cm;
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);
  cm = microsecondsToCentimeters(duration);


  if (cm > 20) {
    {
      int val = digitalRead(IR_SENSOR_PIN);
      if (val == 0)
      {
        count_value++;
      }
      lcd.setCursor(0, 0);
      lcd.print("Assignment Box");
      lcd.setCursor(0, 1);
      lcd.print("Count=");
      lcd.print(count_value );
      delay(500);
    }
  }

  else {


    myservo.write(180);              // tell servo to go to position in variable 'pos'

    lcd.setCursor(0, 0);
    lcd.print("Box FILLED !!!");
    lcd.setCursor(0, 1);
    lcd.print("Count=");

    SIM900A.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode
    SIM900A.println("AT+CMGS=\"+94761962323\"\r"); //Type Your Mobile number to send message
    SIM900A.println("submitted count = " + String(count_value));// Messsage content

  }
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
