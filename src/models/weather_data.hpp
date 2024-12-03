#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct WeatherData {
    double temperature;
    double humidity;
    double windSpeed;
    std::string description;
    std::string cityName;
    
    static WeatherData fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;
};

struct ForecastPoint {
    int timestamp;
    double temperature;
    double humidity;
    std::string description;
};

struct WeatherForecast {
    std::string cityName;
    std::vector<ForecastPoint> points;
    
    static WeatherForecast fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;
};