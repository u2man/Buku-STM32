/*
 * lcd.c
 *
 *  Created on: Nov 5, 2019
 *      Author: Kang Usman
 */


#include "global.h"

#define POLY_X(Z)              ((int32_t)((Points + (Z))->X))
#define POLY_Y(Z)              ((int32_t)((Points + (Z))->Y))

#define ABS(X)  ((X) > 0 ? (X) : -(X))

LCD_DrawPropTypeDef DrawProp;
static LCD_DrvTypeDef  *lcd_drv;

static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
static void SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3);

uint8_t  BSP_LCD_Init(void){
	  uint8_t ret = LCD_ERROR;

		//Turn On LCD Backlight
		HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);

		//LCD Reset
		HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
		//HAL_Delay(2);
		HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
		HAL_Delay(2);
		HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);

		HAL_Delay(10);

	  /* Default value for draw propriety */
	  DrawProp.BackColor = 0xFFFF;
	  DrawProp.pFont     = &Font24;
	  DrawProp.TextColor = 0x0000;

	  printf("LCD ID %d\r\n",ssd1289_drv.ReadID());

	  if(ssd1289_drv.ReadID() == SSD1289_ID)
	  {
	    lcd_drv = &ssd1289_drv;

	    /* LCD Init */
	    lcd_drv->Init();

	    /* Initialize the font */
	    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

	    ret = LCD_OK;

	  }

	  return ret;

}

uint32_t BSP_LCD_GetXSize(void){
	return(lcd_drv->GetLcdPixelWidth());
}

uint32_t BSP_LCD_GetYSize(void){
	return(lcd_drv->GetLcdPixelHeight());
}

uint16_t BSP_LCD_GetTextColor(void){
	return DrawProp.TextColor;
}

uint16_t BSP_LCD_GetBackColor(void){
	return DrawProp.BackColor;
}

void BSP_LCD_SetTextColor(__IO uint16_t Color){
	DrawProp.TextColor = Color;
}

void BSP_LCD_SetBackColor(__IO uint16_t Color){
	DrawProp.BackColor = Color;
}

void BSP_LCD_SetFont(sFONT *fonts){
	DrawProp.pFont = fonts;
}

sFONT *BSP_LCD_GetFont(void){
	return DrawProp.pFont;
}

void  BSP_LCD_Clear(uint16_t Color){
	  uint32_t counter = 0;
	  uint32_t color_backup = DrawProp.TextColor;
	  DrawProp.TextColor = Color;

	  for(counter = 0; counter < BSP_LCD_GetYSize(); counter++)
	  {
	    BSP_LCD_DrawHLine(0, counter, BSP_LCD_GetXSize());
	  }
	  DrawProp.TextColor = color_backup;
	  BSP_LCD_SetTextColor(DrawProp.TextColor);

}

void BSP_LCD_ClearStringLine(uint16_t Line){
	  uint32_t color_backup = DrawProp.TextColor;
	  DrawProp.TextColor = DrawProp.BackColor;;

	  /* Draw a rectangle with background color */
	  BSP_LCD_FillRect(0, (Line * DrawProp.pFont->Height), BSP_LCD_GetXSize(), DrawProp.pFont->Height);

	  DrawProp.TextColor = color_backup;
	  BSP_LCD_SetTextColor(DrawProp.TextColor);

}

void BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr){
	BSP_LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}

void BSP_LCD_DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Line_ModeTypdef Mode){
	  uint16_t refcolumn = 1, i = 0;
	  uint32_t size = 0, xsize = 0;
	  uint8_t  *ptr = Text;

	  /* Get the text size */
	  while (*ptr++) size ++ ;

	  /* Characters number per line */
	  xsize = (BSP_LCD_GetXSize()/DrawProp.pFont->Width);

	  switch (Mode)
	  {
	  case CENTER_MODE:
	    {
	      refcolumn = Xpos + ((xsize - size)* DrawProp.pFont->Width) / 2;
	      break;
	    }
	  case LEFT_MODE:
	    {
	      refcolumn = Xpos;
	      break;
	    }
	  case RIGHT_MODE:
	    {
	      refcolumn =  - Xpos + ((xsize - size)*DrawProp.pFont->Width);
	      break;
	    }
	  default:
	    {
	      refcolumn = Xpos;
	      break;
	    }
	  }

	  /* Send the string character by character on lCD */
	  while ((*Text != 0) & (((BSP_LCD_GetXSize() - (i*DrawProp.pFont->Width)) & 0xFFFF) >= DrawProp.pFont->Width))
	  {
	    /* Display one character on LCD */
	    BSP_LCD_DisplayChar(refcolumn, Ypos, *Text);
	    /* Decrement the column position by 16 */
	    refcolumn += DrawProp.pFont->Width;
	    /* Point on the next character */
	    Text++;
	    i++;
	  }

}

void BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii){
  DrawChar(Xpos, Ypos, &DrawProp.pFont->table[(Ascii-' ') *
  DrawProp.pFont->Height * ((DrawProp.pFont->Width + 7) / 8)]);

}

uint16_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos){
	  uint16_t ret = 0;

	  if(lcd_drv->ReadPixel != NULL)
	  {
	    ret = lcd_drv->ReadPixel(Xpos, Ypos);
	  }

	  return ret;

}

void BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code){
	  if(lcd_drv->WritePixel != NULL)
	  {
	    lcd_drv->WritePixel(Xpos, Ypos, RGB_Code);
	  }

}

void BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length){
	  uint32_t index = 0;

	  if(lcd_drv->DrawHLine != NULL)
	  {
	    lcd_drv->DrawHLine(DrawProp.TextColor, Xpos, Ypos, Length);

	  }
	  else
	  {
		for(index = 0; index < Length; index++)
	    {
	      BSP_LCD_DrawPixel((Xpos + index), Ypos, DrawProp.TextColor);
	    }
	  }

}

void BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length){
	  uint32_t index = 0;

	  if(lcd_drv->DrawVLine != NULL)
	  {
	    lcd_drv->DrawVLine(DrawProp.TextColor, Xpos, Ypos, Length);
	  }
	  else
	  {
	    for(index = 0; index < Length; index++)
	    {
	      BSP_LCD_DrawPixel(Xpos, Ypos + index, DrawProp.TextColor);
	    }
	  }

}

void BSP_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
	  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
	  curpixel = 0;

	  deltax = ABS(x2 - x1);        /* The difference between the x's */
	  deltay = ABS(y2 - y1);        /* The difference between the y's */
	  x = x1;                       /* Start x off at the first pixel */
	  y = y1;                       /* Start y off at the first pixel */

	  if (x2 >= x1)                 /* The x-values are increasing */
	  {
	    xinc1 = 1;
	    xinc2 = 1;
	  }
	  else                          /* The x-values are decreasing */
	  {
	    xinc1 = -1;
	    xinc2 = -1;
	  }

	  if (y2 >= y1)                 /* The y-values are increasing */
	  {
	    yinc1 = 1;
	    yinc2 = 1;
	  }
	  else                          /* The y-values are decreasing */
	  {
	    yinc1 = -1;
	    yinc2 = -1;
	  }

	  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
	  {
	    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
	    yinc2 = 0;                  /* Don't change the y for every iteration */
	    den = deltax;
	    num = deltax / 2;
	    numadd = deltay;
	    numpixels = deltax;         /* There are more x-values than y-values */
	  }
	  else                          /* There is at least one y-value for every x-value */
	  {
	    xinc2 = 0;                  /* Don't change the x for every iteration */
	    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
	    den = deltay;
	    num = deltay / 2;
	    numadd = deltax;
	    numpixels = deltay;         /* There are more y-values than x-values */
	  }

	  for (curpixel = 0; curpixel <= numpixels; curpixel++)
	  {
	    BSP_LCD_DrawPixel(x, y, DrawProp.TextColor);  /* Draw the current pixel */
	    num += numadd;                            /* Increase the numerator by the top of the fraction */
	    if (num >= den)                           /* Check if numerator >= denominator */
	    {
	      num -= den;                             /* Calculate the new numerator value */
	      x += xinc1;                             /* Change the x as appropriate */
	      y += yinc1;                             /* Change the y as appropriate */
	    }
	    x += xinc2;                               /* Change the x as appropriate */
	    y += yinc2;                               /* Change the y as appropriate */
	  }

}

void BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height){
	  /* Draw horizontal lines */
	  BSP_LCD_DrawHLine(Xpos, Ypos, Width);
	  BSP_LCD_DrawHLine(Xpos, (Ypos+ Height), Width);

	  /* Draw vertical lines */
	  BSP_LCD_DrawVLine(Xpos, Ypos, Height);
	  BSP_LCD_DrawVLine((Xpos + Width), Ypos, Height);

}

void BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius){
	  int32_t  D;       /* Decision Variable */
	  uint32_t  CurX;   /* Current X Value */
	  uint32_t  CurY;   /* Current Y Value */

	  D = 3 - (Radius << 1);
	  CurX = 0;
	  CurY = Radius;

	  while (CurX <= CurY)
	  {
	    BSP_LCD_DrawPixel((Xpos + CurX), (Ypos - CurY), DrawProp.TextColor);

	    BSP_LCD_DrawPixel((Xpos - CurX), (Ypos - CurY), DrawProp.TextColor);

	    BSP_LCD_DrawPixel((Xpos + CurY), (Ypos - CurX), DrawProp.TextColor);

	    BSP_LCD_DrawPixel((Xpos - CurY), (Ypos - CurX), DrawProp.TextColor);

	    BSP_LCD_DrawPixel((Xpos + CurX), (Ypos + CurY), DrawProp.TextColor);

	    BSP_LCD_DrawPixel((Xpos - CurX), (Ypos + CurY), DrawProp.TextColor);

	    BSP_LCD_DrawPixel((Xpos + CurY), (Ypos + CurX), DrawProp.TextColor);

	    BSP_LCD_DrawPixel((Xpos - CurY), (Ypos + CurX), DrawProp.TextColor);

	    /* Initialize the font */
	    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

	    if (D < 0)
	    {
	      D += (CurX << 2) + 6;
	    }
	    else
	    {
	      D += ((CurX - CurY) << 2) + 10;
	      CurY--;
	    }
	    CurX++;
	  }

}

void BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount){
	  int16_t X = 0, Y = 0;

	  if(PointCount < 2)
	  {
	    return;
	  }

	  BSP_LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);

	  while(--PointCount)
	  {
	    X = Points->X;
	    Y = Points->Y;
	    Points++;
	    BSP_LCD_DrawLine(X, Y, Points->X, Points->Y);
	  }


}

void BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius){
	  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
	  float K = 0, rad1 = 0, rad2 = 0;

	  rad1 = XRadius;
	  rad2 = YRadius;

	  K = (float)(rad2/rad1);

	  do {
	    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
	    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos+y), DrawProp.TextColor);
	    BSP_LCD_DrawPixel((Xpos+(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);
	    BSP_LCD_DrawPixel((Xpos-(uint16_t)(x/K)), (Ypos-y), DrawProp.TextColor);

	    e2 = err;
	    if (e2 <= x) {
	      err += ++x*2+1;
	      if (-y == x && e2 <= y) e2 = 0;
	    }
	    if (e2 > y) err += ++y*2+1;
	  }
	  while (y <= 0);
}

void BSP_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp){
	  uint32_t height = 0;
	  uint32_t width  = 0;

	  /* Read bitmap width */
	  width = pbmp[18] + (pbmp[19] << 8) + (pbmp[20] << 16)  + (pbmp[21] << 24);

	  /* Read bitmap height */
	  height = pbmp[22] + (pbmp[23] << 8) + (pbmp[24] << 16)  + (pbmp[25] << 24);

	  SetDisplayWindow(Xpos, Ypos, width, height);

	  if(lcd_drv->DrawBitmap != NULL)
	  {
	    lcd_drv->DrawBitmap(Xpos, Ypos, pbmp);
	  }
	  SetDisplayWindow(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

}

void BSP_LCD_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pbmp){
	  SetDisplayWindow(Xpos, Ypos, Xsize, Ysize);

	  if(lcd_drv->DrawRGBImage != NULL)
	  {
	    lcd_drv->DrawRGBImage(Xpos, Ypos, Xsize, Ysize, pbmp);
	  }
	  SetDisplayWindow(0, 0, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

}

void BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height){
	  BSP_LCD_SetTextColor(DrawProp.TextColor);
	  do
	  {
	    BSP_LCD_DrawHLine(Xpos, Ypos++, Width);
	  }
	  while(Height--);

}

void BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius){
	  int32_t  D;        /* Decision Variable */
	  uint32_t  CurX;    /* Current X Value */
	  uint32_t  CurY;    /* Current Y Value */

	  D = 3 - (Radius << 1);

	  CurX = 0;
	  CurY = Radius;

	  BSP_LCD_SetTextColor(DrawProp.TextColor);

	  while (CurX <= CurY)
	  {
	    if(CurY > 0)
	    {
	      BSP_LCD_DrawHLine(Xpos - CurY, Ypos + CurX, 2*CurY);
	      BSP_LCD_DrawHLine(Xpos - CurY, Ypos - CurX, 2*CurY);
	    }

	    if(CurX > 0)
	    {
	      BSP_LCD_DrawHLine(Xpos - CurX, Ypos - CurY, 2*CurX);
	      BSP_LCD_DrawHLine(Xpos - CurX, Ypos + CurY, 2*CurX);
	    }
	    if (D < 0)
	    {
	      D += (CurX << 2) + 6;
	    }
	    else
	    {
	      D += ((CurX - CurY) << 2) + 10;
	      CurY--;
	    }
	    CurX++;
	  }

	  BSP_LCD_SetTextColor(DrawProp.TextColor);
	  BSP_LCD_DrawCircle(Xpos, Ypos, Radius);

}

void BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount){
	  int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
	  uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;

	  IMAGE_LEFT = IMAGE_RIGHT = Points->X;
	  IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

	  for(counter = 1; counter < PointCount; counter++)
	  {
	    pixelX = POLY_X(counter);
	    if(pixelX < IMAGE_LEFT)
	    {
	      IMAGE_LEFT = pixelX;
	    }
	    if(pixelX > IMAGE_RIGHT)
	    {
	      IMAGE_RIGHT = pixelX;
	    }

	    pixelY = POLY_Y(counter);
	    if(pixelY < IMAGE_TOP)
	    {
	      IMAGE_TOP = pixelY;
	    }
	    if(pixelY > IMAGE_BOTTOM)
	    {
	      IMAGE_BOTTOM = pixelY;
	    }
	  }

	  if(PointCount < 2)
	  {
	    return;
	  }

	  X_center = (IMAGE_LEFT + IMAGE_RIGHT)/2;
	  Y_center = (IMAGE_BOTTOM + IMAGE_TOP)/2;

	  X_first = Points->X;
	  Y_first = Points->Y;

	  while(--PointCount)
	  {
	    X = Points->X;
	    Y = Points->Y;
	    Points++;
	    X2 = Points->X;
	    Y2 = Points->Y;

	    FillTriangle(X, X2, X_center, Y, Y2, Y_center);
	    FillTriangle(X, X_center, X2, Y, Y_center, Y2);
	    FillTriangle(X_center, X2, X, Y_center, Y2, Y);
	  }

	  FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
	  FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
	  FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);

}

void BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius){
	  int x = 0, y = -YRadius, err = 2-2*XRadius, e2;
	  float K = 0, rad1 = 0, rad2 = 0;

	  rad1 = XRadius;
	  rad2 = YRadius;

	  K = (float)(rad2/rad1);

	  do
	  {
	    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1));
	    BSP_LCD_DrawHLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1));

	    e2 = err;
	    if (e2 <= x)
	    {
	      err += ++x*2+1;
	      if (-y == x && e2 <= y) e2 = 0;
	    }
	    if (e2 > y) err += ++y*2+1;
	  }
	  while (y <= 0);

}

void BSP_LCD_DisplayOff(void){
	lcd_drv->DisplayOn();
}

void BSP_LCD_DisplayOn(void){
	lcd_drv->DisplayOff();
}

static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c){
	  uint32_t i = 0, j = 0;
	  uint16_t height, width;
	  uint8_t offset;
	  uint8_t *pchar;
	  uint32_t line;

	  height = DrawProp.pFont->Height;
	  width  = DrawProp.pFont->Width;

	  offset =  8 *((width + 7)/8) -  width ;

	  for(i = 0; i < height; i++)
	  {
	    pchar = ((uint8_t *)c + (width + 7)/8 * i);

	    switch(((width + 7)/8))
	    {
	    case 1:
	      line =  pchar[0];
	      break;

	    case 2:
	      line =  (pchar[0]<< 8) | pchar[1];
	      break;

	    case 3:
	    default:
	      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
	      break;
	    }

	    for (j = 0; j < width; j++)
	    {
	      if(line & (1 << (width- j + offset- 1)))
	      {
	        BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp.TextColor);
	      }
	      else
	      {
	        BSP_LCD_DrawPixel((Xpos + j), Ypos, DrawProp.BackColor);
	      }
	    }
	    Ypos++;
	  }

}

static void SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height){
	 if(lcd_drv->SetDisplayWindow != NULL)
	  {
	    lcd_drv->SetDisplayWindow(Xpos, Ypos, Width, Height);
	  }

}

static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3){
	  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
	  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
	  curpixel = 0;

	  deltax = ABS(x2 - x1);        /* The difference between the x's */
	  deltay = ABS(y2 - y1);        /* The difference between the y's */
	  x = x1;                       /* Start x off at the first pixel */
	  y = y1;                       /* Start y off at the first pixel */

	  if (x2 >= x1)                 /* The x-values are increasing */
	  {
	    xinc1 = 1;
	    xinc2 = 1;
	  }
	  else                          /* The x-values are decreasing */
	  {
	    xinc1 = -1;
	    xinc2 = -1;
	  }

	  if (y2 >= y1)                 /* The y-values are increasing */
	  {
	    yinc1 = 1;
	    yinc2 = 1;
	  }
	  else                          /* The y-values are decreasing */
	  {
	    yinc1 = -1;
	    yinc2 = -1;
	  }

	  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
	  {
	    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
	    yinc2 = 0;                  /* Don't change the y for every iteration */
	    den = deltax;
	    num = deltax / 2;
	    numadd = deltay;
	    numpixels = deltax;         /* There are more x-values than y-values */
	  }
	  else                          /* There is at least one y-value for every x-value */
	  {
	    xinc2 = 0;                  /* Don't change the x for every iteration */
	    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
	    den = deltay;
	    num = deltay / 2;
	    numadd = deltax;
	    numpixels = deltay;         /* There are more y-values than x-values */
	  }

	  for (curpixel = 0; curpixel <= numpixels; curpixel++)
	  {
	    BSP_LCD_DrawLine(x, y, x3, y3);

	    num += numadd;              /* Increase the numerator by the top of the fraction */
	    if (num >= den)             /* Check if numerator >= denominator */
	    {
	      num -= den;               /* Calculate the new numerator value */
	      x += xinc1;               /* Change the x as appropriate */
	      y += yinc1;               /* Change the y as appropriate */
	    }
	    x += xinc2;                 /* Change the x as appropriate */
	    y += yinc2;                 /* Change the y as appropriate */
	  }

}

