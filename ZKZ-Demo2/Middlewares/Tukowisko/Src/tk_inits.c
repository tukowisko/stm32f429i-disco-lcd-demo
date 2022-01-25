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

#include "stm32f4xx.h"
#include "tk_inits.h"


#define ILI9341_CS_SET				HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET)
#define ILI9341_CS_RESET			HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET)
#define ILI9341_WRX_SET				HAL_GPIO_WritePin(ILI9341_WRX_PORT, ILI9341_WRX_PIN, GPIO_PIN_SET)
#define ILI9341_WRX_RESET			HAL_GPIO_WritePin(ILI9341_WRX_PORT, ILI9341_WRX_PIN, GPIO_PIN_RESET)

#define	TK_ILI9341_D0(c)						TK_ILI9341_Cmd(c, NULL, 0);
#define	TK_ILI9341_D1(c, d1)					tmp[0] = d1; TK_ILI9341_Cmd(c, (uint8_t *)&tmp, 1);
#define	TK_ILI9341_D2(c, d1, d2)				tmp[0] = d1; tmp[1] = d2; TK_ILI9341_Cmd(c, (uint8_t *)&tmp, 2);
#define	TK_ILI9341_D3(c, d1, d2, d3)			tmp[0] = d1; tmp[1] = d2; tmp[2] = d3; TK_ILI9341_Cmd(c, (uint8_t *)&tmp, 3);
#define	TK_ILI9341_D4(c, d1, d2, d3, d4) 		tmp[0] = d1; tmp[1] = d2; tmp[2] = d3; tmp[3] = d4; TK_ILI9341_Cmd(c, (uint8_t *)&tmp, 4);
#define	TK_ILI9341_D5(c, d1, d2, d3, d4, d5)	tmp[0] = d1; tmp[1] = d2; tmp[2] = d3; tmp[3] = d4; tmp[4] = d5; TK_ILI9341_Cmd(c, (uint8_t *)&tmp, 5);


uint8_t TK_ILI9341_Cmd(uint8_t cmd, uint8_t *data, int dlen) {

	ILI9341_WRX_RESET;
	ILI9341_CS_RESET;
	if (HAL_SPI_Transmit(ILI9341_HSPI, &cmd, 1, 1000) != HAL_OK) {
		ILI9341_CS_SET;
		return HAL_ERROR;
	};
	ILI9341_CS_SET;


	// there are no data to send?
	if ((dlen == 0) || (data == NULL))
		return HAL_OK;

	ILI9341_WRX_SET;
	ILI9341_CS_RESET;
	if (HAL_SPI_Transmit(ILI9341_HSPI, data, dlen, 1000) != HAL_OK) {
		ILI9341_CS_SET;
		return HAL_ERROR;
	};
	ILI9341_CS_SET;
	return HAL_OK;
}

/*
 *
 * Place TK_ILI9341_Init function after MX_SPI5_Init_function.
 *
 */
void TK_ILI9341_Init() {
	uint8_t tmp[32];

    GPIO_InitTypeDef GPIO_InitStruct = {0};


	/* Init WRX pin */
    GPIO_InitStruct.Pin = ILI9341_WRX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(ILI9341_WRX_PORT, &GPIO_InitStruct);

	/* Init CS pin */
    GPIO_InitStruct.Pin = ILI9341_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(ILI9341_CS_PORT, &GPIO_InitStruct);


    ILI9341_CS_SET;
	HAL_Delay(10);

	/* software reset */
	TK_ILI9341_D0(ILI9341_RESET);
	HAL_Delay(10);


	TK_ILI9341_D3(0xCA, 0xC3, 0x08, 0x50);
    TK_ILI9341_D3(ILI9341_POWERB, 0x00, 0xC1, 0x30);
    TK_ILI9341_D4(ILI9341_POWER_SEQ, 0x64, 0x03, 0x12, 0x81);
    TK_ILI9341_D3(ILI9341_DTCA,0x85,0x00,0x78);
    TK_ILI9341_D5(ILI9341_POWERA,0x39,0x2C,0x00,0x34,0x02);
    TK_ILI9341_D1(ILI9341_PRC,0x20);
    TK_ILI9341_D2(ILI9341_DTCB,0x00,0x00);
    TK_ILI9341_D2(ILI9341_FRC,0x00,0x1B);
    TK_ILI9341_D2(ILI9341_DFC,0x0A,0xA2);
    TK_ILI9341_D1(ILI9341_POWER1,0x10);
    TK_ILI9341_D1(ILI9341_POWER2,0x10);
    TK_ILI9341_D2(ILI9341_VCOM1,0x45,0x15);
    TK_ILI9341_D1(ILI9341_VCOM2,0x90);
    TK_ILI9341_D1(ILI9341_MAC,0xC8);
    TK_ILI9341_D1(ILI9341_3GAMMA_EN,0x00);
    TK_ILI9341_D1(ILI9341_RGB_INTERFACE,0xC2);
    TK_ILI9341_D4(ILI9341_DFC,0x0A,0xA7,0x27,0x04);
    TK_ILI9341_D4(ILI9341_COLUMN_ADDR,0x00,0x00,0x00,0xEF);
    TK_ILI9341_D4(ILI9341_PAGE_ADDR,0x00,0x00,0x01,0x3F);
    TK_ILI9341_D3(ILI9341_INTERFACE,0x01,0x00,0x06);
    TK_ILI9341_D0(ILI9341_GRAM);
    HAL_Delay(200);

    TK_ILI9341_D1(ILI9341_GAMMA,0x01);

    /* pgamma */
    uint8_t buf1[15] = { 0x0F,0x29,0x24,0x0C,0x0E,0x09,0x4E,0x78,0x3C,0x09,0x13,0x05,0x17,0x11,0x00 };
    TK_ILI9341_Cmd(ILI9341_PGAMMA, (uint8_t *)&buf1, 15);

    /* ngamma */
    uint8_t buf2[15] = { 0x00,0x16,0x1B,0x04,0x11,0x07,0x31,0x33,0x42,0x05,0x0C,0x0A,0x28,0x2F,0x0F };
    TK_ILI9341_Cmd(ILI9341_NGAMMA, (uint8_t *)&buf2, 15);


    TK_ILI9341_D0(ILI9341_SLEEP_OUT);
    HAL_Delay(200);

    TK_ILI9341_D0(ILI9341_DISPLAY_ON);


    /* GRAM start writing */
    TK_ILI9341_D0(ILI9341_GRAM);
}


/*
 *
 * Place TK_DIMM_InitSequence function at the bottom of MX_FMC_Init function.
 * It is must be called before MX_LTDC_Init function.
 *
 */
void TK_DIMM_InitSequence() {
	FMC_SDRAM_CommandTypeDef fmc_cmd;

    while(HAL_IS_BIT_SET(FMC_SDRAM_DEVICE->SDSR, FMC_SDSR_BUSY)) {};


	/* Configure a clock configuration enable command */
	fmc_cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	fmc_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	fmc_cmd.AutoRefreshNumber = 1;
	fmc_cmd.ModeRegisterDefinition = 0;
	FMC_SDRAM_SendCommand(FMC_SDRAM_DEVICE, &fmc_cmd, HAL_MAX_DELAY);

	HAL_Delay(100);

	/* Configure a PALL (precharge all) command */
	fmc_cmd.CommandMode = FMC_SDRAM_CMD_PALL;
	fmc_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	fmc_cmd.AutoRefreshNumber = 1;
	fmc_cmd.ModeRegisterDefinition = 0;
	FMC_SDRAM_SendCommand(FMC_SDRAM_DEVICE, &fmc_cmd, HAL_MAX_DELAY);

	/* Configure a Auto-Refresh command */
	fmc_cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	fmc_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	fmc_cmd.AutoRefreshNumber = 4;
	fmc_cmd.ModeRegisterDefinition = 0;
	FMC_SDRAM_SendCommand(FMC_SDRAM_DEVICE, &fmc_cmd, HAL_MAX_DELAY);
	/* Send the second command */
	FMC_SDRAM_SendCommand(FMC_SDRAM_DEVICE, &fmc_cmd, HAL_MAX_DELAY);


	/* Configure a load Mode register command*/
	fmc_cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	fmc_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
	fmc_cmd.AutoRefreshNumber = 1;
	/* SDRAM_MODEREG_BURST_LENGTH_2 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3 */
	/* SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE */
	fmc_cmd.ModeRegisterDefinition = 0x0231;
	FMC_SDRAM_SendCommand(FMC_SDRAM_DEVICE, &fmc_cmd, HAL_MAX_DELAY);

	/* Set the refresh rate counter */
	/* 15.52us * 90Mhz(HCLK) - 20 = 1385,8 */
	//FMC_SDRAM_ProgramRefreshRate(FMC_SDRAM_DEVICE, 1386);

	/* 15.52us * 84Mhz(HCLK) - 20 = 1292,08 */
	FMC_SDRAM_ProgramRefreshRate(FMC_SDRAM_DEVICE, 1292);
}


