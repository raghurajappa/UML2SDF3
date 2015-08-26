/**
 * @file
 * @ingroup daGroup
 * Implementation of Temperature Failure Detection (FD) Check.
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
#include "FwDaTempCheck.h"
#include "FwDaTempRecAction.h"
#include "FwProfile/FwSmCore.h"
#include "FwProfile/FwSmConfig.h"
#include "FwProfile/FwSmDCreate.h"
#include "FwProfile/FwPrCore.h"

/* --------------------------------------------------------------------- */
/**
 * Anomaly Detection Check for the Temperature FD Check.
 * This Anomaly Detection Check declares an anomaly (i.e. it sets the
 * value of the check outcome to 1) if the temperature of the Hardware
 * Device is greater than HW_DEV_TEMP_MAX_NOM.
 * @param smDesc the state machine descriptor
 */
static void TempAnomalyDetCheck(FwSmDesc_t smDesc);

/* --------------------------------------------------------------------- */
/**
 * Recovery Action for the Temperature FD Check.
 * This recovery action starts the Temperature Recovery Action procedure.
 * @param smDesc the state machine descriptor
 */
static void TempRecoveryAction(FwSmDesc_t smDesc);

/* --------------------------------------------------------------------- */
FwSmDesc_t GetTempCheckSm() {
	static FwSmDesc_t tempFdcSm = NULL;
	static FDCheckData_t smData;
	FwSmDesc_t esmEnabled;

	if (tempFdcSm != NULL)	/* Temperature FD Check SM was already created */
		return tempFdcSm;

	/* Extend the generic FD Check State Machine */
	tempFdcSm = FwSmCreateDer(GetFailDetCheckSm());

	/* Retrieve the SM embedded in state ENABLED */
	esmEnabled = FwSmGetEmbSm(tempFdcSm, FD_CHECK_ENABLED);

	/* Load the data into the outer SM and into the SM embedded into state ENABLED */
	smData.cntLimit = HW_DEV_TEMP_CNT_LIMIT;
	smData.fdCheckId = tempFDCheckId;
	FwSmSetData(tempFdcSm, &smData);
	FwSmSetData(esmEnabled, &smData);

	/* Override the Anomaly Detection Check in the outer SM */
	FwSmOverrideAction(tempFdcSm, &DefAnomalyDetCheck, &TempAnomalyDetCheck);

	/* Override the Recovery Action in the SM embedded in state ENABLED */
	FwSmOverrideAction(esmEnabled, &DefRecoveryAction, &TempRecoveryAction);

	return tempFdcSm;
}

/* --------------------------------------------------------------------- */
static void TempAnomalyDetCheck(FwSmDesc_t smDesc) {
	FDCheckData_t* tfdCheckData = GetFDCheckData(smDesc);
	if (GetHwDevTemp() > HW_DEV_TEMP_MAX_NOM)
		tfdCheckData->detectionCheckOutcome = anomalyDetected;
	else
		tfdCheckData->detectionCheckOutcome = noAnomalyDetected;
}

/* --------------------------------------------------------------------- */
static void TempRecoveryAction(FwSmDesc_t smDesc) {
	FwPrStart(GetTempRecActionPr());
}


