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

#include "FRADMassPhaseChange.h"

registerMooseObject("FrostApp", FRADMassPhaseChange);

InputParameters
FRADMassPhaseChange::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  
  params.set<bool>("use_displaced_mesh") = true;

  params.addRequiredCoupledVar("pressure", "Pressure Coupled variable");
  params.addRequiredCoupledVar("temperature", "temperature Coupled variable");

  params.addParam<Real>("rho_i",910.0,"ice density (kg/m3)");
  params.addParam<Real>("rho_w",1000.0,"water density (kg/m3)");

  return params;
}

FRADMassPhaseChange::FRADMassPhaseChange(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _Sw(getADMaterialProperty<Real>("SaturationW")),
    _Sp(getADMaterialProperty<Real>("DSaturationWDp")),
    _ST(getADMaterialProperty<Real>("DSaturationWDT")),

    _nPorQp(getADMaterialProperty<Real>("porosityQp")),
    _theta(getADMaterialProperty<Real>("thetaSaturationRatio")),

    _rho_i(getParam<Real>("rho_i")),
    _rho_w(getParam<Real>("rho_w")),

    _p_dot(adCoupledDot("pressure")),
    _T_dot(adCoupledDot("temperature"))

    

{
}

ADReal
FRADMassPhaseChange::precomputeQpResidual()
{
  
  return ((_nPorQp[_qp] * (_rho_w - _rho_i)) / (_rho_w) ) * (_Sp[_qp]*_p_dot[_qp] + _ST[_qp]*_T_dot[_qp]);
  
}
