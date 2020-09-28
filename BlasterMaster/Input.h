#ifndef INPUT_H
#define INPUT_H

#include "useful_stuff.h"
#include "windowsx.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Input final : private NonCopyable
{
    public:
        Input();
        ~Input();

        RESULT Initialize();
		/** Clear all temporary / one-off flags (onclick, etc.)*/
        RESULT Update();
        RESULT Release();

		/** call this to report key down*/
        RESULT keydown(WPARAM, LPARAM);
		/** call this to report key up*/
        RESULT keyup(WPARAM, LPARAM);
		/** call this to report mouse changed location*/
        RESULT mousechange(WPARAM, LPARAM);
		/** Get keycode for character x
			Keycode definition at bottom of file
		*/
        int operator [](char x);

		/** Get mouse location compared to screen. For UI elements.
			Screen coord is (-1,-1) to (1,1) top left to bottom right.*/
		Point MouseToScreen();
		/** Get mouse location compared to screen width. For game elements.
			Screen width is -1 to 1, screen height keep aspect ratio.*/
		Point MouseToField();
		int MouseFlag();
    protected:

    private:
		int vkeyboard[256];
        int keyboard[256];
		Point mouse;
		/*
			!!! why delete this !!! AAAAAHHHHHHHH
			wparam meaning
			0x0001: LButton down
			0x0002: RButton down
			0x0004: SHIFT down.	WARNING: UPDATE IRREGULAR
			0x0008: CTRL down	WARNING: UPDATE IRREGULAR
			0x0010: MButton down
			0x0020: XButton1 down
			0x0040: XButton2 down
			2 upper bit similar but mark whether it has just changed
		*/
		int mouseFlag;
        RESULT exitCode;
};

#define KEY_STATE_DOWN 1
#define KEY_STATE_UP 0
#define KEY_STATE_ON_DOWN 3
#define KEY_STATE_ON_UP 2
//Note: 0x02 is flag for on event.

#endif // INPUT_H
