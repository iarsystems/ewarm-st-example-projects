/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : usb_conf.h
* Author             : MCD Application Team
* Date First Issued  : 27/10/2003
* Description        : USB device configuration file
*
********************************************************************************/


/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/
#define EP_NUM  			(2)

/*-------------------------------------------------------------*/
/* endpoint names */
/*-------------------------------------------------------------*/
#define INT_ENDP            ENDP1  /* interrupt endpoint */

/*-------------------------------------------------------------*/
/* --------------   Buffer Description Table  -----------------*/
/*-------------------------------------------------------------*/
/* buffer table base address */
#define BTABLE_ADDRESS      (0x10)

/* EP0 */
/* rx/tx buffer base address */
#define ENDP0_RXADDR        (0x80)
#define ENDP0_TXADDR        (0x90)

/* interrupt endpoint */
/* rx buffer base address */
#define ENDP1_TXADDR   		(0xA0)

/*-------------------------------------------------------------*/
/* -------------------   ISTR events  -------------------------*/
/*-------------------------------------------------------------*/
/* IMR_MSK */
/* mask defining which events has to be handled */
/* by the device application software */
#define IMR_MSK (CNTR_CTRM  | \
				 CNTR_WKUPM | \
				 CNTR_SUSPM | \
				 CNTR_ERRM  | \
				 CNTR_SOFM  | \
				 CNTR_ESOFM | \
		 		 CNTR_RESETM  \
		 		)

/* CTR service routines */
/* associated to defined endpoints */
#define  EP1_Callback   NOP_Process
#define  EP2_Callback   NOP_Process
#define  EP3_Callback   NOP_Process
#define  EP4_Callback   NOP_Process
#define  EP5_Callback   NOP_Process
#define  EP6_Callback   NOP_Process
#define  EP7_Callback   NOP_Process
#define  EP8_Callback   NOP_Process
#define  EP9_Callback   NOP_Process
#define  EP10_Callback  NOP_Process
#define  EP11_Callback  NOP_Process
#define  EP12_Callback  NOP_Process
#define  EP13_Callback  NOP_Process
#define  EP14_Callback  NOP_Process
#define  EP15_Callback  NOP_Process

/**/
/* ISTR events */
/* user defined service routines */
/*#define CTR_Callback   count_CTR
#define DOVR_Callback  count_DOVR
#define ERR_Callback   count_ERR
#define WKUP_Callback  count_WKUP
#define SUSP_Callback  count_SUSP
#define RESET_Callback count_RESET
#define SOF_Callback   count_SOF
#define ESOF_Callback  count_ESOF*/

/*-------------------------------------------------------------*/
/* Serial Enable/Disable */
/*-------------------------------------------------------------*/
#define SERIAL_AVAILABLE 1

