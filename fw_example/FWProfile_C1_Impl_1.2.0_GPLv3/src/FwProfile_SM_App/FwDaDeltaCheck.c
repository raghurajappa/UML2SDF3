/**
 * @file
 * @ingroup daGroup
 * Implementation of Delta Failure Detection (FD) Check.
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

#include <math.h>
#include <stdlib.h>
#include "FwDaFDCheck.h"
#include "FwDaHwDev.h"
#include "FwDaDeltaCheck.h"
#include "FwProfile/FwSmCore.h"
#include "FwProfile/FwSmConfig.h"
#include "FwProfile/FwSmDCreate.h"

/**
 * Measurement of the current absorbed by the Hardware Device in
 * the previous cycle.
 * The Delta FD Check measures the change across activations in the
 * current absorbed by the Hardware Device.
 * It therefore needs to keep track of the previous current
 * measurement.
 */
static float prevCurMeasurement = 0;

/**
 * Measurement of the temperature of the Hardware Device in
 * the previous cycle.
 * The Delta FD Check measures the change across activations in the
 * temperature of the Hardware Device.
 * It therefore needs to keep track of the previous temperature
 * measurement.
 */
static float prevTempMeasurement = 0;

/* --------------------------------------------------------------------- */
/**
 * Anomaly Detection Check for the Delta FD Check.
 * This Anomaly Detection Check declares an anomaly (i.e. it sets the
 * value of the check outcome to 1) if either of the following conditions
 * is satisfied:
 * - The current absorbed by the Hardware Device since the previous
 *   activation has increased by more than HW_DEV_CUR_MAX_DELTA.
 * - The temperature of the Hardware Device has increased by more than
 *   HW_DEV_TEMP_MAX_DELTA since the previous activation cycle.
 * .
 * @param smDesc the state machine descriptor
 */
static void DeltaAnomalyDetCheck(FwSmDesc_t smDesc);

/* --------------------------------------------------------------------- */
/**
 * Recovery Action for the Delta FD Check.
 * This recovery action switches off the Hardware Device into STANDBY mode.
 * The Hardware Device is switched off by sending it command TR_HW_DEV_OFF.
 * @param smDesc the state machine descriptor
 */
static void DeltaRecoveryAction(FwSmDesc_t smDesc);

/* --------------------------------------------------------------------- */
FwSmDesc_t GetDeltaCheckSm() {
	static FwSmDesc_t delFdcSm = NULL;	/* Delta FD Check SM */
	static FDCheckData_t smData;
	FwSmDesc_t esmEnabled;

	if (delFdcSm != NULL)	/* Current FD Check SM was already created */
		return delFdcSm;

	/* Extend the generic FD Check State Machine */
	delFdcSm = FwSmCreateDer(GetFailDetCheckSm());

	/* Retrieve the SM embedded in state ENABLED */
	esmEnabled = FwSmGetEmbSm(delFdcSm, FD_CHECK_ENABLED);

	/* Load the data into the outer SM and into the SM embedded in state ENABLED */
	smData.cntLimit = HW_DEV_DELTA_CNT_LIMIT;
	smData.fdCheckId = deltaFDCheckId;
	FwSmSetData(delFdcSm, &smData);
	FwSmSetData(esmEnabled, &smData);

	/* Override the Anomaly Detection Check in the outer SM */
	FwSmOverrideAction(delFdcSm, &DefAnomalyDetCheck, &DeltaAnomalyDetCheck);

	/* Override the Recovery Action in the SM embedded in state ENABLED */
	FwSmOverrideAction(esmEnabled, &DefRecoveryAction, &DeltaRecoveryAction);

	return delFdcSm;
}

/* --------------------------------------------------------------------- */
static void DeltaAnomalyDetCheck(FwSmDesc_t smDesc) {
	FDCheckData_t* dfdCheckData = GetFDCheckData(smDesc);

	dfdCheckData->detectionCheckOutcome = noAnomalyDetected;

	/* Check whether the temperature measurement has "jumped" */
	if (fabs(GetHwDevTemp()-prevTempMeasurement) > HW_DEV_TEMP_MAX_DELTA)
		dfdCheckData->detectionCheckOutcome = anomalyDetected;
	else
		prevTempMeasurement = GetHwDevTemp();

	/* Check whether the current measurement has "jumped" */
	if (fabs(GetHwDevCur()-prevCurMeasurement) > HW_DEV_CUR_MAX_DELTA)
		dfdCheckData->detectionCheckOutcome = anomalyDetected;
	else
		prevCurMeasurement = GetHwDevCur();

	return;
}

/* --------------------------------------------------------------------- */
static void DeltaRecoveryAction(FwSmDesc_t smDesc) {
	FwSmMakeTrans(GetHwDevSm(), TR_HW_DEV_OFF);
}


