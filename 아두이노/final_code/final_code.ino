#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h> //mp3 라이브러리 
#include <Adafruit_NeoPixel.h> //네오픽셀 라이브러리 
#include<Servo.h>    //모터 라이브러리 

Servo Motor1;  //Motor1 객체생성 
Servo Motor2;  //Motor2 객체생성 

//안내방송 핀번호
int pin0=0;
int pin1=1;

//초음파 핀번호
#define TRIG1 2 //TRIG1 핀 설정 (초음파 보내는 핀)
#define ECHO1 3 //ECHO1 핀 설정 (초음파 받는 핀)
#define TRIG2 4 //TRIG2 핀 설정 (초음파 보내는 핀)
#define ECHO2 5 //ECHO2 핀 설정 (초음파 받는 핀)
#define TRIG3 6 //TRIG3 핀 설정 (초음파 보내는 핀)
#define ECHO3 7 //ECHO3 핀 설정 (초음파 받는 핀)
#define TRIG4 8 //TRIG4 핀 설정 (초음파 보내는 핀)
#define ECHO4 9 //ECHO4 핀 설정 (초음파 받는 핀)

//모터 핀번호 
int pin11=11;  //핀번호 9번 지정
int pin12=12;  //핀번호 9번 지

//신호등 핀번호
int green=A0;
int yellow=A1;
int red=A2;

//네오픽셀 핀번호
const int pin13=13;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, pin13, NEO_GRB + NEO_KHZ800); //13번핀번호와 픽셀 10개연결


long duration1,duration2,duration3,duration4, distance1,distance2,distance3,distance4; //초음파 거리 1,2,3,4
unsigned long time1; //프로그램 동작 시간 측정때 필요
extern volatile unsigned long timer0_millis;  // 프로그램 동작 시간 초기화때 필요

void setup() 
{   //mp3
    Serial.begin(9600);           //angle,distance 확인용 
    mp3_set_serial (Serial);      // DFPlayer-mini mp3 module 시리얼 세팅
    delay(1);                     // 볼륨값 적용을 위한 delay
    mp3_set_volume (30);          // 볼륨조절 값 0~30
    
    //초음파
    pinMode(TRIG1, OUTPUT);  //TRIG1를 출력으로 설정
    pinMode(ECHO1, INPUT);   //ECHO1를 입력으로 설정 
    pinMode(TRIG2, OUTPUT);  //TRIG2를 출력으로 설정
    pinMode(ECHO2, INPUT);   //ECHO2를 입력으로 설정 
    pinMode(TRIG3, OUTPUT);  //TRIG3를 출력으로 설정
    pinMode(ECHO3, INPUT);   //ECHO3를 입력으로 설정 
    pinMode(TRIG4, OUTPUT);  //TRIG4를 출력으로 설정
    pinMode(ECHO4, INPUT);   //ECHO4를 입력으로 설정 
     
    //서보모터
    Motor1.attach(pin11); //11번에 연결
    Motor2.attach(pin12); //12번연결
    
    //신호등
    pinMode(green,OUTPUT);  //green을 출력으로 설정
    pinMode(yellow,OUTPUT);  //yellow를 출력으로 설정
    pinMode(red,OUTPUT);    //red를 출력으로 설정 
    
    //네오픽셀
    strip.begin();  //네오픽셀 동작 
    strip.setBrightness(15); //네오픽셀 밝기조절 
}

//함수호출
void var1(void); 
void var2(void);
void var3(void);
void var4(void);
void time(void);

void loop() {

   for(int angle=180;angle>=90;angle-=5)  // 차단기 올라갈때 
  {
      Motor2.write(270-angle);  //반시계 중간에 모터 
      Motor1.write(angle);  // 시계방향 보도쪽 모터 
      
      ///////////////서보모터 각도확인/////////////////////////
      Serial.print("\nmotor1:");
      Serial.println(angle);  //angle=0,90확인
      Serial.print("motor2:");
      Serial.println(angle);  //angle=0,90확인
      delay(200);          //딜레이시간 0.2초 
      //////////////////////////////////////////////////////
      
      digitalWrite(green,1);// 초록불on 
      if(angle==90)   //차단기가 완전히 올라갈때
      {
        digitalWrite(green,1);  // 초록불on
        for(int i=0;i<10;i++)
        {
            strip.setPixelColor(i,strip.Color(0,255,0));  // green 색깔네오픽셀 부여
            strip.show();    //네오픽셀 on
            delay(1000);  // 1초간격으로 
            if(i==7)  // 네오픽셀이 두칸남으면
            {
              digitalWrite(green,0);   //초록불 off
              digitalWrite(yellow,1);  //노란불 on   
             }
             if(i==9) // 네오픽셀이 끝나면
             {
                digitalWrite(yellow,0);  // 노란불 off
                digitalWrite(red,1);   // 빨간불 on
             }
          }
             timer0_millis=0; //프로그램 동작시간 초기화
        }
   }
 
   for(int angle=90;angle<=180;angle+=5)   //차단기 내려갈떄 
   {
      digitalWrite(red,1);        //빨간불 on
      Motor2.write(270-angle);   //  시계방향으로 가운데 모터
      Motor1.write(angle);     // 반시계방향 보도쪽 모터 
     
      //////서보모터 각도확인/////////////////////
      Serial.print("\nmotor1:");
      Serial.println(angle);  //angle=0,90확인
      Serial.print("motor2:");
      Serial.println(angle);  //angle=0,90확인
      delay(200); //0.2초 마다 모터내려옴
      //////////////////////////////////////////
      
      if(angle==180)   //차단기가 완전히 내려갈때
      {
        digitalWrite(red,1); //빨간불 on
        int i;
        for(i=0;;i++)
        {     
           if(i==0) //안내방송
           {
             mp3_play (4);    //0004파일 플레이
             delay (4000);    //4초동안플레이 
           }
          
          strip.setPixelColor(i, strip.Color(255, 0, 0)); // RGB로 색상을 결정한다.(red)
          strip.show(); //네오픽셀 보여준다  
          time(); //동작시간측정
          //////////////////1초마다 초음파 거리측정/////////////////////       
          var1(); //초음파1 거리 측정
          var2();  //초음파2 거리측정
          var3();  //초음파3 거리측정
          var4();   // 초음파 4거리측정
          delay(1000);   //1초마다 
          ///////////////////////////////////////////////////////////
          
          Serial.println(i); // 잘구동되는지 확인용
          if(distance1<20 || distance2<20 || distance3<20 || distance4<20) //초음파1,2,3,4 거리가 하나라도 20미만일떄 
           { 
             i--; 
             continue;  
           } 
           
          //차단봉 내려간지 10초지난뒤
          if(time1>10000 && i==9) //시간이 10초이상이고 네오픽셀이끝날때 
            {
              digitalWrite(red,0);  //빨간불 off
              break; // while문 빠져나오면서 차단기 올라감
            }  
           
          }      
        } 
     }
}
    



/////////////////////사용자 정의함수들/////////////////////////////

void var1(void) //초음파 1로 거리 측정
  
{
   digitalWrite(TRIG1, LOW);  //TRIG off 
  delayMicroseconds(2);  //2밀리초동안 멈춤
  digitalWrite(TRIG1, HIGH);  //TRIG on 
  delayMicroseconds(10);  //10밀리초동안멈춤
  digitalWrite(TRIG1, LOW); // TRIG off 
  
  duration1 = pulseIn (ECHO1, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장
  distance1 = duration1 * 17 / 1000; 
  Serial.print("\ndistance1 : "); // 거리표시
  Serial.print(distance1); //측정된 물체로부터 거리값(cm값)확인

}


void var2(void)  //초음파 2로 거리측정 
{
  digitalWrite(TRIG2, LOW);  //TRIG off 
  delayMicroseconds(2);  //2밀리초동안 멈춤
  digitalWrite(TRIG2, HIGH);  //TRIG on 
  delayMicroseconds(10);  //10밀리초동안멈춤
  digitalWrite(TRIG2, LOW); // TRIG off 
  
  duration2 = pulseIn (ECHO2, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장
  distance2 = duration2 * 17 / 1000; 
  Serial.print("\ndistance2 : "); // 거리표시
  Serial.println(distance2); //측정된 물체로부터 거리값(cm값)확인

}



void var3(void)  //초음파 3로 거리측정 
{
  digitalWrite(TRIG3, LOW);  //TRIG off 
  delayMicroseconds(2);  //2밀리초동안 멈춤
  digitalWrite(TRIG3, HIGH);  //TRIG on 
  delayMicroseconds(10);  //10밀리초동안멈춤
  digitalWrite(TRIG3, LOW); // TRIG off 
  
  duration3 = pulseIn (ECHO3, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장
  distance3 = duration3 * 17 / 1000; 
  Serial.print("\ndistance3 : "); // 거리표시
  Serial.println(distance3); //측정된 물체로부터 거리값(cm값)확인

}


void var4(void)  //초음파 4로 거리측정 
{
  digitalWrite(TRIG4, LOW);  //TRIG off 
  delayMicroseconds(2);  //2밀리초동안 멈춤
  digitalWrite(TRIG4, HIGH);  //TRIG on 
  delayMicroseconds(10);  //10밀리초동안멈춤
  digitalWrite(TRIG4, LOW); // TRIG off 
  
  duration4 = pulseIn (ECHO4, HIGH); //물체에 반사되어돌아온 초음파의 시간을 변수에 저장
  distance4 = duration4 * 17 / 1000; 
  Serial.print("\ndistance4 : "); // 거리표시
  Serial.println(distance4); //측정된 물체로부터 거리값(cm값)확인

}



void time(void)  //프로그램 동작시간 측정
{
  Serial.print("\nTime: ");  // time 보여줌
  time1 = millis();  //millis 함수 : https://www.arduino.cc/reference/ko/language/functions/time/millis/
  Serial.println(time1); // 프로그램 시작후 지난 시간 출력
  delay(1000);          // 너무 많은 데이터를 보내지 않기 위해 1초 기다림 
}
