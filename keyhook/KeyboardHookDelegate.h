#pragma once
#include "windows.h"
#include <memory>

class KeyboardHookDelegate : public std::enable_shared_from_this<KeyboardHookDelegate>
{
	friend class KeyboardHook;

public:
	std::shared_ptr<KeyboardHookDelegate> shared()
	{
		return shared_from_this();
	}

protected:
	virtual bool shouldBlockKey(const KBDLLHOOKSTRUCT *) = 0;
};