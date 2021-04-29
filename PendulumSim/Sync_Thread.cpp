#include "Sync_Thread.h"
bool Sync_Thread::Check() {
	if (iCount == iTarget) {
		return true;
	}
	else {
		return false;
	}
}

void Sync_Thread::reset() {
	iCount = 0;
}
void Sync_Thread::sync(int n) {
	unique_lock<mutex> uL{ muteMain };
	iTarget = n;
	iCount++;
	if (iCount == iTarget) {
		cv.notify_all(); //wake up all	
	}
	else {

		cv.wait(uL, std::bind(&Sync_Thread::Check, this));
		iLogout++;
		if (iLogout == n) {
			iCount = 0;
			iLogout = 1;
		}
	}
}