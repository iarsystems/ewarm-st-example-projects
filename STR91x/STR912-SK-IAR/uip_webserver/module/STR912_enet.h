/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : STR912_enet.h
 *    Description : MAC/DMA Controller with DMA (ENET) driver include file
 *
 *    History :
 *    1. Date        : October 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 39 $
 **************************************************************************/
#include <includes.h>

#ifndef __STR912_ENET_H
#define __STR912_ENET_H
#include <io_macros.h>

/* DMA Status/Control Register (ENET_SCR) */
typedef struct{
__REG32 SRESET             : 1;
__REG32 LOOPB              : 1;
__REG32                    : 2;
__REG32 RX_MAX_BURST_SIZE  : 2;
__REG32 TX_MAX_BURST_SIZE  : 2;
__REG32 REVISION           : 8;
__REG32 RX_CHAN_STATUS     : 2;
__REG32 RX_IO_DATA_WIDTH   : 2;
__REG32 RX_FIFO_SIZE       : 4;
__REG32 TX_CHAN_STATUS     : 2;
__REG32 TX_IO_DATA_WIDTH   : 2;
__REG32 TX_FIFO_SIZE       : 4;
} __enet_scr_bits;

/* DMA Interrupt Enable Register (ENET_IER) */
typedef struct{
__REG32 RX_EMPTY_EN      : 1;
__REG32 RX_FULL_EN       : 1;
__REG32 RX_ENTRY_EN      : 1;
__REG32 RX_TO_EN         : 1;
__REG32                  : 1;
__REG32 PACKET_LOST_EN   : 1;
__REG32 RX_NEXT_EN       : 1;
__REG32 RX_DONE_EN       : 1;
__REG32                  : 1;
__REG32 RX_MERR_INT_EN   : 1;
__REG32                  : 5;
__REG32 RX_CURR_DONE_EN  : 1;
__REG32 TX_EMPTY_EN      : 1;
__REG32 TX_FULL_EN       : 1;
__REG32 TX_ENTRY_EN      : 1;
__REG32 TX_TO_EN         : 1;
__REG32                  : 2;
__REG32 TX_NEXT_EN       : 1;
__REG32 TX_DONE_EN       : 1;
__REG32                  : 1;
__REG32 TX_MERR_INT_EN   : 1;
__REG32                  : 2;
__REG32 MAC_8023_INT_EN  : 1;
__REG32                  : 2;
__REG32 TX_CURR_DONE_EN  : 1;
} __enet_ier_bits;

/* DMA Interrupt Status Register (ENET_ISR) */
typedef struct{
__REG32 RX_EMPTY      : 1;
__REG32 RX_FULL       : 1;
__REG32 RX_ENTRY      : 1;
__REG32 RX_TO         : 1;
__REG32               : 1;
__REG32 PACKET_LOST   : 1;
__REG32 RX_NEXT       : 1;
__REG32 RX_DONE       : 1;
__REG32               : 1;
__REG32 RX_MERR_INT   : 1;
__REG32               : 5;
__REG32 RX_CURR_DONE  : 1;
__REG32 TX_EMPTY      : 1;
__REG32 TX_FULL       : 1;
__REG32 TX_ENTRY      : 1;
__REG32 TX_TO         : 1;
__REG32               : 2;
__REG32 TX_NEXT       : 1;
__REG32 TX_DONE       : 1;
__REG32               : 1;
__REG32 TX_MERR_INT   : 1;
__REG32               : 2;
__REG32 MAC_8023_INT  : 1;
__REG32               : 2;
__REG32 TX_CURR_DONE  : 1;
} __enet_isr_bits;

/* Clock Configuration Register (ENET_CCR) */
typedef struct{
__REG32          : 2;
__REG32 SEL_CLK  : 2;
__REG32          :28;
} __enet_ccr_bits;

/* RX Start Register (ENET_RXSTR) */
typedef struct{
__REG32 DMA_EN       : 1;
__REG32              : 1;
__REG32 START_FETCH  : 1;
__REG32              : 2;
__REG32 FILTER_FAIL  : 1;
__REG32 RUNT_FRAME   : 1;
__REG32 COLL_SEEN    : 1;
__REG32 DFETCH_DLY   :16;
__REG32              : 8;
} __enet_rxstr_bits;

/* RX Control Register (ENET_RXCR) */
typedef struct{
__REG32 DMA_XFERCOUNT  :12;
__REG32 CONT_EN        : 1;
__REG32                : 1;
__REG32 NXT_EN         : 1;
__REG32 DLY_EN         : 1;
__REG32                : 1;
__REG32 ENTRY_TRIG     : 5;
__REG32 ADDR_WRAP      :10;
} __enet_rxcr_bits;

/* RX Start Address Register (ENET_RXSAR) */
typedef struct{
__REG32 WRAP_EN   : 1;
__REG32 FIX_ADDR  : 1;
__REG32 RXADDR    :30;
} __enet_rxsar_bits;

/* RX Next Descriptor Address Register (ENET_RXNDAR) */
typedef struct{
__REG32 NPOL_EN   : 1;
__REG32           : 1;
__REG32 DESCADDR  :30;
} __enet_rxndar_bits;

/* RX Current Transfer Count Register (ENET_RXCTCR) */
typedef struct{
__REG32 CXFER  :12;
__REG32        :20;
} __enet_rxctcr_bits;

/* RX Time-Out Register (ENET_RXTOR) */
typedef struct{
__REG32 RXTO   :16;
__REG32        :16;
} __enet_rxtor_bits;


/* RX Status Register (ENET_RXSR) */
typedef struct{
__REG32 EMPTY        : 1;
__REG32 FULL         : 1;
__REG32 ENTRY_T      : 1;
__REG32 DELAY_T      : 1;
__REG32              : 4;
__REG32 IO_POINTER   : 5;
__REG32              : 3;
__REG32 DMA_POINTER  : 5;
__REG32              : 3;
__REG32 ENTRIES      : 6;
__REG32              : 2;
} __enet_rxsr_bits;

/* TX Start Register (ENET_TXSTR) */
typedef struct{
__REG32 DMA_EN       : 1;
__REG32              : 1;
__REG32 START_FETCH  : 1;
__REG32              : 2;
__REG32 UNDER_RUN    : 1;
__REG32              : 2;
__REG32 DFETCH_DLY   :16;
__REG32              : 8;
} __enet_txstr_bits;

/* TX Control Register (ENET_TXCR) */
typedef struct{
__REG32 DMA_XFERCOUNT  :12;
__REG32 CONT_EN        : 1;
__REG32                : 1;
__REG32 NXT_EN         : 1;
__REG32 DLY_EN         : 1;
__REG32                : 1;
__REG32 ENTRY_TRIG     : 5;
__REG32 ADDR_WRAP      :10;
} __enet_txcr_bits;

/* TX Start Address Register (ENET_TXSAR) */
typedef struct{
__REG32 WRAP_EN   : 1;
__REG32 FIX_ADDR  : 1;
__REG32 TXADDR    :30;
} __enet_txsar_bits;

/* TX Next Descriptor Address Register (ENET_TXNDAR) */
typedef struct{
__REG32 NPOL_EN   : 1;
__REG32           : 1;
__REG32 DESCADDR  :30;
} __enet_txndar_bits;

/* TX Current Transfer Count Register (ENET_TXCTCR) */
typedef struct{
__REG32 CXFER  :12;
__REG32        :20;
} __enet_txctcr_bits;

/* TX Time-Out Register (ENET_TXTOR) */
typedef struct{
__REG32 TXTO   :16;
__REG32        :16;
} __enet_txtor_bits;

/* TX Status Register (ENET_TXSR) */
typedef struct{
__REG32 EMPTY        : 1;
__REG32 FULL         : 1;
__REG32 ENTRY_T      : 1;
__REG32 DELAY_T      : 1;
__REG32              : 4;
__REG32 IO_POINTER   : 5;
__REG32              : 3;
__REG32 DMA_POINTER  : 5;
__REG32              : 3;
__REG32 ENTRIES      : 6;
__REG32              : 2;
} __enet_txsr_bits;

/* MAC Control Register (ENET_MCR) */
typedef struct{
__REG32 RCFA  : 1;
__REG32       : 1;
__REG32 RE    : 1;
__REG32 TE    : 1;
__REG32 RVBE  : 1;
__REG32 DCE   : 1;
__REG32 BL    : 2;
__REG32 APR   : 1;
__REG32 RVFF  : 1;
__REG32 DPR   : 1;
__REG32 DBF   : 1;
__REG32 ELC   : 1;
__REG32       : 2;
__REG32 VFM   : 1;
__REG32 PWF   : 1;
__REG32 AFM   : 3;
__REG32 FDM   : 1;
__REG32 LM    : 2;
__REG32 DRO   : 1;
__REG32 PS    : 2;
__REG32       : 4;
__REG32 EN    : 1;
__REG32 RA    : 1;
} __enet_mcr_bits;

/* MII Address Register (ENET_MIIA) */
typedef struct{
__REG32 BUSY   : 1;
__REG32 WR     : 1;
__REG32 PR     : 1;
__REG32        : 3;
__REG32 RADDR  : 5;
__REG32 PADDR  : 5;
__REG32        :16;
} __enet_miia_bits;

/* MII Data Register (ENET_MIID) */
typedef struct{
__REG32 RDATA  :16;
__REG32        :16;
} __enet_miid_bits;

/* MII Control Frame Register (ENET_MCF) */
typedef struct{
__REG32 FCB    : 1;
__REG32 FCE    : 1;
__REG32 PCF    : 1;
__REG32        :13;
__REG32 PTIME  :16;
} __enet_mcf_bits;

/* VLAN1 Register (ENET_VL1) */
typedef struct{
__REG32 VLTAG1  :16;
__REG32 VLID1   :12;
__REG32         : 4;
} __enet_vl1_bits;

/* VLAN2 Register (ENET_VL2) */
typedef struct{
__REG32 VLTAG2  :16;
__REG32 VLID2   :12;
__REG32         : 4;
} __enet_vl2_bits;

/* MAC Transmission Status Register (ENET_MTS) */
typedef struct{
__REG32 FA   : 1;
__REG32      : 1;
__REG32 NC   : 1;
__REG32 LOC  : 1;
__REG32 ED   : 1;
__REG32 LC   : 1;
__REG32 EC   : 1;
__REG32 UR   : 1;
__REG32 DEF  : 1;
__REG32 LCO  : 1;
__REG32 CC   : 4;
__REG32      : 4;
__REG32 BC   :13;
__REG32 PR   : 1;
} __enet_mts_bits;

/* MAC Reception Status Register (ENET_MRS) */
typedef struct{
__REG32 FL   :11;
__REG32      : 2;
__REG32 FCI  : 1;
__REG32 WT   : 1;
__REG32 RF   : 1;
__REG32 OL   : 1;
__REG32 LC   : 1;
__REG32 FT   : 1;
__REG32 ME   : 1;
__REG32 EB   : 1;
__REG32 CE   : 1;
__REG32 VL1  : 1;
__REG32 VL2  : 1;
__REG32 LE   : 1;
__REG32 CF   : 1;
__REG32 UCF  : 1;
__REG32 MCF  : 1;
__REG32 BF   : 1;
__REG32 FF   : 1;
__REG32 PF   : 1;
__REG32 FA   : 1;
} __enet_mrs_bits;


/***************************************************************************
 **
 ** ENET
 **
 ***************************************************************************/
__IO_REG32_BIT(ENET_SCR,            0x7C000000,__READ_WRITE,__enet_scr_bits);
__IO_REG32_BIT(ENET_IER,            0x7C000004,__READ_WRITE,__enet_ier_bits);
__IO_REG32_BIT(ENET_ISR,            0x7C000008,__READ_WRITE,__enet_isr_bits);
__IO_REG32_BIT(ENET_CCR,            0x7C00000C,__READ_WRITE,__enet_ccr_bits);
__IO_REG32_BIT(ENET_RXSTR,          0x7C000010,__READ_WRITE,__enet_rxstr_bits);
__IO_REG32_BIT(ENET_RXCR,           0x7C000014,__READ_WRITE,__enet_rxcr_bits);
__IO_REG32_BIT(ENET_RXSAR,          0x7C000018,__READ_WRITE,__enet_rxsar_bits);
__IO_REG32_BIT(ENET_RXNDAR,         0x7C00001C,__READ_WRITE,__enet_rxndar_bits);
__IO_REG32(    ENET_RXCAR,          0x7C000020,__READ      );
__IO_REG32_BIT(ENET_RXCTCR,         0x7C000024,__READ      ,__enet_rxctcr_bits);
__IO_REG32_BIT(ENET_RXTOR,          0x7C000028,__READ      ,__enet_rxtor_bits);
__IO_REG32_BIT(ENET_RXSR,           0x7C00002C,__READ      ,__enet_rxsr_bits);
__IO_REG32_BIT(ENET_TXSTR,          0x7C000030,__READ_WRITE,__enet_txstr_bits);
__IO_REG32_BIT(ENET_TXCR,           0x7C000034,__READ_WRITE,__enet_txcr_bits);
__IO_REG32_BIT(ENET_TXSAR,          0x7C000038,__READ_WRITE,__enet_txsar_bits);
__IO_REG32_BIT(ENET_TXNDAR,         0x7C00003C,__READ_WRITE,__enet_txndar_bits);
__IO_REG32(    ENET_TXCAR,          0x7C000040,__READ      );
__IO_REG32_BIT(ENET_TXCTCR,         0x7C000044,__READ      ,__enet_txctcr_bits);
__IO_REG32_BIT(ENET_TXTOR,          0x7C000048,__READ      ,__enet_txtor_bits);
__IO_REG32_BIT(ENET_TXSR,           0x7C00004C,__READ      ,__enet_txsr_bits);
__IO_REG32_BIT(ENET_MCR,            0x7C000400,__READ_WRITE,__enet_mcr_bits);
__IO_REG32(    ENET_MAH,            0x7C000404,__READ_WRITE);
__IO_REG32(    ENET_MAL,            0x7C000408,__READ_WRITE);
__IO_REG32(    ENET_MCHA,           0x7C00040C,__READ_WRITE);
__IO_REG32(    ENET_MCLA,           0x7C000410,__READ_WRITE);
__IO_REG32_BIT(ENET_MIIA,           0x7C000414,__READ_WRITE,__enet_miia_bits);
__IO_REG32_BIT(ENET_MIID,           0x7C000418,__READ_WRITE,__enet_miid_bits);
__IO_REG32_BIT(ENET_MCF,            0x7C00041C,__READ_WRITE,__enet_mcf_bits);
__IO_REG32_BIT(ENET_VL1,            0x7C000420,__READ_WRITE,__enet_vl1_bits);
__IO_REG32_BIT(ENET_VL2,            0x7C000424,__READ_WRITE,__enet_vl2_bits);
__IO_REG32_BIT(ENET_MTS,            0x7C000428,__READ      ,__enet_mts_bits);
__IO_REG32_BIT(ENET_MRS,            0x7C00042C,__READ      ,__enet_mrs_bits);
__IO_REG32(    ENET_RX_FIFO_0,      0x7C000100,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_1,      0x7C000104,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_2,      0x7C000108,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_3,      0x7C00010C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_4,      0x7C000110,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_5,      0x7C000114,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_6,      0x7C000118,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_7,      0x7C00011C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_8,      0x7C000120,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_9,      0x7C000124,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_10,     0x7C000128,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_11,     0x7C00012C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_12,     0x7C000130,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_13,     0x7C000134,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_14,     0x7C000138,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_15,     0x7C00013C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_16,     0x7C000140,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_17,     0x7C000144,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_18,     0x7C000148,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_19,     0x7C00014C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_20,     0x7C000150,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_21,     0x7C000154,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_22,     0x7C000158,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_23,     0x7C00015C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_24,     0x7C000160,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_25,     0x7C000164,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_26,     0x7C000168,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_27,     0x7C00016C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_28,     0x7C000170,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_29,     0x7C000174,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_30,     0x7C000178,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_31,     0x7C00017C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_0,      0x7C000200,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_1,      0x7C000204,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_2,      0x7C000208,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_3,      0x7C00020C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_4,      0x7C000210,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_5,      0x7C000214,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_6,      0x7C000218,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_7,      0x7C00021C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_8,      0x7C000220,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_9,      0x7C000224,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_10,     0x7C000228,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_11,     0x7C00022C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_12,     0x7C000230,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_13,     0x7C000234,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_14,     0x7C000238,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_15,     0x7C00023C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_16,     0x7C000240,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_17,     0x7C000244,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_18,     0x7C000248,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_19,     0x7C00024C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_20,     0x7C000250,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_21,     0x7C000254,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_22,     0x7C000258,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_23,     0x7C00025C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_24,     0x7C000260,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_25,     0x7C000264,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_26,     0x7C000268,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_27,     0x7C00026C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_28,     0x7C000270,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_29,     0x7C000274,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_30,     0x7C000278,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_31,     0x7C00027C,__READ_WRITE);

/***************************************************************************
 **
 ** ENET Buffered
 **
 ***************************************************************************/
__IO_REG32_BIT(ENET_SCR_BUF,        0x6C000000,__READ_WRITE,__enet_scr_bits);
__IO_REG32_BIT(ENET_IER_BUF,        0x6C000004,__READ_WRITE,__enet_ier_bits);
__IO_REG32_BIT(ENET_ISR_BUF,        0x6C000008,__READ_WRITE,__enet_isr_bits);
__IO_REG32_BIT(ENET_CCR_BUF,        0x6C00000C,__READ_WRITE,__enet_ccr_bits);
__IO_REG32_BIT(ENET_RXSTR_BUF,      0x6C000010,__READ_WRITE,__enet_rxstr_bits);
__IO_REG32_BIT(ENET_RXCR_BUF,       0x6C000014,__READ_WRITE,__enet_rxcr_bits);
__IO_REG32_BIT(ENET_RXSAR_BUF,      0x6C000018,__READ_WRITE,__enet_rxsar_bits);
__IO_REG32_BIT(ENET_RXNDAR_BUF,     0x6C00001C,__READ_WRITE,__enet_rxndar_bits);
__IO_REG32(    ENET_RXCAR_BUF,      0x6C000020,__READ      );
__IO_REG32_BIT(ENET_RXCTCR_BUF,     0x6C000024,__READ      ,__enet_rxctcr_bits);
__IO_REG32_BIT(ENET_RXTOR_BUF,      0x6C000028,__READ      ,__enet_rxtor_bits);
__IO_REG32_BIT(ENET_RXSR_BUF,       0x6C00002C,__READ      ,__enet_rxsr_bits);
__IO_REG32_BIT(ENET_TXSTR_BUF,      0x6C000030,__READ_WRITE,__enet_txstr_bits);
__IO_REG32_BIT(ENET_TXCR_BUF,       0x6C000034,__READ_WRITE,__enet_txcr_bits);
__IO_REG32_BIT(ENET_TXSAR_BUF,      0x6C000038,__READ_WRITE,__enet_txsar_bits);
__IO_REG32_BIT(ENET_TXNDAR_BUF,     0x6C00003C,__READ_WRITE,__enet_txndar_bits);
__IO_REG32(    ENET_TXCAR_BUF,      0x6C000040,__READ      );
__IO_REG32_BIT(ENET_TXCTCR_BUF,     0x6C000044,__READ      ,__enet_txctcr_bits);
__IO_REG32_BIT(ENET_TXTOR_BUF,      0x6C000048,__READ      ,__enet_txtor_bits);
__IO_REG32_BIT(ENET_TXSR_BUF,       0x6C00004C,__READ      ,__enet_txsr_bits);
__IO_REG32_BIT(ENET_MCR_BUF,        0x6C000400,__READ_WRITE,__enet_mcr_bits);
__IO_REG32(    ENET_MAH_BUF,        0x6C000404,__READ_WRITE);
__IO_REG32(    ENET_MAL_BUF,        0x6C000408,__READ_WRITE);
__IO_REG32(    ENET_MCHA_BUF,       0x6C00040C,__READ_WRITE);
__IO_REG32(    ENET_MCLA_BUF,       0x6C000410,__READ_WRITE);
__IO_REG32_BIT(ENET_MIIA_BUF,       0x6C000414,__READ_WRITE,__enet_miia_bits);
__IO_REG32_BIT(ENET_MIID_BUF,       0x6C000418,__READ_WRITE,__enet_miid_bits);
__IO_REG32_BIT(ENET_MCF_BUF,        0x6C00041C,__READ_WRITE,__enet_mcf_bits);
__IO_REG32_BIT(ENET_VL1_BUF,        0x6C000420,__READ_WRITE,__enet_vl1_bits);
__IO_REG32_BIT(ENET_VL2_BUF,        0x6C000424,__READ_WRITE,__enet_vl2_bits);
__IO_REG32_BIT(ENET_MTS_BUF,        0x6C000428,__READ      ,__enet_mts_bits);
__IO_REG32_BIT(ENET_MRS_BUF,        0x6C00042C,__READ      ,__enet_mrs_bits);
__IO_REG32(    ENET_RX_FIFO_0_BUF,  0x6C000100,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_1_BUF,  0x6C000104,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_2_BUF,  0x6C000108,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_3_BUF,  0x6C00010C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_4_BUF,  0x6C000110,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_5_BUF,  0x6C000114,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_6_BUF,  0x6C000118,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_7_BUF,  0x6C00011C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_8_BUF,  0x6C000120,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_9_BUF,  0x6C000124,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_10_BUF, 0x6C000128,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_11_BUF, 0x6C00012C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_12_BUF, 0x6C000130,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_13_BUF, 0x6C000134,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_14_BUF, 0x6C000138,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_15_BUF, 0x6C00013C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_16_BUF, 0x6C000140,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_17_BUF, 0x6C000144,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_18_BUF, 0x6C000148,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_19_BUF, 0x6C00014C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_20_BUF, 0x6C000150,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_21_BUF, 0x6C000154,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_22_BUF, 0x6C000158,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_23_BUF, 0x6C00015C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_24_BUF, 0x6C000160,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_25_BUF, 0x6C000164,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_26_BUF, 0x6C000168,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_27_BUF, 0x6C00016C,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_28_BUF, 0x6C000170,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_29_BUF, 0x6C000174,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_30_BUF, 0x6C000178,__READ_WRITE);
__IO_REG32(    ENET_RX_FIFO_31_BUF, 0x6C00017C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_0_BUF,  0x6C000200,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_1_BUF,  0x6C000204,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_2_BUF,  0x6C000208,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_3_BUF,  0x6C00020C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_4_BUF,  0x6C000210,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_5_BUF,  0x6C000214,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_6_BUF,  0x6C000218,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_7_BUF,  0x6C00021C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_8_BUF,  0x6C000220,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_9_BUF,  0x6C000224,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_10_BUF, 0x6C000228,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_11_BUF, 0x6C00022C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_12_BUF, 0x6C000230,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_13_BUF, 0x6C000234,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_14_BUF, 0x6C000238,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_15_BUF, 0x6C00023C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_16_BUF, 0x6C000240,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_17_BUF, 0x6C000244,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_18_BUF, 0x6C000248,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_19_BUF, 0x6C00024C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_20_BUF, 0x6C000250,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_21_BUF, 0x6C000254,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_22_BUF, 0x6C000258,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_23_BUF, 0x6C00025C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_24_BUF, 0x6C000260,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_25_BUF, 0x6C000264,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_26_BUF, 0x6C000268,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_27_BUF, 0x6C00026C,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_28_BUF, 0x6C000270,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_29_BUF, 0x6C000274,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_30_BUF, 0x6C000278,__READ_WRITE);
__IO_REG32(    ENET_TX_FIFO_31_BUF, 0x6C00027C,__READ_WRITE);


#define EMAC_MAX_PACKET_SIZE 1520
#define ENET_OK  (1)
#define ENET_NOK (0)

// MMI PHY Address
#define MII_PHY_ADDR        0x15

// Description of common PHY registers STE100
#define MAC_MII_REG_XCR     0x00000000  // XCVR Control Register
#define MAC_MII_REG_XSR     0x00000001  // XCVR Status Register
#define MAC_MII_REG_PID1    0x00000002  // PHY Identifier 1
#define MAC_MII_REG_PID2    0x00000003  // PHY Identifier 2
#define MAC_MII_REG_ANA     0x00000004  // Auto-Negotiation Advertisement Register
#define MAC_MII_REG_ANLPA   0x00000005  // Auto-Negotiation Link Partner Ability Register
#define MAC_MII_REG_ANE     0x00000006  // Auto-Negotiation Expansion Register
#define MAC_MII_REG_XCIIS   0x00000011  // XCVR Configuration Information and Interrupt Status Register
#define MAC_MII_REG_XIE     0x00000012  // XCVR Interrupt Enable Register
#define MAC_MII_REG_100CTR  0x00000013  // 100Base-TX PHY Control/Status Register
#define MAC_MII_REG_XMC     0x00000014  // XCVR Mode Control Register

// ENET_OperatingMode
#define PHY_OPR_MODE        0x2100      //  Set the full-duplex mode at 100 Mb/s

typedef union _EnetRxCR_t
{
  Int32U Data;
  struct {
    Int32U  DMA_XFERCOUNT :12;
    Int32U  CONT_EN       : 1;
    Int32U                : 1;
    Int32U  NXT_EN        : 1;
    Int32U  DLY_EN        : 1;
    Int32U                : 1;
    Int32U  ENTRY_TRIG    : 5;
    Int32U  ADDR_WRAP     :10;
  };
} EnetRxCR_t, * pEnetRxCR_t;

typedef union _EnetRxSR_t
{
  Int32U Data;
  struct {
    Int32U  FrameLength     :11;
    Int32U                  : 1;
    Int32U  Overlength      : 1;
    Int32U  FalseCarrier    : 1;
    Int32U  WatchdogTO      : 1;
    Int32U  RuntFrame       : 1;
    Int32U  Valid           : 1;
    Int32U  LateCollision   : 1;
    Int32U  FrameType       : 1;
    Int32U  MIIError        : 1;
    Int32U  ExtraBits       : 1;
    Int32U  CRCError        : 1;
    Int32U  OneLevelVLAN    : 1;
    Int32U  TwoLevelVLAN    : 1;
    Int32U  LengthError     : 1;
    Int32U  ControlFrame    : 1;
    Int32U  UnsupportedFrame: 1;
    Int32U  MulticastFrame  : 1;
    Int32U  BroadcastFrame  : 1;
    Int32U  FilteringFail   : 1;
    Int32U  PacketFilter    : 1;
    Int32U  FrameAbort      : 1;
  };
} EnetRxSR_t, * pEnetRxSR_t;

typedef union _EnetTxSR_t
{
  Int32U Data;
  struct {
    Int32U  FrameAborted    : 1;
    Int32U                  : 1;
    Int32U  NoCarrier       : 1;
    Int32U  LossOfCarrier   : 1;
    Int32U  ExcessiveDef    : 1;
    Int32U  LateCollision   : 1;
    Int32U  ExcessiveColl   : 1;
    Int32U  UnderRun        : 1;
    Int32U  Deferred        : 1;
    Int32U  LateCollisionObs: 1;
    Int32U  CollisionCount  : 4;
    Int32U                  : 2;
    Int32U  Valid           : 1;
    Int32U                  : 1;
    Int32U  ByteCounter     :13;
    Int32U  PacketRetry     : 1;
  };
} EnetTxSR_t, * pEnetTxSR_t;

typedef union _EnetDmaDesc_t
{
  Int32U Data[4];
  // Rx DMA descriptor
  struct
  {
    EnetRxCR_t              EnetRxCR;
    pInt32U                 pBuffer;
    union _EnetDmaDesc_t *  EnetDmaNextDesc;
    EnetRxSR_t              EnetRxSR;
  } Rx;
  // Tx DMA descriptor
  struct
  {
    EnetRxCR_t              EnetTxCR;
    pInt32U                 pBuffer;
    union _EnetDmaDesc_t *  EnetDmaNextDesc;
    EnetTxSR_t              EnetTxSR;
  } Tx;

} EnetDmaDesc_t, * pEnetDmaDesc_t;

/*************************************************************************
 * Function Name:
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init  MAC/DMA Controller
 *
 *************************************************************************/
void tapdev_init(void);

/*************************************************************************
 * Function Name: tapdev_read
 * Parameters:
 * Return:
 *
 * Description: Read data for MAC/DMA Controller
 *
 *************************************************************************/
Int32U tapdev_read(void * pPacket);

/*************************************************************************
 * Function Name: tapdev_send
 * Parameters:
 * Return:
 *
 * Description: Send data to MAC/DMA Controller
 *
 *************************************************************************/
void tapdev_send (void *pPacket, Int32U size);

/*************************************************************************
 * Function Name: ENET_MIIWriteRegister
 * Parameters: Int8U DevId, Int8U RegAddr, Int32U Value
 * Return: none
 *
 * Description: Writes a value on the PHY registers
 *
 *************************************************************************/
static void ENET_MIIWriteRegister (Int8U DevId, Int8U RegAddr, Int32U Value);

/*************************************************************************
 * Function Name: ENET_MIIReadRegister
 * Parameters: Int8U DevId, Int8U RegAddr, Int32U Value
 * Return: Int32U
 *
 * Description: Read a value from the PHY registers
 *
 *************************************************************************/
static Int32U ENET_MIIReadRegister (Int8U DevId, Int8U RegAddr);

#endif // __STR912_ENET_H

