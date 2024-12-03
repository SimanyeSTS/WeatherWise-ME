#pragma once
#include <memory>
#include "weather_api/weather_client.hpp"

class HttpServer {
public:
    explicit HttpServer(std::unique_ptr<WeatherClient> weather_client);
    ~HttpServer();

    void start(int port);
    void stop();

private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
};