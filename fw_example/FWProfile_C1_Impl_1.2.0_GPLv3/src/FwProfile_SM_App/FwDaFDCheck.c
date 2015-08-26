/**
 * @file
 * @ingroup daGroup
 * Implementation of FD Check State Machine.
 *
 * @author V. Cechticky and A. Pasetti
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

#include <stdio.h>
#include <stdlib.h>
#include "FwProfile/FwSmConfig.h"
#include "FwProfile/FwSmDCreate.h"
#include "FwProfile/FwSmAux.h"
#include "FwProfile/FwSmConstants.h"
#include "FwDaFDCheck.h"

/* --------------------------------------------------------------------- */
/**
 * Entry action of state HEALTHY.
 * This action initializes the counter to zero.
 * @param smDesc the state machine descriptor
 */
static void ResetCounter(FwSmDesc_t smDesc) {
	FDCheckData_t* fdcData = GetFDCheckData(smDesc);
	fdcData->counter = 0;
	return;
}

/* --------------------------------------------------------------------- */
/**
 * Entry action of state SUSPECTED.
 * This action increments the counter by one.
 * @param smDesc the state machine descriptor
 */
static void IncrCounter(FwSmDesc_t smDesc) {
	FDCheckData_t* fdcData = GetFDCheckData(smDesc);
	fdcData->counter = fdcData->counter + 1;
	return;
}

/* --------------------------------------------------------------------- */
/**
 * Guard on the transition from HEALTHY to SUSPECTED.
 * This guard returns true if the outcome of the anomaly detection check
 * was "anomaly detected".
 * @param smDesc the state machine descriptor
 */
static FwSmBool_t IsAnomalyDetected(FwSmDesc_t smDesc) {
	FDCheckData_t* fdcData = GetFDCheckData(smDesc);
	return (fdcData->detectionCheckOutcome == anomalyDetected);
}

/* --------------------------------------------------------------------- */
/**
 * Guard on the transition from SUSPECTED to HEALTHY.
 * This guard returns true if the outcome of the anomaly detection check
 * was "no anomaly detected".
 * @param smDesc the state machine descriptor
 */
static FwSmBool_t IsNoAnomalyDetected(FwSmDesc_t smDesc) {
	FDCheckData_t* fdcData = GetFDCheckData(smDesc);
	return (fdcData->detectionCheckOutcome == noAnomalyDetected);
}

/* --------------------------------------------------------------------- */
/**
 * Guard on the transition from SUSPECTED to FAILED.
 * This guard returns true if the outcome of the anomaly detection check
 * was "anomaly detected" and the counter is equal to <code>cntLimit</code>.
 * @param smDesc the state machine descriptor
 */
static FwSmBool_t HasFailed(FwSmDesc_t smDesc) {
	FDCheckData_t* fdcData = GetFDCheckData(smDesc);
	return ((fdcData->detectionCheckOutcome == anomalyDetected) &&
								(fdcData->counter == fdcData->cntLimit));
}

/* --------------------------------------------------------------------- */
/**
 * Convenience function to print the name of a FD Check Identifier.
 * FD Check Identifiers are defined as instances of the enumerated type
 * <code>::FDCheckId_t</code>.
 * This function translates an identifier to a string holding the
 * name of the corresponding FD Check.
 * @param id the FD Check Identifier
 * @return the string holding the name of the FD Check
 */
static char* PrintFDCheckId(FDCheckId_t id) {
	switch (id) {
		case curFDCheckId: return "Curr. FD Check";
		case tempFDCheckId: return "Temp. FD Check";
		case deltaFDCheckId: return "Delta FD Check";
	}

	return NULL;
}

/* --------------------------------------------------------------------- */
/**
 * Transition action when a new state is entered further to the
 * FD Check State Machine being executed.
 * This transition action is associated to the transition from HEALTHY
 * to SUSPECTED, from SUSPECTED to FAILED and from SUSPECTED back to
 * HEALTHY.
 * This transition action writes a message to standard output.
 * The message identifies the target state towards which the transition
 * is being made and the FD Check which is executing the transition.
 * The logic to identify the source of the transition is as follows (recall
 * that, when the transition action is execute, the current state of a
 * state machine is still the source state of the transition):
 * - If the current state is HEALTHY, then the target state is
 *   SUSPECTED.
 * - If the current state is SUSPECTED and the
 *   <code>detectionCheckOutcome</code> is equal to "Anomaly Detected",
 *   then the target state is FAILED.
 * - If the current state is SUSPECTED and the
 *   <code>detectionCheckOutcome</code> is equal to "No Anomaly Detected",
 *   then the target state is HEALTHY.
 * .
 * @param smDesc the state machine descriptor
 */
static void TransAction(FwSmDesc_t smDesc) {
	FDCheckData_t* fdcData = GetFDCheckData(smDesc);

	if (FwSmGetCurState(smDesc) == FD_CHECK_HEALTHY) {
		printf(", %s --> SUSPECTED", PrintFDCheckId(fdcData->fdCheckId));
		return;
	}

	if ((FwSmGetCurState(smDesc) == FD_CHECK_SUSPECTED) &&
									(fdcData->detectionCheckOutcome == anomalyDetected)) {
		printf(", %s -->    FAILED", PrintFDCheckId(fdcData->fdCheckId));
		return;
	}

	if ((FwSmGetCurState(smDesc) == FD_CHECK_SUSPECTED) &&
									(fdcData->detectionCheckOutcome == noAnomalyDetected)) {
		printf(", %s -->   HEALTHY", PrintFDCheckId(fdcData->fdCheckId));
		return;
	}
}

/* --------------------------------------------------------------------- */
/**
 * Guard on the transition from SUSPECTED back to SUSPECTED.
 * This guard returns true if the outcome of the anomaly detection check
 * was "anomaly detected" and the counter is smaller than <code>cntLimit</code>.
 * @param smDesc the state machine descriptor
 */
static FwSmBool_t RemainSuspected(FwSmDesc_t smDesc) {
	FDCheckData_t* fdcData = GetFDCheckData(smDesc);
	return ((fdcData->detectionCheckOutcome == anomalyDetected) &&
								(fdcData->counter < fdcData->cntLimit));
}


/* --------------------------------------------------------------------- */
FDCheckData_t* GetFDCheckData(FwSmDesc_t smDesc) {
	return (FDCheckData_t*)FwSmGetData(smDesc);
}

/* --------------------------------------------------------------------- */
void DefAnomalyDetCheck(FwSmDesc_t smDesc) {
	FDCheckData_t* fdcData = GetFDCheckData(smDesc);
	fdcData->detectionCheckOutcome = noAnomalyDetected;
	return;
}

/* --------------------------------------------------------------------- */
void DefRecoveryAction(FwSmDesc_t smDesc) {
	return;
}

/* --------------------------------------------------------------------- */
FwSmDesc_t GetFailDetCheckSm() {
	static FwSmDesc_t esmDesc;	/* Descriptor of SM embedded in ENABLED */
	static FwSmDesc_t fdCheckSm = NULL; /* FD Check SM Descriptor */
	const int CPS1 = 1;		/* Identifier of first Choice Pseudo-State */
	const int CPS2 = 2;		/* Identifier of second Choice Pseudo-State */

	if (fdCheckSm != NULL)	/* FD Check SM has already been created */
		return fdCheckSm;

	/* Create the SM embedded in state ENABLED */
	esmDesc = FwSmCreate(3,2,9,4,4);

	/* Configure the SM embedded in state ENABLED */
	FwSmAddState(esmDesc, FD_CHECK_HEALTHY, 1, &ResetCounter, NULL, NULL, NULL);
	FwSmAddState(esmDesc, FD_CHECK_SUSPECTED, 2, &IncrCounter, NULL, NULL, NULL);
	FwSmAddState(esmDesc, FD_CHECK_FAILED, 1, &DefRecoveryAction, NULL, NULL, NULL);
	FwSmAddChoicePseudoState(esmDesc, CPS1, 3);
	FwSmAddChoicePseudoState(esmDesc, CPS2, 1);
	FwSmAddTransIpsToSta(esmDesc, FD_CHECK_HEALTHY, NULL);
	FwSmAddTransStaToSta(esmDesc, FW_TR_EXECUTE, FD_CHECK_HEALTHY, FD_CHECK_SUSPECTED, &TransAction, &IsAnomalyDetected);
	FwSmAddTransStaToCps(esmDesc, FW_TR_EXECUTE, FD_CHECK_SUSPECTED, CPS1, NULL, NULL);
	FwSmAddTransCpsToSta(esmDesc, CPS1, FD_CHECK_HEALTHY, &TransAction, &IsNoAnomalyDetected);
	FwSmAddTransCpsToSta(esmDesc, CPS1, FD_CHECK_SUSPECTED, NULL, &RemainSuspected);
	FwSmAddTransCpsToSta(esmDesc, CPS1, FD_CHECK_FAILED, &TransAction, &HasFailed);
	FwSmAddTransStaToCps(esmDesc, TR_FD_CHECK_RESET, FD_CHECK_SUSPECTED, CPS2, NULL, NULL);
	FwSmAddTransStaToCps(esmDesc, TR_FD_CHECK_RESET, FD_CHECK_FAILED, CPS2, NULL, NULL);
	FwSmAddTransCpsToSta(esmDesc, CPS2, FD_CHECK_HEALTHY, NULL, NULL);

	/* Create the FD Check SM */
	fdCheckSm = FwSmCreate(2,0,3,1,0);

	/* Configure the FD Check SM */
	FwSmAddState(fdCheckSm, FD_CHECK_DISABLED, 1, NULL, NULL, NULL, NULL);
	FwSmAddState(fdCheckSm, FD_CHECK_ENABLED, 1, NULL, NULL, &DefAnomalyDetCheck, esmDesc);
	FwSmAddTransIpsToSta(fdCheckSm, FD_CHECK_DISABLED, NULL);
	FwSmAddTransStaToSta(fdCheckSm, TR_FD_CHECK_ENABLE, FD_CHECK_DISABLED, FD_CHECK_ENABLED, NULL, NULL);
	FwSmAddTransStaToSta(fdCheckSm, TR_FD_CHECK_DISABLE, FD_CHECK_ENABLED, FD_CHECK_DISABLED, NULL, NULL);

	return fdCheckSm;
}
