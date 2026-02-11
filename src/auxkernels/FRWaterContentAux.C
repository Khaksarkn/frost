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

#include "FRWaterContentAux.h"

#include "metaphysicl/raw_type.h"

registerMooseObject("FrostApp", FRWaterContentAux);

InputParameters
FRWaterContentAux::validParams()
{
  InputParameters params = AuxKernel::validParams();

  return params;
}

FRWaterContentAux::FRWaterContentAux(const InputParameters & parameters)
  : AuxKernel(parameters),

    _porosity(getADMaterialProperty<Real>("porosityQp")),
    _waterSaturation(getADMaterialProperty<Real>("SaturationW"))
{
}

Real
FRWaterContentAux::computeValue()
{
  return MetaPhysicL::raw_value(_porosity[_qp] * _waterSaturation[_qp]);
}