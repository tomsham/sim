#ifndef G4ParaPositronium_HH
#define G4ParaPositronium_HH

#include "G4ParticleDefinition.hh"

class G4ParaPositronium : public G4ParticleDefinition
{
private:
	static G4ParaPositronium* theInstance;
    G4ParaPositronium() {}
	~G4ParaPositronium() {}
public:
	static G4ParaPositronium* Definition();
	static G4ParaPositronium* ParaPositroniumDefinition();
	static G4ParaPositronium* ParaPositronium();
};

#endif