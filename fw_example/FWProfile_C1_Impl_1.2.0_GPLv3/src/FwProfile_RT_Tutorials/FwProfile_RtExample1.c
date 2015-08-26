/**
 * RT Container Example for C1 Implementation of the FW Profile.
 * This file contains a complete program to create, configure
 * and use a simple RT Container.
 * The RT Container wraps a thread which, at every notification,
 * writes a message to standard output.
 * The main program sends a sequence of notifications to the RT
 * Container. Each notification should wake up the internal
 * container's thread (the "Activation Thread"). Successive
 * notifications are separated by a 10 ms wait.
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

/* Include FW Profile functions */
#include "FwProfile/FwRtConstants.h"
#include "FwProfile/FwRtConfig.h"
#include "FwProfile/FwRtCore.h"
#include <time.h>

/** This variable holds a time duration of 10 ms */
static struct timespec ten_ms = {0,10000000};

/* -------------------------------------------------------------------------*/
/**
 * Function implementing the user's functional behaviour.
 * In this example, this function prints a message and returns zero.
 * @param rtDesc the RT Container descriptor
 * @return always return zero
 */
FwRtOutcome_t UserFunctionalBehaviour(FwRtDesc_t rtDesc) {
	static int i = 1;
	printf("Activation Thread: Notification %i has been received!\n",i);
	i++;
	return 0;
}

/* -------------------------------------------------------------------------*/
/**
 * Main program for the RT Container Example.
 * This program creates and configures a sample RT Container then sends a few
 * notifications to it. Each notification should wake up the container's thread.
 * @return always returns EXIT_SUCCESS
 */
int main(void) {
	struct FwRtDesc rtDesc;		/* The RT Container Descriptor */
	int i;

	/* Reset the RT Container */
	FwRtReset(&rtDesc);

	/* Attach functional behaviour to RT Container */
	FwRtSetExecFuncBehaviour(&rtDesc,&UserFunctionalBehaviour);

	/* Initialize the RT Container */
	FwRtInit(&rtDesc);

	/* Start the RT Container and send a few notifications to it */
	FwRtStart(&rtDesc);
	for (i=0; i<10; i++) {
		printf("Sending notification %i to container ...\n",i+1);
		FwRtNotify(&rtDesc);
		nanosleep(&ten_ms,NULL);	/* wait ten ms */
	}

	/* Stop the RT Container */
	FwRtStop(&rtDesc);

	/* To ensure orderly shutdown: wait until container thread has terminated */
	FwRtWaitForTermination(&rtDesc);

	/* Shutdown the RT Container */
	FwRtShutdown(&rtDesc);

	return EXIT_SUCCESS;
}
