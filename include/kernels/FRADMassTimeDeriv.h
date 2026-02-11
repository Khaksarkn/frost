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

#include "ADTimeDerivative.h"

class FRADMassTimeDeriv : public ADTimeDerivative
{
public:
  static InputParameters validParams();

  FRADMassTimeDeriv(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADMaterialProperty<Real> & _nPorQp;
  const ADMaterialProperty<Real> & _Sw;
  const ADMaterialProperty<Real> & _betaCompressibility;
};
