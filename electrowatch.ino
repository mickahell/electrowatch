#include "src/Electrowatch.h"
#include "src/settings.h"

Watchy7SEG watchy(settings);

void setup(){
	watchy.init();
}

void loop(){}
