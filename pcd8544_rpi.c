/*
=================================================================================
 Name        : pcd8544_rpi.c
 Version     : 0.1

 Copyright (C) 2012 by Andre Wussow, 2012, desk@binerry.de

 Description :
     A simple PCD8544 LCD (Nokia3310/5110) for Raspberry Pi for displaying some system informations.
	 Makes use of WiringPI-library of Gordon Henderson (https://projects.drogon.net/raspberry-pi/wiringpi/)

	 Recommended connection (http://www.raspberrypi.org/archives/384):
	 LCD pins      Raspberry Pi
	 LCD1 - GND    P06  - GND
	 LCD2 - VCC    P01 - 3.3V
	 LCD3 - CLK    P11 - GPIO0
	 LCD4 - Din    P12 - GPIO1
	 LCD5 - D/C    P13 - GPIO2
	 LCD6 - CS     P15 - GPIO3
	 LCD7 - RST    P16 - GPIO4
	 LCD8 - LED    P01 - 3.3V 

================================================================================
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
================================================================================
 */
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include "PCD8544.h"

// pin setup
int _din = 1;
int _sclk = 0;
int _dc = 2;
int _rst = 4;
int _cs = 3;
  
// lcd contrast 
//may be need modify to fit your screen!  normal: 30- 90 ,default is:45 !!!maybe modify this value!
// int contrast = 45;  
// make screen font more deeper
int contrast = 50;  
  
int main (void)
{
  // print infos
  printf("Raspberry Pi PCD8544 sysinfo display\n");
  printf("========================================\n");
  
  // check wiringPi setup
  if (wiringPiSetup() == -1)
  {
	printf("wiringPi-Error\n");
    exit(1);
  }
  
  // init and clear lcd
  LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
  LCDclear();
  
  // show logo
  LCDshowLogo();
  
  delay(2000);
  
  for (;;)
  {
	  // clear lcd
	  LCDclear();
	  
	  // get system usage / info
	  struct sysinfo sys_info;
	  if(sysinfo(&sys_info) != 0)
	  {
		printf("sysinfo-Error\n");
	  }
	  
	  // uptime
	  char uptimeInfo[13];
	  unsigned long uptime = sys_info.uptime / 60;
	  sprintf(uptimeInfo, "RUN %ld MIN", uptime);
	  
	  // cpu info
	  char cpuInfo[13]; 
	  unsigned long avgCpuLoad = sys_info.loads[0] / 1000;
	  //sprintf(cpuInfo, "CPU %ld%%", avgCpuLoad);
	  sprintf(cpuInfo, "CPU %ld%%", avgCpuLoad);

	  
	  // ram info
	  char ramInfo[13]; 
	  unsigned long freeRam = sys_info.freeram / 1024 / 1024;
	  unsigned long totalRam = sys_info.totalram / 1024 / 1024;
	  sprintf(ramInfo, "RAM %ld/%ld MB", freeRam, totalRam);
          
          // swap info
          char swapInfo[13];
          unsigned long freeSwap = sys_info.freeswap / 1024 / 1024;
          unsigned long totalSwap = sys_info.totalswap / 1024 / 1024;
          sprintf(swapInfo, "SWP %ld/%ld MB", freeSwap, totalSwap);


          // Ip info
          char wlanInfo[13];
	int wlan_fd;
	struct ifreq wlan_ifr;
	wlan_fd = socket(AF_INET, SOCK_DGRAM, 0);
	/* I want to get an IPv4 IP address */
	wlan_ifr.ifr_addr.sa_family = AF_INET;
	/* I want IP address attached to "wlan0" */
	strncpy(wlan_ifr.ifr_name, "wlan0", IFNAMSIZ-1);
	ioctl(wlan_fd, SIOCGIFADDR, &wlan_ifr);
	close(wlan_fd);
	/* display result */
	sprintf(wlanInfo, "W %s", inet_ntoa(((struct sockaddr_in *)&wlan_ifr.ifr_addr)->sin_addr));         
	
          char ethInfo[13];
	int eth_fd;
	struct ifreq eth_ifr;
	eth_fd = socket(AF_INET, SOCK_DGRAM, 0);
	/* I want to get an IPv4 IP address */
	eth_ifr.ifr_addr.sa_family = AF_INET;
	/* I want IP address attached to "eth0" */
	strncpy(eth_ifr.ifr_name, "eth0", IFNAMSIZ-1);
	ioctl(eth_fd, SIOCGIFADDR, &eth_ifr);
	close(eth_fd);
	/* display result */
	sprintf(ethInfo, "E %s", inet_ntoa(((struct sockaddr_in *)&eth_ifr.ifr_addr)->sin_addr));         

	  
	  // build screen
	  //LCDdrawstring(0, 0, "Raspberry Pi:");
	  //LCDdrawline(0, 10, 83, 10, BLACK);
	  LCDdrawstring(0, 0, uptimeInfo);
	  LCDdrawstring(0, 8, cpuInfo);
	  LCDdrawstring(0, 16, ramInfo);
	  LCDdrawstring(0, 24, swapInfo);
	  LCDdrawstring(0, 32, wlanInfo);
	  LCDdrawstring(0, 40, ethInfo);
	  LCDdisplay();
	  
	  delay(1000);
  }
  
    //for (;;){
  //  printf("LED On\n");
  //  digitalWrite(pin, 1);
  //  delay(250);
  //  printf("LED Off\n");
  //  digitalWrite(pin, 0);
  //  delay(250);
  //}

  return 0;
}
