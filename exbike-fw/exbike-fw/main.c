#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "usbdrv.h"
#include "oddebug.h"


PROGMEM const char usbHidReportDescriptor[26] = {
	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
	0x09, 0x05,                    // USAGE (Game Pad)
	0xa1, 0x01,                    // COLLECTION (Application)
	0xa1, 0x00,                    //   COLLECTION (Physical)
	0x05, 0x09,                    //     USAGE_PAGE (Button)
	0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
	0x29, 0x01,                    //     USAGE_MAXIMUM (Button 1)
	0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
	0x25, 0xFF,                    //     LOGICAL_MAXIMUM (255)
	0x75, 0x08,                    //     REPORT_SIZE (8)
	0x95, 0x01,                    //     REPORT_COUNT (1)
	0x81, 0x02,                    //     INPUT (Data,Var,Abs)
	0xc0,                          //     END_COLLECTION
	0xc0                           // END_COLLECTION
};

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    return 0;   /* default for not implemented requests: return no data back to host */
}

int main(void)
{
	uint8_t reportBuffer[1] = {45};
	uint8_t inputState = 0;

    wdt_enable(WDTO_1S);
	
    DDRA &= ~(1 << 7); // Set PA7 as input
    PORTA |= (1 << 7); // Enable internal pull-up resistor on PA7
	
    usbInit();
    sei();
    for(;;){ 
        wdt_reset();
        usbPoll();
		if (!(PINA & (1 << 7)) && inputState == 0) { //PA7 low
			inputState = 1;
			usbSetInterrupt(reportBuffer, 1);
		}
		else if((PINA & (1 << 7))) { //PA7 high
			inputState = 0;
		}
		_delay_ms(25);
    }
}