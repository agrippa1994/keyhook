#include "KeyboardHook.h"

LRESULT WINAPI ll_proc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return KeyboardHook::sharedKeyboardHook().keyProc(nCode, wParam, lParam);
}

KeyboardHook KeyboardHook::_keyboardHook;


KeyboardHook::KeyboardHook() : _hook(0)
{
}

KeyboardHook::~KeyboardHook()
{
	removeHook();
}

bool KeyboardHook::initHook()
{
	if (_hook)
		return false;

	_hook = SetWindowsHookExA(WH_KEYBOARD_LL, ll_proc, GetModuleHandle(NULL), 0);
	return true;
}

bool KeyboardHook::removeHook()
{
	if (_hook == 0)
		return false;

	UnhookWindowsHookEx(_hook);
	_hook = 0;

	return true;
}

KeyboardHook& KeyboardHook::sharedKeyboardHook()
{
	return _keyboardHook;
}

bool KeyboardHook::removeDelegate(std::shared_ptr<KeyboardHookDelegate> delegate)
{
	try
	{
		return _delegates.erase(delegate->shared()) > 0;
	}
	catch (...){
		return false;
	}
}

LRESULT WINAPI KeyboardHook::keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	bool shouldBlock = false;

	for (auto& i : _delegates)
		if (i->shouldBlockKey((KBDLLHOOKSTRUCT *) lParam))
			shouldBlock = true;

	if (shouldBlock)
		return 1;

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}