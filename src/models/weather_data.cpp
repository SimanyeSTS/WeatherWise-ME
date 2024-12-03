#include "models/weather_data.hpp"

WeatherData WeatherData::fromJson(const nlohmann::json& j) {
    WeatherData data;
    data.temperature = j["main"]["temp"];
    data.humidity = j["main"]["humidity"];
    data.windSpeed = j["wind"]["speed"];
    data.description = j["weather"][0]["description"];
    data.cityName = j["name"];
    return data;
}

nlohmann::json WeatherData::toJson() const {
    return {
        {"temperature", temperature},
        {"humidity", humidity},
        {"windSpeed", windSpeed},
        {"description", description},
        {"cityName", cityName}
    };
}

WeatherForecast WeatherForecast::fromJson(const nlohmann::json& j) {
    WeatherForecast forecast;
    forecast.cityName = j["city"]["name"];
    
    for (const auto& point : j["list"]) {
        ForecastPoint fp;
        fp.timestamp = point["dt"];
        fp.temperature = point["main"]["temp"];
        fp.humidity = point["main"]["humidity"];
        fp.description = point["weather"][0]["description"];
        forecast.points.push_back(fp);
    }
    
    return forecast;
}

nlohmann::json WeatherForecast::toJson() const {
    nlohmann::json j;
    j["cityName"] = cityName;
    j["points"] = nlohmann::json::array();
    
    for (const auto& point : points) {
        j["points"].push_back({
            {"timestamp", point.timestamp},
            {"temperature", point.temperature},
            {"humidity", point.humidity},
            {"description", point.description}
        });
    }
    
    return j;
}