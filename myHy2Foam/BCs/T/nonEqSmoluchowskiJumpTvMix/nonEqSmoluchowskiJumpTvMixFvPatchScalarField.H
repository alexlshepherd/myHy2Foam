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
    Foam::nonEqSmoluchowskiJumpTvMixFvPatchScalarField

Description
    Smoluchowski temperature jump boundary condition, vibro-electronic energy
    equation (single vibrational energy pool)

SourceFiles
    nonEqSmoluchowskiJumpTvMixFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef nonEqSmoluchowskiJumpTvMixFvPatchScalarFields_H
#define nonEqSmoluchowskiJumpTvMixFvPatchScalarFields_H

#include "mixedFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                 Class smoluchowskiJumpTvMixFvPatch Declaration
\*---------------------------------------------------------------------------*/

class nonEqSmoluchowskiJumpTvMixFvPatchScalarField
:
    public mixedFvPatchScalarField
{

    // Private data

        //- Name of the species
        word specieName_;

        //- Velocity field name, default = "U"
        word UName_;

        //- Density field name, default = "rho"
        word rhoName_;

        //- Dynamic viscosity field name, default = "mu"
        word muName_;

        //- Thermal diffusivity field name, default = "alphave"
        word alphaName_;

        //- Ratio of specific heat capacities field name, default = "gamma"
        word gammaName_;

        //- Mean free path field name, default = "mfp"
        word mfpName_;

        //- Accommodation coefficient
        scalar accommodationCoeff_;

        //- Wall surface temperature
        scalarField Twall_;

public:

    //- Runtime type information
    TypeName("nonEqSmoluchowskiJumpTvMix");


    // Constructors

        //- Construct from patch and internal field
        nonEqSmoluchowskiJumpTvMixFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        nonEqSmoluchowskiJumpTvMixFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  nonEqSmoluchowskiJumpTvMixFvPatchScalarField onto a new patch
        nonEqSmoluchowskiJumpTvMixFvPatchScalarField
        (
            const nonEqSmoluchowskiJumpTvMixFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new nonEqSmoluchowskiJumpTvMixFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        nonEqSmoluchowskiJumpTvMixFvPatchScalarField
        (
            const nonEqSmoluchowskiJumpTvMixFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new nonEqSmoluchowskiJumpTvMixFvPatchScalarField(*this, iF)
            );
        }

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchField<scalar>&,
                const labelList&
            );


        // Evaluation functions

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
