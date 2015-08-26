/**
 * @file
 * @ingroup daGroup
 * Definition of the interface to access the <i>Hardware Device</i>.
 * The behaviour of the Hardware Device is modelled by one state machine
 * and two attributes.
 * 
 * The state machine is called the <i>Hardware Device State Machine </i>
 * and is shown in the figure below.
 * The HW Device State Machine is a singleton and this file defines a
 * getter function to access the single instance of its descriptor.
 * 
 * The attributes of the Hardware Device are: <i>temperature</i> and
 * <i>current</i>.
 * This file defines getter functions through which the value of the
 * two attributes can be retrieved.
 * @image html HwDev.png
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

#ifndef FWDAHWDEV_H_
#define FWDAHWDEV_H_

#include "FwProfile/FwSmConstants.h"
#include "FwProfile/FwSmCore.h"

/** Name of the ON state in the HW Device State Machine */
#define HW_DEV_ON 1
/** Name of the OFF state in the HW Device State Machine */
#define HW_DEV_OFF 2
/** Name of the STANDBY state in the HW Device State Machine */
#define HW_DEV_SBY 1
/** Name of the OPERATIONAL state in the HW Device State Machine */
#define HW_DEV_OPER 2

/** Name of the transition from OFF to ON in the HW Device State Machine */
#define TR_HW_DEV_ON 1
/** Name of the transition from ON to OFF in the HW Device State Machine */
#define TR_HW_DEV_OFF 2
/** Name of the transition from STANDBY to OPERATIONAL in the HW Device State Machine */
#define TR_HW_DEV_OPER 3
/** Name of the transition from OPERATIONAL to STANDBY in the HW Device State Machine */
#define TR_HW_DEV_SBY 4

/** The maximum value of the temperature of the Hardware Device */
#define HW_DEV_TEMP_MAX 100

/** The maximum value of the current absorbed by the Hardware Device */
#define HW_DEV_CUR_MAX 1

/**
 * Retrieve the descriptor of the Hardware Device State Machine.
 * The Hardware Device State Machine is a singleton.
 * The first time this function is called, it creates and configures
 * the state machine descriptor.
 * Subsequently, it always returns the same descriptor.
 * @return the descriptor of the HW Device State Machine or NULL
 * if the state machine could not be created.
 */
FwSmDesc_t GetHwDevSm();

/**
 * This function returns the temperature of the Hardware Device.
 * In this demo application, the temperature is modelled as a stochastic
 * variable which is uniformly distributed in the interval:
 * [0, HW_DEV_TEMP_MAX].
 * @return the temperature of the Hardware Device.
 */
float GetHwDevTemp();

/**
 * This function returns the current absorbed by the Hardware Device.
 * In this demo application, the current is modelled as a stochastic
 * variable which is uniformly distributed in the interval:
 * [0, HW_DEV_CUR_MAX].
 * @return the current absorbed by the Hardware Device.
 */
float GetHwDevCur();

#endif /* FWDAHWDEV_H_ */
