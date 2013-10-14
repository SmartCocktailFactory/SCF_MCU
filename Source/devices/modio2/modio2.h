/*
 * modio2.h
 *
 *  Created on: Oct 14, 2013
 *      Author: Daniel Steiner
 */

#ifndef MODIO2_H_
#define MODIO2_H_

typedef enum { Error = 0, Success = !Error } Status;

Status ModIO2_enableRelay1();
Status ModIO2_disableRelay1();
Status ModIO2_enableRelay2();
Status ModIO2_disableRelay2();

#endif /* MODIO2_H_ */
