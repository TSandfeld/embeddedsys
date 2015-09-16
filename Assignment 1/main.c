#include <stdio.h>
#include "sensor.h"
#include "filters.h"
void shiftRight();
int length();

int xRaw[13] = {0};
int xLow[33] = {0};
int xHigh[5] = {0};
int xDer = 0;
int xSqr[31] = {0};
int xMWI = 0;
int xPeak[3] = {0};

int main(int argc, char *argv[]) {
	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename,"r");
	int flag = 0;
	while(flag != 1) {
		flag = getFlag();
		xRaw[0] = getNextData(file);
		xLow[0] = lowPassFilter(xRaw,xLow);
		//printf("%d\n",xLow[0]);
		xHigh[0] = highPassFilter(xLow,xHigh);
		//printf("%d\n",xHigh[0]);
		xDer = derPassFilter(xHigh);
		//printf("%d\n",xDer);
		xSqr[0] = sqrPassFilter(xDer);
		//printf("%d\n",xSqr[0]);
		xMWI = mwiPassFilter(xSqr);
		xPeak[0] = xMWI;

		findPeak(xPeak);
		//printf("%d\n",xMWI);
		shiftAll();

	}
	//printArray1(xLow);
	return 0;
}

void shiftRight(int array[], int max){
	for(int i = max; i > 0 ; i--) {
		array[i] = array[i-1];

	}
}

void shiftAll() {
	shiftRight(xRaw,12);
	shiftRight(xLow, 32);
	shiftRight(xHigh,4);
	shiftRight(xSqr,30);
	shiftRight(xPeak,2);
}






