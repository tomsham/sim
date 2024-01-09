#include "G4eeToPositronium.hh"

using namespace std;

G4eeToPositronium::G4eeToPositronium(const G4String& name)
	:G4VEmProcess(name), isInitialised(false)
{
	theGamma = G4Gamma::Gamma();
	theElectron = G4Electron::Electron();
	theParaPositronium = G4ParaPositronium::ParaPositronium();
	theOrthoPositronium = G4OrthoPositronium::OrthoPositronium();
	SetBuildTableFlag(false);
	SetStartFromNullFlag(false);

	//SetCrossSectionType(fEmDecreasing);
	SetSecondaryParticle(theGamma);
	SetSecondaryParticle(theParaPositronium);
	SetSecondaryParticle(theOrthoPositronium);

	SetProcessSubType(fElectromagnetic);
	//SetProcessSubType(fDecay);
	//SetProcessSubType(fAnnihilation);

	enableAtRestDoIt = true;
	enableAlongStepDoIt = false;
	enablePostStepDoIt = true;
	mainSecondaries = 1;
}

G4eeToPositronium::~G4eeToPositronium()
{}

G4bool G4eeToPositronium::IsApplicable(const G4ParticleDefinition& p)
{
	return (&p == G4Positron::Positron());
}

G4double G4eeToPositronium::AtRestGetPhysicalInteractionLength(const G4Track&, G4ForceCondition* condition)
{
	*condition = NotForced;
	return 0.0;
}

//G4double G4eeToPositronium::PostStepGetPhysicalInteractionLength(
//	const G4Track& track,
//	G4double  previousStepSize,
//	G4ForceCondition* condition)
//{
//	*condition = NotForced;
//	G4double length;
//	//length = DBL_MAX;
//	//length = 1/(CrossSection*NumberDensity)
//	length = 0.0001;		// made-up interaction length
//
//	return length;
//}

void G4eeToPositronium::InitialiseProcess(const G4ParticleDefinition*)
{
	if (!isInitialised) {
		isInitialised = true;
		if (!EmModel(0)) { SetEmModel(new G4eeToPositroniumModel()); }
		//EmModel(0)->SetLowEnergyLimit(MinKinEnergy());		// G4VEmProcess::MinKinEnergy = 0.1*keV;
		EmModel(0)->SetLowEnergyLimit(0.*eV);
		EmModel(0)->SetHighEnergyLimit(MaxKinEnergy());		// G4VEmProcess::MaxKinEnergy = 100.0*TeV;
		AddEmModel(1, EmModel(0));
	}
}

void G4eeToPositronium::StreamProcessInfo(std::ostream&) const
{}

G4VParticleChange* G4eeToPositronium::ContentOfProcess(const G4Track& track, const G4Step& step)
// Performs the Positronium formation when both particles are assumed at rest.
{
	fParticleChange.InitializeForPostStep(track);

	DefineMaterial(track.GetMaterialCutsCouple());
	/*
	MaterialCuts are the production cuts for a material.
	Production cuts are cutoff distances of production of secondaries.
	CoupleIndex is the index of a list of active region for determine whether some interaction should be performed
	*/
	size_t idx = CurrentMaterialCutsCoupleIndex();
	G4double ene(0.0);
	G4VEmModel* model = SelectModel(ene, idx);

	// define new weight for primary and secondaries
	G4double weight = fParticleChange.GetParentWeight();

	// sample secondaries, positronium energy loss
	secParticles.clear();
	//G4double electronCut = GetElectronEnergyCut();
	G4double electronCut = GetGammaEnergyCut();
	model->SampleSecondaries(&secParticles, MaterialCutsCouple(), track.GetDynamicParticle(), electronCut);

	// splitting or Russian roulette
	if (biasManager) {
		if (biasManager->SecondaryBiasingRegion(idx)) {
			G4double eloss = 0.0;
			weight *= biasManager->ApplySecondaryBiasing(
				secParticles, track, model, &fParticleChange, eloss,
				idx, electronCut, step.GetPostStepPoint()->GetSafety());
			if (eloss > 0.0) {
				eloss += fParticleChange.GetLocalEnergyDeposit();
				fParticleChange.ProposeLocalEnergyDeposit(eloss);
			}
		}
	}

	// save secondaries
	G4int num = secParticles.size();
	G4int num0 = secParticles.size();
	if (num > 0) {
		fParticleChange.SetNumberOfSecondaries(num);
		G4double edep = fParticleChange.GetLocalEnergyDeposit();
		G4double time = track.GetGlobalTime();

		for (G4int i = 0; i < num; ++i) {
			if (secParticles[i]) {
				G4DynamicParticle* dp = secParticles[i];
				const G4ParticleDefinition* p = dp->GetParticleDefinition();
				G4double e = dp->GetKineticEnergy();
				G4bool good = true;
				if (ApplyCuts()) {
					if (p == theParaPositronium || p == theOrthoPositronium) {
						if (e < electronCut) { good = false; }
					}
					else if (p == theGamma) {
						if (e < GetGammaEnergyCut()) { good = false; }
					}
					else if (p == theElectron) {
						if (e < GetElectronEnergyCut()) { good = false; }
					}
				}
				if (good) {		// added secondary if it is good
					G4Track* t = new G4Track(dp, time, track.GetPosition());
					t->SetTouchableHandle(track.GetTouchableHandle());
					if (biasManager) {
						t->SetWeight(weight * biasManager->GetWeight(i));
					}
					else {
						t->SetWeight(weight);
					}
					pParticleChange->AddSecondary(t);

					// define type of secondary
					if (i < mainSecondaries) { t->SetCreatorModelID(secID); }
					else if (i < num0) {
						if (p == theGamma) {
							t->SetCreatorModelID(fluoID);
						}
						else {
							t->SetCreatorModelID(augerID);
						}
					}
					else {
						t->SetCreatorModelID(biasID);
					}
				}
				else {
					delete dp;
					edep += e;
				}
			}
		}
		fParticleChange.ProposeLocalEnergyDeposit(edep);
	}
	return &fParticleChange;
}

G4VParticleChange* G4eeToPositronium::AtRestDoIt(const G4Track& track, const G4Step& step)
{
	return ContentOfProcess(track, step);
}

G4VParticleChange* G4eeToPositronium::PostStepDoIt(const G4Track& track, const G4Step& step)
{
	return ContentOfProcess(track, step);
}



void G4eeToPositronium::ProcessDescription(std::ostream& out) const
{
	out << "  Positronium formation";
	G4VEmProcess::ProcessDescription(out);
}
