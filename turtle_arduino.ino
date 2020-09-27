#include <SoftwareSerial.h>
#include <Servo.h>


int mode = 0;
// offset mode 1

Servo servo1;
Servo servo2;
//SoftwareSerial btserial(2,3);
//RX 3, TX 2

SoftwareSerial btserial(A1,A0);
//RX A0, TX A1


int buffers[100];
int pos;
int check;


void stop_m(){ // 정지
    servo1.write(90);
    servo2.write(90);
    delay(100);
}

void go_m(int angle){ // 전진
  servo1.write(0);
  servo2.write(180);
  delay(1000+angle);
}
void back_m(int angle){ // 후진
  servo1.write(180);
  servo2.write(0);
  delay(1000+angle);
}
void left_m(int angle){ // 좌회전
    servo1.write(0);
    servo2.write(0);
    delay(1000+angle);
}
void right_m(int angle){ // 우회전
    servo1.write(180);
    servo2.write(180);
    delay(1000+angle);
}

void test(){
        go_m(0);
        stop_m();
        for(int i =0; i<12; i++){
        right_m(-700);
        go_m(0);
        stop_m();
        }
        left_m(0);
        go_m(1000);
        go_m(1000);
}

void buffers_clear(){
  for(int i =0; i<100; i++)
    buffers[i] = 0;
}

void setup() {
  Serial.begin(9600);
  btserial.begin(9600);
  Serial.println("Start");
  servo1.attach(9);
  servo2.attach(10);
  pos = 0;
  mode = 0;
  buffers_clear();
}

void loop() {
  if(mode == 0){
    if(btserial.available()){
      check = btserial.parseInt();
      Serial.println(check);
      if(check == 9){
        Serial.println("시작");
        mode = 1;
        pos = 0;
      }
      else if(check == 6){
        while(check != 5){
          buffers[pos++] = check;
          Serial.println(buffers[pos]);
          check = btserial.parseInt();
        }
        pos = 0;
        mode = 2; // 실행
      }
    }
  }
  else if (mode==1){
    if(btserial.available()){
      check = btserial.parseInt();
      Serial.println(check);
      if(check == 9) mode = 0;
      else if(check == 1) {
        go_m(-1000);
        Serial.println("1");
      }
      else if(check == 2){
        back_m(-1000);
        Serial.println("2");
      }
      else if(check == 3){
        left_m(-1000);
        Serial.println("3");
      }
      else if(check == 4){
        right_m(-1000);
        Serial.println("4");
      }
      else if(check == 7) stop_m();
    }
    //else stop_m();
  }
  else{
    if(btserial.available()){
      if(btserial.parseInt() == 7) {
        stop_m();
        buffers_clear();
        mode = 0;
      }
    }
    else{
      if(buffers[pos] == 0){
        buffers_clear();
        mode = 0;
        stop_m();
      }
      else if(buffers[pos] == 1){ //직진
       go_m(0);
       stop_m();
       Serial.println("1");
      }
      else if(buffers[pos] == 2){ //후진
       back_m(0);
       stop_m();Serial.println("2");
      }
      else if(buffers[pos] == 3){ //좌회전
        left_m(0);
        stop_m(); Serial.println("3");
      }
      else if(buffers[pos] == 4){ //우회전
        right_m(0);   
        stop_m(); Serial.println("4");
      }
      pos++;
    }
  }
}
