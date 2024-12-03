#include "server/http_server.hpp"
#include <httplib.h>
#include <spdlog/spdlog.h>

class HttpServer::Impl {
public:
    explicit Impl(std::unique_ptr<WeatherClient> client)
        : weather_client_(std::move(client)), server_() {
        
        // Set up routes
        server_.Get("/api/weather", [this](const httplib::Request& req, httplib::Response& res) {
            auto city = req.get_param_value("city");
            if (city.empty()) {
                res.status = 400;
                res.set_content("City parameter is required", "text/plain");
                return;
            }
            
            try {
                auto weather = weather_client_->getCurrentWeather(city);
                res.set_content(weather.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                res.status = 500;
                res.set_content(e.what(), "text/plain");
            }
        });
        
        server_.Get("/api/forecast", [this](const httplib::Request& req, httplib::Response& res) {
            auto city = req.get_param_value("city");
            if (city.empty()) {
                res.status = 400;
                res.set_content("City parameter is required", "text/plain");
                return;
            }
            
            try {
                auto forecast = weather_client_->getForecast(city);
                res.set_content(forecast.toJson().dump(), "application/json");
            } catch (const std::exception& e) {
                res.status = 500;
                res.set_content(e.what(), "text/plain");
            }
        });
        
        // Enable CORS
        server_.set_exception_handler([](const auto& req, auto& res, std::exception_ptr ep) {
            res.set_header("Access-Control-Allow-Origin", "*");
        });
    }
    
    void start(int port) {
        spdlog::info("Starting server on port {}", port);
        server_.listen("0.0.0.0", port);
    }
    
    void stop() {
        server_.stop();
    }

private:
    std::unique_ptr<WeatherClient> weather_client_;
    httplib::Server server_;
};

HttpServer::HttpServer(std::unique_ptr<WeatherClient> weather_client)
    : pimpl(std::make_unique<Impl>(std::move(weather_client))) {}

HttpServer::~HttpServer() = default;

void HttpServer::start(int port) {
    pimpl->start(port);
}

void HttpServer::stop() {
    pimpl->stop();
}