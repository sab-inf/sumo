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
# @date    2016-11-25

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

# go to shape mode
netedit.shapeMode()

# go to shape mode
netedit.changeElement("poi")

# create poi
netedit.leftClick(referencePosition, 100, 50)

# enable ID
netedit.changeDefaultBoolValue(netedit.attrs.POI.create.idEnable)

# create poi
netedit.leftClick(referencePosition, 120, 50)

# set invalid ID
netedit.changeDefaultValue(netedit.attrs.POI.create.id, ";;;;;;")

# try to create poi
netedit.leftClick(referencePosition, 140, 50)

# set invalid ID
netedit.changeDefaultValue(netedit.attrs.POI.create.id, "poi_0")

# try to create poi
netedit.leftClick(referencePosition, 160, 50)

# set invalid ID
netedit.changeDefaultValue(netedit.attrs.POI.create.id, "customID")

# create POI
netedit.leftClick(referencePosition, 180, 50)

# Check undo redo
netedit.undo(referencePosition, 4)
netedit.redo(referencePosition, 4)

# save shapes
netedit.saveAdditionals(referencePosition)

# save network
netedit.saveNetwork(referencePosition)

# quit netedit
netedit.quit(neteditProcess)
