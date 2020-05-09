#include "DELETTHIS.h"

int DELETTHIS::amount = 0;
float DELETTHIS::totalTime = 0.0f;
int DELETTHIS::amount2 = 0;
float DELETTHIS::totalTime2 = 0.0f;


void DELETTHIS::AddTime(float time) {
	amount++;
	totalTime += time;
}

float DELETTHIS::GetAverage() {
	return totalTime / (float)amount;
}

void DELETTHIS::AddTime2(float time) {
	amount2++;
	totalTime2 += time;
}

float DELETTHIS::GetAverage2() {
	return totalTime2 / (float)amount2;
}