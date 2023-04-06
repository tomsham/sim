#include "action.hh"

MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::BuildForMaster() const
{
	MyRunAction* runAction = new MyRunAction();
	SetUserAction(runAction);
}

void MyActionInitialization::Build() const
{
	// Create a object which is a the particle gun that is set by generator.cc
	// Run the particle gun
	MyPrimaryGenerator* generator = new MyPrimaryGenerator();
	SetUserAction(generator);

	// Run the simulation and save the result
	MyRunAction* runAction = new MyRunAction();
	SetUserAction(runAction);

	MyEventAction* eventAction = new MyEventAction(runAction);
	SetUserAction(eventAction);

	MySteppingAction* steppingAction = new MySteppingAction(eventAction);
	SetUserAction(steppingAction);
}