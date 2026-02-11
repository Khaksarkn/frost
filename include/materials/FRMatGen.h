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

#include "ADMaterial.h"

#include <complex>

class FRMatGen : public ADMaterial
{
public:
  FRMatGen(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties() override;

private:
//------
  ADMaterialProperty<Real> & _Sw;
  ADMaterialProperty<Real> & _Sice;
  ADMaterialProperty<Real> & _Sp;
  ADMaterialProperty<Real> & _ST;

  MaterialProperty<Real> & _piQp;

  ADMaterialProperty<Real> & _k_r;

  ADMaterialProperty<Real> & _theta;

  ADMaterialProperty<Real> & _nPorQp;

  ADMaterialProperty<Real> & _betaCompressQp;

  ADMaterialProperty<Real> & _kici;

  ADMaterialProperty<RealGradient> & _advectionVelocity;

  ADMaterialProperty<RealGradient> & _darcyVelocity;

  ADMaterialProperty<RealTensorValue> & _mobilitySat;

  ADMaterialProperty<Real> & _rhoCEffSat;

  ADMaterialProperty<RealTensorValue> & _lambda;

//------
  const VariableValue & _pwQp;
  const VariableValue & _TQp;

  const VariableGradient & _pwQp_grad;
//------

  const RealTensorValue _kPermeabilitySat;

  const RealVectorValue _g_vec;

  const int _SFCC_option;
  const Real _alpha;
  const Real _beta;
  const Real _gamma;
  const Real _W_param;

  const Real _Si_max;

  const Real _rho_i;
  const Real _rho_w;
  const Real _rho_s;

  const Real _c_i;
  const Real _c_w;
  const Real _c_s;

  const RealTensorValue _lambda_form;

  const Real _lambda_i;
  const Real _lambda_w;
  const Real _lambda_s;
  
  const int _lambda_option;
  const Real _T_freeze;

  const Real _L_f;
  const Real _T_0;

  const Real _m_hyd;
  const int _k_r_option;
  const Real _k_r_min;
  const Real _impedance_factor;

  const Real _nPor;

  const Real _betaCompress;

  const Real _gammaW;

  const int _kici_option;

  const int _theta_option;

  
};