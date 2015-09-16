
int lowPassFilter(int xArr[], int yArr[]){
	int y = 2*yArr[1]-yArr[2] + (xArr[0]-2*xArr[6]+xArr[12])/32;
	return y;
}

int highPassFilter(int xArr[], int yArr[]) {
	int y = yArr[1]-(xArr[0]/32)+xArr[16]-xArr[17]+(xArr[32]/32);
	return y;
}

int derPassFilter(int xArr[]) {
	int y = (2*xArr[0]+xArr[1]-xArr[3]-2*xArr[4])/8;
	return y;
}

int sqrPassFilter(int xArr) {
	int y = xArr*xArr;
	return y;
}

int mwiPassFilter(int xArr[]) {
	int y;
	for(int i = 0; i<31; i++) {
		y += xArr[i];
	}
	return y/30;
}
