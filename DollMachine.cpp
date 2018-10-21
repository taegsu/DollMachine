
int buttonMove_pin[8];
// 기계부 움직임 버튼
// b7 = 7; 앞
// b6 = 6; 뒤
// b5 = 5; 오른쪽
// b4 = 4; 왼쪽
// b3 = 3; 위로 올리기쪽
// b2 = 2; 아래로 내리기

int sensor_pin[7];
//센서(기계부가 끝에 위치했는지 확인)
// s1 = 8; //앞
// s2 = 9; //뒤
// s3 = 10; //오른쪽
// s4 = 11; //왼쪽
// s5 = 12; //위로 올리기 HIGH
// s6 = 13; //아래로 내리기

int buttonCrane_pin[7];
//집게 힘조절 버튼
// b1 = A2; //집게1 UP
// b2 = A3; //집게1 DOWN
// b3 = A6; //집게2 UP
// b4 = A7; //집게2 DOWN
// b5 = A10; //집게3 UP
// b6 = A11; //집게3 DOWN

int pwm_pin[7];
int dir_pin[7];
// 모터드라이버 핀
// pwm1 = A0;
// dir1 = A1;
// pwm2 = A4;
// dir2 = A5;
// pwm3 = A8;
// dir3 = A9;
// pwm4 = A12;
// dir4 = A13;
// pwm5 = A14;
// dir5 = A15;
// pwm6 = 44;
// dir6 = 45;

int count1 = 0; int count2  = 0; int count3 = 0; int timeCount = 0;
boolean setPowerClawsAll = false;
boolean setPowerFirstClaws = false; boolean setPowerSecondsClaws = false; boolean setPowerThirdClaws = false;

void setup() {
  Serial.begin(9600);
  
  buttonCrane_pin[1] = A2; //집게버튼 핀 설정
  buttonCrane_pin[2] = A3;
  buttonCrane_pin[3] = A6;
  buttonCrane_pin[4] = A7;
  buttonCrane_pin[5] = A10;
  buttonCrane_pin[6] = A11;

  pwm_pin[1] = A0; //pwm 핀 설정
  pwm_pin[2] = A4;
  pwm_pin[3] = A8;
  pwm_pin[4] = A12;
  pwm_pin[5] = A14;
  pwm_pin[6] = 44;

  dir_pin[1] = A1; //모터방향 핀 설정
  dir_pin[2] = A5;
  dir_pin[3] = A9;
  dir_pin[4] = A13;
  dir_pin[5] = A15;
  dir_pin[6] = 45;
  
  for(int i = 2; i < 8; i++){ //움직임 버튼 초기화
    buttonMove_pin[i] = i;
    pinMode(buttonMove_pin[i],INPUT);
    digitalWrite(buttonMove_pin[i],HIGH);
  }

  for(int i = 1; i < 7; i++){ //센서 초기화
    sensor_pin[i] = i + 7;
    pinMode(sensor_pin[i],INPUT);
    digitalWrite(sensor_pin[i],LOW);
  }

  digitalWrite(sensor_pin[2],HIGH);
  //digitalWrite(sensor_pin[6],HIGH); 

  for(int i = 1; i < 7; i++){ // 집게버튼 초기화
    pinMode(buttonCrane_pin[i],INPUT);
    digitalWrite(buttonCrane_pin[i],HIGH);
  }

  for(int i = 1; i < 7; i++){ //모터 초기화
    pinMode(pwm_pin[i],OUTPUT);
    pinMode(dir_pin[i],OUTPUT);
  }

  

  attachInterrupt(buttonMove_pin[2],catchCrane,FALLING);//내리기
  attachInterrupt(buttonMove_pin[7],craneFront,FALLING);//앞으로오기
  attachInterrupt(buttonMove_pin[6],craneBack,FALLING);//뒤로가기
  attachInterrupt(buttonMove_pin[7],craneLeft,FALLING);//왼쪽으로가기
  attachInterrupt(buttonMove_pin[6],craneRight,FALLING);//오른쪽으로가기

}

void loop() {
  
  if(setPowerFirstClaws == true && setPowerSecondsClaws == true && setPowerThirdClaws == true)
    setPowerClawsAll = true;
  else if (setPowerFirstClaws == false && setPowerSecondsClaws == false && setPowerThirdClaws == false)
    setPowerClawsAll = false;

  if(timeCount == 0){
    for(;;){ //20초동안 움직이기
    
      if(digitalRead(buttonMove_pin[7]) == LOW){
        craneFront();
      }
      else if(digitalRead(buttonMove_pin[6]) == LOW){
        craneBack();
      }
      else if(digitalRead(buttonMove_pin[5]) == LOW){
        craneLeft(); 
      }
      else if(digitalRead(buttonMove_pin[4]) == LOW){
        craneRight();
      }
      else 
        craneStop();
  
      delay(10);      //20초될때까지 카운트~
      timeCount ++;
      if(timeCount == 1000){
        craneStop();
        Serial.println("이제뽑아 임마!!!!!!!!!!!!");
        break;
      }
    }
 }

  if(setPowerClawsAll == false){    
    /*for(;;){ //첫번째 집게발 힘 조절시간을 줌
      reGripFirstClaws();
      if(setPowerFirstClaws){
        Serial.print("집게1조절");
        break;         
      }
    }

    for(;;){//두번째 집게발 힘 조절시간을 줌
      reGripSecondsClaws();
      if(setPowerSecondsClaws){
        Serial.print("집게2조절");
        break;         
      }
    }

    for(;;){//세번째 집게발 힘 조절시간을 줌
      reGripThirdClaws();
      if(setPowerThirdClaws){
        Serial.print("집게3조절");
        break;         
      }
    }*/
    reGripAllClaws();
    gripZero();
    setPowerClawsAll = true;
  }
  else{
    for(;;){ //내려가기 버튼 기다림
      if (digitalRead(buttonMove_pin[2]) == LOW){
        catchCrane();  //인형을 설정한 힘만큼 집음
        returnCraneStartPoint(); // 출발위치로 돌아감
        break;
      }
    }
  }
}

void craneDown(){
    digitalWrite(dir_pin[3], HIGH);
    digitalWrite(pwm_pin[3], LOW);
}

void craneUp(){
    digitalWrite(dir_pin[3], LOW);
    digitalWrite(pwm_pin[3], HIGH);
}

void craneLeft(){
    Serial.println("왼쪽");
    digitalWrite(dir_pin[2], LOW);
    digitalWrite(pwm_pin[2], HIGH);
}

void craneRight(){
    Serial.println("오른쪽");
    digitalWrite(dir_pin[2], HIGH);
    digitalWrite(pwm_pin[2], LOW);
}

void craneBack(){
    Serial.println("뒤로");
    digitalWrite(dir_pin[1], LOW);
    digitalWrite(pwm_pin[1], HIGH);
}

void craneFront(){
    Serial.println("앞으로");
    digitalWrite(dir_pin[1], HIGH);
    digitalWrite(pwm_pin[1], LOW);
}
void craneStop(){
    digitalWrite(dir_pin[3], LOW);
    digitalWrite(pwm_pin[3], LOW);

    digitalWrite(dir_pin[2], LOW);
    digitalWrite(pwm_pin[2], LOW);

    digitalWrite(dir_pin[1], LOW);
    digitalWrite(pwm_pin[1], LOW);
}

void gripFirstClaws(){
     digitalWrite(dir_pin[4], LOW);
     digitalWrite(pwm_pin[4], HIGH);
}

void gripSecondsClaws(){
     digitalWrite(dir_pin[5], LOW);
     digitalWrite(pwm_pin[5], HIGH);
}

void gripThirdClaws(){
     digitalWrite(dir_pin[6], LOW);
     digitalWrite(pwm_pin[6], HIGH);
}


void gripZero(){ // 집게 벌리기
    digitalWrite(dir_pin[4], HIGH);
    digitalWrite(pwm_pin[4], LOW);
    delay(100*count1);

    digitalWrite(dir_pin[4], LOW);
    digitalWrite(pwm_pin[4], LOW);
    
    digitalWrite(dir_pin[5], HIGH);
    digitalWrite(pwm_pin[5], LOW);
    delay(100*count2);

    digitalWrite(dir_pin[5], LOW);
    digitalWrite(pwm_pin[5], LOW);
    
    digitalWrite(dir_pin[6], HIGH);
    digitalWrite(pwm_pin[6], LOW);
    delay(100*count3);

    digitalWrite(dir_pin[6], LOW);
    digitalWrite(pwm_pin[6], LOW);  
}

void reGripAllClaws(){
  if(setPowerClawsAll == false){
    for(;;){
      if(digitalRead(buttonCrane_pin[1]) == LOW && setPowerFirstClaws == false){ 
        reGripFirstClaws(); 
      }
      else if(digitalRead(buttonCrane_pin[3]) == LOW && setPowerSecondsClaws == false){
        reGripSecondsClaws();
      }
      else if(digitalRead(buttonCrane_pin[5])==LOW && setPowerThirdClaws == false)
      {
        reGripThirdClaws();
      }
      else if(setPowerFirstClaws == true && setPowerSecondsClaws == true && setPowerThirdClaws == true )
        break;
    }       
  }  
}


void reGripFirstClaws()
{
  if(setPowerClawsAll == false){ //첫번째 집게 힘설정 안했을때

    for(;;){
      if(digitalRead(buttonCrane_pin[1]) == LOW) 
      {
        gripFirstClaws();
        delay(100);
        count1++;
        setPowerFirstClaws = true;
      }
      else if(digitalRead(buttonCrane_pin[1]) == HIGH && setPowerFirstClaws == true)
      {
        break;
      }
    }
    digitalWrite(dir_pin[4], LOW);
    digitalWrite(pwm_pin[4], LOW);
  }
  else{                    //첫번째 집게 힘설정 한후
      gripFirstClaws();
      delay(100 * count1);
      digitalWrite(dir_pin[4], LOW);
      digitalWrite(pwm_pin[4], LOW);
      setPowerFirstClaws = false;
      Serial.println("집게1집기!");
  }
}

void reGripSecondsClaws()
{
 
 if(setPowerClawsAll == false){

    for(;;){
      if(digitalRead(buttonCrane_pin[3]) == LOW)
      {
        gripSecondsClaws();
        delay(100);
        count2++;
        setPowerSecondsClaws = true;
      }
      else if(digitalRead(buttonCrane_pin[3]) == HIGH && setPowerSecondsClaws == true)
      {
        break;
      }
    }
    digitalWrite(dir_pin[5], LOW);
    digitalWrite(pwm_pin[5], LOW);
    
  }
  else{
      gripSecondsClaws();
      delay(100 * count2);
      digitalWrite(dir_pin[5], LOW);
      digitalWrite(pwm_pin[5], LOW);
      setPowerSecondsClaws = false;
      Serial.println("집게2집기");
  }
}

void reGripThirdClaws()
{

  if(setPowerClawsAll == false){

    for(;;){
      if(digitalRead(buttonCrane_pin[5])==LOW)
      {
        gripThirdClaws();
        delay(100);
        count3++;
        setPowerThirdClaws = true;
      }
      else if(digitalRead(buttonCrane_pin[5]) == HIGH && setPowerThirdClaws == true)
      {
        break;
      }
    }
    digitalWrite(dir_pin[6], LOW);
    digitalWrite(pwm_pin[6], LOW);
    
  }
  else{
      gripThirdClaws();
      
      delay(100 * count3);
      
      digitalWrite(dir_pin[6], LOW);
      digitalWrite(pwm_pin[6], LOW);
      setPowerThirdClaws = false;
      Serial.println("집게3집기");
  }
}

void catchCrane(){
    Serial.print("catch!!");  
    craneDown();
    for(;;){
      if(digitalRead(sensor_pin[2]) == LOW) //센서 안닫으면
      {
        Serial.println("멈출때까지!");
        craneStop();
        reGripFirstClaws();
        reGripSecondsClaws();
        reGripThirdClaws();
        break;
      }
    }   
}

void returnCraneStartPoint() 
{
  if(digitalRead(sensor_pin[3]) == LOW){ //집게 위로 올라옴
    craneUp();
    Serial.println("return~~~up~");
    for(;;){
      if (digitalRead(sensor_pin[3]) == HIGH){
        Serial.println("위센서 닫았다!");
        craneStop();
        break;
      }
    }
  }

  if(digitalRead(sensor_pin[5] == LOW) ){ //집게 왼쪽으로 감
    craneLeft();
    Serial.println("return LEFT~~~~~");
    for(;;){
      if (digitalRead(sensor_pin[5]) == HIGH){
        Serial.println("왼쪽센서 닫았다!");
        craneStop();
        break;
      }
    }
  }
    
  if(digitalRead(sensor_pin[6] == LOW) ){ //집게 뒤로 출발지점까지
    craneBack();
    Serial.print("return back~~~");
    for(;;){
      if (digitalRead(sensor_pin[6]) == HIGH){
        Serial.println("뒤쪽센서 닫았다!!!");
        craneStop();
        gripZero();
        count1 = 0;
        count2 = 0;
        count3 = 0;
        timeCount = 0;
        break;
      }
    }
  }
     
}