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

#include "FRADTempTimeDeriv.h"

registerMooseObject("FrostApp", FRADTempTimeDeriv);

InputParameters
FRADTempTimeDeriv::validParams()
{
  InputParameters params = ADTimeDerivative::validParams();
  
  params.set<bool>("use_displaced_mesh") = true;

  return params;
}

FRADTempTimeDeriv::FRADTempTimeDeriv(const InputParameters & parameters)
  : ADTimeDerivative(parameters),
    _rho_c_eff(getADMaterialProperty<Real>("rho_c_eff_sat"))
{
}

ADReal
FRADTempTimeDeriv::precomputeQpResidual()
{
  return _rho_c_eff[_qp] * ADTimeDerivative::precomputeQpResidual();
  
}
