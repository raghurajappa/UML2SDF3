/**
 * @file
 * @ingroup daGroup
 * Demo application for the C1 Implementation of the FW Profile.
 * This file defines the <code>main</code> program for the demo application.
 * The demo application shows how state machines can be used to implement
 * a set of <i>Failure Detection (FD) Checks</i> for a <i>Hardware Device</i>
 * and how procedures can be used to implement the <i>Recovery Actions</i>
 * handling failures in the hardware device.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FwProfile/FwSmConstants.h"
#include "FwProfile/FwSmCore.h"
#include "FwProfile/FwSmConfig.h"
#include "FwProfile/FwPrConstants.h"
#include "FwProfile/FwPrCore.h"
#include "FwProfile/FwPrConfig.h"
#include "FwDaHwDev.h"
#include "FwDaFDCheck.h"
#include "FwDaCurCheck.h"
#include "FwDaCurRecAction.h"
#include "FwDaTempCheck.h"
#include "FwDaTempRecAction.h"
#include "FwDaDeltaCheck.h"

/** Number of cycles executed by the Demo Application */
#define N_CYCLE 400

/**
 * Main program for the demo application.
 * The demo application consists of two parts: an initialization part and a
 * cyclical part.
 * In the initialization part, the following is done:
 * - The Hardware Device State Machine is created and started
 * - The FD Check State Machines are created and started
 * - The Recovery Action Procedures are created
 * - The configuration of all state machine is checked and the program
 *   exits with a failure code if configuration errors are found
 * - The configuration of all procedures is checked and the program
 *   exits with a failure code if configuration errors are found
 * - The Hardware Device State Machine is commanded into its operational state
 * - The FD Checks are enabled
 * .
 * In the cyclical part, the following is done:
 * - The Hardware Device state machine is executed,
 * - The FD Checks state machines are executed,
 * - If an FD Check detects a failure and commands the Hardware Device into
 *   STANDBY or switches it off, then the FD Checks are reset and the Hardware
 *   Device is switched back on.
 * .
 * The cycle is executed N_CYCLE times.
 * @return returns EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int main() {
	int i;
	FwSmDesc_t hwDevSm = GetHwDevSm();			/* The HW Device State Machine */
	FwSmDesc_t tempCheck = GetTempCheckSm();	/* The Temperature FD Check State Machine */
	FwSmDesc_t curCheck = GetCurCheckSm();		/* The Current FD Check State Machine */
	FwSmDesc_t deltaCheck = GetDeltaCheckSm();	/* The Delta FD Check State Machine */
	FwPrDesc_t curRecAction = GetCurRecActionPr();		/* The Current Recovery Action Procedure */
	FwPrDesc_t tempRecAction = GetTempRecActionPr();	/* The Temperature Recovery Action Procedure */

	/* Check configuration of all state machines */
	if (FwSmCheckRec(hwDevSm) != smSuccess)
		return EXIT_FAILURE;
	if (FwSmCheckRec(tempCheck) != smSuccess)
		return EXIT_FAILURE;
	if (FwSmCheckRec(curCheck) != smSuccess)
		return EXIT_FAILURE;
	if (FwSmCheckRec(deltaCheck) != smSuccess)
		return EXIT_FAILURE;

	/* Check configuration of all procedures */
	if (FwPrCheck(curRecAction) != prSuccess)
		return EXIT_FAILURE;
	if (FwPrCheck(tempRecAction) != prSuccess)
		return EXIT_FAILURE;

	/* Start all state machines */
	FwSmStart(hwDevSm);
	FwSmStart(tempCheck);
	FwSmStart(curCheck);
	FwSmStart(deltaCheck);

	/* Switch on the Hardware Device (HW Device SM enters STANDBY Mode) */
	FwSmMakeTrans(hwDevSm, TR_HW_DEV_ON);

	/* Command the Hardware Device into its operational state */
	FwSmMakeTrans(hwDevSm, TR_HW_DEV_OPER);

	/* Enable all the FD Checks */
	FwSmMakeTrans(tempCheck, TR_FD_CHECK_ENABLE);
	FwSmMakeTrans(curCheck, TR_FD_CHECK_ENABLE);
	FwSmMakeTrans(deltaCheck, TR_FD_CHECK_ENABLE);

	for (i=0; i<N_CYCLE; i++) {
		FwSmExecute(hwDevSm);		/* Execute HW Device State Machine */
		printf("\nTemp = %3.0f, Current = %3.2f", GetHwDevTemp(), GetHwDevCur());

		/* Execute the three FD Check State Machines */
		FwSmExecute(tempCheck);
		FwSmExecute(curCheck);
		FwSmExecute(deltaCheck);

		/* Execute the Recovery Action Procedures */
		FwPrExecute(curRecAction);
		FwPrExecute(tempRecAction);

		if (FwSmGetCurState(hwDevSm) == HW_DEV_OFF) {
			/* Stop any recovery actions which may still be running */
			FwPrStop(curRecAction);
			FwPrStop(tempRecAction);
			printf("\nStarting again after a failure has been detected and handled ...");
			/* Switch On HW Device State Machine */
			FwSmMakeTrans(hwDevSm, TR_HW_DEV_ON);
			FwSmMakeTrans(hwDevSm, TR_HW_DEV_OPER);
			/* Reset the three FD Check State Machines */
			FwSmMakeTrans(tempCheck, TR_FD_CHECK_RESET);
			FwSmMakeTrans(curCheck, TR_FD_CHECK_RESET);
			FwSmMakeTrans(deltaCheck, TR_FD_CHECK_RESET);
		}
	}

	printf("\n");
	return EXIT_SUCCESS;
}
