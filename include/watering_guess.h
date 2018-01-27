#ifndef WATERING_GUESS_H
#define WATERING_GUESS_H

#include "weather_fetcher.h"

//Watering time max/min constant (minutes)
#define WATERING_TIME_AT_0_DEGREES 0.0
#define WATERING_TIME_AT_30_DEGREES 20.0


//Plants angle (E=0, N=90, W=180, S=270,)
#define PLANTS_ANGLE 185.0
#define ANGLE_WIDTH 30.0


//Numbers of structs (Array length)
#define HOURS 15

//Declaring typedef array of structs
typedef struct
{
	float temperature;
	float wind_speed;
	float wind_direction;
	int weather_id;
	int wind_incidence;
	float watering_multiplier;
	int watering_time;
	char weather_type[100];
} weather_data_struct;


class WateringGuess {
	private:
		void data_parsing(int times);
		float watering_multiplier(int weather_id, int times, int wind_incidence, float wind_direction, char weather_type[]);
		int wind_incidence_function(float wind_direction, int times);
		void new_data(weather_data_struct * weather);
		void square_brackets_remover(char * data);
		void data_log(float watering_time_per_degree, int wind_incidence, float watering_multiplier, int watering_time, int times, char * weather_type,
	                float temperature);
		
		int times = 0;


		//Declaring global array of structs
		weather_data_struct weather[HOURS]; //given for 9:00, 12:00, 15:00, 18:00, 21:00, 24:00, 3:00, 6:00 if fetched at 8:00 in the morning.
		

	public:
		void test();
		int giveGuess();
}; 

#endif