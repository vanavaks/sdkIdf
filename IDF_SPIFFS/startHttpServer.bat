#for OTA example working folder must contain file OTA.OTA.bin
#esp8266 send request to this server for getting this file

@echo on
echo starting python http server

cd e:\myProjects\ESP8266\Projects\RTOS3_SPIFFS_1\build
#for python 3.x
python -m http.server 8070
#for python 2.x
#python -m SimpleHTTPServer 8070

pause
