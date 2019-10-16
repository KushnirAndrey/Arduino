/*
 * Скрипт отвечает за управление турникетом,
 * Используются пины D:2,3,4,5,6
 * пины D:12,13 используются для лампочек состояния турникета (Открыт/закрыт) (ДЛЯ ТЕСТА)
 * msg - сообщение, которое мы получаем на вход порта, т.е. режим работы турникета (1-6)
 * Выходные данные:
 *  /enter1 - совершен вход (можно поменять)
 *  /exit1 - совершен выход (можно поменять)
 */

uint8_t msg;
const uint8_t buttonA = 2;
const uint8_t buttonB = 3;
const uint8_t buttonS = 4;
const uint8_t passA = 5;
const uint8_t passB = 6;
const int fall_down_time = 300;


void setup(){
  // Инициализация пинов платы Arduino
  Serial.begin(9600);
  pinMode(12, OUTPUT);  // красная лампочка запрета прохода
  digitalWrite(12, HIGH);  // включаем красную лампочку при запуске программы
  pinMode(buttonA, OUTPUT);
  digitalWrite(buttonA, LOW);
  pinMode(buttonB, OUTPUT);
  digitalWrite(buttonB, LOW);
  pinMode(buttonS, OUTPUT);
  digitalWrite(buttonS, LOW);
  pinMode(passA, INPUT);
  digitalWrite(passA, HIGH);
  pinMode(passB, INPUT);
  digitalWrite(passB, HIGH);
}
/*
 * led_open(), led_close() две тестовые функции, 
 * горит зеленая лампочка - проход разрешен
 * горит красная - проход запрещен
 */
void led_open(){
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
}
void led_close(){
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
}
/*
 * РАБОТАЮТ ТОЛЬКО В РЕЖИМАХ ОДНОКРАТНОГО ПРОХОДА
 */
bool check_roll(int pin, int tm = millis()){/* tm - время когда была 
                                             вызвана функция
                                        pin - с какого пина
                                              считывать значения*/
  /*
   * Функция считывает значения с датчиков турникета.
   * И либо ждет, когда совершится поворот турникета, либо когда пройдет ~5сек.
   * Возвращаемые значения:
   *  Если совершен поворот то функция возвращает 1
   *  Если прошло 5 сек и поворота не было возвращает 0
   *  
   *  Также тут загораются и гаснут лампочки(красная, зеленая) (--ДЛЯ ТЕСТА--)
   *  (Лампочки корректно работают только в режима однократного прохода, чтобы можно было
   *  наглядно все видеть=))
   *  
   */
  led_open();
  int time = millis();
  while ((time - tm) <= 6000){//5сек
    if (digitalRead(pin) != 1){
      led_close();
      return 1;
    }
    time = millis();
  }
  led_close();
  Serial.println("/timeout");
  return 0;
}


void turnstile(char mode){
  /*Режимы работы для первого турникета
  mode - режим работы турникета (1 - 6)*/
  switch (mode){
    case '1'://A-->B
      digitalWrite(buttonA, HIGH);
      // delay(fall_down_time);
      if (check_roll(passA))
        Serial.println("/enter");
      digitalWrite(buttonA, LOW);
      // Serial.println("A-->B");
      break;
    case '2'://B-->A
      digitalWrite(buttonB, HIGH);
      delay(fall_down_time);
      
      if (check_roll(passB))
        Serial.println("/exit");
      digitalWrite(buttonB, LOW);
      // Serial.println("B-->A");
      break;
    case '3'://Постоянный A-->B
      digitalWrite(buttonA, HIGH);
      digitalWrite(buttonS, HIGH);
      delay(fall_down_time);
      digitalWrite(buttonA, LOW);
      digitalWrite(buttonS, LOW);
      // Serial.println("Постоянный A-->B");
      break;
    case '4'://Постоянный B-->A
      digitalWrite(buttonS, HIGH);
      digitalWrite(buttonB, HIGH);
      delay(fall_down_time);
      digitalWrite(buttonS, LOW);
      digitalWrite(buttonB, LOW);
      // Serial.println("Постоянный B-->A");
      break;
    case '5':
      digitalWrite(buttonS, HIGH);
      delay(fall_down_time);
      digitalWrite(buttonS, LOW);
      // Serial.println("STOP");
      break;
    case '6':
      digitalWrite(buttonA, HIGH);
      digitalWrite(buttonB, HIGH);
      digitalWrite(buttonS, HIGH);
      delay(fall_down_time);
      digitalWrite(buttonA, LOW);
      digitalWrite(buttonB, LOW);
      digitalWrite(buttonS, LOW);
      // Serial.println("Свободный проход в обе стороны A<--->B");
      break;
    }
  }


void loop(){
  // Считываем значения serial порта
  if (Serial.available() > 0)  // Если буфер порта не пуст считываем его
      turnstile(Serial.read()); //  Модуль управления первым турникетом
  }
