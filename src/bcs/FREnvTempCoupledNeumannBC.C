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

#include "FREnvTempCoupledNeumannBC.h"

registerMooseObject("FrostApp", FREnvTempCoupledNeumannBC);

InputParameters
FREnvTempCoupledNeumannBC::validParams()
{
    InputParameters params = IntegratedBC::validParams();

    params.addParam<Real>("lambdaE", 1.0, "value multiplied");
    params.addParam<Real>("temperatureE", 273.0, "environment temperature");

    params.addRequiredCoupledVar("temperature", "value on the BC");
    return params;
}

FREnvTempCoupledNeumannBC::FREnvTempCoupledNeumannBC(const InputParameters & parameters)
    : IntegratedBC(parameters),
    // getting the required parameters from the input file
      _lambdaE(getParam<Real>("lambdaE")),        //The multiplier
      _temperatureE(getParam<Real>("temperatureE")),//The environment temperature
      _coupledTemperature_val(coupledValue("temperature")) //Coupled variable
{
}

Real
FREnvTempCoupledNeumannBC::computeQpResidual()
{
    return _test[_i][_qp] * _lambdaE * (_coupledTemperature_val[_qp] - _temperatureE);
}

