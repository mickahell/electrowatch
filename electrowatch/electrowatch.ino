#include "Electrowatch.h"
#include "settings.h"

Watchy7SEG watchy(settings);

void setup(){
	watchy.init();
}

void loop(){}
