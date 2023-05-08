# myHy2Foam
Extension of the openFoam solver hy2Foam to include turbulent terms. Modified equation files to account for turbulent diffusive and heat transfer terms. The heat capacity and Mach number are now written to file. Heat capacity is now calculated using the total heat capacities, rather than just the translational-rotational parts.

This github page is in reference to a university research project and the code is not well tested, user discretion is advised. These modifications were made with single temperature flows of neutral particles in mind, so this isn't likely to work with two temperature or ionized flows. Contributions to the main [hyStrath](https://hystrath.github.io/) project will be submitted shortly.

## Changes
Added a the heat capactiy ratio, gamma, as a volScalarField that is written to file. Calculation of gamma, and subsequently the Mach number, were modified to use the total (translational, rotational, vibrational, etc.) heat cpacites. The current release of hy2Foam, Fleming, uses only the tranlstional-rotational heat capacities to calculate the speed of sound, the reasons for this are unclear. 

The species conservation equation, 'YEqn.H', is modified to now include turbulent terms, making use of the turbulent Schmidt number defined in the transportProperties dictionary:
```
transportModels
{
  ...
  diffusionModelParameters
  {
    ...
    TurbulentSchmidtNumber   0.7;
  }
}
```
The viscous energy equation now includes turbulent thermal diffusivity terms. These changes have been only applied to the single temperature equations, so further modifications may need to be made to work with a two temperature model. 

## Known Issues
The turbulent Schmidt number must be defined in all cases, currently this parameter has no default value, and is still used in the laminar case, however since the turbulent viscosity `mut()` is zero this shouldn't be an issue. An error is occasionaly raised when myHy2Foam is initially run, the simmulation will then procede as normal and can be largely ignored without issue. It has been observed that a case using "too many" or "too little" processors , when running a case in parallel with `adjustableRunTime` enabled, will start with timesteps on the order of 1e-100s. This behaviour has not been observed in hy2Foam, and to mitigate it one can start with a fixed time step initially and change back later, or just simply change the number of processors.
