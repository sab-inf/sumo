/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2002-2022 German Aerospace Center (DLR) and others.
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
/// @file    MSActuatedTrafficLightLogic.h
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    Sept 2002
///
// An actuated (adaptive) traffic light logic
/****************************************************************************/
#pragma once
#include <config.h>

#include <utility>
#include <vector>
#include <bitset>
#include <map>
#include <microsim/MSEventControl.h>
#include <microsim/traffic_lights/MSTrafficLightLogic.h>
#include <microsim/output/MSDetectorControl.h>
#include <microsim/output/MSInductLoop.h>
#include "MSSimpleTrafficLightLogic.h"


// ===========================================================================
// class declarations
// ===========================================================================
class NLDetectorBuilder;

// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class MSActuatedTrafficLightLogic
 * @brief An actuated (adaptive) traffic light logic
 */
class MSActuatedTrafficLightLogic : public MSSimpleTrafficLightLogic {
public:

    typedef std::map<std::string, std::string> ConditionMap;
    typedef std::vector<std::tuple<std::string, std::string, std::string> > AssignmentMap;


    /** @brief Constructor
     * @param[in] tlcontrol The tls control responsible for this tls
     * @param[in] id This tls' id
     * @param[in] programID This tls' sub-id (program id)
     * @param[in] phases Definitions of the phases
     * @param[in] step The initial phase index
     * @param[in] delay The time to wait before the first switch
     * @param[in] parameter The parameter to use for tls set-up
     */
    MSActuatedTrafficLightLogic(MSTLLogicControl& tlcontrol,
                                const std::string& id, const std::string& programID,
                                const SUMOTime offset,
                                const MSSimpleTrafficLightLogic::Phases& phases,
                                int step, SUMOTime delay,
                                const std::map<std::string, std::string>& parameter,
                                const std::string& basePath,
                                const ConditionMap& conditions = ConditionMap(),
                                const AssignmentMap& assignments = AssignmentMap());


    /** @brief Initialises the tls with information about incoming lanes
     * @param[in] nb The detector builder
     * @exception ProcessError If something fails on initialisation
     */
    void init(NLDetectorBuilder& nb);


    /// @brief Destructor
    ~MSActuatedTrafficLightLogic();



    /// @name Switching and setting current rows
    /// @{

    /** @brief Switches to the next phase
     * @return The time of the next switch
     * @see MSTrafficLightLogic::trySwitch
     */
    SUMOTime trySwitch();
    /// @}

    SUMOTime getMinDur(int step = -1) const override;
    SUMOTime getMaxDur(int step = -1) const override;
    SUMOTime getEarliestEnd(int step = -1) const override;
    SUMOTime getLatestEnd(int step = -1) const override;

    /// @name Changing phases and phase durations
    /// @{

    /** @brief Changes the current phase and her duration
     * @param[in] tlcontrol The responsible traffic lights control
     * @param[in] simStep The current simulation step
     * @param[in] step Index of the phase to use
     * @param[in] stepDuration The left duration of the phase
     * @see MSTrafficLightLogic::changeStepAndDuration
     */
    void changeStepAndDuration(MSTLLogicControl& tlcontrol, SUMOTime simStep,
                               int step, SUMOTime stepDuration);

    /// @brief called when switching programs
    void activateProgram();
    void deactivateProgram();

    bool showDetectors() const {
        return myShowDetectors;
    }

    void setShowDetectors(bool show);

    /**@brief Sets a parameter and updates internal constants */
    void setParameter(const std::string& key, const std::string& value);

    /// @brief retrieve all detectors used by this program
    std::map<std::string, double> getDetectorStates() const;

    /// @brief return all named conditions defined for this traffic light
    std::map<std::string, double> getConditions() const;

protected:
    /// @brief initialize custom switching rules
    void initAttributeOverride();
    void initSwitchingRules();

    struct InductLoopInfo {
        InductLoopInfo(MSInductLoop* _loop, int numPhases, double _maxGap):
            loop(_loop),
            servedPhase(numPhases, false),
            maxGap(_maxGap)
        {}
        MSInductLoop* loop;
        SUMOTime lastGreenTime = 0;
        std::vector<bool> servedPhase;
        double maxGap;
    };

    /// @brief Definition of a map from phases to induct loops controlling them
    typedef std::vector<std::vector<InductLoopInfo*> > InductLoopMap;

    /// @name "actuated" algorithm methods
    /// @{

    /** @brief Returns the minimum duration of the current phase
     * @param[in] detectionGap The minimum detection gap for the current phase
     * @return The minimum duration of the current phase
     */
    SUMOTime duration(const double detectionGap) const;

    /// @brief get the minimum min duration for all stretchable phases that affect the given lane
    SUMOTime getMinimumMinDuration(MSLane* lane) const;

    /** @brief Return the minimum detection gap of all detectors if the current phase should be extended and double::max otherwise
     */
    double gapControl();


    /// @brief return whether there is a major link from the given lane in the given phase
    bool hasMajor(const std::string& state, const LaneVector& lanes) const;
    /// @}

    /// @brief select among candidate phases based on detector states
    int decideNextPhase();

    /// @brief select among candidate phases based on detector states and custom switching rules
    int decideNextPhaseCustom(bool mustSwitch);

    /// @brief evaluate custom switching condition
    double evalExpression(const std::string& condition) const;

    /// @brief evaluate atomic expression
    double evalTernaryExpression(double a, const std::string& o, double b, const std::string& condition) const;

    /// @brief evaluate atomic expression
    double evalAtomicExpression(const std::string& expr) const;

    int getDetectorPriority(const InductLoopInfo& loopInfo) const;

    /// @brief count the number of active detectors for the given step
    int getPhasePriority(int step) const;

    /// @brief get the green phase following step
    int getTarget(int step);

    /// @brief whether the current phase cannot be continued due to linkMaxDur constraints
    bool maxLinkDurationReached();

    /// @brief whether the target phase is acceptable in light of linkMaxDur constraints
    bool canExtendLinkGreen(int target);

    /// @brief the minimum duratin for keeping the current phase due to linkMinDur constraints
    SUMOTime getLinkMinDuration(int target) const;

    template<typename T, SumoXMLTag Tag>
    const T* retrieveDetExpression(const std::string& arg, const std::string& expr, bool tryPrefix) const {
        const T* det = dynamic_cast<const T*>(
                           MSNet::getInstance()->getDetectorControl().getTypedDetectors(Tag).get(
                               (tryPrefix ? myDetectorPrefix : "") + arg));
        if (det == nullptr) {
            if (tryPrefix) {
                // try again without prefix
                return retrieveDetExpression<T, Tag>(arg, expr, false);
            } else {
                throw ProcessError("Unknown detector '" + arg + "' in expression '" + expr + "'");
            }
        } else {
            return det;
        }
    }

protected:
    /// @brief A map from phase to induction loops to be used for gap control
    InductLoopMap myInductLoopsForPhase;

    std::vector<InductLoopInfo> myInductLoops;

    /// The maximum gap to check in seconds
    double myMaxGap;

    /// The passing time used in seconds
    double myPassingTime;

    /// The detector distance in seconds
    double myDetectorGap;

    /// The time threshold to avoid starved phases
    SUMOTime myInactiveThreshold;

    /// Whether the detectors shall be shown in the GUI
    bool myShowDetectors;

    /// The output file for generated detectors
    std::string myFile;

    /// The frequency for aggregating detector output
    SUMOTime myFreq;

    /// Whether detector output separates by vType
    std::string myVehicleTypes;

    /// @brief last time trySwitch was called
    SUMOTime myLastTrySwitchTime;

    /// @brief consecutive time that the given link index has been green
    std::vector<SUMOTime> myLinkGreenTimes;
    std::vector<SUMOTime> myLinkRedTimes;
    /// @brief maximum consecutive time that the given link may remain green
    std::vector<SUMOTime> myLinkMaxGreenTimes;
    /// @brief minimum consecutive time that the given link must remain green
    std::vector<SUMOTime> myLinkMinGreenTimes;

    /// @brief The custom switching conditions
    ConditionMap myConditions;

    /// @brief The condition assignments
    AssignmentMap myAssignments;


    /// @brief the conditions which shall be listed in GUITLLogicPhasesTrackerWindow
    std::set<std::string> myListedConditions;

    /// @brief whether the next switch time was requested via TraCI
    bool myTraCISwitch;

    struct SwitchingRules {
        bool enabled = false;
    };

    std::vector<SwitchingRules> mySwitchingRules;

    const std::string myDetectorPrefix;

    static const std::vector<std::string> OPERATOR_PRECEDENCE;
};
