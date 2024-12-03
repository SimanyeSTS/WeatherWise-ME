#include "weather_api/weather_client.hpp"
#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <sstream>

class WeatherClient::Impl {
public:
    explicit Impl(std::string api_key) : api_key_(std::move(api_key)) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~Impl() {
        curl_global_cleanup();
    }

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        auto* str = static_cast<std::string*>(userp);
        str->append(static_cast<char*>(contents), size * nmemb);
        return size * nmemb;
    }

    std::string makeRequest(const std::string& url) {
        std::string response_string;
        auto* curl = curl_easy_init();
        
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
            
            auto res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(res));
            }
            
            curl_easy_cleanup(curl);
        }
        
        return response_string;
    }

    std::string api_key_;
};

WeatherClient::WeatherClient(std::string api_key)
    : pimpl(std::make_unique<Impl>(std::move(api_key))) {}

WeatherClient::~WeatherClient() = default;

WeatherData WeatherClient::getCurrentWeather(const std::string& city) {
    std::stringstream url;
    url << "http://api.openweathermap.org/data/2.5/weather?q="
        << city << "&appid=" << pimpl->api_key_ << "&units=metric";
    
    auto response = pimpl->makeRequest(url.str());
    auto json = nlohmann::json::parse(response);
    
    return WeatherData::fromJson(json);
}

WeatherForecast WeatherClient::getForecast(const std::string& city) {
    std::stringstream url;
    url << "http://api.openweathermap.org/data/2.5/forecast?q="
        << city << "&appid=" << pimpl->api_key_ << "&units=metric";
    
    auto response = pimpl->makeRequest(url.str());
    auto json = nlohmann::json::parse(response);
    
    return WeatherForecast::fromJson(json);
}