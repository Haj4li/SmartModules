This project is a small part of Homino (a NodeMCU-based smart house project). The code includes functionalities such as controlling room lights, monitoring gas levels, detecting flames, measuring temperature and humidity, and parking assistance using an ultrasonic module.

Table of Contents

    Prerequisites
    Code Structure
    Configuration
    Main Functions
    Web Server
    API Endpoints
    Authentication
    Setup
    Usage

Prerequisites

To run this code on your NodeMCU device, you need the following components:

    NodeMCU board
    DHT11 temperature and humidity sensor
    Photocell sensor
    Buzzer
    Ultrasonic module
    Flame sensor
    WiFi connection

Code Structure

The code is organized into several files, each serving a specific purpose:

    Helper.h: Contains helper functions and classes for interacting with hardware modules.
    Auth.h: Contains functions for authentication and session management.
    Smart.h: Defines the hardware modules used in the smart house project.
    Pages.h: Contains HTML pages for the web server.

Configuration

Before running the code, you need to configure the following settings:

    WiFi SSID and password: Set the ssid and password variables in the code to match your WiFi network credentials.

Main Functions

The main functions in the code are as follows:
GetData()

This function is called periodically to fetch data from various sensors and update the state of the hardware modules. It performs the following tasks:

    Reads temperature and humidity values from the DHT11 sensor.
    Checks the flame sensor and gas levels to determine if there is a fire or a gas leak.
    Controls the alarm module based on the sensor readings.
    Measures the distance using the ultrasonic module to provide parking assistance.

handleRoot()

This function handles the root URL ("/") and generates the main web page. It displays the following information:

    Gas level
    Distance measured by the ultrasonic module
    House status (normal, on fire, gas leak, intruder)
    Humidity level
    Temperature

The page also includes a button to toggle the room lights and a link to log out.
haldleSetValue()

This function handles the "/setvalue" endpoint and toggles the state of the room lights module. It is called when the toggle button on the web page is clicked.
handleLogin_Page()

This function handles the "/login" endpoint and generates the login page. It displays a login form where the user can entertheir username and password.
handleGetValue()

This function handles the "/getvalue" endpoint and returns the current values of various sensors and modules. It is called periodically by the web page to update the displayed values.
validateLogin()

This function validates the username and password submitted through the login form. If the credentials are correct, it authenticates the user and redirects them to the main page. Otherwise, it increments the login attempts counter and triggers the alarm module if the maximum number of attempts is reached.
Web Server

The code sets up an HTTP server using the ESP8266WebServer library to handle incoming requests and serve web pages.
API Endpoints

The code defines the following API endpoints:

    "/" (root): Displays the main page with sensor values and control buttons.
    "/login": Displays the login page and handles the login form submission.
    "/loginvalid": Validates the login credentials and authenticates the user if they are correct.
    "/setvalue": Toggles the state of the room lights module.
    "/getvalue": Returns the current values of various sensors and modules.

Authentication

The code includes a simple authentication mechanism using cookies. The user is required to log in with a valid username and password to access the main page. After successful login, a session cookie is set to maintain the user's authenticated state.
Setup

The code sets up the hardware modules, initializes the WiFi connection, and starts the web server.
Usage

To use the smart house system:

    Connect the required hardware modules to your NodeMCU board.
    Set the WiFi SSID and password in the code to match your network credentials.
    Flash the code to your NodeMCU board.
    Power on the NodeMCU board.
    Connect your device to the WiFi network created by the NodeMCU (if using AP mode) or to your existing WiFi network (if using STA mode).
    Open a web browser and navigate to the IP address of the NodeMCU board.
    Log in with the provided username and password.
    Use the web interface to monitor sensor values, control the room lights, and view the status of the smart house.
