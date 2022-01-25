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


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx.h"
#include "tk_grs.h"
#include "ssd1306_fonts.h"

extern DMA2D_HandleTypeDef hdma2d;


char	tk_console_data[CONSOLE_COLUMNS*CONSOLE_LINES];
uint16_t	tk_console_pos = 0;
uint16_t	tk_console_actual_line = 0;
uint16_t	tk_console_actual_column = 0;
uint8_t		tk_console_newline = 0;



void tk_drawchars(int xpos, int ypos, FontDef *font, uint16_t color, uint16_t bgcolor, char *str) {
	uint8_t ch;
	uint16_t c;

	uint32_t inc1 = VIDEODISP_WIDTH - font->FontWidth;
	uint32_t inc2 = font->FontWidth - font->FontHeight*VIDEODISP_WIDTH;

	__IO uint16_t *vmem = (uint16_t *)(VIDEOMEM_ADDR_LAYER1 + ypos * VIDEOMEM_LINE_SIZE + xpos * VIDEOMEM_PIXEL_SIZE);

	while ((ch = *str++) != 0) {
	    for(uint16_t i = 0; i < font->FontHeight; i++) {
	    	if ((ch < ' ') || (ch > '~'))
	    		ch = ' ';
	        uint16_t b = font->data[(ch - 32) * font->FontHeight + i];
	        for(uint16_t j = 0; j < font->FontWidth; j++) {
	            if((b << j) & 0x8000)  {
	            	c = color;
	            } else {
		        	c = bgcolor;
	            }
	            *vmem++ = c;
	        }
	        vmem += inc1;
	    }
	    vmem += inc2;
	};

}


/*
 * color_argb:
 *   8 bits - alpha
 *   8 bits - red
 *   8 bits - green
 *   8 bits - blue
 *
 *
 */
void tk_grs_fillrect(int xpos, int ypos, int width, int height, uint32_t color_argb) {
  DMA2D_HandleTypeDef hdma2d;

  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_R2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
  hdma2d.Init.OutputOffset = VIDEODISP_WIDTH - width;

  HAL_DMA2D_Init(&hdma2d);
  HAL_DMA2D_Start_IT(&hdma2d, color_argb, VIDEOMEM_ADDR_LAYER1 + ypos * VIDEOMEM_LINE_SIZE + xpos * VIDEOMEM_PIXEL_SIZE, width, height);
  HAL_DMA2D_PollForTransfer(&hdma2d, 100);
}


void tk_console_next_line() {
	int x;

	for (x = tk_console_actual_column; x < CONSOLE_COLUMNS; x++)
		tk_console_data[tk_console_pos++] = ' ';

	tk_console_actual_column = 0;

	if (tk_console_actual_line < CONSOLE_LINES - 1) {
		tk_console_actual_line++;
		return;
	};

	//scroll
	tk_console_pos -= CONSOLE_COLUMNS;
	memset((uint8_t *)&tk_console_data[tk_console_pos], 0, CONSOLE_COLUMNS);

	__IO uint16_t *vmem_dst = (uint16_t *)(VIDEOMEM_ADDR_LAYER1 + CONSOLE_YPOS * VIDEOMEM_LINE_SIZE + CONSOLE_XPOS * VIDEOMEM_PIXEL_SIZE);
	__IO uint16_t *vmem_src = (uint16_t *)(VIDEOMEM_ADDR_LAYER1 + (CONSOLE_YPOS + CONSOLE_FONT->FontHeight) * VIDEOMEM_LINE_SIZE + CONSOLE_XPOS * VIDEOMEM_PIXEL_SIZE);

	// todo: DMA
	memmove((uint16_t *)vmem_dst, (uint16_t *)vmem_src, (CONSOLE_LINES - 1) * CONSOLE_FONT->FontHeight * VIDEOMEM_LINE_SIZE);


	// clear last line
	tk_grs_fillrect(CONSOLE_XPOS,		// xpos
			CONSOLE_YPOS + (CONSOLE_LINES - 1) * CONSOLE_FONT->FontHeight,	// ypos
			CONSOLE_COLUMNS*CONSOLE_FONT->FontWidth, // width
			CONSOLE_FONT->FontHeight, // height
			0x0); // color





}

void tk_console_append(char *str) {
	uint8_t ch;
	char buf[2];


	buf[1] = '\0';


	while ((ch = *str++) != 0) {

		if (ch == '\n') {
			// process previous \n request
			if (tk_console_newline == 1)
				tk_console_next_line();
			//
			tk_console_newline = 1;
			continue;
		}

		// do not display control chars
		if (ch < ' ')
			continue;

		if (tk_console_newline != 0) {
			tk_console_newline = 0;
			tk_console_next_line();
		};

		uint16_t xpos = tk_console_actual_column * CONSOLE_FONT->FontWidth + CONSOLE_XPOS;
		uint16_t ypos = tk_console_actual_line * CONSOLE_FONT->FontHeight + CONSOLE_YPOS;
		buf[0] = ch;
		tk_drawchars(xpos, ypos, CONSOLE_FONT, 0xFFFF, 0x0000, (char *)&buf);

		tk_console_data[tk_console_pos++] = ch;
		tk_console_actual_column++;

		if (tk_console_actual_column >= CONSOLE_COLUMNS - 1)
			tk_console_newline = 2;

	};



}



void tk_grs_clear() {
	/* no DMA2D version */
	//for (uint32_t x = 0; x < 240*320; x++)
	//	*(__IO uint16_t*) (VIDEOMEM_ADDR_LAYER1 + x*2) = 0x0;

	/* DMA2D version */
	tk_grs_fillrect(0, 0, VIDEODISP_WIDTH, VIDEODISP_HEIGHT, 0);
}

void tk_grs_testscreen() {

	  /* RGB */
	  for (uint32_t y = 0; y < 150; y++) {

		  uint16_t col = 0b1111100000000000;
		  if (y > 50)
			  col = 0b0000011111100000;
		  if (y > 100)
			  col = 0b0000000000011111;

		  for (uint32_t x = 0; x < 240; x++) {
			  *(__IO uint16_t*) (VIDEOMEM_ADDR_LAYER1 + 2*(y*240+x)) = col;
		  }
	  }
}

void tk_printf(char *fmt, ...) {
	char buf[64];

	buf[sizeof(buf) - 1] = '\0';
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	tk_console_append((char *)&buf);
	va_end(ap);

}

