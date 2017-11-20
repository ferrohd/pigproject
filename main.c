#include <stdio.h>
#include <stdlib.h>


//Watering time max/min constant (MINUTES OR SECONDS?)

#define WATERING_TIME_AT_0_DEGREES 0
#define WATERING_TIME_AT_30_DEGREES 20


//Plants angle (E=0, N=90, W=180, S=270,)

#define PLANTS_ANGLE 185


//Numbers of informations (Array lenght)

#define HOURS 15 // 0-7 new/this day | 8-15 past day


//Declaring global variable time

int time;


//Declaring functions

float watering_multiplier(int weather_id, int time, int wind_incidence, float wind_direction);
int wind_incidence_function(float wind_direction, int time);
//void struct_switch(struct weather);



int main(void) { 
	
	struct 
	{
		int temperature;
		float wind_speed;
		float wind_direction;
		int weather_id;
		float wind_incidence;
		float watering_multiplier;
	} weather[HOURS];		//given for 9:00, 12:00, 15:00, 18:00, 21:00, 24:00, 3:00, 6:00 if fetched at 8:00 in the morning.


	
	int waterting_time_per_degree, watering_time;

	
	//Calculate watering_time_per_degree based on constants (#define)
	
	waterting_time_per_degree = (WATERING_TIME_AT_30_DEGREES - WATERING_TIME_AT_0_DEGREES)/30;

	
	//Calculating watering_multiplier based on weather_id and time

	weather[time].watering_multiplier = watering_multiplier(weather[time].weather_id, time, weather[time].wind_incidence, weather[time].wind_direction);


	//Calculate watering time with weather multiplier

	watering_time = waterting_time_per_degree * weather[time].temperature * weather[time].watering_multiplier;

	
	//Calculating wind incidence (If it rains, will the rain fall in the directions of the plants? 1=Yes, 0=No)

	weather[time].wind_incidence = wind_incidence_function(weather[time].wind_direction, time);

	return 0;
}


//When time increse switch structs palces

//void struct_switch (struct weather) {
//	int i;
//	for (i = HOURS; i = 1; i--) {
//		weather[i] = weather[i-1];
//		//Now you can assing weather[0] the new values
//	}
//	printf("La temperatura dell'array 1 è: %d\n",weather[1].temperature );
//}


//Calculating wind_incidence based on angle (If it rains, will the rain fall in the directions of the plants? 1=Yes, 0=No)

int wind_incidence_function(float wind_direction, int time) {
	if ((PLANTS_ANGLE >= (wind_direction-30)) && (PLANTS_ANGLE <= (wind_direction+30))) {
		return 1;
	}
	return 0;
}


//Function that calculates weather multiplier for every weather_id WITH wind_incidence (rain, sun, drizzle apocalypse etc..)

float watering_multiplier(int weather_id, int time, int wind_incidence, float wind_direction) {


	//SNOW

	if ((weather_id >= 600) && (weather_id <= 622)) {
		return 0;
	}


	//EXTREME

	if (weather_id == 906) { //HAIL (Grandine)
		return 0;
	}
	if (weather_id == 904) { //HOT
		return 1,25;
	}
	if ((weather_id >= 900) && (weather_id <= 902)) { //TORNADO/HURRYCANE
		return 0;
	}


	//THUNDERSTORM

	if ((weather_id >= 200) && (weather_id <= 232) && (wind_incidence_function(wind_direction, time))) {
		if (weather_id == 200) { //THUNDERSTORM WITH LIGHT RAIN
			return 0,70;
		}
		if (weather_id == 201) { //THUNDERSTORM WITH RAIN
			return 0,50;
		}
		if (weather_id == 202) { //THUNDERSTORM WITH HEAVY RAIN
			return 0,25;
		}
		if (weather_id == 210) { //LIGHT THUNDERSTORM
			return 0,90;
		}
		if (weather_id == 211) { //THUNDERSTORM
			return 0,80;
		}
		if (weather_id == 212) { //HEAVY THUNDERSTORM
			return 0,60;
		}
		if (weather_id == 221) { //RUGGED THUNDERSTORM (Temporale irregolare)
			return 0,70;
		}
		if (weather_id == 230) { //THUNDERSTORM WITH LIGHT DRIZZLE
			return 0,80;
		}
		if (weather_id == 231) { //THUNDERSTORM WITH DRIZZLE
			return 0,70;
		}
		if (weather_id == 232) { //THUNDERSTORM WITH HEAVY DRIZZLE
			return 0,55;
		}
	}


	//DRIZZLE

	if ((weather_id >= 300) && (weather_id <= 321) && (wind_incidence_function(wind_direction, time))) {
		if (weather_id == 300) { //LIGHT INTENSITY DRIZZLE
			return 0,90;
		}
		if (weather_id == 301) { //DRIZZLE
			return 0,80;
		}
		if (weather_id == 302) { //HEAVY INTENSITY DRIZZLE
			return 0,50;
		}
		if (weather_id == 310) { //LIGHT INTENSITY DRIZZLE RAIN
			return 0,65;
		}
		if (weather_id == 311) { //DRIZZLE RAIN
			return 0,50;
		}
		if (weather_id == 312) { //HEAVY INTENSITY DRIZZLE RAIN
			return 0,40;
		}
		if (weather_id == 313) { //SHOWER RAIN AND DRIZZLE
			return 0,30;
		}
		if (weather_id == 314) { //HEAVY SHOWER RAIN AND DRIZZLE
			return 0,20;
		}
		if (weather_id == 321) { //SHOWER DRIZZLE
			return 0,35;
		}
	}


	//RAIN

	if ((weather_id >= 500) && (weather_id <= 520) && (wind_incidence_function(wind_direction, time))) {
		if (weather_id == 500) { //LIGHT RAIN
			return 0,80;
		}
		if (weather_id == 501) { //MODERATE RAIN
			return 0,70;
		}
		if (weather_id == 502) { //HEAVY INTENSITY RAIN
			return 0,40;
		}
		if (weather_id == 503) { //VERY HEAVY RAIN
			return 0,30;
		}
		if (weather_id == 504) { //EXTREME RAIN
			return 0,10;
		}
		if (weather_id == 511) { //FREEZING RAIN
			return 0;
		}
		if ((weather_id >= 520) && (weather_id <= 531)) { //LIGHT INTNENSITY SHOWER RAIN/SHOWER RAIN/RAGGED SHOWER RAIN
			return 0;
		}
	}
	return 1;
}