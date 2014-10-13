#!/bin/sh
cc -o rasp_lcd_info pcd8544_rpi.c PCD8544.c  -L/usr/local/lib -lwiringPi
sudo service rasp_lcd_info stop
sudo cp ./rasp_lcd_info /usr/local/bin
if [ ! -x /etc/init.d/rasp_lcd_info ]; then
  sudo cp ./rasp_lcd_info.init /etc/init.d/rasp_lcd_info
  sudo chmod a+x /etc/init.d/rasp_lcd_info
fi
if [ ! -x /etc/rc3.d/S99rasp_lcd_info ]; then
  #sudo ln -sf /etc/init.d/rasp_lcd_info /etc/rc3.d/S99rasp_lcd_info
  #sudo chmod a+x /etc/rc3.d/S99rasp_lcd_info
  sudo update-rc.d -f rasp_lcd_info defaults 99
fi
sudo service rasp_lcd_info start
