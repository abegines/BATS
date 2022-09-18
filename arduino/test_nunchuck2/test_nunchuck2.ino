// https://www.prometec.net/nunchuk/
#include "Wire.h" //Libreria I2C
#include "WiiChuck.h" //Libreria WiiChuck

WiiChuck chuck = WiiChuck(); //Creamos la instancia de WiiChuck

void setup()
{
 Serial.begin(115200); //Configuramos la velocidad del puerto serie
 chuck.begin(); //Iniciamos WiiChuck

}

void loop()
{
 delay(20);
 chuck.update(); //Actualizamos los valores

 //Imprimimos los valores por pantalla

 Serial.print("Eje X: ");
 Serial.print(chuck.readJoyX()); //Eje X del Joystick
 Serial.print(" ");
 Serial.print("Eje Y: ");
 Serial.print(chuck.readJoyY()); //Eje X del Joystick
 Serial.print(" ");

 //Boton Z
 if (chuck.buttonZ)
 Serial.print("Z");
 else
 Serial.print("-");

 Serial.print(" ");

 //Boton C
 if (chuck.buttonC)
 Serial.println("C");
 else
 Serial.println("-");
}
