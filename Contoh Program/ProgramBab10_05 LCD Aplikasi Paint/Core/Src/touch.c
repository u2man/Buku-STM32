/*
 * touch.c
 *
 *  Created on: Nov 21, 2019
 *      Author: Kang Usman
 */


#include "global.h"
Coordinate ScreenSample[3];
Coordinate DisplaySample[3] ={
		{10,10},
		{160, 230},
		{310, 10}
};
Matrix matrix ;

void Draw_BlackCircle(uint16_t Xpos,uint16_t Ypos);
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);

void touch_readPosition(Coordinate * displayPtr){
	Coordinate  *screenPtr;

	screenPtr = ads7843_readXY();
	if (screenPtr->status==TOUCH_RELEASED){
		return;
	}

	if( matrix.Divider != 0 )
	  {
	    /* XD = AX+BY+C */
	    displayPtr->x = ( (matrix.An * screenPtr->x) +
	                      (matrix.Bn * screenPtr->y) +
	                       matrix.Cn
	                    ) / matrix.Divider ;
		/* YD = DX+EY+F */
	    displayPtr->y = ( (matrix.Dn * screenPtr->x) +
	                      (matrix.En * screenPtr->y) +
	                       matrix.Fn
	                    ) / matrix.Divider ;

	    displayPtr->status=screenPtr->status;
	  }
}

void touch_calibration(void){
	uint8_t i;
	Coordinate * Ptr;

	printf("TOuch Panel Calibration\r\n");

	  /* Clear the LCD */
	  BSP_LCD_Clear(LCD_COLOR_WHITE);

	  /* Set Touchscreen Demo description */
	  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

	  BSP_LCD_SetFont(&Font12);
	  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 - 27, (uint8_t*)"Before using the Touchscreen", CENTER_MODE);
	  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 - 12, (uint8_t*)"you need to calibrate it.", CENTER_MODE);
	  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2 + 3, (uint8_t*)"Press on the black circles", CENTER_MODE);

	 for(i=0;i<3;i++){
		 BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		 BSP_LCD_FillCircle(DisplaySample[i].x, DisplaySample[i].y, 2);
		 do{
			 Ptr = ads7843_readXY();
		 }while(Ptr == (void*)0);
		 ScreenSample[i].x= Ptr->x;
		 ScreenSample[i].y= Ptr->y;
		 BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		 BSP_LCD_FillCircle(DisplaySample[i].x, DisplaySample[i].y, 2);
		 HAL_Delay(500);
	 }

	 setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );

}


FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr){
	  FunctionalState retTHRESHOLD = ENABLE ;

	  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
	                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	  if( matrixPtr->Divider == 0 )
	  {
	    retTHRESHOLD = DISABLE;
	  }
	  else
	  {
	    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
	                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
	                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
	    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
	                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
	                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
	    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
	                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
	    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
	                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
	    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
	                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
	                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
	  }
	  return( retTHRESHOLD ) ;

}
