#include "Electrowatch.h"

const uint8_t BATTERY_SEGMENT_WIDTH = 7;
const uint8_t BATTERY_SEGMENT_HEIGHT = 11;
const uint8_t BATTERY_SEGMENT_SPACING = 9;
const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;
const int FAST_OPT_STATE = 3;

RTC_DATA_ATTR int PSTEPS = 0;
RTC_DATA_ATTR bool GET_DATA = true;
RTC_DATA_ATTR struct blagueData BLAGUE_DU_JOUR;

void Watchy7SEG::drawWatchFace() {
	//Serial.begin(115200);
	display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
	display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	drawTime();
	drawDate();
	drawSteps();
	drawWeather();
	drawBattery();
	display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	if(BLE_CONFIGURED) {
		display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	}

	setupFS();
	syncAPI();
	getBlagueDuJour(10);
}

void Watchy7SEG::drawTime() {
	display.setFont(&DSEG7_Classic_Bold_53);
	display.setCursor(5, 53+5);
	int displayHour;
	if(HOUR_12_24==12) {
		displayHour = ((currentTime.Hour+11)%12)+1;
	} else {
		displayHour = currentTime.Hour;
	}
	if(displayHour < 10) {
		display.print("0");
	}
	display.print(displayHour);
	display.print(":");
	if(currentTime.Minute < 10) {
		display.print("0");
	}
	display.println(currentTime.Minute);
}

void Watchy7SEG::drawDate() {
	display.setFont(&Seven_Segment10pt7b);

	int16_t  x1, y1;
	uint16_t w, h;

	String dayOfWeek = dayStr(currentTime.Wday);
	display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);
	if(currentTime.Wday == 4) {
		w = w - 5;
	}
	display.setCursor(85 - w, 85);
	display.println(dayOfWeek);

	String month = monthShortStr(currentTime.Month);
	display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
	display.setCursor(85 - w, 110);
	display.println(month);

	display.setFont(&DSEG7_Classic_Bold_25);
	display.setCursor(5, 120);
	if(currentTime.Day < 10) {
		display.print("0");
	}
	display.println(currentTime.Day);
	display.setCursor(5, 150);
	display.println(tmYearToCalendar(currentTime.Year));// offset from 1970, since year is stored in uint8_t
}
void Watchy7SEG::drawSteps() {
	// reset step counter at midnight
	if (currentTime.Hour == 0 && currentTime.Minute == 0){
		sensor.resetStepCounter();
	}
	uint32_t stepCount = sensor.getCounter();
	display.drawBitmap(10, 165, steps, 19, 23, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	display.setCursor(35, 190);
	display.println(stepCount);
}
void Watchy7SEG::drawBattery() {
	display.drawBitmap(154, 73, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	display.fillRect(159, 78, 27, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments
	int8_t batteryLevel = 0;
	float VBAT = getBatteryVoltage();
	if(VBAT > 4.1) {
		batteryLevel = 3;
	}
	else if(VBAT > 3.95 && VBAT <= 4.1) {
		batteryLevel = 2;
	}
	else if(VBAT > 3.80 && VBAT <= 3.95) {
		batteryLevel = 1;
	}
	else if(VBAT <= 3.80) {
		batteryLevel = 0;
	}

	for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++) {
		display.fillRect(159 + (batterySegments * BATTERY_SEGMENT_SPACING), 78, BATTERY_SEGMENT_WIDTH, BATTERY_SEGMENT_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	}
}

void Watchy7SEG::drawWeather() {

	weatherData currentWeather = getWeatherData();

	int8_t temperature = currentWeather.temperature;
	int16_t weatherConditionCode = currentWeather.weatherConditionCode;

	display.setFont(&DSEG7_Classic_Regular_39);
	int16_t  x1, y1;
	uint16_t w, h;
	display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
	if(159 - w - x1 > 87) {
		display.setCursor(159 - w - x1, 150);
	} else {
		display.setFont(&DSEG7_Classic_Bold_25);
		display.getTextBounds(String(temperature), 0, 0, &x1, &y1, &w, &h);
		display.setCursor(159 - w - x1, 136);
	}
	display.println(temperature);
	display.drawBitmap(165, 110, currentWeather.isMetric ? celsius : fahrenheit, 26, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
	const unsigned char* weatherIcon;

	if(WIFI_CONFIGURED) {
		//https://openweathermap.org/weather-conditions
		if(weatherConditionCode > 801){//Cloudy
		  weatherIcon = cloudy;
		}else if(weatherConditionCode == 801){//Few Clouds
		  weatherIcon = cloudsun;
		}else if(weatherConditionCode == 800){//Clear
		  weatherIcon = sunny;
		}else if(weatherConditionCode >=700){//Atmosphere
		  weatherIcon = atmosphere;
		}else if(weatherConditionCode >=600){//Snow
		  weatherIcon = snow;
		}else if(weatherConditionCode >=500){//Rain
		  weatherIcon = rain;
		}else if(weatherConditionCode >=300){//Drizzle
		  weatherIcon = drizzle;
		}else if(weatherConditionCode >=200){//Thunderstorm
		  weatherIcon = thunderstorm;
		}else 
		return;
	}else{
		weatherIcon = chip;
	}
	
	display.drawBitmap(145, 158, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void Watchy7SEG::setupFS() {
	LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED);
	FSData::createDir(LittleFS, DATA_FOLDER);
	FSData::createDir(LittleFS, STEPS_FOLDER);
	FSData::createDir(LittleFS, JOKES_FOLDER);
	LittleFS.end();
}

void Watchy7SEG::syncAPI() {
	int cSteps = sensor.getCounter();
	if (currentTime.Hour == 0 && currentTime.Minute == 0) {
		PSTEPS = cSteps;
	}
	if (currentTime.Minute == 59 && cSteps > PSTEPS) {
		String day_api = (currentTime.Day < 10) ? ("0" + String(currentTime.Day)) : String(currentTime.Day);
		String month_api = (currentTime.Month < 10) ? ("0" + String(currentTime.Month)) : String(currentTime.Month);
		String date_api = day_api + "-" + month_api + "-" + String(currentTime.Year + 1970);

		String hour_api = (currentTime.Hour < 10) ? ("0" + String(currentTime.Hour)) : String(currentTime.Hour);
		int aSteps = cSteps - PSTEPS;
		String json_steps = "{\"event_type\":\"" + String(ENDPOINT_API) + "\",\"client_payload\":{\"data-name\":\"" + String(ENDPOINT_STEPS) + "\",\"date\":\"" + date_api + "\",\"hour\":\"" + hour_api + "\",\"data\":\"" + String(aSteps) + "\"}}";
		
		LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED);
		FSData file_system;
		if (connectWiFi()) {
			// Steps
			SendData::pushAPIData(json_steps);

			// Sync old data steps
			file_system.listDir(LittleFS, STEPS_FOLDER);
			for(const String& file : file_system.files) {
				String file_name = String(STEPS_FOLDER) + "/" + file;
				const char * fname = file_name.c_str();
				file_system.readFile(LittleFS, fname);

				SendData::pushAPIData(String(file_system.content));
				FSData::deleteFile(LittleFS, fname);
			}

			// turn off radios
			WiFi.mode(WIFI_OFF);
			btStop();
		} else { // No WiFi, register into a file
			String file_name = String(STEPS_FOLDER) + "/" + date_api + "_" + hour_api + ".txt";
			const char * fname = file_name.c_str();
			const char * json_content = json_steps.c_str();
			FSData::writeFile(LittleFS, fname, json_content);
		}
		LittleFS.end();
		PSTEPS = cSteps;
	}
}

void Watchy7SEG::getBlagueDuJour(int nb_blague) {
	if (currentTime.Hour == 0 && currentTime.Minute == 0) {
		GET_DATA = true;
	}
	if (GET_DATA) {
		LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED);
		FSData file_system;
		if (connectWiFi()) {
			HTTPClient https;

			WiFiClientSecure newSecure;
			newSecure.setInsecure();
			https.begin(newSecure, BLAGUE_URL);

			https.addHeader("Authorization", "Bearer " + String(BLAGUE_TOKEN));

			for (int count = 0; count < nb_blague; count++) {
				int httpsResponseCode = https.GET();

				if (200 <= httpsResponseCode && httpsResponseCode < 300) {
					String payload = https.getString();
					JSONVar responseObject = JSON.parse(payload);
					BLAGUE_DU_JOUR.type = JSONVar::stringify(responseObject["type"]);
					BLAGUE_DU_JOUR.blague = JSONVar::stringify(responseObject["joke"]);
					BLAGUE_DU_JOUR.answer = JSONVar::stringify(responseObject["answer"]);

					String file_name = String(JOKES_FOLDER) + "/" + String(count) + "_blaguedujour.txt";
					const char * fname = file_name.c_str();
					const char * fcontent = payload.c_str();
					FSData::writeFile(LittleFS, fname, fcontent);
				}
			}
			https.end();
			WiFi.mode(WIFI_OFF);
			btStop();
		} else {
			String file_name = String(JOKES_FOLDER) + "/" + String(esp_random() % 10) + "_blaguedujour.txt";
			const char * fname = file_name.c_str();
			file_system.readFile(LittleFS, fname);
			JSONVar responseObject = JSON.parse(file_system.content);
			BLAGUE_DU_JOUR.type = JSONVar::stringify(responseObject["type"]);
			BLAGUE_DU_JOUR.blague = JSONVar::stringify(responseObject["joke"]);
			BLAGUE_DU_JOUR.answer = JSONVar::stringify(responseObject["answer"]);
		}
		LittleFS.end();
		GET_DATA = false;
	}
}

void Watchy7SEG::showJoke() {
	GET_DATA = true;
	getBlagueDuJour(1);

	display.setFullWindow();
	display.fillScreen(GxEPD_BLACK);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_WHITE);
	display.setCursor(30, 20);
	display.println("Joke : " + BLAGUE_DU_JOUR.type);
	display.println(BLAGUE_DU_JOUR.blague);
	display.println(BLAGUE_DU_JOUR.answer);
	display.print("hahahahah");
	display.display(false); // full refresh

	guiState = FAST_OPT_STATE;
}

/***********************/
//
// Override WatchyLib
//
/***********************/

bool Watchy7SEG::connectWiFi() {
	if (WL_CONNECT_FAILED == WiFi.begin(WIFI_SSID, WIFI_PASS)) {
		WIFI_CONFIGURED = false;
	} else {
		if (WL_CONNECTED == WiFi.waitForConnectResult()) { // attempt to connect for 10s
			WIFI_CONFIGURED = true;
		} else { // connection failed, time out
			WIFI_CONFIGURED = false;
			// turn off radios
			WiFi.mode(WIFI_OFF);
			btStop();
		}
	}
	return WIFI_CONFIGURED;
}

void Watchy7SEG::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  // Menu Button
  if (wakeupBit & MENU_BTN_MASK) {
	if (guiState ==
		WATCHFACE_STATE) { // enter menu state if coming from watch face
	  showMenu(menuIndex, false);
	} else if (guiState ==
			   MAIN_MENU_STATE) { // if already in menu, then select menu item
	  switch (menuIndex) {
	  case 0:
		showAbout();
		break;
	  case 1:
		showBuzz();
		break;
	  case 2:
		showAccelerometer();
		break;
	  case 3:
		setTime();
		break;
	  case 4:
		setupWifi();
		break;
	  case 5:
		showUpdateFW();
		break;
	  case 6:
		showSyncNTP();
		break;
	  default:
		break;
	  }
	} else if (guiState == FW_UPDATE_STATE) {
	  updateFWBegin();
	}
  }
  // Back Button
  else if (wakeupBit & BACK_BTN_MASK) {
	if (guiState == MAIN_MENU_STATE || guiState == FAST_OPT_STATE) { // exit to watch face if already in menu
	  RTC.read(currentTime);
	  showWatchFace(false);
	} else if (guiState == APP_STATE) {
	  showMenu(menuIndex, false); // exit to menu if already in app
	} else if (guiState == FW_UPDATE_STATE) {
	  showMenu(menuIndex, false); // exit to menu if already in app
	} else if (guiState == WATCHFACE_STATE) {
	  return;
	}
  }
  // Up Button
  else if (wakeupBit & UP_BTN_MASK) {
	if (guiState == MAIN_MENU_STATE) { // increment menu index
	  menuIndex--;
	  if (menuIndex < 0) {
		menuIndex = MENU_LENGTH - 1;
	  }
	  showMenu(menuIndex, true);
	} else if (guiState == WATCHFACE_STATE) {
	  showJoke();
	}
  }
  // Down Button
  else if (wakeupBit & DOWN_BTN_MASK) {
	if (guiState == MAIN_MENU_STATE) { // decrement menu index
	  menuIndex++;
	  if (menuIndex > MENU_LENGTH - 1) {
		menuIndex = 0;
	  }
	  showMenu(menuIndex, true);
	} else if (guiState == WATCHFACE_STATE) {
	  return;
	}
  }

  /***************** fast menu *****************/
  bool timeout     = false;
  long lastTimeout = millis();
  pinMode(MENU_BTN_PIN, INPUT);
  pinMode(BACK_BTN_PIN, INPUT);
  pinMode(UP_BTN_PIN, INPUT);
  pinMode(DOWN_BTN_PIN, INPUT);
  while (!timeout) {
	if (millis() - lastTimeout > 5000) {
	  timeout = true;
	} else {
	  if (digitalRead(MENU_BTN_PIN) == 1) {
		lastTimeout = millis();
		if (guiState ==
			MAIN_MENU_STATE) { // if already in menu, then select menu item
		  switch (menuIndex) {
		  case 0:
			showAbout();
			break;
		  case 1:
			showBuzz();
			break;
		  case 2:
			showAccelerometer();
			break;
		  case 3:
			setTime();
			break;
		  case 4:
			setupWifi();
			break;
		  case 5:
			showUpdateFW();
			break;
		  case 6:
			showSyncNTP();
			break;
		  default:
			break;
		  }
		} else if (guiState == FW_UPDATE_STATE) {
		  updateFWBegin();
		}
	  } else if (digitalRead(BACK_BTN_PIN) == 1) {
		lastTimeout = millis();
		if (guiState ==
			MAIN_MENU_STATE || guiState == FAST_OPT_STATE) { // exit to watch face if already in menu
		  RTC.read(currentTime);
		  showWatchFace(false);
		  break; // leave loop
		} else if (guiState == APP_STATE) {
		  showMenu(menuIndex, false); // exit to menu if already in app
		} else if (guiState == FW_UPDATE_STATE) {
		  showMenu(menuIndex, false); // exit to menu if already in app
		}
	  } else if (digitalRead(UP_BTN_PIN) == 1) {
		lastTimeout = millis();
		if (guiState == MAIN_MENU_STATE) { // increment menu index
		  menuIndex--;
		  if (menuIndex < 0) {
			menuIndex = MENU_LENGTH - 1;
		  }
		  showFastMenu(menuIndex);
		}
	  } else if (digitalRead(DOWN_BTN_PIN) == 1) {
		lastTimeout = millis();
		if (guiState == MAIN_MENU_STATE) { // decrement menu index
		  menuIndex++;
		  if (menuIndex > MENU_LENGTH - 1) {
			menuIndex = 0;
		  }
		  showFastMenu(menuIndex);
		}
	  }
	}
  }
}
