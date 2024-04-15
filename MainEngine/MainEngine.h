
#ifndef MAINMANAGER_CLASS_H
#define MAINMANAGER_CLASS_H

#include <vector>
#include <string>
#include <map>
#include <list>


class MainEngine
{

public:

	static MainEngine* _mainmanagerptr;

	static MainEngine* GetInstance()
	{
		if (_mainmanagerptr == nullptr)
		{
			_mainmanagerptr = new MainEngine();
		}

		return _mainmanagerptr;
	}

	~MainEngine();

	MainEngine();
	void Init();



	bool ToReset = false;

	float simulationspeed = 1.0f;

	bool pauseapp = true;

	bool stepapp = false;

	bool bool1;
	bool bool2;
	bool bool3;
	bool bool4;
	bool bool5;

private:



};


#endif

