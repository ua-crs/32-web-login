/*
 *  Project 32-web-login - main.cpp
 *      Server validates login
 */

#include <Arduino.h>

#ifdef ESP32

#include <WiFi.h>
#include <WebServer.h>

#define ServerObject    WebServer

#else

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define ServerObject    ESP8266WebServer
#endif

#include "wifi_ruts.h"

ServerObject server(80);    // Create a webserver object that listens for HTTP request on port 80 

//#define VALID_USER      "user"
//#define VALID_PASS      "secret"

void
handleRoot( void )
{
    Serial.println( __FUNCTION__ );
    // When URI / is requested, send a web page with a button to toggle the LED
    server.send
    (
        200,
        "text/html",
        "<form action=\"/login\" method=\"POST\"><input type=\"text\" name=\"username\" "
        "placeholder=\"Username\"></br><input type=\"password\" name=\"password\" placeholder=\"Password\"></br><input "
        "type=\"submit\" value=\"Login\"></form>"
    );
}

void
handleLogin( void )
{  
    // If a POST request is made to URI /login
    Serial.println( __FUNCTION__ );

    // POST request has username and password data ?  
    if( !server.hasArg("username") || !server.hasArg("password") ||
        server.arg("username") == NULL || server.arg("password") == NULL )
    {
        server.send(400, "text/plain", "400: Invalid Request");
        return;
    }
    if( server.arg("username") == VALID_USER && server.arg("password") == VALID_PASS )
        server.send(200, "text/html", "<h1>Welcome, " + server.arg("username") + "!</h1><p>Login successful</p>");
    else
        server.send(401, "text/plain", "401: Unauthorized");
}

void
handleNotFound( void )
{
    Serial.println( __FUNCTION__ );
    server.send(404, "text/plain",
                "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void
setup(void)
{
    Serial.begin(SERIAL_BAUD); // Start the Serial communication to send messages to the computer
    delay(10);
    Serial.println();

    connect_wifi(MY_SSID, MY_PASS);

    server.on( "/",         HTTP_GET,   handleRoot);
    server.on( "/login",    HTTP_POST,  handleLogin);
    server.onNotFound( handleNotFound ); // When a client requests an unknown URI

    server.begin(); // Actually start the server
    Serial.println("HTTP server started");
}

void
loop(void)
{
    server.handleClient(); // Listen for HTTP requests from clients
}
