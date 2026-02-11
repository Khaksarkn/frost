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

#include "FRMatGen.h"

registerMooseObject("FrostApp", FRMatGen);

InputParameters
FRMatGen::validParams()
{
  InputParameters params = ADMaterial::validParams();

  params.addCoupledVar("PoreWaterPressure","the water pressure in the pores");

  params.addCoupledVar("Temperature","temperature at each point");

  params.addParam<int>("SFCC_option","option for soil freezing characteristic curve (1: Van Genuchten, 2: Exponential)");
  params.addParam<Real>("alpha","alpha in Van Genuchtens equation (1/Pa), e.g. 0.1e-6");
  params.addParam<Real>("beta","beta in Van Genuchtens equation (unitless), e.g. 3.5");
  params.addParam<Real>("gamma","gamma in Van Genuchtens equation (unitless), e.g. 8.0");
  params.addParam<Real>("W_param", "Width parameter W in exponential SFCC (K), e.g. 0.5");

  params.addParam<Real>("Si_max","Maximum saturation for the ice phase (unitless), e.g. 0.9");

  params.addParam<Real>("rho_i","ice density (kg/m3), e.g. 910.0");
  params.addParam<Real>("rho_w","water density (kg/m3), e.g. 1000.0");
  params.addParam<Real>("rho_s","water density (kg/m3), e.g. 2650.0");
  
  params.addParam<Real>("c_i","ice specific heat capacity (J/kg/K), e.g. 2095.0");
  params.addParam<Real>("c_w","water specific heat capacity (J/kg/K), e.g. 4290.0");
  params.addParam<Real>("c_s","soil specific heat capacity (J/kg/K), e.g. 800.0");

  params.addRequiredParam<RealTensorValue>("lambda_form","form of the lambda vector variable e.g. '1 0 0 0 1 0 0 0 1' ");

  params.addParam<Real>("lambda_i","Thermal cond ice, e.g. 2.2. thermal cond of frozen soil if lambda_option = 2 or 3");
  params.addParam<Real>("lambda_w","Thermal cond water, e.g. 0.6. thermal cond of unfrozen soil if lambda_option = 2 or 3");
  params.addParam<Real>("lambda_s","Thermal cond soil, e.g. 1.5. Unused if lambda_option = 2 or 3");
  params.addParam<int>("lambda_option","option for lambda interpolation method (1: pow, 2:linear sat-based, 3: linear temp-based)");
  params.addParam<Real>("T_freeze","full freezing temperature (K) for lambda option 3, e.g. 269.15 (-4 C)");

  params.addParam<Real>("L_f","latent heat of fusion in freezing (J/kg), e.g. 334000.0");
  params.addParam<Real>("T_0","reference temperature (273.15 K) in atmospheric conditions (K)");

  params.addParam<Real>("m_hyd","model parameter for relative permeability in the MVG model, e.g. 0.87");
  params.addParam<int>("k_r_option","option for relative permeability model (opt 1: MVG unbounded, opt 2: MVG bounded, opt 3: constant k_r, opt 4: exp model)");
  params.addParam<Real>("k_r_min","minimum relative permeability for bounded models (opts 2 and 4), e.g. 1e-6, or constant value for opt 3");
  params.addParam<Real>("impedance_factor","impedance factor for exp model (opt 4), e.g. 50.0");

  params.addParam<Real>("porosity","constant porosity, e.g. 0.44");

  params.addParam<Real>("betaCompressibility","compressibility factor of the porous medium, e.g. 1e-8 1/Pa");

  params.addParam<Real>("gammaW","unit weight of water, i.e. 9810 N/m3");

  params.addRequiredParam<RealTensorValue>("permeabilitySat","permeability tensor in saturation class");

  params.addRequiredParam<RealVectorValue>("gravityVector","direction of gravity (positive)");

  params.addParam<int>("kici_option","1:full (interpolating), 2:simple(ice only)");
  
  params.addParam<int>("theta_option","1: Si/Sw, 2: omit (theta=0)");

  return params;
}

FRMatGen::FRMatGen(const InputParameters & parameters)
  : ADMaterial(parameters),
//-----
  _Sw(declareADProperty<Real>("SaturationW")),
  _Sice(declareADProperty<Real>("SaturationIce")),
  _Sp(declareADProperty<Real>("DSaturationWDp")),
  _ST(declareADProperty<Real>("DSaturationWDT")),

  _piQp(declareProperty<Real>("icePressure")),

  _k_r(declareADProperty<Real>("relativePermeability(k_r)")),

  _theta(declareADProperty<Real>("thetaSaturationRatio")),

  _nPorQp(declareADProperty<Real>("porosityQp")),

  _betaCompressQp(declareADProperty<Real>("beta(compressibility)")),

  _kici(declareADProperty<Real>("kici")),

  _advectionVelocity(declareADProperty<RealGradient>("advectionVelocity")),

  _darcyVelocity(declareADProperty<RealGradient>("darcyVelocity")),

  _mobilitySat(declareADProperty<RealTensorValue>("mobilitySat")),

  _rhoCEffSat(declareADProperty<Real>("rho_c_eff_sat")),

  _lambda(declareADProperty<RealTensorValue>("lambda_sat")),

//-----
  _pwQp(coupledValue("PoreWaterPressure")),
  _TQp(coupledValue("Temperature")),

  _pwQp_grad(coupledGradient("PoreWaterPressure")),
//-----

  _kPermeabilitySat(getParam<RealTensorValue>("permeabilitySat")),

  _g_vec(getParam<RealVectorValue>("gravityVector")),

  _SFCC_option(getParam<int>("SFCC_option")),
  _alpha(getParam<Real>("alpha")),
  _beta(getParam<Real>("beta")),
  _gamma(getParam<Real>("gamma")),
  _W_param(getParam<Real>("W_param")),

  _Si_max(getParam<Real>("Si_max")),

  _rho_i(getParam<Real>("rho_i")),
  _rho_w(getParam<Real>("rho_w")),
  _rho_s(getParam<Real>("rho_s")),

  _c_i(getParam<Real>("c_i")),
  _c_w(getParam<Real>("c_w")),
  _c_s(getParam<Real>("c_s")),

  _lambda_form(getParam<RealTensorValue>("lambda_form")),

  _lambda_i(getParam<Real>("lambda_i")),
  _lambda_w(getParam<Real>("lambda_w")),
  _lambda_s(getParam<Real>("lambda_s")),

  _lambda_option(getParam<int>("lambda_option")),
  _T_freeze(getParam<Real>("T_freeze")),

  _L_f(getParam<Real>("L_f")),
  _T_0(getParam<Real>("T_0")),

  _m_hyd(getParam<Real>("m_hyd")),
  _k_r_option(getParam<int>("k_r_option")),
  _k_r_min(getParam<Real>("k_r_min")),
  _impedance_factor(getParam<Real>("impedance_factor")),

  _nPor(getParam<Real>("porosity")),

  _betaCompress(getParam<Real>("betaCompressibility")),

  _gammaW(getParam<Real>("gammaW")),

  _kici_option(getParam<int>("kici_option")),

  _theta_option(getParam<int>("theta_option"))
{
}

void
FRMatGen::computeQpProperties()
{

  // -----------------------------------------------------
  // 1) Compute ice pressure ALWAYS
  // -----------------------------------------------------
  if (_TQp[_qp] < _T_0)
  {
      _piQp[_qp] = (_rho_i/_rho_w)*_pwQp[_qp]
                  - _rho_i * _L_f * std::log(_TQp[_qp] / _T_0);
  }
  else
  {
      _piQp[_qp] = 0.0;
  }
  // -----------------------------------------------------

  if (_SFCC_option == 1)
  {
    if (_TQp[_qp] < _T_0 && _piQp[_qp] > _pwQp[_qp])
    {
        // --- Sw ---
        _Sw[_qp] = 1.0 - _Si_max * ( 1.0 -
            std::pow(1.0 + std::pow(_alpha * (_piQp[_qp] - _pwQp[_qp]), _beta), -_gamma));

        // --- dSw/dp ---
        _Sp[_qp] = (-_alpha * _beta * _gamma * _Si_max)
            * std::pow(1.0 + std::pow(_alpha * (_piQp[_qp] - _pwQp[_qp]), _beta), -_gamma - 1.0)
            * std::pow(_alpha * (_piQp[_qp] - _pwQp[_qp]), _beta - 1.0)
            * (_rho_i/_rho_w - 1.0);

        // --- dSw/dT ---
        _ST[_qp] = (-_alpha * _beta * _gamma * _Si_max)
            * std::pow(1.0 + std::pow(_alpha * (_piQp[_qp] - _pwQp[_qp]), _beta), -_gamma - 1.0)
            * std::pow(_alpha * (_piQp[_qp] - _pwQp[_qp]), _beta - 1.0)
            * (- _rho_i * _L_f / _TQp[_qp]);
    }
    else {
        _Sw[_qp] = 1.0;
        _Sp[_qp] = 0.0;
        _ST[_qp] = 0.0;
    }
  }
  else if (_SFCC_option == 2)
  {
    if (_TQp[_qp] >= _T_0)
    {
      _Sw[_qp] = 1.0;
      _Sp[_qp] = 0.0;
      _ST[_qp] = 0.0;
    }
    else
    {
      Real dT = _TQp[_qp] - _T_0;
      Real expTerm = std::exp( - (dT*dT) / (_W_param*_W_param) );
      Real _Sw_res = 1.0 - _Si_max;

      // ---- Sw ----
      _Sw[_qp] = (1.0 - _Sw_res) * expTerm + _Sw_res;

      // ---- dSw/dp = 0 ----
      _Sp[_qp] = 0.0;

      // ---- dSw/dT ----
      Real dExpdT = expTerm * (-2.0 * dT / (_W_param*_W_param));
      _ST[_qp] = (1.0 - _Sw_res) * dExpdT;
    }
  }





  _Sice[_qp] = 1.0 - _Sw[_qp];

  _nPorQp[_qp] = _nPor;

  _betaCompressQp[_qp] = _betaCompress;
  

  if (_k_r_option == 1) {
    // MVG unbounded
    _k_r[_qp] = std::sqrt(_Sw[_qp]) * std::pow( ( 1.0 - std::pow( (1.0 - std::pow(_Sw[_qp], 1.0/_m_hyd) ) ,_m_hyd) ) , 2.0);
  }
  else if (_k_r_option == 2) {
    // MVG bounded
    _k_r[_qp] = std::max( std::sqrt(_Sw[_qp]) * std::pow( ( 1.0 - std::pow( (1.0 - std::pow(_Sw[_qp], 1.0/_m_hyd) ) ,_m_hyd) ) , 2.0) , _k_r_min);
  }
  else if (_k_r_option == 3) {
    // constant k_r
    _k_r[_qp] = _k_r_min;
  }
  else if (_k_r_option == 4) {
    // exponential model
    _k_r[_qp] = std::max( std::pow(10.0, -_impedance_factor * _nPorQp[_qp] * (1.0 - _Sw[_qp]) ) , _k_r_min);
  }
  else {
    mooseError("k_r_option parameter in FRMatGen material must be 1, 2, 3 or 4.");
  }

  
  //-----
  if(_theta_option == 1)
    _theta[_qp] = (1.0 - _Sw[_qp]) / (_Sw[_qp]);
  else if(_theta_option == 2)
    _theta[_qp] = 0.0;
  else
    mooseError("theta_option parameter in FRMatGen material must be 1 or 2.");
  //-----
  
  if(_kici_option == 1)
    _kici[_qp] = (_nPorQp[_qp] * _rho_i) / (_Sw[_qp] + (_rho_i/_rho_w) * (1.0-_Sw[_qp]));
  else if(_kici_option == 2)
    _kici[_qp] = (_nPorQp[_qp] * _rho_i);
  else
    mooseError("kici_option parameter in FRMatGen material must be 1 or 2.");

  

//-----

  _mobilitySat[_qp] = (_k_r[_qp] / _gammaW) * _kPermeabilitySat;

  _darcyVelocity[_qp] = -_mobilitySat[_qp] * (_pwQp_grad[_qp] - _gammaW * (-_g_vec));
  
  _advectionVelocity[_qp] = (_rho_w * _c_w + _theta[_qp] * _rho_i * _c_i) * _darcyVelocity[_qp];

  _rhoCEffSat[_qp] =  (1.0-_nPorQp[_qp]) * _rho_s * _c_s + 
                      _nPorQp[_qp] * _Sw[_qp] * _rho_w * _c_w + 
                      _nPorQp[_qp] * (1.0-_Sw[_qp]) * _rho_i * _c_i;



  if(_lambda_option == 1){
    _lambda[_qp] =  (std::pow(_lambda_s , (1.0 - _nPorQp[_qp]))) *
                  (std::pow(_lambda_w , _nPorQp[_qp] * _Sw[_qp])) *
                  (std::pow(_lambda_i , _nPorQp[_qp] * (1.0 - _Sw[_qp]))) *
                  _lambda_form;
  }
  else if(_lambda_option == 2){
    _lambda[_qp] = (  (_Sw[_qp])        * _lambda_w + 
                      (1.0 - _Sw[_qp])  * _lambda_i ) *
                      _lambda_form;
  }
  else if(_lambda_option == 3){
    
    if(_TQp[_qp] >= (_T_0)){
      _lambda[_qp] = _lambda_w * _lambda_form;
    }
    else if(_TQp[_qp] <= _T_freeze){
      _lambda[_qp] = _lambda_i * _lambda_form;
    }
    else{
      Real frac = (_TQp[_qp] - _T_freeze) / (_T_0 - _T_freeze);
      _lambda[_qp] = ( frac * _lambda_w + (1.0 - frac) * _lambda_i ) * _lambda_form;
    }
    
  }
  else if(_lambda_option == 4){
    _lambda[_qp] = (  (1.0 - _nPorQp[_qp])              * _lambda_s + 
                      (_nPorQp[_qp] * _Sw[_qp])         * _lambda_w + 
                      (_nPorQp[_qp] * (1.0 - _Sw[_qp])) * _lambda_i) *
                      _lambda_form;
  }
  else{
    mooseError("lambda_option parameter in FRMatGen material must be 1, 2, 3 or 4.");
  }


  


}

