#!/usr/bin/env python
# Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
# Copyright (C) 2009-2022 German Aerospace Center (DLR) and others.
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# https://www.eclipse.org/legal/epl-2.0/
# This Source Code may also be made available under the following Secondary
# Licenses when the conditions for such availability set forth in the Eclipse
# Public License 2.0 are satisfied: GNU General Public License, version 2
# or later which is available at
# https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later

# @file    test.py
# @author  Pablo Alvarez Lopez
# @date    2019-07-16

# import common functions for netedit tests
import os
import sys

testRoot = os.path.join(os.environ.get('SUMO_HOME', '.'), 'tests')
neteditTestRoot = os.path.join(
    os.environ.get('TEXTTEST_HOME', testRoot), 'netedit')
sys.path.append(neteditTestRoot)
import neteditTestFunctions as netedit  # noqa

# Open netedit
neteditProcess, referencePosition = netedit.setupAndStart(neteditTestRoot, ['--gui-testing-debug-gl'])

# go to demand mode
netedit.supermodeDemand()

# force save additionals
netedit.forceSaveAdditionals()

# go to stop mode
netedit.stopMode()

# change stop type with a valid value
netedit.changeStopType("stopContainerStop")

# create stop
netedit.leftClick(referencePosition, 290, 150)

# go to inspect mode
netedit.inspectMode()

# inspect stop
netedit.leftClick(referencePosition, 290, 150)

# change value
netedit.modifyBoolAttribute(netedit.attrs.stop.inspect.durationEnable, False)

# change value
netedit.modifyBoolAttribute(netedit.attrs.stop.inspect.durationEnable, False)

# change value
netedit.modifyAttribute(netedit.attrs.stop.inspect.duration, "dummyValue", False)

# change value
netedit.modifyAttribute(netedit.attrs.stop.inspect.duration, "", False)

# change value
netedit.modifyAttribute(netedit.attrs.stop.inspect.duration, "-5", False)

# change value
netedit.modifyAttribute(netedit.attrs.stop.inspect.duration, "0", False)

# change value
netedit.modifyAttribute(netedit.attrs.stop.inspect.duration, "3.5", False)

# Check undo redo
netedit.undo(referencePosition, 2)
netedit.redo(referencePosition, 2)

# save additionals
netedit.saveAdditionals(referencePosition)

# save routes
netedit.saveRoutes(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
