/* LDROM dumper
 * Copyright (C) 2015 ReservedField
 */

#include <stdio.h>
#include <M451Series.h>
#include <Display.h>
#include <Font.h>
#include <Button.h>
#include <USB_VirtualCOM.h>

int main() {
	uint8_t data[4];
	uint32_t addr;
	char txtBuf[9];

	SYS_UnlockReg();
	USB_VirtualCOM_Init();

	Display_PutText(0, 0, "Open COM\nterminal\nand push\nFIRE.", FONT_DEJAVU_8PT);
	Display_Update();

	while(!(Button_GetState() & BUTTON_MASK_FIRE));

	Display_Clear();
	Display_PutText(0, 0, "Dump...", FONT_DEJAVU_8PT);
	Display_Update();

	FMC_Open();
	for(addr = 0x00100000; addr < 0x00101000; addr += 4) {
		// Preserve endianess
		*((uint32_t *) data) = FMC_Read(addr);
		sprintf(txtBuf, "%02x%02x%02x%02x", data[0], data[1], data[2], data[3]);
		USB_VirtualCOM_SendString(txtBuf);
	}
	FMC_Close();

	Display_Clear();
	Display_PutText(0, 0, "Success.", FONT_DEJAVU_8PT);
	Display_Update();
}
