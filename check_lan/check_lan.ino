/*
initialization arduino's pins
3 - red led
4 - green led
5 - blue led
TX from router - RX on Arduino
RX from router - TX on Arduino
GND - GND
*/
int red_led = 3;
int green_led = 4; 
int blue_led = 5;
String str_from_serial = "";


void red_led_on()
{
  digitalWrite(blue_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, HIGH); 
};


void green_led_on()
{
  digitalWrite(blue_led, LOW);
  digitalWrite(red_led, LOW); 
  digitalWrite(green_led, HIGH);
};


void setup() 
{
  Serial.begin(115200);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(blue_led, OUTPUT);
  digitalWrite(red_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(blue_led, HIGH);
};


void loop()  
{
  if (Serial.available() > 0) 
  {
    char char_from_serial = Serial.read();
    if ((char_from_serial == '\n') or (char_from_serial == '\r')) 
    {
      if ((str_from_serial == "red") or (str_from_serial == "red\n"))
        red_led_on();
      else if ((str_from_serial == "green") or (str_from_serial == "green\n"))
        green_led_on();
      str_from_serial = "";
    }
    else 
      str_from_serial += char_from_serial;
  }
}
