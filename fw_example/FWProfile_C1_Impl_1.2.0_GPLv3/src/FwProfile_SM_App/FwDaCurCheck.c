/**
 * @file
 * @ingroup daGroup
 * Implementation of Current Failure Detection (FD) Check.
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

#include "FwDaFDCheck.h"
#include "FwDaHwDev.h"
#include "FwDaCurCheck.h"
#include "FwDaCurRecAction.h"
#include "FwProfile/FwSmCore.h"
#include "FwProfile/FwSmConfig.h"
#include "FwProfile/FwSmDCreate.h"
#include "FwProfile/FwPrCore.h"

/* --------------------------------------------------------------------- */
/**
 * Anomaly Detection Check for the Current FD Check.
 * This Anomaly Detection Check declares an anomaly (i.e. it sets the
 * value of the check outcome to 1) if the current absorbed by the
 * Hardware Device is greater than HW_DEV_CUR_MAX_NOM.
 * @param smDesc the state machine descriptor
 */
static void CurAnomalyDetCheck(FwSmDesc_t smDesc);

/* --------------------------------------------------------------------- */
/**
 * Recovery Action for the Current FD Check.
 * This recovery action starts the Current Recovery Action Procedure
 * of <code>FwDaCurRecAction.h</code>.
 * @param smDesc the state machine descriptor
 */
static void CurRecoveryAction(FwSmDesc_t smDesc);

/* --------------------------------------------------------------------- */
FwSmDesc_t GetCurCheckSm() {
	static FwSmDesc_t curFdcSm = NULL;
	static FDCheckData_t smData;
	FwSmDesc_t esmEnabled;

	if (curFdcSm != NULL)	/* Current FD Check SM was already created */
		return curFdcSm;

	/* Extend the generic FD Check State Machine */
	curFdcSm = FwSmCreateDer(GetFailDetCheckSm());

	/* Retrieve the SM embedded in state ENABLED */
	esmEnabled = FwSmGetEmbSm(curFdcSm, FD_CHECK_ENABLED);

	/* Load the data into the outer SM and into the SM embedded in state ENABLED */
	smData.cntLimit = HW_DEV_CUR_CNT_LIMIT;
	smData.fdCheckId = curFDCheckId;
	FwSmSetData(curFdcSm, &smData);
	FwSmSetData(esmEnabled, &smData);

	/* Override the Anomaly Detection Check in the outer SM */
	FwSmOverrideAction(curFdcSm, &DefAnomalyDetCheck, &CurAnomalyDetCheck);

	/* Override the Recovery Action in the SM embedded in state ENABLED */
	FwSmOverrideAction(esmEnabled, &DefRecoveryAction, &CurRecoveryAction);

	return curFdcSm;
}

/* --------------------------------------------------------------------- */
static void CurAnomalyDetCheck(FwSmDesc_t smDesc) {
	FDCheckData_t* cfdCheckData = GetFDCheckData(smDesc);
	if (GetHwDevCur() > HW_DEV_CUR_MAX_NOM)
		cfdCheckData->detectionCheckOutcome = anomalyDetected;
	else
		cfdCheckData->detectionCheckOutcome = noAnomalyDetected;
}

/* --------------------------------------------------------------------- */
static void CurRecoveryAction(FwSmDesc_t smDesc) {
	FwPrStart(GetCurRecActionPr());
}


