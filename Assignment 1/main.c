
#include <stdio.h>
#include "sensor.h"
#include "filters.h"
#include "peaksearch.h"
void shiftRight();
void shiftAll();

int xRaw[13] = {0};
int xLow[33] = {0};
int xHigh[5] = {0};
int xDer = 0;
int xSqr[30] = {0};
int xMWI = 0;
int xPeak[3] = {0};

int main(int argc, char *argv[]) {
	static const char filename[] = "ECG900K.txt";
	FILE *file = fopen(filename,"r");
	int flag = 0;
	while(flag != 1) {
		xRaw[0] = getNextData(file);
		flag = getFlag();
		if(flag == 1) {
			break;
		} else {
			xLow[0] = lowPassFilter(xRaw,xLow);
			//printf("%d\n",xLow[0]); //output korrekt
			xHigh[0] = highPassFilter(xLow,xHigh);
			//printf("%d\n",xHigh[0]); //output korrekt
			xDer = derPassFilter(xHigh);
			//printf("%d\n",xDer); //output korrekt
			xSqr[0] = sqrPassFilter(xDer);
			//printf("%d\n",xSqr[0]); //output korrekt
			xMWI = mwiPassFilter(xSqr);
			//printf("%d\n",xMWI); // output korrekt
			xPeak[0] = xMWI;
			findPeak(xPeak);
			shiftAll();
		}
	}
	return 0;
}

void shiftRight(int array[], int max){
	for(int i = max; i > 0 ; i--) {
		array[i] = array[i-1];

	}
}

void shiftAll() {
	shiftRight(xRaw,12);
	shiftRight(xLow,32);
	shiftRight(xHigh,4);
	shiftRight(xSqr,29);
	shiftRight(xPeak,2);
}

