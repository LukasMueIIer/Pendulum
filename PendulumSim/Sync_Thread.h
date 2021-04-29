#pragma once
#include <mutex>
#include <condition_variable>

using namespace std;

class Sync_Thread
{
private:
	int iCount = 0;
	int iTarget = 0;
	int iLogout = 1;
	mutex muteMain;
	condition_variable cv;
public:
	void sync(int n);
	void reset();
private:
	bool Check();
};

