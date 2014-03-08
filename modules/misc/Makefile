###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Optional Environment variables
<<<<<<< HEAD
# MOOSE_DIR        - Root directory of the MOOSE project
# MODULE_DIR       - Location of the MOOSE modules directory
=======
# MOOSE_DIR        - Root directory of the MOOSE project 
# HERD_TRUNK_DIR   - Location of the HERD repository
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
# FRAMEWORK_DIR    - Location of the MOOSE framework
#
###############################################################################
MODULE_DIR         ?= $(shell dirname `pwd`)
MOOSE_DIR          ?= $(shell dirname $(MODULE_DIR))
FRAMEWORK_DIR      ?= $(MOOSE_DIR)/framework
<<<<<<< HEAD
ADDITIONAL_CPPFLAGS += -Wall -Wextra
=======
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)
###############################################################################

# framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

<<<<<<< HEAD
# Module
MODULE_NAME := misc
include $(MODULE_DIR)/modules.mk
=======
# dep apps
APPLICATION_DIR    := $(MODULE_DIR)/misc
APPLICATION_NAME   := misc
BUILD_EXEC         := yes
DEP_APPS           := $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))
include            $(FRAMEWORK_DIR)/app.mk
>>>>>>> d297f50cb1 (Merging Modules into MOOSE #2460)

###############################################################################
# Additional special case targets should be added here
