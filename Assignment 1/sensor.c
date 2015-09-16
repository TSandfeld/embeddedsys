#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

static int flag = 0;
int getNextData(FILE *file){
	int value;
		if(fscanf(file,"%i",&value) == EOF) {
			flag = 1;
		} else {
			flag = 0;
		}
	return value;

}

int getFlag() {
	return flag;
}

