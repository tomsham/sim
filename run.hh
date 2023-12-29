#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4RootAnalysisManager.hh"
#include "G4Run.hh"


class MyRunAction : public G4UserRunAction
{
public:
	MyRunAction();
	~MyRunAction();
	void CreateDataFile(G4AnalysisManager*);
	void CreateDataFile_Vertex(G4AnalysisManager*);
	void CreateDataFile_Step(G4AnalysisManager*);
	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
};

#endif