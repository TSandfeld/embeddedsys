#include <stdio.h>

void addToPeaks();
void calcRR();
void findNPKF();
void findThreshold1();
void findThreshold2();
void searchBack();
void addMissCount();

int PEAKS[10] = {0}; //Valgt ud fra peak-data.
int PeakIndex[10] = {0};
int RPEAKS[10] = {0};
int RRCALC[2] = {0};
int RecentRR[8] = {[0 ... 7] = 155};
int RecentRR_OK[8] = {[0 ... 7] = 155};
int RR_AVERAGE1 = 0;
int RR_AVERAGE2 = 0;
int RR_LOW = 0;
int RR_HIGH = 0;
int RR_MISS = 0;
int SPKF = 0;
int NPKF = 0;
int RR = 0;
int THRESHOLD1 = 2000; //Valgt ud fra graf over MWI-punkter.
int THRESHOLD2 = 1000; //Threshold1 *0.5
int RPeaksCount, PeaksCount, DataIndex, RRMISSCount, PulseCount = 0;

void findPeak(int array[]) {
	if(array[0] < array[1] && array[1] > array[2]) {
		int peak = array[1];
		addToPeaks(peak);
		if(peak > THRESHOLD1) {
			/*
			printf("Latest R-Peak detected was %d at time %.3f s\n", peak,  ((DataIndex*4.0)/1000.0)+0.1875);
			printf("Current pulse is: %.3f\n",60*PulseCount/(((DataIndex*4.0)/1000.0)+0.1875));
			if(peak < 2000) {
				printf("WARNING: Significant pulse drop!\n");
			}*/
			RRCALC[0] = DataIndex;
			calcRR(RRCALC[1],RRCALC[0], peak);
		} else {
			findNPKF(peak);
			findThreshold1();
			findThreshold2();
		}
	}
	DataIndex++;
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
		sum += RecentRR[i]/8;
	}
	RR_AVERAGE1 = sum;
}
void findAVG2() {
	int sum = 0;
	for(int i = 0; i < 8; i++) {
		sum += RecentRR_OK[i]/8;
	}
	RR_AVERAGE2 = sum;
}

void findRRLow() {
	RR_LOW = 0.92*RR_AVERAGE2;
}
void findRRHigh() {
	RR_HIGH = 1.16*RR_AVERAGE2;
}
void findRRMiss() {
	RR_MISS = 1.66*RR_AVERAGE2;
}
void addToRPeak(int number) { //Efter 200 rpeaks resettes counter, og der s�ttes ind fra plads 0.
	for(int i = 9; i > 0; i--) {
		RPEAKS[i] = RPEAKS[i-1];
	}
	RPEAKS[0] = number;
	PulseCount++;
	printf("At index %d added RPEAK = %d\n", RRCALC[1], number);
}
void addToPeaks(int number) {
	for(int i = 9; i > 0; i--) {
		PEAKS[i] = PEAKS[i-1];
		PeakIndex[i] = PeakIndex[i-1];
	}
	PeakIndex[0] = DataIndex;
		PEAKS[0] = number;
}
void shiftBothRecent() {
	for(int i = 7; i > 0; i--) {
		RecentRR[i] = RecentRR[i-1];
		RecentRR_OK[i] = RecentRR_OK[i-1];
	}
}
void shiftRecent() { //shifter kun recentRR!
	for(int i = 7; i > 0; i--) {
		RecentRR[i] = RecentRR[i-1];
	}
}

void calcRR(int t1, int t2, int peak) {
	RR = t2-t1;
	/*
	if(RR < 0) {
		RR = RR*-1;
	}
	*/
	if(RR > RR_LOW && RR < RR_HIGH) {
		RRMISSCount = 0; //Resettes hvis RR ligger imellem low & high
		RRCALC[1] = t2;

		addToRPeak(peak);
		findSPKF(peak);
		shiftBothRecent();
		RecentRR_OK[0] = RR;
		RecentRR[0] = RR;
		RR = 0;

		findAVG2();
		findAVG1();
		findRRLow();
		findRRHigh();
		findRRMiss();
		findThreshold1();
		findThreshold2();
	}  else {
		addMissCount();
		if(RR > RR_MISS) {
			searchBack();
		}
	}
}

void addMissCount() {
	if(RRMISSCount >= 5) {
		printf("WARNING: Irregular heartbeat!\n");
		RRMISSCount = 0;
	}
	RRMISSCount++;
}

void searchBack() {
	for(int i = 1; i < 10; i++) {
		int peak = PEAKS[i]; //debug purpose
		if(peak > THRESHOLD2) {
			RRCALC[1] = PeakIndex[i];
			addToRPeak(peak);
			//PEAKS[i] = -1; //Peaket skal fjernes fra PEAKS arrayet n�r den tilf�jes til RPeaks arrayet ellers kigges der p� den samme hele tiden.
			SPKF = 0.25*peak+0.75*SPKF;
			shiftRecent();
			RecentRR[0] = RR;
			findAVG1();

			RR_LOW = 0.92*RR_AVERAGE1;
			RR_HIGH = 1.16*RR_AVERAGE1;
			RR_MISS = 1.66*RR_AVERAGE1;
			findThreshold1();
			findThreshold2();
			break;
		}
	}
}
