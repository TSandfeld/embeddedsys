int PEAKS[200] = {0}; //Valgt ud fra peak-data.
int RPEAKS[200] = {0};
int RRCALC[2] = {0};
int RecentRR[8] = {0}; //husk at shifte!
int RecentRR_OK[8] = {0};
int RR_AVERAGE1, RR_AVERAGE2, RR_LOW, RR_HIGH, RR_MISS = 0;
int SPKF, THRESHOLD2, NPKF, RR = 0;
int THRESHOLD1 = 3000; //Valgt ud fra graf over MWI-punkter.
int RPeaksCount, PeaksCount = 0;

void findPeak(int array[]) {
	if(array[0] < array[1] && array[1] > array[2]) {
		addToPeaks(array[1]);
		//printf("0 %d 1 %d 2 %d\n", array[0],PEAKS[0],array[2]);
		if(PEAKS[PeaksCount-1] > THRESHOLD1) {
			RRCALC[0] = PEAKS[PeaksCount-1];
			calcRR(RRCALC[1],RRCALC[0]);
			RRCALC[1] = RRCALC[0];
			//printf("%d\n", PEAKS[0]);
		} else {
			findNPKF(PEAKS[PeaksCount-1]);
			findThreshold1();
			findThreshold2();
		}
	}

}

void findThreshold1() {
	THRESHOLD1 = NPKF + 0.25*(SPKF-NPKF);
}

void findThreshold2() {
	THRESHOLD2 = 0.5*THRESHOLD1;
}
void findSPKF(int x) {
	SPKF = 0.125 * x +0.875*SPKF;
}
void findNPKF(int x) {
	NPKF = 0.125*x + 0.875*NPKF;
}
void findAVG1() {
	int sum = 0;
	for(int i = 0; i < 8; i++) {
		sum += RecentRR[i];
	}
	RR_AVERAGE1 = sum/8;
}
void findAVG2() {
	int sum = 0;
	for(int i = 0; i < 8; i++) {
		sum += RecentRR_OK[i];
	}
	RR_AVERAGE2 = sum/8;
}
void findRRLow() {
	RR_LOW = 0.92*RR_AVERAGE2;
}
void findRRHigh() {
	RR_LOW = 1.16*RR_AVERAGE2;
}
void findRRMiss() {
	RR_LOW = 1.66*RR_AVERAGE2;
}
void addToRPeak(int number) { //Efter 200 rpeaks resettes counter, og der sættes ind fra plads 0.
	if(RPeaksCount >=200) {
		RPeaksCount = 0;
	}
	RPEAKS[RPeaksCount] = number;
	RPeaksCount++;
	printf("RPEAK = %d\n",number);
}
void addToPeaks(int number) {
	if(PeaksCount >= 200) {
		PeaksCount = 0;
	}
	PEAKS[PeaksCount] = number;
	PeaksCount++;
}
void shiftRecents() {
	for(int i = 8; i > 0; i--) {
		RecentRR[i] = RecentRR[i-1];
		RecentRR_OK[i] = RecentRR_OK[i-1];
	}
}
void shiftRecent() { //shifter kun recentRR!!!!
	for(int i = 8; i > 0; i--) {
		RecentRR[i] = RecentRR[i-1];
	}
}

void calcRR(int t1, int t2) {
	RR = t2-t1;
	if(RR > RR_LOW && RR < RR_HIGH) {
		addToRPeak(PEAKS[PeaksCount-1]);
		findSPKF(RPEAKS[RPeaksCount-1]);
		RecentRR_OK[0] = RR;
		RecentRR[0] = RR;
		shiftRecent();
		findAVG2();
		findAVG1();
		findRRLow();
		findRRHigh();
		findRRMiss();
		findThreshold1();
		findThreshold2();
	} else if(RR > RR_MISS) {
			searchBack();
	}
}
void searchBack() {
	for(int i = 200; i > 0; i--) {
		if(PEAKS[i] > THRESHOLD2) {
			addToRPeak(PEAKS[i]);
			SPKF = 0.25*PEAKS[i]+0.75*SPKF;
			RecentRR[0] = RR;
			shiftRecent();
			findAVG1();
			findRRLow();
			findRRHigh();
			findRRMiss();
			findThreshold1();
			findThreshold2();
			//break; //virker desværre ikke særlig godt.
		}
	}
}
