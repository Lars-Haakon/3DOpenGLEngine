#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard
{
public:
	static const char KEY_ESC;
	static const char KEY_SPACE;
	static const char KEY_RETURN;
	static const char KEY_A;
	static const char KEY_B;
	static const char KEY_C;
	static const char KEY_D;
	static const char KEY_E;
	static const char KEY_F;
	static const char KEY_G;
	static const char KEY_H;
	static const char KEY_I;
	static const char KEY_J;
	static const char KEY_K;
	static const char KEY_L;
	static const char KEY_M;
	static const char KEY_N;
	static const char KEY_O;
	static const char KEY_P;
	static const char KEY_Q;
	static const char KEY_R;
	static const char KEY_S;
	static const char KEY_T;
	static const char KEY_U;
	static const char KEY_V;
	static const char KEY_W;
	static const char KEY_X;
	static const char KEY_Y;
	static const char KEY_Z;

	static void update();

	static bool getKey(int keyCode);
	static void setKey(int keyCode, bool state);
	static bool getKeyDown(int keyCode);
	static bool getKeyUp(int keyCode);
private:
	static bool lastKeyStates[];
	static bool keyStates[];
};

#endif