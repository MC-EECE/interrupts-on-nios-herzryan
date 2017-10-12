#include "alt_types.h"
#include "io.h"
#include "system.h"
#include "seven_seg_hex.h"
#include "seven_seg_bcd.h"

extern volatile alt_u8 lead_zero;
extern volatile alt_u8 display_as_bcd;

void toggleBCDHEX() {
	display_as_bcd = !display_as_bcd;
}
void HexDisplay(alt_u32 * base, alt_u16 num) {
	alt_u8 lead_zero = 0x01&IORD(SLIDER_SWITCHES_BASE, 0);
	if (display_as_bcd) {
		seven_seg_bcd(base, num, lead_zero);  //Displays BCD
	} else {
		seven_seg_hex(base, num, lead_zero);  //Displays HEX
	}
}
