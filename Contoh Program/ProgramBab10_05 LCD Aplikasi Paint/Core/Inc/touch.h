/*
 * touch.h
 *
 *  Created on: Nov 21, 2019
 *      Author: Kang Usman
 */

#ifndef INC_TOUCH_H_
#define INC_TOUCH_H_

typedef struct Matrix
{
long double An,
            Bn,
            Cn,
            Dn,
            En,
            Fn,
            Divider ;
} Matrix ;

void touch_calibration(void);
void touch_readPosition(Coordinate * displayPtr);


#endif /* INC_TOUCH_H_ */
