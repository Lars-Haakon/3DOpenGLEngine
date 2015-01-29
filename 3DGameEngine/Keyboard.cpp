#include "Keyboard.h"

const char Keyboard::KEY_ESC = 0x1B;
const char Keyboard::KEY_SPACE = 0x20;
const char Keyboard::KEY_RETURN = 0xD;
const char Keyboard::KEY_A = 0x41;
const char Keyboard::KEY_B = 0x42;
const char Keyboard::KEY_C = 0x43;
const char Keyboard::KEY_D = 0x44;
const char Keyboard::KEY_E = 0x45;
const char Keyboard::KEY_F = 0x46;
const char Keyboard::KEY_G = 0x47;
const char Keyboard::KEY_H = 0x48;
const char Keyboard::KEY_I = 0x49;
const char Keyboard::KEY_J = 0x4A;
const char Keyboard::KEY_K = 0x4B;
const char Keyboard::KEY_L = 0x4C;
const char Keyboard::KEY_M = 0x4D;
const char Keyboard::KEY_N = 0x4E;
const char Keyboard::KEY_O = 0x4F;
const char Keyboard::KEY_P = 0x50;
const char Keyboard::KEY_Q = 0x51;
const char Keyboard::KEY_R = 0x52;
const char Keyboard::KEY_S = 0x53;
const char Keyboard::KEY_T = 0x54;
const char Keyboard::KEY_U = 0x55;
const char Keyboard::KEY_V = 0x56;
const char Keyboard::KEY_W = 0x57;
const char Keyboard::KEY_X = 0x58;
const char Keyboard::KEY_Y = 0x59;
const char Keyboard::KEY_Z = 0x5A;

bool Keyboard::lastKeyStates[256];
bool Keyboard::keyStates[256];

void Keyboard::update()
{
	for (int i = 0; i < 256; i++)
		lastKeyStates[i] = keyStates[i];
}

bool Keyboard::getKey(int keyCode)
{
	return keyStates[keyCode];
}

void Keyboard::setKey(int keyCode, bool state)
{
	keyStates[keyCode] = state;
}

bool Keyboard::getKeyDown(int keyCode)
{
	return keyStates[keyCode] && !lastKeyStates[keyCode];
}

bool Keyboard::getKeyUp(int keyCode)
{
	return !keyStates[keyCode] && lastKeyStates[keyCode];
}