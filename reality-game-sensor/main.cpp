/* 
 * The entry point for compiling our sensor class.
 * 
 * By uncommenting those header files, you can use classes provided in them.
 * 
 * The architecture contains several threads. A special one is the event thread that dispatch events.
 * The process logic is divided into 3 threads: sensor, motion, and wifi
 *  - The sensor thread will register a event that get data from the real world. (1ms)
 *  - The motion thread determine the user motion type based on the sensor data in the buffer. (10ms)
 *  - The wifi thread send motion type data to the game server for further processing. (100ms)
 */

#include "Callback.h"
#include "mbed.h"
#include <cstdio>

// wifi module header
#include "my_wifi_sensor.h"

// BLE module header
// #include "my_ble_tag.h"
// #include "my_ble_sensor.h"

// handle console output
static BufferedSerial serial_port(USBTX, USBRX);
FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port; 
}

// EventQueue
EventQueue event_queue(64 * EVENTS_EVENT_SIZE);
Thread sensor_thread;
Thread event_thread;

int main() 
{
    /* Wifi Data Sensor */

    // Declare a  new wifi sensor instance
    WifiDataSensor* wifi_sensor = new WifiDataSensor(event_queue);
    
    // Start event dispatch
    event_thread.start(callback(&event_queue, &EventQueue::dispatch_forever));

    // Start sensor sensing
    sensor_thread.start(callback(wifi_sensor, &WifiDataSensor::startSensing));

    // Connect to wifi
    wifi_sensor->connectWifi();

    // Connect to host computer
    wifi_sensor->connectHost();

    // Start sending data to host computer
    wifi_sensor->start();

    /* BLE Tag */
    // BLE &ble = BLE::Instance();
    // events::EventQueue event_queue;
    // RealGameTaggingService real_game_service;

    // // this process will handle basic ble setup and advertising for us 
    // GattServerTagProcess ble_process(event_queue, ble);

    // // once it's done it will let us continue with our demo 
    // ble_process.on_init(callback(&real_game_service, &RealGameTaggingService::start));

    // ble_process.start();

    /* BLE Data Sensor */
    // BLE &ble = BLE::Instance();
    // events::EventQueue event_queue;
    // BLEDataSensor motion_sensor;

    // event_thread.start(callback(&event_queue, &EventQueue::dispatch_forever));
    // sensor_thread.start(callback(&motion_sensor, &BLEDataSensor::startSensing));

    // // this process will handle basic ble setup and advertising for us 
    // GattServerMotionProcess ble_process(event_queue, ble);

    // // once it's done it will let us continue with our demo 
    // ble_process.on_init(callback(&motion_sensor, &BLEDataSensor::start));

    // ble_process.start();

    

    return 0;
}