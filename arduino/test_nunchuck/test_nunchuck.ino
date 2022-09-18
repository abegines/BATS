#include "WiiChuck.h" // v 0.3.2 

Accessory chuck ; 

void setup()
{
 Serial.begin(115200); 
 chuck.begin();
  if (chuck.type == Unknown) {
    chuck.type = NUNCHUCK;

  }
}

void loop()
{
 delay(100);
 chuck.readData();
 //chuck.printInputs(); 

     /*
    values[1]=map(getJoyX(),0,255,0,255);
    values[2]=map(getJoyY(),0,255,0,255);
    values[3]=map(getRollAngle(),0,1024,0,256);
    values[4]=map(getPitchAngle(),0,1024,0,256);
    values[5]=map(getAccelX(),0,1024,0,256);
    values[6]=map(getAccelY(),0,1024,0,256);

    values[7]=map(getAccelZ(),0,1024,0,256);
    values[8]=0;
    values[9]=0;
    values[10]=0;
    values[11]=getButtonZ()?255:0;
    values[12]=getButtonC()?255:0;
    values[13]=0;
    values[14]=0;

    values[15]=0;
    values[16]=0;
    values[17]=0;

    values[18]=0;
    values[19]=0;
    */
 //Serial.print("X: ");
 Serial.println(chuck.values[0]); 
 /*Serial.print(" ");
 Serial.print("Y: ");
 Serial.print(chuck.values[2]); 
 Serial.print(" ");

 Serial.print(chuck.values[11] ? "Z" : "-");
 Serial.println(chuck.values[12] ? "C" : "-");*/
}
