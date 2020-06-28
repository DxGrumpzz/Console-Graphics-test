
#include "ConsoleEngine.hpp"
#include "IScence.hpp"


class SineScene : public IScence
{
private:
	ConsoleEngine& _consoleEngine;


public:

	SineScene(ConsoleEngine& consoleEngine) :
		_consoleEngine(consoleEngine)
	{
	};

public:

	virtual void DrawScene(float deltaTime) override
	{

	};


};