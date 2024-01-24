#pragma once
#include "../Core/Application.h"

//function to make a new application, marked as extern so that users can define their own derived application class
extern Application* EntryApp();

class ServiceLocator
{
public:
	static void Init()
	{
		MainService = EntryApp();
	}
	static void CleanUp()
	{
		delete MainService;
	}
	static Application* GetMainService()
	{
		return MainService;
	}
	static DefaultGraphics& GetGraphics()
	{
		return MainService->GetGraphics();
	}
	static TimeManager& GetTimeService()
	{
		return MainService->m_timeManager;
	}
	static Application* MainService;
};