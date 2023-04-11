#include <chrono>
#include <iostream>
#include <Windows.h>
#include <random>
#include <string>

//https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa299374(v=vs.60)?redirectedfrom=MSDN
constexpr auto VSCAN_KEY_W = 0x11;
constexpr auto VSCAN_KEY_LSHIFT = 0x2A;
constexpr auto VSCAN_KEY_LCONTROL = 0x1D;

constexpr auto VK_KEY_W = 0x57;
constexpr auto VK_KEY_G = 0x47;
constexpr auto VK_KEY_X = 0x58;

void KeyEvent(WORD key, bool press) {
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = key;
	if (press) {
		//For key press Flag=0
		ip.ki.dwFlags = KEYEVENTF_SCANCODE;
	} else {
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	}
	SendInput(1, &ip, sizeof(INPUT));
}

static bool moving = false;
static std::string lastLine = "";
static std::string nextLine = "Waiting";

void Move(bool val) {
	moving = val;
	KeyEvent(VSCAN_KEY_W, moving);
	KeyEvent(VSCAN_KEY_LCONTROL, moving);
}

int main() {
	SetConsoleTitleA("Auto Runner");
	std::cout << "Pills' Auto Runner" << std::endl;
	std::cout << "Press X to start running." << std::endl;
	std::cout << "Press X Again or W to stop running." << std::endl;
	std::cout << "Press SHIFT+X to exit." << std::endl << std::endl;

	bool waitingRelease = false;
	bool running = true;

	while (running) {
		if (!waitingRelease && (GetKeyState(VK_KEY_X) & 0x8000)) {
			waitingRelease = true;
			Move(!moving);
		} else if (waitingRelease && !(GetKeyState(VK_KEY_X) & 0x8000)) {
			waitingRelease = false;
		} else if (moving && !(GetKeyState(VK_KEY_W) & 0x8000)) {
			Move(false);
		}

		if ((GetKeyState(VK_LSHIFT) & 0x8000) && (GetKeyState(VK_KEY_X) & 0x8000)) {
			running = false;
			Move(false);
		}

		if (lastLine != nextLine) {
			if (nextLine.length() < lastLine.length()) {
				int diff = (int)(lastLine.length() - nextLine.length());
				nextLine.append(diff, ' ');
			}

			//system("cls");
			std::cout << nextLine << "\r";
			lastLine = nextLine;
		}
		
		Sleep(16);
	}
}
