#include "G4eeToPositroniumModel.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4TrackStatus.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4ParaPositronium.hh"
#include "G4OrthoPositronium.hh"
#include "G4Gamma.hh"

#include "Randomize.hh"
#include "G4ParticleChangeForGamma.hh"

#include "G4Log.hh"
#include "G4Exp.hh"

using namespace std;

G4eeToPositroniumModel::G4eeToPositroniumModel(
    const G4ParticleDefinition*,
    const G4String& nam)
    :G4VEmModel(nam),
    pi_rcl2(pi*classic_electr_radius*classic_electr_radius)
{
    theGamma = G4Gamma::Gamma();
    theParaPositronium = G4ParaPositronium::ParaPositronium();
    theOrthoPositronium = G4OrthoPositronium::OrthoPositronium();
    fParticleChange = nullptr;
}

G4eeToPositroniumModel::~G4eeToPositroniumModel()
{}

void G4eeToPositroniumModel::Initialise(
    const G4ParticleDefinition*,
    const G4DataVector&)
{
    if (fParticleChange) { return; }
    fParticleChange = GetParticleChangeForGamma();    // used for discrete electromagnetic processes
}

G4double G4eeToPositroniumModel::ComputeCrossSectionPerElectron(G4double kineticEnergy)
{
  // Calculates the cross section per electron of annihilation into two photons
  // from the Heilter formula.

  G4double ekin  = std::max(eV,kineticEnergy);   

  G4double tau   = ekin/electron_mass_c2;
  G4double gam   = tau + 1.0;
  G4double gamma2= gam*gam;
  G4double bg2   = tau * (tau+2.0);
  G4double bg    = sqrt(bg2);

  G4double cross = pi_rcl2*((gamma2+4*gam+1.)*G4Log(gam+bg) - (gam+3.)*bg)
                 / (bg2*(gam+1.));
  return cross;  
}

G4double G4eeToPositroniumModel::ComputeCrossSectionPerAtom(
                                    const G4ParticleDefinition*,
                                    G4double kineticEnergy, G4double Z,
				    G4double, G4double, G4double)
{
  // Calculates the cross section per atom of annihilation into two photons
  
  G4double cross = Z*ComputeCrossSectionPerElectron(kineticEnergy);
  return cross;  
}


G4double G4eeToPositroniumModel::CrossSectionPerVolume(
					const G4Material* material,
					const G4ParticleDefinition*,
					      G4double kineticEnergy,
					      G4double, G4double)
{
  // Calculates the cross section per volume of annihilation into two photons
  
  G4double eDensity = material->GetElectronDensity();
  G4double cross = eDensity*ComputeCrossSectionPerElectron(kineticEnergy);
  return cross;
}

void G4eeToPositroniumModel::SampleSecondaries(
    vector<G4DynamicParticle*>* vdp,
    const G4MaterialCutsCouple*,
    const G4DynamicParticle* dp,
    G4double tmin,
    G4double maxEnergy)
{
    G4double posiKinEnergy = dp->GetKineticEnergy();    // the kinectic energy of positron
    G4DynamicParticle *aPositronium, *aGamma1, *aGamma2;

    CLHEP::HepRandomEngine* rndmEngine = G4Random::getTheEngine();  // "rndmEngine->flat()" returns in range [0, 1)

    // Case 1: less than & equal to 6.8*eV, binding energy of positronium at ground state
    if (posiKinEnergy <= 6.8*eV) {
        G4double Br_p_Ps = 0.25;        // branching ratio of para-positronium in vacuum
        // momemtum direction for positronium at rest
        G4ThreeVector momentum(0., 0., 0.);

        // the branching ratio should vary in different material, not complete yet.
        if (rndmEngine->flat() < Br_p_Ps) {
            aPositronium = new G4DynamicParticle(theParaPositronium, momentum);
        }
        else {
            aPositronium = new G4DynamicParticle(theOrthoPositronium, momentum);
        }
        vdp->push_back(aPositronium);
    }
    // Case 2: greater than 6.8*eV, Positron interacts in flight
    else {
        G4ThreeVector posiDirection = dp->GetMomentumDirection();

        G4double tau = posiKinEnergy / electron_mass_c2;
        G4double gam = tau + 1.0;
        G4double tau2 = tau + 2.0;
        G4double sqgrate = sqrt(tau / tau2) * 0.5;
        G4double sqg2m1 = sqrt(tau * tau2);

        // limits of the energy sampling
        G4double epsilmin = 0.5 - sqgrate;
        G4double epsilmax = 0.5 + sqgrate;
        G4double epsilqot = epsilmax / epsilmin;

        //
        // sample the energy rate of the created gammas
        //
        G4double epsil, greject;

        do {
            epsil = epsilmin * G4Exp(G4Log(epsilqot) * rndmEngine->flat());
            greject = 1. - epsil + (2. * gam * epsil - 1.) / (epsil * tau2 * tau2);
            // Loop checking, 03-Aug-2015, Vladimir Ivanchenko
        } while (greject < rndmEngine->flat());

        //
        // scattered Gamma angles. ( Z - axis along the parent positron)
        //

        G4double cost = (epsil * tau2 - 1.) / (epsil * sqg2m1);
        if (std::abs(cost) > 1.0) {
            G4cout << "### G4eeToTwoGammaModel WARNING cost= " << cost
                << " positron Ekin(MeV)= " << posiKinEnergy
                << " gamma epsil= " << epsil
                << G4endl;
            if (cost > 1.0) cost = 1.0;
            else cost = -1.0;
        }
        G4double sint = sqrt((1. + cost) * (1. - cost));
        G4double phi = twopi * rndmEngine->flat();

        //
        // kinematic of the created pair
        //

        G4double totalEnergy = posiKinEnergy + 2.0 * electron_mass_c2;
        G4double phot1Energy = epsil * totalEnergy;

        G4ThreeVector phot1Direction(sint * cos(phi), sint * sin(phi), cost);
        phot1Direction.rotateUz(posiDirection);
        aGamma1 = new G4DynamicParticle(theGamma, phot1Direction, phot1Energy);
        phi = twopi * rndmEngine->flat();
        G4double cosphi = cos(phi);
        G4double sinphi = sin(phi);
        G4ThreeVector pol(cosphi, sinphi, 0.0);
        pol.rotateUz(phot1Direction);
        aGamma1->SetPolarization(pol.x(), pol.y(), pol.z());

        G4double phot2Energy = (1. - epsil) * totalEnergy;
        G4double posiP = sqrt(posiKinEnergy * (posiKinEnergy + 2. * electron_mass_c2));
        G4ThreeVector dir = posiDirection * posiP - phot1Direction * phot1Energy;
        G4ThreeVector phot2Direction = dir.unit();

        // create G4DynamicParticle object for the particle2
        aGamma2 = new G4DynamicParticle(theGamma, phot2Direction, phot2Energy);

        //!!! likely problematic direction to be checked
        pol.set(-sinphi, cosphi, 0.0);
        pol.rotateUz(phot1Direction);
        cost = pol * phot2Direction;
        pol -= cost * phot2Direction;
        pol = pol.unit();
        aGamma2->SetPolarization(pol.x(), pol.y(), pol.z());
        /*
        G4cout << "Annihilation on fly: e0= " << posiKinEnergy
         << " m= " << electron_mass_c2
         << " e1= " << phot1Energy
         << " e2= " << phot2Energy << " dir= " <<  dir
         << " -> " << phot1Direction << " "
         << phot2Direction << G4endl;
        */
        vdp->push_back(aGamma1);
        vdp->push_back(aGamma2);
    }

    // kill primary positron
    //G4cout << "Positron is killed" << G4endl;
    //G4cout << "positron KinEnergy: " << posiKinEnergy/keV << "keV" << G4endl;
    fParticleChange->ProposeTrackStatus(fStopAndKill);
    //fParticleChange->SetProposedKineticEnergy(0.0);
}