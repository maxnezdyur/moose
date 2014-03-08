<<<<<<< HEAD
//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ContactTestApp.h"
=======
#include "ContactApp.h"
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
#include "MooseInit.h"
#include "Moose.h"
#include "MooseApp.h"
#include "AppFactory.h"

// Create a performance log
PerfLog Moose::perf_log("Contact");

// Begin the main program.
<<<<<<< HEAD
int
main(int argc, char * argv[])
=======
int main(int argc, char *argv[])
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
{
  // Initialize MPI, solvers and MOOSE
  MooseInit init(argc, argv);

  // Register this application's MooseApp and any it depends on
<<<<<<< HEAD
  ContactTestApp::registerApps();

  // Create an instance of the application and store it in a smart pointer for easy cleanup
  std::shared_ptr<MooseApp> app = AppFactory::createAppShared("ContactTestApp", argc, argv);

  app->setCheckUnusedFlag(true);
  app->setErrorOverridden();
=======
  ContactApp::registerApps();

  // This creates dynamic memory that we're responsible for deleting
  MooseApp * app = AppFactory::createApp("ContactApp", argc, argv);
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

  // Execute the application
  app->run();

<<<<<<< HEAD
=======
  // Free up the memory we created earlier
  delete app;

>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
  return 0;
}
