/******************************************************************************/
/*                           This file is part of                             */
/*                      FROST, a MOOSE-based application                      */
/*            thermal-hydraulic modeling of soils during freeze-thaw          */
/*                                                                            */
/*               Copyright (C) 2026 by Kian Khaksar and Biao Li               */
/*                 Concordia University, Montreal, QC, Canada                 */
/*                                                                            */
/*            Licensed under GNU GENERAL PUBLIC LICENSE Version 3,            */
/*                       please see LICENSE for details                       */
/*                  or http://www.gnu.org/licenses/gpl.html                   */
/******************************************************************************/


#pragma once

#include "ADDiffusion.h"

class FRADTempConduction : public ADDiffusion
{
    public:

        FRADTempConduction(const InputParameters & parameters);

        static InputParameters validParams();

    protected:

        virtual ADRealVectorValue precomputeQpResidual() override;

        const ADMaterialProperty<RealTensorValue> & _lambda;

};
