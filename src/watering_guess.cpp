/*
  Copyright (c) 2017-2018 Alessandro Ferrara and Andrea Taglia

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/cJSON.h"

#include <fstream>
#include <iostream>

#include "../include/watering_guess.h"

#define ANSI_COLOR_GREEN   "\x1b[1;92m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int WateringGuess::giveGuess() {

	//Declaring variable(s)
	float watering_time_per_degree;

	//Parses data and makes room for new data
	new_data(weather);

	//Calculate watering_time_per_degree based on constants (#define)
	watering_time_per_degree = (WATERING_TIME_AT_30_DEGREES - WATERING_TIME_AT_0_DEGREES) / 30;

	//Calculating wind incidence (If it rains, will the rain fall in the directions of the plants? 1=Yes, 0=No)
	weather[times].wind_incidence = wind_incidence_function(weather[times].wind_direction, times);

	//Calculating watering_multiplier based on weather_id and time
	weather[times].watering_multiplier = watering_multiplier(weather[times].weather_id, times, weather[times].wind_incidence, weather[times].wind_direction, weather[times].weather_type);

	//Calculate watering time with weather multiplier
	weather[times].watering_time = watering_time_per_degree * weather[times].temperature * weather[times].watering_multiplier;

	//Printing data on screen/console
	data_log(watering_time_per_degree, weather[times].wind_incidence, weather[times].watering_multiplier, weather[times].watering_time, times, weather[times].weather_type, weather[times].temperature);

	return 0;
}


//Prints to screen/console data based on time
void WateringGuess::data_log(float watering_time_per_degree, int wind_incidence, float watering_multiplier, int watering_time, int times, char * weather_type,
                float temperature) {
	//Get current time
	time_t theTime = time(NULL);
	struct tm *aTime = localtime(&theTime);
	int hour=aTime->tm_hour;
	int min=aTime->tm_min;

	//print to console
	printf("\n\n---------DATA---------\n");
	printf("Time: %d:%d\n", hour, min);
	printf("Temperature: %.2f (Celsius)\n", temperature);
	printf("Watering time per degree set as: %.2f (minutes)\n", watering_time_per_degree);
	printf("Wind incidence: %d (1=Yes 0=No)\n", weather[times].wind_incidence);
	printf("Watering multiplier: %.2f\n", weather[times].watering_multiplier);
	printf("Weather is: %s\n", weather[times].weather_type);
	printf("________________________________\n");
	printf("|  WATERING TIME: %d (minutes) |\n", weather[times].watering_time);
	printf("--------------------------------\n");

	//append data to index file
	std::ofstream ofs;
	ofs.open ("src/index.html", std::ofstream::out | std::ofstream::app);
	ofs << "<br>\n\n___________DATA__________\n<br>"
		"Time: " << hour << ":" << min << "\n<br>"
		"Temperature: " << temperature << " (Celsius)\n<br>"
		"Watering time per degree set as: " << watering_time_per_degree << " (minutes)\n<br>"
		"Wind incidence: " << weather[times].wind_incidence << " (1=Yes 0=No)\n<br>"
		"Watering multiplier: " << weather[times].watering_multiplier << "\n<br>"
		"Weather is: " << weather[times].weather_type <<"\n<br>"
		"<b>WATERING TIME: " << weather[times].watering_time << " (minutes)</b>\n<br>";
	ofs.close();
}


//Switch the place of the structs to make space for the new parsed data
void WateringGuess::new_data(weather_data_struct * weather) {
	weather_data_struct * weather_pointer = weather;
	for (weather_pointer = weather_pointer +  (HOURS - 1); weather > weather_pointer; weather_pointer--) {
		*weather_pointer = *(weather_pointer - 1);
	}
	//New fetched data is saved in 
	data_parsing(0);
}


//Calculating wind_incidence based on angle (If it rains, will the rain fall in the directions of the plants? 1=Yes, 0=No)
int WateringGuess::wind_incidence_function(float wind_direction, int times) {
	if ((PLANTS_ANGLE >= (wind_direction - ANGLE_WIDTH)) && (PLANTS_ANGLE <= (wind_direction + ANGLE_WIDTH))) {
		return 1;
	}
	return 0;
}


//Function that calculates weather multiplier for every weather_id WITH wind_incidence (rain, sun, drizzle apocalypse etc..)
float WateringGuess::watering_multiplier(int weather_id, int times, int wind_incidence, float wind_direction, char weather_type[]) {

	//SNOW

	if ((weather_id >= 600) && (weather_id <= 622)) {
		return 0;
	}

	//CLEAR

	if ((weather_id >= 800) && (weather_id <= 804)) {
		return 1;
	}

	//EXTREME

	if ((weather_id >= 900) && (weather_id <= 902) && (wind_incidence_function(wind_direction, times))) {
		if ((weather_id >= 900) && (weather_id <= 902)) {	//TORNADO/HURRiCANE
			return 0;
		}
		if (weather_id == 906) {	//HAIL
			return 0;
		}
		if (weather_id == 904) {	//HOT
			return 1.25;
		}

	}

	//THUNDERSTORM

	if ((weather_id >= 200) && (weather_id <= 232) && (wind_incidence_function(wind_direction, times))) {
		if (weather_id == 200) { //THUNDERSTORM WITH LIGHT RAIN
			return 0.70;
		}
		if (weather_id == 201) { //THUNDERSTORM WITH RAIN
			return 0.50;
		}
		if (weather_id == 202) { //THUNDERSTORM WITH HEAVY RAIN
			return 0.25;
		}
		if (weather_id == 210) { //LIGHT THUNDERSTORM
			return 0.90;
		}
		if (weather_id == 211) { //THUNDERSTORM
			return 0.80;
		}
		if (weather_id == 212) { //HEAVY THUNDERSTORM
			return 0.60;
		}
		if (weather_id == 221) { //RUGGED THUNDERSTORM
			return 0.70;
		}
		if (weather_id == 230) { //THUNDERSTORM WITH LIGHT DRIZZLE
			return 0.80;
		}
		if (weather_id == 231) { //THUNDERSTORM WITH DRIZZLE
			return 0.70;
		}
		if (weather_id == 232) { //THUNDERSTORM WITH HEAVY DRIZZLE
			return 0.55;
		}
	}

	//DRIZZLE

	if ((weather_id >= 300) && (weather_id <= 321) && (wind_incidence_function(wind_direction, times))) {
		if (weather_id == 300) { //LIGHT INTENSITY DRIZZLE
			return 0.90;
		}
		if (weather_id == 301) { //DRIZZLE
			return 0.80;
		}
		if (weather_id == 302) { //HEAVY INTENSITY DRIZZLE
			return 0.50;
		}
		if (weather_id == 310) { //LIGHT INTENSITY DRIZZLE RAIN
			return 0.65;
		}
		if (weather_id == 311) { //DRIZZLE RAIN
			return 0.50;
		}
		if (weather_id == 312) { //HEAVY INTENSITY DRIZZLE RAIN
			return 0.40;
		}
		if (weather_id == 313) { //SHOWER RAIN AND DRIZZLE
			return 0.30;
		}
		if (weather_id == 314) { //HEAVY SHOWER RAIN AND DRIZZLE
			return 0.20;
		}
		if (weather_id == 321) { //SHOWER DRIZZLE
			return 0.35;
		}
	}

	//RAIN

	if ((weather_id >= 500) && (weather_id <= 520) && (wind_incidence_function(wind_direction, times))) {
		if (weather_id == 500) { //LIGHT RAIN
			return 0.80;
		}
		if (weather_id == 501) { //MODERATE RAIN
			return 0.70;
		}
		if (weather_id == 502) { //HEAVY INTENSITY RAIN
			return 0.40;
		}
		if (weather_id == 503) { //VERY HEAVY RAIN
			return 0.30;
		}
		if (weather_id == 504) { //EXTREME RAIN
			return 0.10;
		}
		if (weather_id == 511) { //FREEZING RAIN
			return 0;
		}
		if ((weather_id >= 520) && (weather_id <= 531)) { //LIGHT INTENSITY SHOWER RAIN/SHOWER RAIN/RAGGED SHOWER RAIN
			return 0;
		}
	}
	return 1;
}


//Just for testing, not intended for use in the final version
void WateringGuess::test() {
	int * time_pointer;
	time_pointer = &times;
	printf("Set time: ");
	scanf("%d", time_pointer);
	printf("Set temperature: ");
	scanf("%f", &weather[times].temperature);
	printf("Set wind speed: ");
	scanf("%f", &weather[times].wind_speed);
	printf("Set wind direction: ");
	scanf("%f", &weather[times].wind_direction);
	printf("Set weather id: ");
	scanf("%d", &weather[times].weather_id);
}

//Parses fetched JSON data and assing to variables
void WateringGuess::data_parsing(int times) {

	std::string s;
	WeatherFetcher fetcher;
	fetcher.fetchCurrentWeather(s);
	std::vector<char> data(s.begin(), s.end());
    data.push_back('\0');

	//Removes the square brackets that the parsing library doesn't recognize
	square_brackets_remover(&*data.begin());

	//Begin parsing
	cJSON * root = cJSON_Parse(&*data.begin());

	//Parses weather_id
	cJSON *weather_parsed = cJSON_GetObjectItemCaseSensitive(root, "weather");
	cJSON *id_item = cJSON_GetObjectItemCaseSensitive(weather_parsed, "id");
	if (cJSON_IsNumber(id_item)) {
		weather[times].weather_id = (int) id_item->valuedouble;
	}

	//Parses weather description
	cJSON *description_item = cJSON_GetObjectItemCaseSensitive(weather_parsed, "description");
	strcpy(weather[times].weather_type, description_item->valuestring);
	char *p = weather[times].weather_type;
	while (*p) {
		*p = toupper(*p);
		p++;
	}

	//Parses temperature
	cJSON *main_data = cJSON_GetObjectItemCaseSensitive(root, "main");
	cJSON *temp_item = cJSON_GetObjectItemCaseSensitive(main_data, "temp");
	if (cJSON_IsNumber(temp_item)) {
		weather[times].temperature = (float) temp_item->valuedouble - 273.15;
	}

	//Parses wind_speed
	cJSON *wind = cJSON_GetObjectItemCaseSensitive(root, "wind");
	cJSON *wind_speed_item = cJSON_GetObjectItemCaseSensitive(wind, "speed");
	if (cJSON_IsNumber(wind_speed_item)) {
		weather[times].wind_speed = (float) wind_speed_item->valuedouble;
	}

	//Parses wind_angle
	cJSON *wind_deg_item = cJSON_GetObjectItemCaseSensitive(wind, "deg");
	if (cJSON_IsNumber(wind_deg_item)) {
		weather[times].wind_direction = (float) wind_deg_item->valuedouble;
	}

	//Empty the heap
	cJSON_Delete(root);
}

//Removes the square brackets that the parsing library doesn't recognize
void WateringGuess::square_brackets_remover(char * data) {
	char *data_pointer = data;
	short state = 0;
	while (*data_pointer != '\0' && state < 4) {
		if (*data_pointer == '[') {
			*data_pointer = ' ';
			state = 1;
		} else if (*data_pointer == '{' && state==1){
			state = 2;
		} else if (*data_pointer == '{' && state==2){
			//we found second weather object
			*data_pointer = ' ';
			state = 3;
		} else if (state == 3){
			if (*data_pointer == ']'){
				state = 4;
			}
			*data_pointer = ' ';
		} else if (*data_pointer == ']') {
			state = 5;
			*data_pointer = ' ';
		}
		data_pointer++;
	}
	if (state == 4)	*data_pointer = ' ';
}