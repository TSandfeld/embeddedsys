#include <stdio.h>
#include "sensor.h"
#include "filters.h"
void shiftRight();

int xRaw[33] = {0};
int xLow[33] = {0};
int xHigh[33] = {0};
int xDer[33] = {0};
int xSqr[33] = {0};
int xMWI[33] = {0};

int main(int argc, char *argv[]) {
	static const char filename[] = "ECG.txt";
	FILE *file = fopen(filename,"r");
	int value ;
	int flag = 0;
	while(flag != 1) {
		flag = getFlag();
		xRaw[0] = getNextData(file);
		xLow[0] = lowPassFilter(xRaw,xLow);
		//printf("%d\n",xLow[0]);
		xHigh[0] = highPassFilter(xLow,xHigh);
		printf("%d\n",xHigh[0]);
		/*xDer[0] = derPassFilter(xHigh,xDer);
		xSqr[0] = sqrPassFilter(xDer, xSqr);
		xMWI[0] = mwiPassFilter(xSqr,xMWI);*/
		shiftRight();
	}
	//printArray1(xLow);
	return 0;
}

void shiftRight(){
	for(int i = 32; i > 0; i--) {
		xRaw[i] = xRaw[i-1];
		xLow[i] = xLow[i-1];
		xHigh[i] = xHigh[i-1];
		xDer[i] = xDer[i-1];
		xSqr[i] = xSqr[i-1];
		xMWI[i] = xMWI[i-1];
	}
}

void printArray1(int array[]) {
	for(int i = 0; i < sizeof(array); i++) {
		printf("%d",array[i]);
		printf("\n");
	}

}

