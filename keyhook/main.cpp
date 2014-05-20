#include <iostream>
#include <string>

#include "KeyboardHook.h"
#include "KeyboardHookDelegate.h"

class Delegate : public KeyboardHookDelegate
{
protected:
	virtual bool shouldBlockKey(const KBDLLHOOKSTRUCT *p) final
	{
		std::cout << p->vkCode << std::endl;
		return false;
	}
};

int main()
{
	auto& hook = KeyboardHook::sharedKeyboardHook();
	auto delegate = hook.addDelegate<Delegate>();

	hook.initHook();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}