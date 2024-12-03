#include "weather_api/weather_client.hpp"
#include "server/http_server.hpp"
#include <spdlog/spdlog.h>
#include <cstdlib>

int main() {
    try {
        // Get API key from environment
        const char* api_key = std::getenv("OPENWEATHER_API_KEY");
        if (!api_key) {
            spdlog::error("OPENWEATHER_API_KEY environment variable not set");
            return 1;
        }
        
        // Create weather client
        auto weather_client = std::make_unique<WeatherClient>(api_key);
        
        // Create and start server
        HttpServer server(std::move(weather_client));
        server.start(8080);
        
        return 0;
    } catch (const std::exception& e) {
        spdlog::error("Fatal error: {}", e.what());
        return 1;
    }
}