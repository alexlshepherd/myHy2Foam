//---------------------------------------------------------
// Introduction of the thermo for chem. reac. flows
//---------------------------------------------------------
Info<< "Creating reaction model\n" << endl;

autoPtr<hTC2Models::rho2HTCModel> reaction
(
    hTC2Models::rho2HTCModel::New(mesh)
);

rho2ReactionThermo& thermo = reaction->thermo();

thermo.validate(args.executable(), "h", "e");

//---------------------------------------------------------
// Initialisation of the two-temperature model
//---------------------------------------------------------

thermo.initialise2T();

bool downgradeSingleVibMode = thermo.downgradeSingleVibMode();
bool downgradeSingleTv = thermo.downgradeSingleTv();
bool downgradeSingleT = thermo.downgradeSingleTemperature();

bool hyDev = thermo.lookupOrDefault<bool>("hyDev", false);
bool injectors = thermo.lookupOrDefault<bool>("injectors", false);
bool hyLight = thermo.hyLight();

//---------------------------------------------------------
// Loading some general thermophysical quantities
//---------------------------------------------------------
Info<< "\nReading thermophysical properties\n" << endl;

const volScalarField& psi = thermo.psi();
volScalarField& p = thermo.p();
volScalarField& rho = thermo.rho();

// Mixture composition
basic2MultiComponentMixture& composition = thermo.composition();

// Energy fields
volScalarField& et = thermo.het();
volScalarField& ev = thermo.hevel();
PtrList<volScalarField>& hevk = composition.hevel();
volScalarField& e = composition.e();

// Solver quantities
Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);

volScalarField Mach
(
    IOobject
    (
        "Mach",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    //mag(U)/sqrt(thermo.Cp_t()/thermo.Cv_t()/psi)
    
    // Changed to include total heat capacities, not just
    // translational-rotational. CvMix must be used instead
    // not sure why - Alex 01/23/2023
    
    mag(U)/sqrt(thermo.CpMix()/thermo.CvMix()/psi)
);

volScalarField gamma 
(
    // Write out heat capacity ratio - Alex 01/24/2023
    IOobject
    (
        "gamma",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    thermo.CpMix()/thermo.CvMix()
);

// Both Mach and gamma fields are updated in upwindInterpolation.H,
// this wouldn't work otherwise. Not sure why. - Alex

volVectorField rhoU
(
    IOobject
    (
        "rhoU",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    rho*U
);

volScalarField rhoE
(
    IOobject
    (
        "rhoE",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    rho*(e + 0.5*magSqr(U))
);

surfaceScalarField pos
(
    IOobject
    (
        "pos",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedScalar("pos", dimless, 1.0)
);

surfaceScalarField neg
(
    IOobject
    (
        "neg",
        runTime.timeName(),
        mesh
    ),
    mesh,
    dimensionedScalar("neg", dimless, -1.0)
);

surfaceScalarField phi("phi", mesh.Sf() & fvc::interpolate(rhoU));

//---------------------------------------------------------
// Introduction of the turbulence model
//---------------------------------------------------------
Info<< "Creating turbulence model\n" << endl;
autoPtr<compressible::turbulenceModel> turbulence
(
    compressible::turbulenceModel::New
    (
        rho,
        U,
        phi,
        thermo
    )
);

// Set the turbulence model into the reaction model
reaction->setTurbulence(turbulence());

//---------------------------------------------------------
// Initialisation of the transport quantities
//---------------------------------------------------------
autoPtr<mixingRule> transport
(
    mixingRule::New
    (
        thermo,
        turbulence()
    )
);

const volScalarField& mu = thermo.mu();

//- Bulk viscosity
/*const scalar pi = constant::mathematical::pi;
const scalar ZrotInf = 18.0;
const dimensionedScalar TZ ("TZ", dimTemperature, 91.5);
volScalarField Zrot =
    ZrotInf/(1.0 + sqr(pi)/2.0*sqrt(TZ/thermo.T()) 
  + (sqr(pi)/4.0 + pi)*TZ/thermo.T());
volScalarField Z = 3.0*pi/(4.0*(thermo.zetar()+3.0))*Zrot;

volScalarField mub = mu*(2.0*thermo.zetar()*Z)/(3.0*(thermo.zetar()+3.0));
volScalarField bulkToShearRatio
(
    IOobject
    (
        "bulkToShearRatio",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("scalarZero", dimless, 0.0)
);*/

bool inviscid = true;
if (max(mu.primitiveField()) > 0.0)
{
    inviscid = false;
}

//---------------------------------------------------------
// Definition of the rarefaction parameters
//---------------------------------------------------------
autoPtr<rarefactionParameter> rarefactionParameters
(
    rarefactionParameter::New
    (
        thermo,
        turbulence()
    )
);
rarefactionParameters->correct(U);


//---------------------------------------------------------
// Heat flux vectors
//---------------------------------------------------------

bool writeWallHeatFlux =
    thermo.transportDictionary().subDict("transportModels")
        .lookupOrDefault<bool>("writeWallHeatFlux", false);

//- Wall heat flux
volScalarField wallHeatFlux
(
    IOobject
    (
        "wallHeatFlux",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("wallHeatFlux", dimensionSet(1, 0, -3, 0, 0), 0.0)
);

if (writeWallHeatFlux)
{
    wallHeatFlux.writeOpt() = IOobject::AUTO_WRITE;
}

