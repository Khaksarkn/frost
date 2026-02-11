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

#include "FRADTempPhaseChange.h"

registerMooseObject("FrostApp", FRADTempPhaseChange);

InputParameters
FRADTempPhaseChange::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  
  

  params.addRequiredCoupledVar("pressure", "Pressure Coupled variable");
  params.addRequiredCoupledVar("temperature", "temperature Coupled variable");

  params.addParam<Real>("L_f",334000.0,"latent heat of fusion in freezing (J/kg)");

  return params;
}

FRADTempPhaseChange::FRADTempPhaseChange(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _Sw(getADMaterialProperty<Real>("SaturationW")),
    _Sp(getADMaterialProperty<Real>("DSaturationWDp")),
    _ST(getADMaterialProperty<Real>("DSaturationWDT")),
    _kici(getADMaterialProperty<Real>("kici")),

    _p_dot(adCoupledDot("pressure")),
    _T_dot(adCoupledDot("temperature")),

    _L_f(getParam<Real>("L_f"))

{
}

ADReal
FRADTempPhaseChange::precomputeQpResidual()
{
  
  return _L_f * _kici[_qp] * _Sp[_qp]*_p_dot[_qp] + _L_f * _kici[_qp] * _ST[_qp]*_T_dot[_qp];
  
}
