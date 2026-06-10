#ifndef ELECTROWATCH_H
#define ELECTROWATCH_H

#include <Watchy.h>
#include "Seven_Segment10pt7b.h"
#include "DSEG7_Classic_Regular_15.h"
#include "DSEG7_Classic_Bold_25.h"
#include "DSEG7_Classic_Regular_39.h"
#include "icons.h"
#include "../parameters.h"

#include "sendData.h"
#include "dataFile.h"

#include "skating/SessionEngine.h"
#include "skating/PushDetector.h"

typedef struct blagueData {
  String type;
  String blague;
  String answer;
} blagueData;

class Watchy7SEG : public Watchy{
    using Watchy::Watchy;
    public:
        bool connectWiFi();
        void menu();
        void showMenu(byte menuIndex, bool partialRefresh);
        void menuButton();
        void backButton();
        void upButton();
        void downButton();
        void handleButtonPress();
        void drawWatchFace();
        uint32_t getEpochTime();

        void drawTime();
        void drawDate();
        void drawSteps();
        void drawWeather();
        void drawBattery();
        void setupFS();
        void syncAPI();
        void getBlagueDuJour(int nb_blague);
        void showJoke();
        void setupSecondaryWifi();

		void updateSkating();
		void drawSkatingUI();
      
    private:
        SessionEngine session;
        PushDetector pushDetector;
};

extern RTC_DATA_ATTR int PSTEPS;
extern RTC_DATA_ATTR bool GET_DATA;
extern RTC_DATA_ATTR blagueData BLAGUE_DU_JOUR;
extern RTC_DATA_ATTR String WIFI_SSID;
extern RTC_DATA_ATTR String WIFI_PASS;
extern RTC_DATA_ATTR bool WIFI_2ND;
extern RTC_DATA_ATTR bool skatingMode;

#endif
