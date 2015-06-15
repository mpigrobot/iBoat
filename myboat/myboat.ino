#include <Servo.h> 

Servo boatservo;
int servocontrolpin = 9, motorpin1 = 7, motorpin2 = 8, motorcontrolpin = 5;

String comdata = "";
int comlength = 0;
int mark = 0;
long orderdata[50] = {0};

void setup()
{
  Serial1.begin(115200);
  boatservo.attach(servocontrolpin);
  pinMode(motorpin1,OUTPUT);
  pinMode(motorpin2,OUTPUT);
  pinMode(motorcontrolpin,OUTPUT);
}

void loop()
{
  int ordercount = 0;
  //ordercount   
  while (Serial1.available() > 0)
  {
    comdata += char(Serial1.read());
    delay(2);
    mark = 1;
  }

  if(mark == 1) 
  {
    comlength = comdata.length();
    for(int i = 0; i < comdata.length() ; i++)
    {
      if(comdata[i] == ',')
      {
        ordercount++;
      }
      else
      {
//如果没有逗号的话，就将读到的数字*10加上以前读入的数字，
//并且(comdata[i] - '0')就是将字符'0'的ASCII码转换成数字0（下面不再叙述此问题，直接视作数字0）。
//比如输入数字是12345，有5次没有碰到逗号的机会，就会执行5次此语句。
//因为左边的数字先获取到，并且numdata[0]等于0，
//所以第一次循环是numdata[0] = 0*10+1 = 1
//第二次numdata[0]等于1，循环是numdata[0] = 1*10+2 = 12
//第三次是numdata[0]等于12，循环是numdata[0] = 12*10+3 = 123
//第四次是numdata[0]等于123，循环是numdata[0] = 123*10+4 = 1234
//如此类推，字符串将被变成数字0。
        orderdata[ordercount] = orderdata[ordercount] * 10 + (comdata[i] - 48);
      }
    }
//comdata的字符串已经全部转换到numdata了，清空comdata以便下一次使用，
//如果不请空的话，本次结果极有可能干扰下一次。
    comdata = String("");
//循环输出numdata的内容，并且写到PWM引脚
    for(int i = 0; i < comlength/4; i++)
    {
      switch(orderdata[i])
      {
        case 637:
        boatservo.write(137);
        Serial1.println("L2,");
        break;
        
        case 604:
        boatservo.write(104);
        Serial1.println("L1,");
        break;
        
        case 558:
        boatservo.write(58);
        Serial1.println("R1,");
        break;
        
        case 534:
        boatservo.write(34);
        Serial1.println("R2,");
        break;
        
        case 581:
        boatservo.write(81);
        Serial1.println("MID,");
        break;
        
        case 300:
        digitalWrite(motorpin1,LOW);
        digitalWrite(motorpin2,HIGH);
        analogWrite(motorcontrolpin,200);
        Serial1.println("R,");
        break;
        
        case 226:
        digitalWrite(motorpin1,HIGH);
        digitalWrite(motorpin2,LOW);
        analogWrite(motorcontrolpin,226);
        Serial1.println("D1,");
        break;
        
        case 254:
        digitalWrite(motorpin1,HIGH);
        digitalWrite(motorpin2,LOW);
        analogWrite(motorcontrolpin,254);
        Serial1.println("D2,");
        break;
        
        case 256:
        analogWrite(motorcontrolpin,0);
        Serial1.println("N,");
        break;
      }
      orderdata[i] = 0;
//输出之后必须将读到数据的mark置0，不置0下次循环就不能使用了。
      mark = 0;
    }
  }
}
