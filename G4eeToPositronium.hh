#ifndef G4eeToPositronium_hh
#define G4eeToPositronium_hh

#include "G4VEmProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4MaterialCutsCouple.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4ParaPositronium.hh"
#include "G4OrthoPositronium.hh"

#include "G4eeToPositroniumModel.hh"
#include "G4EmBiasingManager.hh"


class G4ParticleDefinition;

class G4eeToPositronium : public G4VEmProcess
{

public:

	explicit G4eeToPositronium(const G4String& name = "eplus2Ps");

	virtual ~G4eeToPositronium() override;

	// true for positron only
	virtual G4bool IsApplicable(const G4ParticleDefinition& particle) final;

	virtual G4VParticleChange* ContentOfProcess(
		const G4Track& track,
		const G4Step& stepData);

	virtual G4VParticleChange* AtRestDoIt(
		const G4Track& track,
		const G4Step& stepData) override;

	virtual G4double AtRestGetPhysicalInteractionLength(
		const G4Track& track,
		G4ForceCondition* condition) override;

	virtual G4VParticleChange* PostStepDoIt(
		const G4Track& track,
		const G4Step& stepData) override;

	/*virtual G4double PostStepGetPhysicalInteractionLength(
		const G4Track& track,
		G4double  previousStepSize,
		G4ForceCondition* condition) override;*/

	// print documentation in html format
	virtual void ProcessDescription(std::ostream&) const override;

	//G4eeToPositronium & operator=(const G4eeToPositronium &right) = delete;
	//G4eeToPositronium(const G4eeToPositronium&) = delete;

protected:

	// Print out of the class parameters
	virtual void StreamProcessInfo(std::ostream& outFile) const override;

	virtual void InitialiseProcess(const G4ParticleDefinition*) override;

private:
	//std::vector<G4DynamicParticle*>* GenerateSecondaries(const G4DynamicParticle*);
	G4bool isInitialised;
	const G4ParticleDefinition* theGamma;
	const G4ParticleDefinition* theElectron;
	const G4ParticleDefinition* theParaPositronium;
	const G4ParticleDefinition* theOrthoPositronium;
};

#endif