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
