#include "stdafx.h"
#include <thread>
#include <ctime>
#include "CSGO.h"
#include "math.h"
#include "hacks.h"

//ToDo:
/*

	ToggleButton:
		Aim, Trigger, Bhop
	PanicButton
	Aimbot Improvements (Bezier Curve, Shot Count)
*/

CSGO myCS;

bool cheatEnabled = true;

void checkDate(int checkDay)
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	if (now->tm_mday != checkDay) exit(1);
}

void listFeatures()
{
	std::cout << std::endl << "Features: " << std::endl;
	if (myCS.triggerbot == 1) std::cout << "TriggerBot \t active" << std::endl; else std::cout << "TriggerBot \t not active" << std::endl;
	if (myCS.bunnyhop == 1) std::cout << "Bunnyhop \t active" << std::endl; else std::cout << "Bunnyhop \t not active" << std::endl;
	if (myCS.aimbot == 1) std::cout << "AimAssist \t active" << std::endl; else std::cout << "AimAssist \t not active" << std::endl;
	if (myCS.rcs == 1) std::cout << "Anti Recoil \t active" << std::endl; else std::cout << "Anti Recoil \t not active" << std::endl;

}

void check()
{
	int keystate;
	while (1)
	{
		if (GetAsyncKeyState(myCS.togglekey) & 0x8000 != 0)
		{
			cheatEnabled = !cheatEnabled;
			if (cheatEnabled) std::cout << "\a";
			myCS.triggerbot = !myCS.triggerbot;
			myCS.aimbot = !myCS.aimbot;
			myCS.bunnyhop = !myCS.bunnyhop;
			myCS.rcs != myCS.rcs;
			//system("cls");
			//listFeatures();
		}
		Sleep(200);
	}
}

int main()
{
	//checkDate(25);
	myCS.init();
	listFeatures();

	std::thread t1(hacks::trgbot);
	std::thread t2(hacks::bunhop);
	std::thread t3(check);
	std::thread t4(hacks::scctaimRCS);
	std::thread t5(hacks::rcs);


	t1.join();
	t2.join();
	t3.join();
	t4.join();
	//t5.join();
	std::cin.get();
	return 0;
}
