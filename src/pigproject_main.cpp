#include <thread>
#include <chrono>
#include <iostream>

#include "server.h"
#include "watering_guess.h"

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

int main(int argc, char const *argv[]){

	logo();
	
	Server server;
	server.run();

	WateringGuess wg;
	//fetch current weather and give guess on it
	while(true){
		wg.giveGuess();
		std::this_thread::sleep_for(std::chrono::seconds(25));
	}

	return 0;
} 
