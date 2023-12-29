#ifndef G4eeToPositroniumModel_hh
#define G4eeToPositroniumModel_hh

#include "G4VEmModel.hh"

class G4ParticleChangeForGamma;

class G4eeToPositroniumModel : public G4VEmModel
{

public:

	explicit G4eeToPositroniumModel(
		const G4ParticleDefinition* p = 0,
		const G4String& nam = "eplus2Ps");

	virtual ~G4eeToPositroniumModel();

	virtual void Initialise(
		const G4ParticleDefinition*,
		const G4DataVector&) override;

	virtual G4double ComputeCrossSectionPerElectron(G4double kinEnergy);

	virtual G4double ComputeCrossSectionPerAtom(
		const G4ParticleDefinition*,
		G4double kinEnergy,
		G4double Z,
		G4double A = 0.,
		G4double cutEnergy = 0.,
		G4double maxEnergy = DBL_MAX) override;

	virtual G4double CrossSectionPerVolume(const G4Material*,
		const G4ParticleDefinition*,
		G4double kineticEnergy,
		G4double cutEnergy = 0.0,
		G4double maxEnergy = DBL_MAX) override;

	virtual void SampleSecondaries(
		std::vector<G4DynamicParticle*>*,
		const G4MaterialCutsCouple*,
		const G4DynamicParticle*,
		G4double tmin,
		G4double maxEnergy) override;

private:

	// hide assignment operator
	G4eeToPositroniumModel & operator=(const  G4eeToPositroniumModel &right) = delete;
	G4eeToPositroniumModel(const  G4eeToPositroniumModel&) = delete;

	G4double pi_rcl2;

	const G4ParticleDefinition* theGamma;
	const G4ParticleDefinition* theParaPositronium;
	const G4ParticleDefinition* theOrthoPositronium;
	G4ParticleChangeForGamma* fParticleChange;
};
#endif
