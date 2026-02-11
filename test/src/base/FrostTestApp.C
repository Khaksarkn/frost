//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "FrostTestApp.h"
#include "FrostApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
FrostTestApp::validParams()
{
  InputParameters params = FrostApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

FrostTestApp::FrostTestApp(const InputParameters & parameters) : MooseApp(parameters)
{
  FrostTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

FrostTestApp::~FrostTestApp() {}

void
FrostTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  FrostApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"FrostTestApp"});
    Registry::registerActionsTo(af, {"FrostTestApp"});
  }
}

void
FrostTestApp::registerApps()
{
  registerApp(FrostApp);
  registerApp(FrostTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
FrostTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  FrostTestApp::registerAll(f, af, s);
}
extern "C" void
FrostTestApp__registerApps()
{
  FrostTestApp::registerApps();
}
