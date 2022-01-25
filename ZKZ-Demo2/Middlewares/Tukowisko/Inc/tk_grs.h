/*
 * MIT License
 *
 * Copyright (c) 2022 Artur Zalewski
 *
 * This software is licensed under below terms.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TUKOWISKO_INC_TK_GRS_H_
#define TUKOWISKO_INC_TK_GRS_H_

#include "ssd1306_fonts.h"



#define	VIDEOMEM_ADDR_LAYER1		(uint32_t)0xD0000000
#define	VIDEOMEM_ADDR_LAYER2		(uint32_t)0xD0050000

#define VIDEODISP_WIDTH				240
#define VIDEODISP_HEIGHT			320

#define	VIDEOMEM_PIXEL_SIZE			2
#define	VIDEOMEM_LINE_SIZE			VIDEODISP_WIDTH*VIDEOMEM_PIXEL_SIZE


#define	CONSOLE_FONT				(&Font_7x10)
#define CONSOLE_COLUMNS				34
#define	CONSOLE_LINES				17
#define	CONSOLE_XPOS				0
#define	CONSOLE_YPOS				150


void tk_grs_clear();
void tk_grs_testscreen();
void tk_grs_fillrect(int xpos, int ypos, int width, int height, uint32_t color_argb);
void tk_printf(char *fmt, ...);

#endif /* TUKOWISKO_INC_TK_GRS_H_ */
