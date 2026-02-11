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

#include "AuxKernel.h"

class FRWaterContentAux : public AuxKernel
{
public:
  static InputParameters validParams();

  FRWaterContentAux(const InputParameters & parameters);

protected:

  virtual Real computeValue() override;
  
  const ADMaterialProperty<Real> & _porosity;
  const ADMaterialProperty<Real> & _waterSaturation;
};