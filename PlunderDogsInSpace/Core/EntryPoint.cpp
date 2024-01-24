#include "Application.h"
#include "ServiceLocator.h"

int main()
{
	//Application* app = EntryApp();
	ServiceLocator::Init();
	if (ServiceLocator::GetMainService()->Init())
	{
		ServiceLocator::GetMainService()->Run();
	}
	ServiceLocator::CleanUp();
}