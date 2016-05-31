#include "hwa_ntc.h"
#include "sys_adc.h"

#define OFFSE			(-10)
#define NUM_OF_TEMP		(100)
CONST UINT16 c_u16_ntcTable[NUM_OF_TEMP] = 
{
139+OFFSE,
145+OFFSE,
152+OFFSE,
158+OFFSE,
165+OFFSE,
172+OFFSE,
179+OFFSE,
187+OFFSE,
194+OFFSE,
202+OFFSE,
210+OFFSE,
218+OFFSE,
226+OFFSE,
234+OFFSE,
243+OFFSE,
251+OFFSE,
260+OFFSE,
269+OFFSE,
278+OFFSE,
288+OFFSE,
297+OFFSE,
306+OFFSE,
316+OFFSE,
326+OFFSE,
336+OFFSE,
346+OFFSE,
356+OFFSE,
366+OFFSE,
376+OFFSE,
387+OFFSE,
397+OFFSE,
407+OFFSE,
418+OFFSE,
428+OFFSE,
439+OFFSE,
449+OFFSE,
460+OFFSE,
470+OFFSE,
480+OFFSE,
491+OFFSE,
501+OFFSE,
511+OFFSE,
522+OFFSE,
532+OFFSE,
542+OFFSE,
552+OFFSE,
562+OFFSE,
572+OFFSE,
581+OFFSE,
591+OFFSE,
601+OFFSE,
610+OFFSE,
619+OFFSE,
628+OFFSE,
637+OFFSE,
646+OFFSE,
655+OFFSE,
664+OFFSE,
672+OFFSE,
680+OFFSE,
689+OFFSE,
697+OFFSE,
704+OFFSE,
712+OFFSE,
720+OFFSE,
727+OFFSE,
735+OFFSE,
742+OFFSE,
749+OFFSE,
755+OFFSE,
762+OFFSE,
769+OFFSE,
775+OFFSE,
781+OFFSE,
787+OFFSE,
793+OFFSE,
799+OFFSE,
805+OFFSE,
810+OFFSE,
816+OFFSE,
821+OFFSE,
826+OFFSE,
831+OFFSE,
836+OFFSE,
841+OFFSE,
845+OFFSE,
850+OFFSE,
854+OFFSE,
859+OFFSE,
863+OFFSE,
867+OFFSE,
871+OFFSE,
875+OFFSE,
879+OFFSE,
882+OFFSE,
886+OFFSE,
889+OFFSE,
893+OFFSE,
896+OFFSE,
899+OFFSE,
//100¡æ
};


void hwa_ntcInit(void)
{
	sys_adcInit();
}

#define AD_VALUE_BUFF_LEN		100
UINT32 adValueSum = 0;
static UINT8 adValueIndex = 0;
void hwa_ntcHandler10ms(void)
{
	if(adValueIndex < AD_VALUE_BUFF_LEN)
	{
		adValueSum += sys_adcValue(4);
		adValueIndex++;
	}
}


static UINT8 u8_tempOld = 0;
UINT16 u16_adValue = 0;
void hwa_ntcHandler500ms(void)
{
	UINT16 u16_thresholdH;
	UINT16 u16_thresholdL;
	UINT8 u8_tempNew;
	u16_adValue = (u16_adValue *2 + adValueSum/adValueIndex*3)/5;
	adValueSum = 0;
	adValueIndex = 0;
	for(u8_tempNew=0; u8_tempNew< NUM_OF_TEMP; u8_tempNew++)
	{
		if(u16_adValue < c_u16_ntcTable[u8_tempNew])
		{
			break;
		}
	}
	if(u8_tempNew == 0)									//NTC¿ªÂ·
	{
		u8_tempOld = 0;
	}
	else if(u8_tempNew > c_u16_ntcTable[NUM_OF_TEMP-1])	//NTC¶ÌÂ·
	{
		u8_tempOld =  100;
	}
	else
	{
		if(u8_tempNew > u8_tempOld)
		{
			u16_thresholdH = (c_u16_ntcTable[u8_tempOld+1]-c_u16_ntcTable[u8_tempOld])/4+c_u16_ntcTable[u8_tempOld];
			if(u16_adValue >= u16_thresholdH)
			{
				u8_tempOld = u8_tempNew;
			}
			else
			{
				u8_tempOld = u8_tempNew-1;
			}
		}
		else if(u8_tempNew < u8_tempOld)
		{
			u16_thresholdL = c_u16_ntcTable[u8_tempNew]-(c_u16_ntcTable[u8_tempNew]-c_u16_ntcTable[u8_tempNew-1])/4;
			if(u16_adValue <= u16_thresholdL)
			{
				u8_tempOld = u8_tempNew;
			}
			else
			{
				u8_tempOld = u8_tempNew+1;
			}
		}
	}
}

UINT8 hwa_ntcGetTemp(void)
{
    return u8_tempOld;
}

