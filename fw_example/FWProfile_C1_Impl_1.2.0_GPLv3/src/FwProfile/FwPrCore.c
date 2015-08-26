/**
 * @file
 * @ingroup prGroup
 * Implements Core Functions of the FW Procedure.
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

#include "FwPrCore.h"
#include "FwPrPrivate.h"
#include <stdlib.h>

/* ----------------------------------------------------------------------------------------------------------------- */
FwPrBool_t PrDummyGuard(FwPrDesc_t prDesc) {
	return 1;
}

/* ----------------------------------------------------------------------------------------------------------------- */
void FwPrStart(FwPrDesc_t prDesc) {
	if (prDesc->curNode == 0) {
		prDesc->curNode = -1;
		prDesc->prExecCnt = 0;
		prDesc->nodeExecCnt = 0;
	}
}	

/* ----------------------------------------------------------------------------------------------------------------- */
void FwPrStop(FwPrDesc_t prDesc) {
	prDesc->curNode = 0;
}

/* ----------------------------------------------------------------------------------------------------------------- */
void FwPrExecute(FwPrDesc_t prDesc) {
	PrANode_t* curNode;
	PrDNode_t* decNode;
	PrFlow_t* flow;
	FwPrCounterS1_t i;
	PrBaseDesc_t* prBase = prDesc->prBase;
	FwPrCounterS1_t trueGuardFound;

	/* check if procedure is started */
	if (prDesc->curNode == 0)   /* procedure is stopped */
		return;
	else {
		prDesc->prExecCnt++;
		prDesc->nodeExecCnt++;
	}

	if (prDesc->curNode == -1)	/* procedure is at initial node */
		flow = &(prBase->flows[0]);
	else {
		curNode = &(prBase->aNodes[prDesc->curNode - 1]);  /* procedure is at an action node */
		flow = &(prBase->flows[curNode->iFlow]);
	}

	while (prDesc->prGuards[flow->iGuard](prDesc) != 0) {
		/* Target of flow is a final node */
		if (flow->dest == 0) {
			prDesc->curNode = 0;
			return;
		}
		/* Target of flow is an action node */
		if (flow->dest > 0) {
			prDesc->curNode = flow->dest;
			prDesc->nodeExecCnt = 0;
			curNode = &(prBase->aNodes[(prDesc->curNode)-1]);
			prDesc->prActions[curNode->iAction](prDesc);
			flow = &(prBase->flows[curNode->iFlow]);
		} else {	/* Target of flow is a decision node */
			trueGuardFound = 0;
			decNode = &(prBase->dNodes[(-flow->dest)-1]);
			for (i=0; i<decNode->nOfOutTrans; i++) {
				flow = &(prBase->flows[decNode->outFlowIndex + i]);
				if (prDesc->prGuards[flow->iGuard](prDesc) != 0) {
					trueGuardFound = 1;
					break;
				}
			}
			if (trueGuardFound == 0) {
				prDesc->errCode = prFlowErr;
				return;
			}
		}
	}
	return;
}

/* ----------------------------------------------------------------------------------------------------------------- */
void FwPrRun(FwPrDesc_t prDesc) {
	FwPrStart(prDesc);
	FwPrExecute(prDesc);
	FwPrStop(prDesc);
	return;
}

/* ----------------------------------------------------------------------------------------------------------------- */
FwPrCounterS1_t FwPrGetCurNode(FwPrDesc_t prDesc) {
	return prDesc->curNode;
}

/* ----------------------------------------------------------------------------------------------------------------- */
FwPrBool_t FwPrIsStarted(FwPrDesc_t prDesc) {
	if (prDesc->curNode == 0)
		return 0;
	else
		return 1;
}

/* ----------------------------------------------------------------------------------------------------------------- */
FwPrErrCode_t FwPrGetErrCode(FwPrDesc_t prDesc) {
	return prDesc->errCode;
}

/* ----------------------------------------------------------------------------------------------------------------- */
FwPrCounterU3_t FwPrGetExecCnt(FwPrDesc_t prDesc) {
	return prDesc->prExecCnt;
}

/* ----------------------------------------------------------------------------------------------------------------- */
FwPrCounterU3_t FwPrGetNodeExecCnt(FwPrDesc_t prDesc) {
	return prDesc->nodeExecCnt;
}
