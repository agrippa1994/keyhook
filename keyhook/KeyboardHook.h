#pragma once
#include "windows.h"
#include "KeyboardHookDelegate.h"

#include <memory>
#include <set>
#include <type_traits>

class KeyboardHook
{
	friend LRESULT WINAPI ll_proc(int nCode, WPARAM wParam, LPARAM lParam);

	static KeyboardHook _keyboardHook;

	HHOOK _hook;
	std::set< std::shared_ptr<KeyboardHookDelegate> > _delegates;

public:
	KeyboardHook();
	~KeyboardHook();

	bool initHook();
	bool removeHook();

	static KeyboardHook& sharedKeyboardHook();

	template<typename T, typename = std::enable_if<std::is_base_of<KeyboardHookDelegate, T>::value>::type>
	std::shared_ptr<T> addDelegate()
	{
		std::shared_ptr<T> ptr(new T);
		auto iterator = _delegates.insert(ptr);

		if (iterator.second)
			return ptr;

		return nullptr;
	}
	
	bool removeDelegate(std::shared_ptr<KeyboardHookDelegate> delegate);

private:
	LRESULT WINAPI keyProc(int nCode, WPARAM wParam, LPARAM lParam);
};