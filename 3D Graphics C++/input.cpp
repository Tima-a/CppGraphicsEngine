namespace Input
{
	struct key
	{
		int LeftMouse = 0x01;
		int RightMouse = 0x02;
		int MiddleMouse = 0x04;
		int Backspace = 0x08;
		int Tab = 0x09;
		int Enter = 0x0D;
		int Pause = 0x13;
		int CapsLock = 0x14;
		int Escape = 0x1B;
		int Space = 0x20;
		int PageUp = 0x21;
		int PageDown = 0x22;
		int End = 0x23;
		int Home = 0x24;
		int LeftArrow = 0x25;
		int UpArrow = 0x26;
		int RightArrow = 0x27;
		int DownArrow = 0x28;
		int Select = 0x29;
		int Print = 0x2A;
		int PrintScreen = 0x2C;
		int Ins = 0x2D;
		int Delete = 0x2E;
		int Help = 0x2F;
		int digit0 = 0x30;
		int digit1 = 0x31;
		int digit2 = 0x32;
		int digit3 = 0x33;
		int digit4 = 0x34;
		int digit5 = 0x35;
		int digit6 = 0x36;
		int digit7 = 0x37;
		int digit8 = 0x38;
		int digit9 = 0x39;
		int A = 0x41;
		int B = 0x42;
		int C = 0x43;
		int D = 0x44;
		int E = 0x45;
		int F = 0x46;
		int G = 0x47;
		int H = 0x48;
		int I = 0x49;
		int J = 0x4A;
		int K = 0x4B;
		int L = 0x4C;
		int M = 0x4D;
		int N = 0x4E;
		int O = 0x4F;
		int P = 0x50;
		int Q = 0x51;
		int R = 0x52;
		int S = 0x53;
		int T = 0x54;
		int U = 0x55;
		int V = 0x56;
		int W = 0x57;
		int X = 0x58;
		int Y = 0x59;
		int Z = 0x5A;
		int LeftWindows = 0x5B;
		int RightWindows = 0x5C;
		int Applications = 0x5D;
		int Sleep = 0x5F;
		int Numpad0 = 0x60;
		int Numpad1 = 0x61;
		int Numpad2 = 0x62;
		int Numpad3 = 0x63;
		int Numpad4 = 0x64;
		int Numpad5 = 0x65;
		int Numpad6 = 0x66;
		int Numpad7 = 0x67;
		int Numpad8 = 0x68;
		int Numpad9 = 0x69;
		int Multiply = 0x6A;
		int Add = 0x6B;
		int Separator = 0x6C;
		int Substract = 0x6D;
		int Decimal = 0x6E;
		int Divide = 0x6F;
		int F1 = 0x70;
		int F2 = 0x71;
		int F3 = 0x72;
		int F4 = 0x73;
		int F5 = 0x74;
		int F6 = 0x75;
		int F7 = 0x76;
		int F8 = 0x77;
		int F9 = 0x78;
		int F10 = 0x79;
		int F11 = 0x7A;
		int F12 = 0x7B;
		int F13 = 0x7C;
		int F14 = 0x7D;
		int F15 = 0x7E;
		int F16 = 0x7F;
		int F17 = 0x80;
		int F18 = 0x81;
		int F19 = 0x82;
		int F20 = 0x83;
		int F21 = 0x84;
		int F22 = 0x85;
		int F23 = 0x86;
		int F24 = 0x87;
		int NumLock = 0x90;
		int Scroll = 0x91;
		int LeftShift = 0xA0;
		int RightShift = 0xA1;
		int LeftControl = 0xA2;
		int RightControl = 0xA3;
		int LeftAlt = 0xA4;
		int RightAlt = 0xA5;
		int VolumeMute = 0xAD;
		int VolumeDown = 0xAE;
		int VolumeUp = 0xAF;
		int Play = 0xFA;
		int Zoom = 0xFB;
	} Keys;
	bool KeyPressed(int keycode_)
	{
		if ((GetKeyState(keycode_) & 0x8000))
		{
			return true;
		}
		else
		{
			false;
		}
	}
}