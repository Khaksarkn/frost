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

#include "FRADTempConduction.h"


registerMooseObject("FrostApp", FRADTempConduction);

InputParameters
FRADTempConduction::validParams()
{
  InputParameters params = ADDiffusion::validParams();
  params.set<bool>("use_displaced_mesh") = true;
  return params;
}

FRADTempConduction::FRADTempConduction(const InputParameters & parameters)
  : ADDiffusion(parameters), 
  _lambda(getADMaterialProperty<RealTensorValue>("lambda_sat"))
{
}

ADRealVectorValue
FRADTempConduction::precomputeQpResidual()
{
  return _lambda[_qp] * ADDiffusion::precomputeQpResidual();
}

