#include<EEPROM.h>
void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);//trig
  pinMode(3,INPUT);//echo
  pinMode(5,OUTPUT);//motorlf
  pinMode(6,OUTPUT);//motorlb
  pinMode(7,OUTPUT);//motorrf
  pinMode(8,OUTPUT);//motorrb
  pinMode(13,OUTPUT);
  Serial.begin(9600);
}
  String f="";
  int a=0;
  int d=0;  String s="";
void stop(){
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  writer();
}
void hardleft(){
  digitalWrite(5,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
}
void hardright(){
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
}
void left(){
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
}
void right(){
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
}
void forward()
{
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  int l0 = analogRead(A0);
  int al0 = analogRead(A1);
  int f0 = analogRead(A2);
  int ar0 = analogRead(A3);
  int r0 = analogRead(A4);
  int s0 = analogRead(A5);
  int l=(l0<500)?1:0;
  int al=(al0<500)?1:0;
  int f=(f0<500)?1:0;
  int ar=(ar0<500)?1:0;
  int r=(r0<500)?1:0;
  int st =(s0<500)?1:0;
  int runcheck=EEPROM.read(0);
  if(runcheck==0)
  {
     if(l==1&&al==1&&f==1&&ar==1&&r==1&&st==1){
     stop();
     digitalWrite(13,HIGH);}
     else if(l==1&&(f==1 || r==1))
     {
      hardleft();
      a=3;
     }
     else
     if(f==1 && r==1)
     {
      forward();
      a=4;
     }
     else
     if(l==0&&r==0&&f==0&&al==0&&ar==0)
     {
      if(a==0 || a==4)
      {
        d=1;
        hardleft();
      }
      else
      if(a==1 || a==3)
      hardleft();
      else if(a==2)
      hardright();
     }
     else
     if(f==1 ||(al==1 && ar==1))
     {
      forward();
      if(a==3)
      s=s+"l";
      if(a==4)
      s=s+"f";
      if(d==1)
      {
        s=s+"b";
        d=0;
      }
      a=0;
     }
     else
     if(l==1){
     hardleft();a=1;}
     else
     if(r==1){
     hardright();a=2;}
     else
     if(al==1){
     left();a=1;}
     else
     if(ar==1){
     right();a=2;}
  }
  else
  if(runcheck==1)
  {
    if(l==1&&al==1&&f==1&&ar==1&&r==1&&st==1)
     {
     stop();
     runcheck=0;EEPROM.update(0,0);digitalWrite(13,HIGH);
     }
     else if(l==1 && r==1)
     {
      if(EEPROM.read(d)==2){
      hardright();delay(100);a=2;d++;}
      else{
      hardleft();delay(100); a=1;d++;}
     }
     else if(l==1 && f==1)
     {
      if(EEPROM.read(d)==1){
      forward();delay(100);a=0;d++;}
      else{
      hardleft();delay(100);a=1;d++;}
     }
     else if(r==1 && f==1)
     {
      if(EEPROM.read(d)==2){
      hardright();delay(100);a=2;d++;}
      else{
      forward();delay(100);a=0;d++;}
     }
      if(l==0&&r==0&&f==0&&al==0&&ar==0)
     {
      if(a==0 || a==4)
      {
        hardleft();
      }
      else
      if(a==1 || a==3)
      hardleft();
      else if(a==2)
      hardright();
     }
     else
     if(f==1 ||(al==1 && ar==1))
     {
      forward();
      a=0;
     }
    
     else
     if(l==1){
     hardleft();a=1;}
     else
     if(r==1){
     hardright();a=2;}
     else
     if(al==1){
     left();a=1;}
     else
     if(ar==1){
     right();a=2;}
  }
}
void writer(){
  Serial.println(s);
  for(int i=0;i<s.length()-1;i++)
     {
      char c0 = s.charAt(i);
      if(c0=='b')
      {
        if(f.charAt(f.length()-1)=='l' && s.charAt(i+1)=='f'){
        f=f.substring(0,f.length()-1)+"r";i++;}
        if(f.charAt(f.length()-1)=='l' && s.charAt(i+1)=='l'){
        f=f.substring(0,f.length()-1)+"f";i++;}
        if(f.charAt(f.length()-1)=='f' && s.charAt(i+1)=='l'){
        f=f.substring(0,f.length()-1)+"r";i++;}
      }
      else
      f=f+s.substring(i,i+1);
     }int addr=1;
     if(s.charAt(s.length()-2)!='b')
     f=f+s.substring(s.length()-1,s.length());
     EEPROM.update(0,1);       
     Serial.println(f);
     for(int i=0;i<f.length();i++)
     {
      int val=0;
      if(f.charAt(i)=='l')
      val=0;
      if(f.charAt(i)=='f')
      val=1;
      if(f.charAt(i)=='r')
      val=2;
      EEPROM.update(addr,val);
      addr+=1;
     }
}