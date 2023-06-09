/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2016-2021 hyStrath
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of hyStrath, a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::nonEqMaxwellSlipUFvPatchVectorField

Description
    Maxwell slip boundary condition including thermal creep and surface
    curvature terms that can be optionally switched off.

SourceFiles
    fixedRhoFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef nonEqMaxwellSlipUFvPatchVectorField_H
#define nonEqMaxwellSlipUFvPatchVectorField_H

#include "mixedFixedValueSlipFvPatchFields.H"
#include "Switch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class maxwellSlipUFvPatch Declaration
\*---------------------------------------------------------------------------*/

class nonEqMaxwellSlipUFvPatchVectorField
:
    public mixedFixedValueSlipFvPatchVectorField
{
    // Private data

        //- Temperature field name, default = "Tt"
        word TName_;

        //- Density field name, default = "rho"
        word rhoName_;

        //- Compressibility field name, default = "thermo:psi"
        //word psiName_; // DELETION VINCENT 28/02/2016

        //- Dynamic viscosity field name, default = "mu"
        word muName_;

        //- tauMC field name, default = "tauMC"
        word tauMCName_;

        //- Mean free path field name, default = "mfp" // NEW VINCENT 28/02/2016
        word mfpName_;

        // Accommodation coefficient
        scalar accommodationCoeff_;

        // Wall velocity vector
        vector vectorUwall_; // NEW VINCENT 21/10/2016

        // Wall velocity
        vectorField Uwall_;

        // Include thermal creep term (default on)
        Switch thermalCreep_;

        // Include boundary curvature term (default on)
        Switch curvature_;


public:

    //- Runtime type information
    TypeName("nonEqMaxwellSlipU");


    // Constructors

        //- Construct from patch and internal field
        nonEqMaxwellSlipUFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        nonEqMaxwellSlipUFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  nonEqMaxwellSlipUFvPatchVectorField onto a new patch
        nonEqMaxwellSlipUFvPatchVectorField
        (
            const nonEqMaxwellSlipUFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new nonEqMaxwellSlipUFvPatchVectorField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        nonEqMaxwellSlipUFvPatchVectorField
        (
            const nonEqMaxwellSlipUFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new nonEqMaxwellSlipUFvPatchVectorField(*this, iF)
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
