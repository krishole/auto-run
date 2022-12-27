#include <chrono>
#include <iostream>
#include <Windows.h>
#include <random>
#include <string>

constexpr auto VK_KEY_W = 0x11;
constexpr auto VK_KEY_LSHIFT = 0x2A;

constexpr auto VK_KEY_G = 0x47;

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

int main() {
	std::string lastLine = "";
	std::string nextLine = "Waiting";

	bool doOnce = false;
	bool waitingRelease = false;
	bool moving = false;

	while (true) {
		if (!waitingRelease && (GetKeyState(VK_KEY_G) & 0x8000)) {
			waitingRelease = true;
		
			moving = !moving;
			KeyEvent(VK_KEY_W, moving);
			KeyEvent(VK_KEY_LSHIFT, moving);
			nextLine = moving ? "Moving" : "Waiting";
		} else if (waitingRelease && !(GetKeyState(VK_KEY_G) & 0x8000)) {
			waitingRelease = false;
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
