/**
 * @file
 * @ingroup daGroup
 * Definition of the <i>Failure Detection (FD) Check State
 * Machine</i>.
 * A FD Check State Machine is a state machine which encapsulates
 * a <i>Failure Detection (FD) Check</i> for the Hardware Device.
 * A Failure Detection Check is a check which is performed periodically to
 * monitor the health of the Hardware Device.
 * The FD Check State Machine defines the generic behaviour which is
 * shared by all FD Checks.
 * A specific FD Check is encapsulated in a state machine which is
 * obtained by extending the FD Check State Machine.
 * 
 * An FD Check can be in one of two states: ENABLED or DISABLED.
 * When an FD Check is in state DISABLED, it does not perform any
 * monitoring action.
 * When an FD Check is in state ENABLED, it performs the
 * <i>Anomaly Detection Check</i>.
 * The Anomaly Detection Check is a function which monitors a
 * certain aspect of the health of the Hardware Device and which,
 * every time it is called, can return one of two outcomes:
 * <i>anomaly detected</i> or <i>no anomaly detected</i>.
 * 
 * State ENABLED is sub-divided into three sub-states: HEALTHY,
 * SUSPECTED, and FAILED.
 * State HEALTHY indicates a situation where no anomalies have
 * been detected in the Hardware Device.
 * State SUSPECTED indicates a situation where anomalies have been
 * detected but the number of consecutive anomalies is less than
 * <code>cntLimit</code>.
 * State FAILED indicates a situation where more than <code>cntLimit</code>
 * consecutive anomalies have been detected and where the Hardware
 * Device has consequently been declared to have failed.
 * 
 * When state FAILED is entered, the <i>Recovery Action</i> is executed.
 * This action is intended to remove the failure which has been detected
 * by the FD Check.
 * 
 * The FD Check State Machine is shown in the figure below.
 * The stereotype "AP" designate actions which can be overridden
 * when the state machine is extended.
 * Specific FD Checks specialize the generic behaviour shown in the
 * figure by providing their own Anomaly Detection Check and their
 * own Recovery Action.
 * The default implementation provided by the FD Check State Machine
 * for these two actions is:
 * - The default Anomaly Detection Check always returns: "no anomaly
 *   detected".
 * - The default Recovery Action returns without doing anything.
 * .
 * @image html FailDetCheck.png
 * @author Vaclav Cechticky, Alessandro Pasetti
 * @copyright P&P Software GmbH, 2013, All Rights Reserved
 * @version 1.2.0_GPLv3
 *
 * This file is part of FwProfile.
 *
 * FwProfile is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FwProfile is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FwProfile.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FWDAFDCHECK_H_
#define FWDAFDCHECK_H_

#include <stdio.h>
#include "FwProfile/FwSmCore.h"

/** Name of the DISABLED state in the FD Check State Machine */
#define FD_CHECK_DISABLED 1
/** Name of the ENABLED state in the FD Check State Machine */
#define FD_CHECK_ENABLED 2
/** Name of the HEALTHY sub-state in the FD Check State Machine */
#define FD_CHECK_HEALTHY 1
/** Name of the SUSPECTED state in the FD Check State Machine */
#define FD_CHECK_SUSPECTED 2
/** Name of the FAILED state in the FD Check State Machine */
#define FD_CHECK_FAILED 3

/** Name of the transition to enable a FD Check */
#define TR_FD_CHECK_ENABLE 1
/** Name of the transition to disable a FD Check */
#define TR_FD_CHECK_DISABLE 2
/** Name of the transition to reset a FD Check */
#define TR_FD_CHECK_RESET 3

/** Type for the identifiers of the FD Checks. */
typedef enum {
	/** Identifier of the Current FD Check (see <code>FwDaCurCheck.h</code>) */
	curFDCheckId = 1,
	/** Identifier of the Temperature FD Check (see <code>FwDaTempCheck.h</code>) */
	tempFDCheckId = 2,
	/** Identifier of the Delta FD Check (see <code>FwDaDeltaCheck.h</code>) */
	deltaFDCheckId = 3
} FDCheckId_t;

/** Type for the outcome of the Anomaly Detection Check. */
typedef enum {
	/** Outcome generated when the Anomaly Detection Check detects no anomaly */
	noAnomalyDetected = 0,
	/** Outcome generated when the Anomaly Detection Check detects an anomaly */
	anomalyDetected = 1
} FDCheckOutcome_t;

/**
 * Type for the data of an FD Check State Machine.
 * The fields in this structure represent data which are used by a
 * generic FD Check State Machine.
 * An instance of this data structure is stored in the state machine
 * descriptor of each FD Check State Machine and is consequently
 * available to all functions implementing actions and guards of
 * an FD Check State Machine.
 */
typedef struct FDCheckData {
	/**
	 * Identity of the FD Check to which this data structure is attached.
	 */
	FDCheckId_t fdCheckId;
	/**
	 * The number of consecutive anomalies which have been detected by
	 * the Anomaly Detection Check.
	 */
	int counter;
	/**
	 * The number of consecutive anomalies which must be detected in order for
	 * the FD Check to enter state FAILED.
	 */
	int cntLimit;
	/**
	 * The outcome of the last call to the Anomaly Detection Check.
	 */
	FDCheckOutcome_t detectionCheckOutcome;
} FDCheckData_t;

/**
 * Get the pointer to the state machine data of an FD Check State Machine.
 * This function is a wrapper for the <code>::FwSmGetData</code> function.
 * Function <code>::FwSmGetData</code> is defined at the level of the State
 * Machine Module and returns the pointer to the state machine data as a
 * pointer to <code>void</code>.
 * This function cast this pointer to point to <code>FDCheckData_t</code>.
 * @param smDesc the state machine descriptor
 * @return pointer to the state machine data
 */
FDCheckData_t* GetFDCheckData(FwSmDesc_t smDesc);

/**
 * Retrieve the descriptor of the FD Check State Machine.
 * The FD Check State Machine is a singleton.
 * The first time this function is called, it creates and configures
 * the state machine descriptor.
 * Subsequently, it always returns the same descriptor.
 * @return the descriptor of the FD Check State Machine or NULL
 * if the state machine could not be created.
 */
FwSmDesc_t GetFailDetCheckSm();

/**
 * Default implementation of the Anomaly Detection Check.
 * This default implementation always returns: "no anomaly detected".
 * @param smDesc the state machine descriptor
 */
void DefAnomalyDetCheck(FwSmDesc_t smDesc);

/**
 * Default implementation of the Recovery Action.
 * This default implementation returns without doing anything.
 * @param smDesc the state machine descriptor
 */
void DefRecoveryAction(FwSmDesc_t smDesc);

#endif /* FWDAFDCHECK_H_ */
