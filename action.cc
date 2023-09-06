#include "action.hh"

MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::BuildForMaster() const
{
    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);
}

void MyActionInitialization::Build() const
{
    //implement MyPrimaryGenerator
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);
    //implement MyRunAction
    MyRunAction *runAction = new MyRunAction();
    SetUserAction(runAction);
}
