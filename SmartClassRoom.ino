//Include sections
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

//Define Component to Arduino Pins
#define SS_PIN 10
#define RST_PIN 9


#define Red_LED 8
#define Green_LED 7

#define Buzzer 5
#define lcdpin 2
// Your threshold value
//PrintWriter output ;
int sensorThres = 1000;
//initialize the library with the numbers of the interface pins
LiquidCrystal lcd(lcdpin, A4, A3, A2, A1, A0);
//Servo DoorLock;
MFRC522 mfrc522(SS_PIN, RST_PIN);
struct student {
  String ID;
  unsigned long entry_Time;
  unsigned long leave_Time;
  unsigned long total_Time;
  bool IN;
  bool accepted;
};
struct teacher {
  String ID;
  unsigned long session_Start;
  unsigned long session_End;
   double session_Time;
  bool IN;
};
 student s1 = {"32 4C 40 10",0,0,0,false,false};
  student s2 = {"8B 45 DF DB",0,0,0,false,false};
  student s3 = {"11 28 DF DB",0,0,0,false,false};
  teacher T = {"25 FE 48 BE",0,0,0,false};
unsigned long count;
unsigned int students;
void setup() 
{  count=0;
  students=0;
  // put your setup code here, to run once:
  //set the pins as an input/output
  pinMode(Red_LED,OUTPUT);
  pinMode(Green_LED,OUTPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(A5, INPUT);
 // pinMode(smokesensor,INPUT);
//  lcd.contrast();
  Serial.begin(9600); 
  //Initialise the LCD to 16x2 Character Format
  lcd.begin(16,2);
  //Initialise Servo and RFID
  SPI.begin();
  mfrc522.PCD_Init(); 
  
  digitalWrite(Red_LED,HIGH);
   digitalWrite(Green_LED,LOW);
}

void loop() 
{
  count=millis()/1000;
  lcd.clear();
  int analogSensor = analogRead(A5);

  //Serial.print("Pin A0: ");
  //Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  if (analogSensor < sensorThres)
  {
    digitalWrite(Red_LED, LOW);
    digitalWrite(Green_LED, HIGH);
    //delay(500);
    //digitalWrite(Red_LED, LOW);
    digitalWrite(Buzzer,HIGH);
    lcd.clear();
    lcd.print("EMERGENCYYYY");    
  }
else{
  if(!T.IN )
  {lcd.print("T Put Your Card");}
  else{lcd.print(students);}
  //lcd.clear();
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
      digitalWrite(Red_LED,LOW);
      digitalWrite(Green_LED,HIGH);
      delay(500);
      digitalWrite(Green_LED,LOW);
      digitalWrite(Red_LED,HIGH);

  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  delay(500);
  Serial.println();
  content.toUpperCase();
  
  //lcd.clear();
  //lcd.print(content);
  delay(1000);
  if (content.substring(1) == T.ID) //teacher
  { 
    if(!T.IN)
    {  
      T.IN=true;
      T.session_Start=count;
      if(s1.IN)
      {
        s1.entry_Time=count;
      }
      if(s2.IN)
      {
        s2.entry_Time=count;
      }
      if(s3.IN)
      {
        s3.entry_Time=count;
      }
    }
    else
    {
      T.IN=false;
      T.session_End=count;
      T.session_Time=T.session_End-T.session_Start;
      if(s1.IN)
      {s1.IN=false;
        s1.leave_Time=count;
        s1.total_Time += s1.leave_Time - s1.entry_Time ;
      }
      if(s2.IN)
      {s2.IN=false;
      s2.leave_Time=count;
      s2.total_Time += s2.leave_Time - s2.entry_Time ;
      }
    if(s3.IN)
      {s3.IN=false;
      s3.leave_Time=count;
      s3.total_Time += s3.leave_Time - s3.entry_Time ;
      }
  
      if(s1.total_Time>=T.session_Time*0.75){
          s1.accepted = true ;
        }
       if(s2.total_Time>=T.session_Time*0.75){
          s2.accepted = true ;
        }
        if(s3.total_Time>=T.session_Time*0.75){
          s3.accepted = true ;
        }
    Serial.println(s1.accepted);
    Serial.println(s2.accepted);
    Serial.println(s3.accepted);  
    s1.entry_Time=0;
    s1.leave_Time=0;
    s1.total_Time=0;
    s2.entry_Time=0;
    s2.leave_Time=0;
    s2.total_Time=0;
    s3.entry_Time=0;
    s3.leave_Time=0;
    s3.total_Time=0;
    }
   
  }

  if (content.substring(1) == s1.ID) //teacher
  { 
    if(!s1.IN)
    { 
      s1.IN=true;
      if(T.IN){
      s1.entry_Time=count;
      Serial.println("s1 enter");
      Serial.println(s1.entry_Time);}
      students++;
    }
    else
    {
      s1.IN=false;
      if(T.IN){
      s1.leave_Time=count;
      s1.total_Time += s1.leave_Time - s1.entry_Time ;
            Serial.println("s1 leave");
      Serial.println(s1.leave_Time);
      Serial.println(s1.total_Time);
}
     students--;
      
      
      
      
    }
   
  }

  if (content.substring(1) == s2.ID) //teacher
  { 
    if(!s2.IN)
    { 
      s2.IN=true;
      if(T.IN){s2.entry_Time=count;
       Serial.println("s2 enter");
      Serial.println(s2.entry_Time);}
      students++;
    }
    else
    {
      s2.IN=false;
      if(T.IN){s2.leave_Time=count;
      s2.total_Time += s2.leave_Time - s2.entry_Time ;
      Serial.println("s2 leave");
      Serial.println(s2.leave_Time);
      Serial.println(s2.total_Time);}
     students--;
      

      
      
      
    }
   
  }

  if (content.substring(1) == s3.ID) //teacher
  { 
    if(!s3.IN)
    { s3.IN=true;
    students++;
      if(T.IN){
      s3.entry_Time=count;
       Serial.println("s3 enter");
      Serial.println(s3.entry_Time);}
      
    }
    else
    {
      s3.IN=false;
     if(T.IN){ s3.leave_Time=count;
     s3.total_Time += s3.leave_Time - s3.entry_Time ;
           Serial.println("s3 leave");
      Serial.println(s3.leave_Time);
      Serial.println(s3.total_Time);}
     students--;
     

      
      
      
    }
   
  }

}

}
