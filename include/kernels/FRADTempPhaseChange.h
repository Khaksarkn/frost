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

#include "ADKernelValue.h"

class FRADTempPhaseChange : public ADKernelValue
{
public:
  static InputParameters validParams();

  FRADTempPhaseChange(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADMaterialProperty<Real> & _Sw;
  const ADMaterialProperty<Real> & _Sp;
  const ADMaterialProperty<Real> & _ST;

  const ADMaterialProperty<Real> & _kici;

  const ADVariableValue & _p_dot;
  const ADVariableValue & _T_dot;

  const Real _L_f;
};
