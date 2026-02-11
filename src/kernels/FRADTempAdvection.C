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

#include "FRADTempAdvection.h"

registerMooseObject("FrostApp", FRADTempAdvection);

InputParameters
FRADTempAdvection::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  
  return params;
}

FRADTempAdvection::FRADTempAdvection(const InputParameters & parameters)
  : ADKernelValue(parameters),
    _advectionVelocity(getADMaterialProperty<RealGradient>("advectionVelocity"))

{
}

ADReal
FRADTempAdvection::precomputeQpResidual()
{
  
  return _advectionVelocity[_qp] * _grad_u[_qp];
  
}
