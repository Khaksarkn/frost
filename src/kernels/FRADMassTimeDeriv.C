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

#include "FRADMassTimeDeriv.h"

registerMooseObject("FrostApp", FRADMassTimeDeriv);

InputParameters
FRADMassTimeDeriv::validParams()
{
  InputParameters params = ADTimeDerivative::validParams();
  
  params.set<bool>("use_displaced_mesh") = true;

  return params;
}

FRADMassTimeDeriv::FRADMassTimeDeriv(const InputParameters & parameters)
  : ADTimeDerivative(parameters),
    _nPorQp(getADMaterialProperty<Real>("porosityQp")),
    _Sw(getADMaterialProperty<Real>("SaturationW")),
    _betaCompressibility(getADMaterialProperty<Real>("beta(compressibility)"))
{
}

ADReal
FRADMassTimeDeriv::precomputeQpResidual()
{
  return (_nPorQp[_qp] * _Sw[_qp] * _betaCompressibility[_qp]) * ADTimeDerivative::precomputeQpResidual();
}
