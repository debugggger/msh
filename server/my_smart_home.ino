#include <SoftwareSerial.h>
 
#define DEBUG true
 
// Делает RX линию Arduino выводом 2, а TX линию Arduino выводом 3.
// Это означает, что вам необходимо подключить TX линию от ESP к выводу 2 Arduino,
// а RX линию от ESP к выводу 3 Arduino. 
SoftwareSerial esp8266(2,3); 


void setup()
{
  Serial.begin(9600);
  esp8266.begin(57600); // скорость передачи вашего ESP может отличаться
  
  pinMode(11,OUTPUT);
  digitalWrite(11,LOW);
  
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);
   
  // перезапустить модуль 
  sendCommand("AT+RST\r\n",2000,DEBUG);
  // настроить как точку доступа  
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); 
  sendCommand("AT+CWJAP=\"EUGEN-SAMS_Network_1\",\"9216411011\"\r\n",3000,DEBUG);
  delay(10000);
  // получить ip адрес
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); 
  // настроить для нескольких соединений
  sendCommand("AT+CIPMUX=1\r\n",1000,DEBUG); 
  // включить сервер на порту 80
  sendCommand("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
  
  Serial.println("Server Ready");
}
 
void loop()
{
  if(esp8266.available()) // проверить, послал ли esp сообщение
  {
 
    
    if(esp8266.find("+IPD,"))
    {
      // ждать, когда последовательный буфер заполнится (прочитаются все последовательные данные)  
      delay(1000);    
      // получить id подключения, чтобы мы могли отключиться
      int connectionId = esp8266.read()-48;
      // вычитаем 48 потому, что функция read() возвращает
      // десятичное значение в ASCII, а 0 (первое десятичное число) начинается с 48
                                     
          
      esp8266.find("pin="); // передвинуть курсор к "pin="
          
      int pinNumber = (esp8266.read()-48); // получить первую цифру, т.е., если вывод 13, то 1-ая цифра равна 1
      int secondNumber = (esp8266.read()-48);
      if(secondNumber>=0 && secondNumber<=9)
      {
        pinNumber*=10;
        pinNumber +=secondNumber; // получить вторую цифру, т.е., если вывод 13, то 2-ая цифра равна 3, 
                                  // и добавить ее к первой цифре
      }

      Serial.print("pinNumber: ");
      if (pinNumber == LOW)
        Serial.println ("OFF");
      if (pinNumber == HIGH)
        Serial.println ("ON");
      digitalWrite(pinNumber, !digitalRead(pinNumber)); // переключить состояние вывода    
        Serial.print("pinNumber: ");
      if (pinNumber == LOW)
        Serial.println ("OFF");
      if (pinNumber == HIGH)
        Serial.println ("ON");
     
      // собрать строку, которая посылается обратно на устройство, 
      // которое запросило переключение вывода
      String content;
      content = "Pin ";
      content += pinNumber;
      content += " is ";
     
      if(digitalRead(pinNumber))
      {
        content += "ON";
      }
      else
      {
        content += "OFF";
      }
     
      sendHTTPResponse(connectionId,content);
     
     // создать команду закрытия соединения
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // добавить id соединения
     closeCommand+="\r\n";
     
     sendCommand(closeCommand,1000,DEBUG); // закрыть соединение
    }
  }
}

/*
 * Name: sendData
 * Description: Функция, используемая для отправки данных на ESP8266.
 * Params: command - данные/команда для отправки; timeout - время ожидания отклика; debug - печатать в консоль?(true = да, false = нет)
 * Returns: Отклик от esp8266 (если есть отклик)
 */ 
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
    
  int dataSize = command.length();
  char data[dataSize];
  command.toCharArray(data,dataSize);
           
  esp8266.write(data,dataSize); // передача символов на esp8266
  if(debug)
  {
    Serial.println("\r\n====== HTTP Response From Arduino ======");
    Serial.write(data,dataSize);
    Serial.println("\r\n========================================");
  }
    
  long int time = millis();
    
  while( (time+timeout) > millis())
  {
    while(esp8266.available())
    {
  
      // У esp есть данные, поэтому вывести их в консоль
      char c = esp8266.read(); // прочитать следующий символ.
      response+=c;
    }  
  }
    
  if(debug)
  {
    Serial.print(response);
  }
  
  return response;
}

/*
 * Name: sendHTTPResponse
 * Description: Функция, которая посылает HTTP 200, HTML UTF-8 отклик
 */ 
void sendHTTPResponse(int connectionId, String content)
{
     
  // build HTTP response
  String httpResponse;
  String httpHeader;
  // HTTP Header
  httpHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n"; 
  httpHeader += "Content-Length: ";
  httpHeader += content.length();
  httpHeader += "\r\n";
  httpHeader +="Connection: close\r\n\r\n";
  // Здесь в коде баг: последний символ в "content" не посылается, поэтому я добавил дополнительный пробел
  httpResponse = httpHeader + content + " ";
  sendCIPData(connectionId,httpResponse);
}
 
/*
 * Name: sendCIPDATA
 * Description: посылает команду CIPSEND=<id_подключения>,<данные>
 */
void sendCIPData(int connectionId, String data)
{
  String cipSend = "AT+CIPSEND=";
  cipSend += connectionId;
  cipSend += ",";
  cipSend +=data.length();
  cipSend +="\r\n";
  sendCommand(cipSend,1000,DEBUG);
  sendData(data,1000,DEBUG);
}
 
/*
 * Name: sendCommand
 * Description: Функция, используемая для отправки данных на ESP8266.
 * Params: command - данные/команда для отправки; timeout - время ожидания отклика; debug - печатать в консоль?(true = да, false = нет)
 * Returns: Отклик от esp8266 (если есть отклик)
*/
String sendCommand(String command, const int timeout, boolean debug)
{
  String response = "";
  
  esp8266.print(command); // передача символов на esp8266
  
  long int time = millis();
  
  while( (time+timeout) > millis())
  {
    while(esp8266.available())
    {
  
      // У esp есть данные, поэтому вывести их в консоль
      char c = esp8266.read(); // прочитать следующий символ.
      response+=c;
    }  
  }
  
  if(debug)
  {
    Serial.print(response);
  }
    
  return response;
}
