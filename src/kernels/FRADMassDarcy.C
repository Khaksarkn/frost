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

#include "FRADMassDarcy.h"


registerMooseObject("FrostApp", FRADMassDarcy);

InputParameters
FRADMassDarcy::validParams()
{
  InputParameters params = ADKernelGrad::validParams();
  params.set<bool>("use_displaced_mesh") = true;

  params.addRequiredParam<RealVectorValue>("gravityVector","direction of gravity (positive)");
  params.addParam<Real>("g_gravity",9.81,"g(m/s^2)");
  params.addParam<Real>("rho_w",1000.0,"water density (kg/m3)");

  return params;
}

FRADMassDarcy::FRADMassDarcy(const InputParameters & parameters)
  : ADKernelGrad(parameters), 
  _darcyVelocity(getADMaterialProperty<RealGradient>("darcyVelocity")),
  _mobilitySat(getADMaterialProperty<RealTensorValue>("mobilitySat")),

  _g_vec(getParam<RealVectorValue>("gravityVector")),
  _g_gravity(getParam<Real>("g_gravity")),
  _rho_w(getParam<Real>("rho_w"))
{
}

ADRealVectorValue
FRADMassDarcy::precomputeQpResidual()
{
  return _mobilitySat[_qp] * (_grad_u[_qp]) - _mobilitySat[_qp] * _rho_w * _g_gravity * (-_g_vec);
}

