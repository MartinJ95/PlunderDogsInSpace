#include "Application.h"

//function to make a new application, marked as extern so that users can define their own derived application class
extern Application* EntryApp();

int main()
{
	Application* app = EntryApp();
}