#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>

#define node_address 18
uint8_t gateway_address=10;

int analogpin=3;
uint8_t buf[50];
uint8_t len = sizeof(buf);
uint8_t from;

/* configure driver */
RH_RF95 driver(8,3); // for Adafruit feather mo LoRa
//RH_RF95 driver; 
RHReliableDatagram manager(driver,node_address);


/* Function to convert float to character to send the message */
 char *ftoa(char *a, double f, int precision)
{
 long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  char *ret = a;
 long heiltal = (long)f;
 itoa(heiltal, a, 10);
 while (*a != '\0') a++;
 *a++ = '.';
 long desimal = abs((long)((f - heiltal) * p[precision]));
 itoa(desimal, a, 10);
 return ret;
}

void setup() 
{
  pinMode(4, OUTPUT); // for Adafruit feather mo LoRa 
  digitalWrite(4, HIGH); // for Adafruit feather mo LoRa
  Serial.begin(9600);
  //while (!Serial) ; // Wait for serial port to be available
  /* initialise the LoRa modem */
  if (manager.init())
    {
      Serial.println("LoRa Successfully Initialised ");
    }
  else
    {
      Serial.println("LoRa initialization failed");
    }
  /* Set the desired LoRa parameters */  
  driver.setFrequency(433.0);
  driver.setTxPower(20,false);  
}

void loop()
{
  char data[10];
  char mes[10];
  int Level=analogRead(analogpin);
  ftoa(mes,Level,2);
  sprintf(data,"$%d#%s",node_address,mes);
  Serial.print("Data to be sent:");
  Serial.println(data);
  if(manager.sendtoWait((uint8_t*)data,sizeof(data),gateway_address))
    {
      Serial.println("Data transmission success");
    }
  else
    {
      Serial.println("Data transmission failed");
    }
  
  delay(47000);
}


