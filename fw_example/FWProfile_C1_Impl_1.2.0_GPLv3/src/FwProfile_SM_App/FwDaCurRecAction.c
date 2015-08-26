/**
 * @file
 * @ingroup daGroup
 * Implementation of Current Recovery Action.
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

#include <stdlib.h>
#include "FwDaCurRecAction.h"
#include "FwDaHwDev.h"
#include "FwDaCurCheck.h"
#include "FwProfile/FwPrCore.h"
#include "FwProfile/FwPrConfig.h"
#include "FwProfile/FwPrDCreate.h"

/* --------------------------------------------------------------------- */
/**
 * Switch Off Hardware Device.
 * The Hardware Device is switch off by sending it command TR_HW_DEV_OFF.
 * @param prDesc the procedure descriptor
 */
static void HwDevSwitchOff(FwPrDesc_t prDesc);

/* --------------------------------------------------------------------- */
/**
 * Command Hardware Device to Stand-By.
 * The Hardware Device is commanded to Stand-By by sending it command
 * TR_HW_DEV_SBY.
 * @param prDesc the procedure descriptor
 */
static void HwDevToStandBy(FwPrDesc_t prDesc);

/* --------------------------------------------------------------------- */
/**
 * Wait CUR_REC_ACTION_N1 cycles.
 * This function is used as a guard in a procedure.
 * It returns true if the procedure has been executed at least
 * CUR_REC_ACTION_N1 times since the current node was entered.
 * Thus this guard holds the procedure at the current node for
 * CUR_REC_ACTION_N1 execution cycles.
 * @param prDesc the procedure descriptor
 */
static FwPrBool_t WaitN1Cycle(FwPrDesc_t prDesc);

/* --------------------------------------------------------------------- */
FwPrDesc_t GetCurRecActionPr() {
	static FwPrDesc_t curRecAction = NULL;
	const FwPrCounterS1_t nOfANodes = 2;	/* Number of action nodes */
	const FwPrCounterS1_t nOfDNodes = 0;	/* Number of decision nodes */
	const FwPrCounterS1_t nOfFlows = 3;		/* Number of control flows */
	const FwPrCounterS1_t nOfActions = 2;	/* Number of actions */
	const FwPrCounterS1_t nOfGuards = 1;	/* Number of guards */

	if (curRecAction != NULL)	/* Current Recovery Action was already created */
		return curRecAction;

	/* Create and configure the procedure */
	curRecAction = FwPrCreate(nOfANodes,nOfDNodes,nOfFlows,nOfActions,nOfGuards);

	FwPrAddActionNode(curRecAction, 1, &HwDevToStandBy);
	FwPrAddActionNode(curRecAction, 2, &HwDevSwitchOff);

	FwPrAddFlowIniToAct(curRecAction, 1, NULL);
	FwPrAddFlowActToAct(curRecAction, 1, 2, &WaitN1Cycle);
	FwPrAddFlowActToFin(curRecAction, 2, NULL);

	return curRecAction;
}

/* --------------------------------------------------------------------- */
static void HwDevSwitchOff(FwPrDesc_t prDesc) {
	FwSmMakeTrans(GetHwDevSm(), TR_HW_DEV_OFF);
}

/* --------------------------------------------------------------------- */
static void HwDevToStandBy(FwPrDesc_t prDesc) {
	FwSmMakeTrans(GetHwDevSm(), TR_HW_DEV_SBY);
}

/* --------------------------------------------------------------------- */
static FwPrBool_t WaitN1Cycle(FwPrDesc_t prDesc) {
	if (FwPrGetNodeExecCnt(prDesc) >= CUR_REC_ACTION_N1)
		return 1;
	else
		return 0;
}
