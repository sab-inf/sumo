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
neteditProcess, referencePosition = netedit.setupAndStart(neteditTestRoot)

# Go to data supermode
netedit.supermodeData()

# change to edgeData
netedit.edgeData()

# create dataSet
netedit.createDataSet()

# create data interval
netedit.createDataInterval()

# create edgeData
netedit.leftClick(referencePosition, 366, 120)

# create second edgeData
netedit.leftClick(referencePosition, 450, 220)

# go to select mode
netedit.selectMode()

# select all using invert
netedit.selectionInvert()

# go to inspect mode
netedit.inspectMode()

# inspect edgeData
netedit.leftClick(referencePosition, 366, 120)

# check double parameters
netedit.checkDoubleParameters(referencePosition, 1, False, 0, 30)

# save data elements
netedit.saveDatas(referencePosition, True, 0, 30)

# save network
netedit.saveNetwork(referencePosition, True, 0, 30)

# quit netedit
netedit.quit(neteditProcess)
