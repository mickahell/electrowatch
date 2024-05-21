#include "sendData.h"

void SendData::pushAPIData(String json) {

  HTTPClient https;

  WiFiClientSecure newSecure;
  newSecure.setInsecure();
  https.begin(newSecure, API_URL);

  https.addHeader("Accept",  "application/vnd.github+json");
  https.addHeader("Content-Type", "application/json");
  https.addHeader("Authorization", "token " + String(API_TOKEN));
  int httpsResponseCode = https.POST(json);

  if (200 <= httpsResponseCode && httpsResponseCode < 300) {
    //Serial.println("- success send to Github");
  } else {
    //Serial.println("- not send to Github");
    //Serial.println(httpsResponseCode);
    //String response = https.getString();
    //Serial.println(response);
    //Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpsResponseCode).c_str());
  }
  https.end();
}
