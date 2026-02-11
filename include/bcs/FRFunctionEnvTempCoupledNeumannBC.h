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

#include "IntegratedBC.h"

class Function;

class FRFunctionEnvTempCoupledNeumannBC : public IntegratedBC
{
    public:
        FRFunctionEnvTempCoupledNeumannBC(const InputParameters & parameters);

        static InputParameters validParams();

    protected:
        virtual Real computeQpResidual() override;

    private:
        Real _lambdaE;

        const Function & _temperatureE;

        const VariableValue & _coupledTemperature_val;
};