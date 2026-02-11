#include "FrostApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
FrostApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

FrostApp::FrostApp(const InputParameters & parameters) : MooseApp(parameters)
{
  FrostApp::registerAll(_factory, _action_factory, _syntax);
}

FrostApp::~FrostApp() {}

void
FrostApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAllObjects<FrostApp>(f, af, syntax);
  Registry::registerObjectsTo(f, {"FrostApp"});
  Registry::registerActionsTo(af, {"FrostApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
FrostApp::registerApps()
{
  registerApp(FrostApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
FrostApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  FrostApp::registerAll(f, af, s);
}
extern "C" void
FrostApp__registerApps()
{
  FrostApp::registerApps();
}
