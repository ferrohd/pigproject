#ifndef WEATHER_FETCHER_H
#define	WEATHER_FETCHER_H

#include <pistache/net.h>
#include <pistache/http.h>
#include <pistache/client.h>

class WeatherFetcher
{
public:
	WeatherFetcher();
	~WeatherFetcher();
	
	void fetchCurrentWeather(std::string &s);

private:
	Pistache::Http::Client client;
};

#endif 
