/**
 * @file
 * @ingroup daGroup
 * Implementation of HW Device State Machine.
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
#include "FwDaHwDev.h"

/**
 * The last measurement of the temperature of the Hardware Device.
 * The temperature measurement is updated by the do-action of
 * state ON in the Hardware Device State Machine.
 */
static float tempMeasurement = 0;

/**
 * The last measurement of the current absorbed by the Hardware
 * Device.
 * The temperature measurement is updated by the do-action of
 * state ON in the Hardware Device State Machine.
 */
static float curMeasurement = 0;

/* --------------------------------------------------------------------- */
/**
 * Entry action for the ON state.
 * This entry action writes a message to standard output.
 * @param smDesc the state machine descriptor
 */
static void OnEntry(FwSmDesc_t smDesc) {
	fprintf(stdout, "\nHW Device switched on");
}

/* --------------------------------------------------------------------- */
/**
 * Entry action for the OFF state.
 * This entry action writes a message to standard output.
 * @param smDesc the state machine descriptor
 */
static void OffEntry(FwSmDesc_t smDesc) {
	fprintf(stdout, "\nHW Device switched off");
}

/* --------------------------------------------------------------------- */
/**
 * Entry action for the STANDBY state.
 * This entry action writes a message to standard output.
 * @param smDesc the state machine descriptor
 */
static void SbyEntry(FwSmDesc_t smDesc) {
	fprintf(stdout, "\nHW Device in STANDBY");
}

/* --------------------------------------------------------------------- */
/**
 * Entry action for the OPERATIONAL state.
 * This entry action writes a message to standard output.
 * @param smDesc the state machine descriptor
 */
static void OperEntry(FwSmDesc_t smDesc) {
	fprintf(stdout, "\nHW Device operational");
}

/* --------------------------------------------------------------------- */
/**
 * Do-action for the ON state.
 * This action collects the measurements of the temperature and of the
 * current of the Hardware Device.
 * In this demo application, the measurement process is not modelled
 * explicitly.
 * Instead, the temperature measurement is modelled a stochastic
 * variable which is uniformly distributed in the interval:
 * [0, HW_DEV_TEMP_MAX] and, similarly, the current measurement
 * modelled a stochastic variable which is uniformly distributed
 * in the interval: [0, HW_DEV_CUR_MAX].
 * @param smDesc the state machine descriptor
 */
static void CollectMeasurements(FwSmDesc_t smDesc) {
	tempMeasurement = (rand()%1000)*HW_DEV_TEMP_MAX/1000.0;
	curMeasurement = (rand()%1000)*HW_DEV_CUR_MAX/1000.0;
}

/* --------------------------------------------------------------------- */
FwSmDesc_t GetHwDevSm() {
	static FwSmDesc_t hwDevSm = NULL; 	/* HW Device SM */
	static FwSmDesc_t esmDesc;			/* SM embedded in state ON of HW Device SM */

	if (hwDevSm != NULL) 	/* SM has already been created and configured */
		return hwDevSm;

	/* Create the SM embedded in state ON */
	esmDesc = FwSmCreate(2, 0, 3, 2, 0);

	/* Configure the SM embedded in state ON */
	FwSmAddState(esmDesc, HW_DEV_SBY, 1, &SbyEntry, NULL, NULL, NULL);
	FwSmAddState(esmDesc, HW_DEV_OPER, 1, &OperEntry, NULL, NULL, NULL);
	FwSmAddTransIpsToSta(esmDesc, HW_DEV_SBY, NULL);
	FwSmAddTransStaToSta(esmDesc, TR_HW_DEV_OPER, HW_DEV_SBY, HW_DEV_OPER, NULL, NULL);
	FwSmAddTransStaToSta(esmDesc, TR_HW_DEV_SBY, HW_DEV_OPER, HW_DEV_SBY, NULL, NULL);

	/* Create the HW Device State Machine */
	hwDevSm = FwSmCreate(2, 0, 3, 3, 0);

	/* Configure the HW Device State Machine */
	FwSmSetData(hwDevSm, NULL);	/* NB: There are no data attached to this SM */
	FwSmAddState(hwDevSm, HW_DEV_ON, 1, &OnEntry, NULL, &CollectMeasurements, esmDesc);
	FwSmAddState(hwDevSm, HW_DEV_OFF, 1, &OffEntry, NULL, NULL, NULL);
	FwSmAddTransIpsToSta(hwDevSm, HW_DEV_OFF, NULL);
	FwSmAddTransStaToSta(hwDevSm, TR_HW_DEV_ON, HW_DEV_OFF, HW_DEV_ON, NULL, NULL);
	FwSmAddTransStaToSta(hwDevSm, TR_HW_DEV_OFF, HW_DEV_ON, HW_DEV_OFF, NULL, NULL);

	return hwDevSm;
}

/* --------------------------------------------------------------------- */
float GetHwDevTemp() {
	return tempMeasurement;
}

/* --------------------------------------------------------------------- */
float GetHwDevCur() {
	return curMeasurement;
}

