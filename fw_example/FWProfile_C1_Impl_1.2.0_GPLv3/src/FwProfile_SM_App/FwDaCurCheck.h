/**
 * @file
 * @ingroup daGroup
 * Definition of the <i>Current Failure Detection (FD)
 * Check</i>.
 * This FD Check is encapsulated in a state machine which is derived
 * from the FD Check State Machine.
 * The Current Check reports "anomaly detected" if the
 * current absorbed by the hardware device is above a certain
 * threshold.
 * If the FD Check declares a failure, its recovery action
 * consists in starting the Current Recovery Action Procedure
 * of <code>FwDaCurRecAction.h</code>.
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

#ifndef FWDACURCHECK_H_
#define FWDACURCHECK_H_

/** Identifier of the Current FD Check. */
#define CUR_FD_CHECK_ID 1

/**
 * Maximum nominal current absorbed by the Hardware Device.
 * If the current absorbed by the Hardware Device exceeds this
 * limit, the Current FD Check declares an anomaly and if the
 * anomaly persists for longer than HW_DEV_CUR_CNT_LIMIT
 * cycles, the Hardware Device must be switched off.
 */
#define HW_DEV_CUR_MAX_NOM 0.75

/**
 * Counter limit for the Current FD Check.
 * If the Current FD Check detects an anomaly for longer
 * than HW_DEV_CUR_CNT_LIMIT cycles, then a failure is
 * declared and the Recovery Action associated to the check
 * is executed.
 */
#define HW_DEV_CUR_CNT_LIMIT 2

/**
 * Retrieve the descriptor of the Current FD Check State Machine.
 * The Current FD Check State Machine is a singleton.
 * The first time this function is called, it creates and configures
 * the state machine descriptor.
 * Subsequently, it always returns the same descriptor.
 * 
 * The Current FD Check State Machine is built as an extension
 * of the generic FD Check State Machine
 * (see <code>FwDaFailDetCheck.h</code>) with the following changes:
 * - The Default Anomaly Detection Check (function
 *   <code>::DefAnomalyDetCheck</code>) is overridden to
 *   report "anomaly detected" if the current absorbed by the
 *   hardware device is above HW_DEV_CUR_MAX_NOM.
 * - The Default Recovery Action (function
 *   <code>::DefRecoveryAction</code>) is overridden to
 *   send a switch-off command to the Hardware Device.
 * - The counter limit <code>cntLimit</code> is set to
 *   HW_DEV_CUR_CNT_LIMIT.
 * .
 * Thus, this FD Check monitors the current absorbed by the Hardware
 * Device and, if it finds that it is out-of-limits for more
 * than HW_DEV_CUR_CNT_LIMIT consecutive cycles, it switches
 * off the Hardware Device.
 * @return the descriptor of the FD Check State Machine or NULL
 * if the state machine could not be created.
 */
FwSmDesc_t GetCurCheckSm();

#endif /* FWDACURCHECK_H_ */
