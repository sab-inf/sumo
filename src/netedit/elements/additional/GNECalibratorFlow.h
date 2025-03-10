/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2022 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    GNECalibratorFlow.h
/// @author  Pablo Alvarez Lopez
/// @date    Nov 2015
///
// Flow used by GNECalibrators
/****************************************************************************/
#pragma once
#include <config.h>
#include <utils/vehicle/SUMOVehicleParameter.h>

#include "GNEAdditional.h"

// ===========================================================================
// class declaration
// ===========================================================================

class GNECalibrator;
class GNECalibratorDialog;

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class GNECalibratorFlow
 * flow flow used by GNECalibrators
 */
class GNECalibratorFlow : public GNEAdditional, public SUMOVehicleParameter {

public:
    /// @brief default constructor
    GNECalibratorFlow(GNENet* net);

    /// @brief default constructor (used only in GNECalibratorDialog)
    GNECalibratorFlow(GNEAdditional* calibratorParent, GNEDemandElement* vehicleType, GNEDemandElement* route);

    /// @brief parameter constructor
    GNECalibratorFlow(GNEAdditional* calibratorParent, GNEDemandElement* vehicleType, GNEDemandElement* route, const SUMOVehicleParameter& vehicleParameters);

    /// @brief destructor
    ~GNECalibratorFlow();

    /**@brief writte additional element into a xml file
     * @param[in] device device in which write parameters of additional element
     */
    void writeAdditional(OutputDevice& device) const;

    /**@brief get move operation
    * @note returned GNEMoveOperation can be nullptr
    */
    GNEMoveOperation* getMoveOperation();

    /// @name Functions related with geometry of element
    /// @{
    /// @brief update pre-computed geometry information
    void updateGeometry();

    /// @brief Returns position of additional in view
    Position getPositionInView() const;

    /// @brief update centering boundary (implies change in RTREE)
    void updateCenteringBoundary(const bool updateGrid);

    /// @brief split geometry
    void splitEdgeGeometry(const double splitPosition, const GNENetworkElement* originalElement, const GNENetworkElement* newElement, GNEUndoList* undoList);
    /// @}

    /// @name inherited from GUIGlObject
    /// @{
    /**@brief Returns the name of the parent object
     * @return This object's parent id
     */
    std::string getParentName() const;

    /**@brief Draws the object
     * @param[in] s The settings for the current view (may influence drawing)
     * @see GUIGlObject::drawGL
     */
    void drawGL(const GUIVisualizationSettings& s) const;
    /// @}

    /// @brief inherited from GNEAttributeCarrier
    /// @{
    /* @brief method for getting the Attribute of an XML key
     * @param[in] key The attribute key
     * @return string with the value associated to key
     */
    std::string getAttribute(SumoXMLAttr key) const;

    /* @brief method for getting the Attribute of an XML key in double format (to avoid unnecessary parse<double>(...) for certain attributes)
     * @param[in] key The attribute key
     * @return double with the value associated to key
     */
    double getAttributeDouble(SumoXMLAttr key) const;

    /* @brief method for setting the attribute and letting the object perform additional changes
     * @param[in] key The attribute key
     * @param[in] value The new value
     * @param[in] undoList The undoList on which to register changes
     * @param[in] net optionally the GNENet to inform about gui updates
     */
    void setAttribute(SumoXMLAttr key, const std::string& value, GNEUndoList* undoList);

    /* @brief method for setting the attribute and letting the object perform additional changes
     * @param[in] key The attribute key
     * @param[in] value The new value
     * @param[in] undoList The undoList on which to register changes
     */
    bool isValid(SumoXMLAttr key, const std::string& value);

    /* @brief method for check if the value for certain attribute is set
     * @param[in] key The attribute key
     */
    bool isAttributeEnabled(SumoXMLAttr key) const;

    /// @brief get PopPup ID (Used in AC Hierarchy)
    std::string getPopUpID() const;

    /// @brief get Hierarchy Name (Used in AC Hierarchy)
    std::string getHierarchyName() const;
    /// @}

private:
    /// @brief method for setting the attribute and nothing else
    void setAttribute(SumoXMLAttr key, const std::string& value);

    /// @brief set move shape
    void setMoveShape(const GNEMoveResult& moveResult);

    /// @brief commit move shape
    void commitMoveShape(const GNEMoveResult& moveResult, GNEUndoList* undoList);

    /// @brief toogle attribute
    void toogleAttribute(SumoXMLAttr key, const bool value, const int previousParameters);

    /// @brief Invalidated copy constructor.
    GNECalibratorFlow(const GNECalibratorFlow&) = delete;

    /// @brief Invalidated assignment operator
    GNECalibratorFlow& operator=(const GNECalibratorFlow&) = delete;
};
