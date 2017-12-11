#include <atomic>
#include <stdio.h>
#include <iostream>

#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/client.h>

using namespace Pistache;
using namespace Pistache::Http;

#include "weather_fetcher.h"


WeatherFetcher::WeatherFetcher(){
    std::cout << "weather_fetcher ready" << std::endl;
}


WeatherFetcher::~WeatherFetcher(){
    std::cout << "destroying weather_fetcher..." << std::endl;
    client.shutdown();
}

char* WeatherFetcher::fetchCurrentWeather(){
    std::cout << "making request" << std::endl;
    std::string page = "http://api.openweathermap.org/data/2.5/weather?q=MIlan,it&mode=json&appid=013b5d74095d80de286d5f0d93a85fe5";
    int count = 1;

    auto opts = Http::Client::options()
        .threads(1)
        .maxConnectionsPerHost(8);
    client.init(opts);

    std::vector<Async::Promise<Http::Response>> responses;

    std::atomic<size_t> completedRequests(0);
    std::atomic<size_t> failedRequests(0);

    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < count; ++i) {
        auto resp = client.get(page).cookie(Http::Cookie("FOO", "bar")).send();
        resp.then([&](Http::Response response) {
            ++completedRequests;
            std::cout << "Response code = " << response.code() << std::endl;
            auto body = response.body();
            if (!body.empty())
                std::cout << "Response body = " << body << std::endl;
        }, Async::IgnoreException);
        responses.push_back(std::move(resp));
    }

    auto sync = Async::whenAll(responses.begin(), responses.end());
    Async::Barrier<std::vector<Http::Response>> barrier(sync);

    barrier.wait_for(std::chrono::seconds(5));

    auto end = std::chrono::system_clock::now();
    std::cout << "Summary of execution" << std::endl
              << "Total number of requests sent     : " << count << std::endl
              << "Total number of responses received: " << completedRequests.load() << std::endl
              << "Total number of requests failed   : " << failedRequests.load() << std::endl
              << "Total time of execution           : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

}