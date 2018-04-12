<p align="center"><img src="https://user-images.githubusercontent.com/16491613/34329009-a2a5b4cc-e8f0-11e7-9f2c-5f3dd142605f.png"></p>
<br>

[![Build Status](https://travis-ci.org/ferrohd/PigProject---Watering-Guess.svg?branch=master)](https://travis-ci.org/FerroHD/PigProject---Watering-Guess) 
[
![MIT License](https://camo.githubusercontent.com/3ccf4c50a1576b0dd30b286717451fa56b783512/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f4c6963656e73652d4d49542d79656c6c6f772e737667)](https://opensource.org/licenses/MIT)

----------

# PigProject - Watering Guess

Source code of the [PigProject](https://pigproject.tk/). Calculates the irrigation time of a plant based on weather conditions such as wind, rain and temperature. The project is still under development but will be released soon.

## Monitoring / Data Log

This script provide a webserver that provide access to log of retrieved data and the calculated irrigation time after every fetch. The webserver runs on port 9000 just to avoid to keep busy other ports (maybe you want to run other services). You can reach the server by connecting to:
```bash
YOUR_DEVICE_IP:9000
```
Along with that it prints the log in the console too, here is and example:
```bash
______ _      ______          _           _   
| ___ (_)     | ___ \        (_)         | |  
| |_/ /_  __ _| |_/ / __ ___  _  ___  ___| |_ 
|  __/| |/ _` |  __/ '__/ _ \| |/ _ \/ __| __|
| |   | | (_| | |  | | | (_) | |  __/ (__| |_ 
\_|   |_|\__, \_|  |_|  \___/| |\___|\___|\__|
          __/ |             _/ |              
         |___/             |__/               

Starting server...
weather_fetcher ready
making request...
Server up and running on port 9000


---------DATA---------
Time: 0
Temperature: 1.70 (Celsius)
Watering time per degree set as: 0.67 (minutes)
Wind incidence: 0 (1=Yes 0=No)
Watering multiplier: 1.00
Weather is: CLEAR SKY
________________________________
|  WATERING TIME: 1 (minutes) |
--------------------------------
```
## Internet connection

This script uses third-party WebAPI from [OpenWeatherMap](https://openweathermap.org/) to fetch weather data. *Needless* to remember that it needs 24/7 internet connection.

## Authors

* **Alessandro Ferrara**  - [FerroHD](https://github.com/FerroHD)
* **Andrea Taglia**  - [andreataglia](https://github.com/andreataglia)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
