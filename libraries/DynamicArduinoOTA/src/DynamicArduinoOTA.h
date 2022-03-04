#ifndef __DYNAMIC_ARDUINO_OTA_H
#define __DYNAMIC_ARDUINO_OTA_H

#include <WiFi.h>
#include <functional>
#include "Update.h"

#define INT_BUFFER_SIZE 16

typedef enum {
  OTA_IDLE,
  OTA_WAITAUTH,
  OTA_RUNUPDATE
} ota_state_t;

typedef enum {
  OTA_AUTH_ERROR,
  OTA_BEGIN_ERROR,
  OTA_CONNECT_ERROR,
  OTA_RECEIVE_ERROR,
  OTA_END_ERROR
} ota_error_t;

class DynamicArduinoOTAClass
{
  public:
    typedef std::function<void(void)> THandlerFunction;
    typedef std::function<void(ota_error_t)> THandlerFunction_Error;
    typedef std::function<void(unsigned int, unsigned int)> THandlerFunction_Progress;

    DynamicArduinoOTAClass();
    ~DynamicArduinoOTAClass();

    //Sets the service port. Default 3232
    DynamicArduinoOTAClass& setPort(uint16_t port);

    //Sets the device hostname. Default esp32-xxxxxx
    DynamicArduinoOTAClass& setHostname(const char *hostname);
    String getHostname();

    //Sets the password that will be required for OTA. Default NULL
    DynamicArduinoOTAClass& setPassword(const char *password);

    //Sets the password as above but in the form MD5(password). Default NULL
    DynamicArduinoOTAClass& setPasswordHash(const char *password);

    //Sets the partition label to write to when updating SPIFFS. Default NULL
    DynamicArduinoOTAClass &setPartitionLabel(const char *partition_label);
    String getPartitionLabel();

    //Sets if the device should be rebooted after successful update. Default true
    DynamicArduinoOTAClass& setRebootOnSuccess(bool reboot);

    //Sets if the device should advertise itself to Arduino IDE. Default true
    DynamicArduinoOTAClass& setMdnsEnabled(bool enabled);

    //This callback will be called when OTA connection has begun
    DynamicArduinoOTAClass& onStart(THandlerFunction fn);

    //This callback will be called when OTA has finished
    DynamicArduinoOTAClass& onEnd(THandlerFunction fn);

    //This callback will be called when OTA encountered Error
    DynamicArduinoOTAClass& onError(THandlerFunction_Error fn);

    //This callback will be called when OTA is receiving data
    DynamicArduinoOTAClass& onProgress(THandlerFunction_Progress fn);

    //Starts the DynamicArduinoOTA service
    void begin();

    //Ends the DynamicArduinoOTA service
    void end();

    //Call this in loop() to run the service
    void handle();

    //Gets update command type after OTA has started. Either U_FLASH or U_SPIFFS
    int getCommand();

    void setTimeout(int timeoutInMillis);

  private:
    int _port;
    String _password;
    String _hostname;
    String _partition_label;
    String _nonce;
    WiFiUDP _udp_ota;
    bool _initialized;
    bool _rebootOnSuccess;
    bool _mdnsEnabled;
    ota_state_t _state;
    int _size;
    int _cmd;
    int _ota_port;
    int _ota_timeout;
    IPAddress _ota_ip;
    String _md5;

    THandlerFunction _start_callback;
    THandlerFunction _end_callback;
    THandlerFunction_Error _error_callback;
    THandlerFunction_Progress _progress_callback;

    void _runUpdate(void);
    void _onRx(void);
    int parseInt(void);
    String readStringUntil(char end);

	uint8_t *buf;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_ARDUINOOTA)
extern DynamicArduinoOTAClass DynamicArduinoOTA;
#endif

#endif /* __DYNAMIC_ARDUINO_OTA_H */