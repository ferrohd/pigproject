#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cJSON.h"

//Watering time max/min constant (minutes)
#define WATERING_TIME_AT_0_DEGREES 0.0
#define WATERING_TIME_AT_30_DEGREES 20.0


//Plants angle (E=0, N=90, W=180, S=270,)
#define PLANTS_ANGLE 185.0
#define ANGLE_WIDTH 30.0


//Numbers of structs (Array length)
#define HOURS 15


//Declaring global variable time
int time = 0;

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

//Declaring global array of structs
weather_data_struct weather[HOURS]; //given for 9:00, 12:00, 15:00, 18:00, 21:00, 24:00, 3:00, 6:00 if fetched at 8:00 in the morning.


//Declaring functions
void test();
void logo(void);
void data_parsing(int time);
float watering_multiplier(int weather_id, int time, int wind_incidence, float wind_direction, char weather_type[]);
int wind_incidence_function(float wind_direction, int time);
void new_data(weather_data_struct * weather);
void square_brackets_remover(char * data);
void data_print(float watering_time_per_degree, int wind_incidence, float watering_multiplier, int watering_time, int time, char * weather_type,
                float temperature);

int main(void) {
	
	//Declaring variable(s)
    float watering_time_per_degree;

	//Just for testing
	//test();
	
	//Parses data and makes room for new data
	new_data(weather);
		
	//Prints our beautiful logo
	logo();

	//Calculate watering_time_per_degree based on constants (#define)
	watering_time_per_degree = (WATERING_TIME_AT_30_DEGREES - WATERING_TIME_AT_0_DEGREES)/30;

	//Calculating wind incidence (If it rains, will the rain fall in the directions of the plants? 1=Yes, 0=No)
	weather[time].wind_incidence = wind_incidence_function(weather[time].wind_direction, time);
	
	//Calculating watering_multiplier based on weather_id and time
	weather[time].watering_multiplier = watering_multiplier(weather[time].weather_id, time, weather[time].wind_incidence, weather[time].wind_direction, weather[time].weather_type);

	//Calculate watering time with weather multiplier
	weather[time].watering_time = watering_time_per_degree * weather[time].temperature * weather[time].watering_multiplier;

    //Printing data on screen/console
	data_print(watering_time_per_degree, weather[time].wind_incidence, weather[time].watering_multiplier, weather[time].watering_time, time, weather[time].weather_type, weather[time].temperature);
	
	return 0;
}


//Prints to screen/console data based on time
void data_print(float watering_time_per_degree, int wind_incidence, float watering_multiplier, int watering_time, int time, char * weather_type,
                float temperature) {
	printf("---------DATA---------\n\n");
	printf("Time: %d\n", time);
	printf("Temperature: %.2f (Celsius)\n", temperature);
	printf("Watering time per degree set as: %.2f (minutes)\n", watering_time_per_degree);
	printf("Wind incidence: %d (1=Yes 0=No)\n", weather[time].wind_incidence);
	printf("Watering multiplier: %.2f\n", weather[time].watering_multiplier);
	printf("Weather is: %s\n", weather[time].weather_type);
	printf("________________________________\n");
	printf("|  WATERING TIME: %d (minutes) |\n", weather[time].watering_time);
	printf("--------------------------------\n");
}


//Prints our beautiful logo
void logo(void) {
	printf("______ _      ______          _           _   \n");
	printf("| ___ (_)     | ___ \\        (_)         | |  \n");
	printf("| |_/ /_  __ _| |_/ / __ ___  _  ___  ___| |_ \n");
	printf("|  __/| |/ _` |  __/ '__/ _ \\| |/ _ \\/ __| __|\n");
	printf("| |   | | (_| | |  | | | (_) | |  __/ (__| |_ \n");
	printf("\\_|   |_|\\__, \\_|  |_|  \\___/| |\\___|\\___|\\__|\n");
	printf("          __/ |             _/ |              \n");
	printf("         |___/             |__/               \n\n");
}


//Switch the place of the structs to make space for the new parsed data
void new_data(weather_data_struct * weather) {
	weather_data_struct * weather_pointer = weather;
	for (weather_pointer = weather_pointer +  (HOURS - 1); weather > weather_pointer; weather_pointer--) {
		*weather_pointer = *(weather_pointer-1);
	}
	data_parsing(0);
}


//Calculating wind_incidence based on angle (If it rains, will the rain fall in the directions of the plants? 1=Yes, 0=No)
int wind_incidence_function(float wind_direction, int time) {
	if ((PLANTS_ANGLE >= (wind_direction - ANGLE_WIDTH)) && (PLANTS_ANGLE <= (wind_direction + ANGLE_WIDTH))) {
		return 1;
	}
	return 0;
}


//Function that calculates weather multiplier for every weather_id WITH wind_incidence (rain, sun, drizzle apocalypse etc..)
float watering_multiplier(int weather_id, int time, int wind_incidence, float wind_direction, char weather_type[]) {

	//SNOW

	if ((weather_id >= 600) && (weather_id <= 622)) {
		return 0;
	}

	//CLEAR

	if ((weather_id >= 800) && (weather_id <= 804)) {
		return 1;
	}

	//EXTREME

	if ((weather_id >= 900) && (weather_id <= 902) && (wind_incidence_function(wind_direction, time))) {
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

	if ((weather_id >= 200) && (weather_id <= 232) && (wind_incidence_function(wind_direction, time))) {
		if (weather_id == 200) { //THUNDERSTORM WITH LIGHT RAIN
			return 0.70;
		}
		if (weather_id == 201) {	//THUNDERSTORM WITH RAIN
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

	if ((weather_id >= 300) && (weather_id <= 321) && (wind_incidence_function(wind_direction, time))) {
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

	if ((weather_id >= 500) && (weather_id <= 520) && (wind_incidence_function(wind_direction, time))) {
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
void test() {
    int * time_pointer;
    time_pointer = &time;
    printf("Set time: ");
    scanf("%d", time_pointer);
    printf("Set temperature: ");
    scanf("%f", &weather[time].temperature);
    printf("Set wind speed: ");
    scanf("%f", &weather[time].wind_speed);
    printf("Set wind direction: ");
    scanf("%f", &weather[time].wind_direction);
    printf("Set weather id: ");
    scanf("%d", &weather[time].weather_id);
}

//Parses fetched JSON data and assing to variables
void data_parsing(int time) { /*EXAMPLE DATA*/
	char data[] = {"{\"coord\":{\"lon\":9.19,\"lat\":45.47},\"weather\":[{\"id\":803,\"main\":\"Clouds\",\"description\":\"broken clouds\",\"icon\":\"04d\"}],\"base\":\"stations\",\"main\":{\"temp\":280.15,\"pressure\":1030,\"humidity\":70,\"temp_min\":278.15,\"temp_max\":282.15},\"visibility\":10000,\"wind\":{\"speed\":1.5},\"clouds\":{\"all\":75},\"dt\":1512644100,\"sys\":{\"type\":1,\"id\":5800,\"message\":0.0108,\"country\":\"IT\",\"sunrise\":1512629389,\"sunset\":1512661197},\"id\":3173435,\"name\":\"Milan\",\"cod\":200}\0"};
	
	//Removes the square brackets that the parsing library doesn't recognize
	square_brackets_remover(data);
	
	//Begin parsing
	cJSON * root = cJSON_Parse(data);

	//Parses weather_id
	cJSON *weather_parsed = cJSON_GetObjectItemCaseSensitive(root, "weather");
	cJSON *id_item = cJSON_GetObjectItemCaseSensitive(weather_parsed, "id");
	if (cJSON_IsNumber(id_item)) {
		weather[time].weather_id = (int) id_item->valuedouble;
	}

	//Parses weather description
	cJSON *description_item = cJSON_GetObjectItemCaseSensitive(weather_parsed, "description");
	strcpy(weather[time].weather_type, description_item->valuestring);
	char *p = weather[time].weather_type;
	while (*p) {
		*p = toupper(*p);
		p++;
	}

	//Parses temperature
	cJSON *main_data = cJSON_GetObjectItemCaseSensitive(root, "main");
	cJSON *temp_item = cJSON_GetObjectItemCaseSensitive(main_data, "temp");
	if (cJSON_IsNumber(temp_item)) {
		weather[time].temperature = (float) temp_item->valuedouble - 273.15;
	}

	//Parses wind_speed
	cJSON *wind = cJSON_GetObjectItemCaseSensitive(root, "wind");
	cJSON *wind_speed_item = cJSON_GetObjectItemCaseSensitive(wind, "speed");
	if (cJSON_IsNumber(wind_speed_item)) {
		weather[time].wind_speed = (float) wind_speed_item->valuedouble;
	}

	//Parses wind_angle
	cJSON *wind_deg_item = cJSON_GetObjectItemCaseSensitive(wind, "deg");
	if (cJSON_IsNumber(wind_deg_item)) {
		weather[time].wind_direction = (float) wind_deg_item->valuedouble;
	}

	//Empty the heap
	cJSON_Delete(root);
}

//Removes the square brackets that the parsing library doesn't recognize
void square_brackets_remover(char * data) {
	char *data_pointer = data;
	while (*data_pointer != '\0') {
		if ((*data_pointer == '[') || (*data_pointer == ']')) {
			*data_pointer = ' ';
		}
		data_pointer++;
	}
}