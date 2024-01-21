#include <../Engine/Inc/NEngine.h>

using namespace NEngine;

class Student
{
public:
	Student(string name) : name(name), mark(0) {}
	string name;
	int mark;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	{
		auto studentPool = Core::TypedAllocator<Student>("StudentPool", 100);

		Student* a = studentPool.New("Jon");
		Student* b = studentPool.New("Naashon");

		studentPool.Delete(a);
		studentPool.Delete(b);
	}
	

	return 0;
}