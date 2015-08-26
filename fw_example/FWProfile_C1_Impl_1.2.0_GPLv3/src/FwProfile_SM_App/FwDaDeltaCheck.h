/**
 * @file
 * @ingroup daGroup
 * Definition of the <i>Delta Failure Detection (FD)
 * Check</i>.
 * This FD Check is encapsulated in a state machine which is derived
 * from the FD Check State Machine.
 * The Delta Check reports "anomaly detected" if either the
 * current absorbed by the hardware device or its temperature
 * changes by more than a certain threshold from one activation
 * to the next (i.e. the check detects a "jump" in the value of
 * either the current or the temperature of the hardware device).
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

#ifndef FWDADELTACHECK_H_
#define FWDADELTACHECK_H_

/**
 * Maximum nominal change in current absorbed by the Hardware Device.
 * If the current absorbed by the Hardware Device changes from one
 * activation cycle to the next by more than HW_DEV_CUR_MAX_DELTA
 * the Delta FD Check declares an anomaly and if the
 * anomaly persists for longer than HW_DEV_DELTA_CNT_LIMIT
 * cycles, the Hardware Device must be put in Stand-By.
 */
#define HW_DEV_CUR_MAX_DELTA 0.9

/**
 * Maximum nominal change in temperature of the Hardware Device.
 * If the temperature of the Hardware Device changes from one
 * activation cycle to the next by more than HW_DEV_TEMP_MAX_DELTA
 * the Delta FD Check declares an anomaly and if the
 * anomaly persists for longer than HW_DEV_DELTA_CNT_LIMIT
 * cycles, the Hardware Device must be put in Stand-By.
 */
#define HW_DEV_TEMP_MAX_DELTA 80

/**
 * Counter limit for the Delta FD Check.
 * If the Delta FD Check detects an anomaly for longer
 * than HW_DEV_DELTA_CNT_LIMIT cycles, then a failure is
 * declared and the Recovery Action associated to the check
 * is executed.
 */
#define HW_DEV_DELTA_CNT_LIMIT 4

/**
 * Retrieve the descriptor of the Delta FD Check State Machine.
 * The Delta FD Check State Machine is a singleton.
 * The first time this function is called, it creates and configures
 * the state machine descriptor.
 * Subsequently, it always returns the same descriptor.
 * 
 * The Delta FD Check State Machine is built as an extension
 * of the generic FD Check State Machine
 * (see <code>FwDaFailDetCheck.h</code>) with the following changes:
 * - The Default Anomaly Detection Check (function
 *   <code>::DefAnomalyDetCheck</code>) is overridden to
 *   report "anomaly detected" if the current absorbed by the
 *   hardware device or its temperature have changed by more than,
 *   respectively, HW_DEV_CUR_MAX_DELTA and HW_DEV_TEMP_MAX_DELTA.
 * - The Default Recovery Action (function
 *   <code>::DefRecoveryAction</code>) is overridden to
 *   send a command to switch off the Hardware Device.
 * - The counter limit <code>cntLimit</code> is set to
 *   HW_DEV_DELTA_CNT_LIMIT.
 * .
 * Thus, this FD Check monitors the current absorbed by the Hardware
 * Device and its temperature, if it finds that either has jumped by
 * more than a certain threshold for more than HW_DEV_DELTA_CNT_LIMIT
 * consecutive cycles, it commands the Hardware Device into
 * STANDBY mode.
 * @return the descriptor of the FD Check State Machine or NULL
 * if the state machine could not be created.
 */
FwSmDesc_t GetDeltaCheckSm();

#endif /* FWDADELTACHECK_H_ */
