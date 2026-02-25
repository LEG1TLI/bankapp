#include "crow.h"
#include <iostream>

int main()
{
    //create a webserver
    crow::SimpleApp app;


    //defines endpoints for the webserver
    CROW_ROUTE(app, "/")
    //lambda function to handle requests to the path 
    ([](){
        return "Hello! Bank Server is Running!";
    });
    //access port 8080 and run server with multiple threads
    app.port(8080).multithreaded().run();
}