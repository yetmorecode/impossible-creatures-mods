/////////////////////////////////////////////////////////////////////
// File    : InputTypes.h
// Desc    : 
// Created : Wednesday, February 21, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//
#pragma once

#include <Math/Vec2.h>

namespace Plat
{

	enum InputKey
	{
		//	Keyboard buttons (with some minor ascii matching)

		KEY_A = 'A',
		KEY_B,	KEY_C,	KEY_D,	KEY_E,	KEY_F,	KEY_G,	KEY_H,	KEY_I,	KEY_J,	
		KEY_K,	KEY_L,	KEY_M,	KEY_N,	KEY_O,	KEY_P,	KEY_Q,	KEY_R,	KEY_S,	
		KEY_T,	KEY_U,	KEY_V,	KEY_W,	KEY_X,	KEY_Y,	KEY_Z,

		KEY_0 = '0',
		KEY_1,	KEY_2,	KEY_3,	KEY_4,	KEY_5,	KEY_6,	KEY_7,	KEY_8,	KEY_9,

		KEY_Backspace	= 0x08,
		KEY_Tab			= 0x09,
		KEY_Enter		= 0x0d,
		KEY_Escape		= 0x1b,
		KEY_Space		= 0x20,
		KEY_Apostrophe	= 0x27,
		KEY_Comma		= 0x2c,
		KEY_Minus		= 0x2d,
		KEY_Period		= 0x2e,
		KEY_Slash		= 0x2f,
		KEY_Semicolon	= 0x3b,
		KEY_Equal		= 0x3d,
		KEY_LBracket	= 0x5b,		//	[ {
		KEY_Backslash	= 0x5c,
		KEY_RBracket	= 0x5d,		//	] }
		KEY_Grave		= 0x60,			//	` ~

		//	Non-ascii keyboard keys (using high-ascii range)
		KEY_Up			= 0x80,
		KEY_Down,
		KEY_Left,
		KEY_Right,
		KEY_Control,
		KEY_Shift,
		KEY_Alt,
		KEY_CapsLock,
		KEY_NumLock,
		KEY_ScrollLock,
		KEY_Insert,
		KEY_Delete,
		KEY_Home,
		KEY_End,
		KEY_PageUp,
		KEY_PageDown,
		KEY_F1,	KEY_F2,	KEY_F3,	KEY_F4,	KEY_F5,	KEY_F6,	KEY_F7,	KEY_F8,	KEY_F9,
		KEY_F10,		KEY_F11,		KEY_F12,
		KEY_PrintScreen,
		KEY_Pause,

		// NumberPad keys.
		KEY_NumPad0,
		KEY_NumPad1,
		KEY_NumPad2,
		KEY_NumPad3,
		KEY_NumPad4,
		KEY_NumPad5,
		KEY_NumPad6,
		KEY_NumPad7,
		KEY_NumPad8,
		KEY_NumPad9,
		KEY_NumPadMultiply,
		KEY_NumPadAdd,
		KEY_NumPadSeparator,
		KEY_NumPadSubtract,
		KEY_NumPadDecimal,
		KEY_NumPadDivide,


		//	Mouse buttons
		KEY_MouseLeft,
		KEY_MouseRight,
		KEY_MouseMiddle,
		KEY_MouseTop,
		KEY_MouseBottom,

		//	Screen edge buttons
		KEY_ScreenLeft,
		KEY_ScreenRight,
		KEY_ScreenTop,
		KEY_ScreenBottom,

		//	UI-keys
		KEY_Close,		//	Alt-F4 or close button on window

		KEY_Invalid,
		KEY_Count = KEY_Invalid,
	};
	enum InputEventType
	{
		IET_MouseMotion,
		IET_MouseDrag,		//signifies button was down & mouse was moved enough to constitute a "drag"
		IET_WheelMotion,
		IET_KeyPress,
		IET_KeyRelease,
		IET_KeyRepeat,
		IET_KeyClick,
		IET_KeyDoubleClick,
		IET_TextInfoChange,	// this event is generated when textcapture is enabled
							// it signifies that either the text has changed or the caret has moved
		IET_TextComplete,	// This event means the text was completed, i.e. like enter, text capture should be canceld
		IET_TextCanceled,	// This event means the text wasn't completed, like hitting escape.
		IET_TextTabbed,		// This event means that the tab key was pressed while in text capture mode


		IET_Invalid,
		IET_Count = IET_Invalid,
	};
	enum InputFilterType
	{
		IFT_Mouse,		// signifies that a mouse event occured
		IFT_KeyBoard,	// signifies that a keyboard event occured

		IFT_Invalid,
		IFT_Count = IFT_Invalid,
	};
	struct InputEvent
	{
		InputEventType	type;
		InputFilterType	filter;
		union
		{
			InputKey	key;		//	key for this event (KEY_Invalid if mouse motion)
			float		wheelDelta;
		};
		Vec2f		mousePos;	//	actual mouse position (only valid when filter == IFT_Mouse)
		Vec2f		mouseDelta; //	delta of the mouse position, NOT the cursor position!!!
		double		time;
	};
	struct ComboKey
	{
		int			numkeys;
		InputKey	key[4];
	};

};