#ifndef WEATHER_FETCHER_H
#define	WEATHER_FETCHER_H

class WeatherFetcher
{
public:
	WeatherFetcher();
	~WeatherFetcher();
	
	char* fetchCurrentWeather();

private:
	Http::Client client;
};

#endif 
