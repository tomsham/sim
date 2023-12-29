#ifndef G4OrthoPositronium_HH
#define G4OrthoPositronium_HH

#include "G4ParticleDefinition.hh"

class G4OrthoPositronium : public G4ParticleDefinition
{
private:
	static G4OrthoPositronium* theInstance;
    G4OrthoPositronium() {}
	~G4OrthoPositronium() {}
public:
	static G4OrthoPositronium* Definition();
	static G4OrthoPositronium* OrthoPositroniumDefinition();
	static G4OrthoPositronium* OrthoPositronium();
};

#endif