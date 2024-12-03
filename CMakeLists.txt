cmake_minimum_required(VERSION 3.15)
project(weatherwise_backend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Add source files
add_executable(weatherwise_server
    src/main.cpp
    src/weather_api/weather_client.cpp
    src/server/http_server.cpp
    src/models/weather_data.cpp
)

target_include_directories(weatherwise_server PRIVATE include)
target_link_libraries(weatherwise_server PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
    fmt::fmt
    spdlog::spdlog
)

# include/weather_api/weather_client.hpp
#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "models/weather_data.hpp"

class WeatherClient {
public:
    explicit WeatherClient(std::string api_key);
    ~WeatherClient();

    WeatherData getCurrentWeather(const std::string& city);
    WeatherForecast getForecast(const std::string& city);

private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
    std::string makeRequest(const std::string& endpoint, const std::string& params);
};