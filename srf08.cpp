 /************************************************ */
 /*  Educational tutorial for Arduino in robotics  */
 /*  Vyukovy Tutorial pro pouziti Arduina v robotice*/
 /*  File: srf08.cpp                               */
 /*  Author: Jan Beran                             */
 /*  Date: autumn 2019                             */
 /*                                                */
 /* This file is a part of author´s bachelor thesis*/
 /*                                                */
 /**************************************************/

#include <Arduino.h>
#include <Wire.h>
#include "srf08.h"

Sonars::Sonars()
{
  Wire.begin();
}

void Sonars::set_measurement(byte unit, uint8_t address)
{
  Wire.beginTransmission(address);
  Wire.write(REG_CMD);
  Wire.write(unit);
  Wire.endTransmission();
  /*V dokumentaci se pise 60 ms, ale to nemusi stacit*/
  delay(100);
}

sonar_data Sonars::get_distances(byte unit)
{
  uint16_t data[6];
  this->set_measurement(unit, SRF08_ADDRESS_BROADCAST);
  
  /* Ignorovani prvnich dvou registru*/
  for(int i = 0; i < NUM_OF_SONARS; i++)
  {
    Wire.beginTransmission(srf08_addresses[i]);
    Wire.write(0x02);                           
    Wire.endTransmission();

    Wire.requestFrom(srf08_addresses[i], 2); 
    while(Wire.available() < 0);
    byte high =  Wire.read();
    byte low =  Wire.read();
    data[i] = (uint16_t)(high << 8)+low;
  }

  sonar_data ret_data;
  ret_data.front = data[0];
  ret_data.front_left = data[1];
  ret_data.front_right = data [2];
  ret_data.back_left = data[3];
  ret_data.back_right = data[4];
  ret_data.back =  data[5];

  return ret_data;
}
