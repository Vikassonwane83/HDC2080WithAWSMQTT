# ESP32 Wrover E HDC2080 Temperature Sensor Library

This project involves building a library for the HDC2080 temperature sensor and uploading real-time sensor data to an AWS server using the ESP32 Wrover-E microcontroller.

## Specifications

- **Micro-controller**: ESP32 Wrover-E
- **Espressif IDF Version**: 4.4
- **Datasheet**: [ESP32 Wrover-E Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wrover-e_esp32-wrover-ie_datasheet_en.pdf)
- **Environment**: Espressif’s Development Framework ([ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/windows-setup.html))

## Key Points to Look Out For

1. Library building based on sensor datasheet.
2. Comprehensive handling of WiFi and AWS fail cases.
3. Maintenance of WiFi SSID and Password and last temperature data in FAT file system.
4. Creation of different components for AWS, WiFi, Sensor, and Storage implementations.
5. RTOS implementation for all components and global variable sharing.

## Usage

- Replace dummy endpoint URL and certificate IDs with actual AWS MQTT Broker details before deployment.
- Configure WiFi SSID and Password in the FAT file system.

## Components

- **AWS Component**: Responsible for handling communication with AWS services.
- **WiFi Component**: Manages WiFi connectivity and credentials.
- **Sensor Component**: Implements functionality related to the HDC2080 temperature sensor.
- **Storage Component**: Handles storage of WiFi credentials and last temperature data.

## Build Instructions

1. Clone this repository.
2. Set up ESP-IDF environment.
3. Configure WiFi SSID and Password in `wifi_component`.
4. Replace dummy AWS endpoint URL and certificate IDs in `aws_component`.
5. Build and flash the project to ESP32 Wrover-E.

## Contributors

- Vikas Sonwane (vikas.sonwane83@gmail.com)

Feel free to contribute by opening issues, providing feedback, or submitting pull requests. Happy coding!
