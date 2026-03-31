;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Start up code for SPEAR600 bootloader,
;; Enable I cache, set supervisor stack and jumps
;; to bootloader
;;
;; Copyright 2011 IAR Systems. All rights reserved.
;;
;; $Revision: 39 $
;;

        MODULE  ?bootloader

        SECTION .bootloader_entry:CODE:ROOT(2)
        PUBLIC  __boot_entry
                ARM
__boot_entry
        b         __boot_entry__

        SECTION BOOT_STACK:DATA:NOROOT(3)
        SECTION .bootloader:CODE:NOROOT(2)
		    EXTERN	bootload
                ARM	; Always ARM mode after reset	
__boot_entry__
CP_DIS_MASK         DEFINE  0xFFFFEFFA
 // Disable Addr translation, D cache and enable I cache
        MRC         p15,0,R1,C1,C0,0
        LDR         R0,=CP_DIS_MASK      ;; 0xFFFFEFFA
        AND         R1,R1,R0
        ORR         R1,R1,#(1<<12)
        MCR         p15,0,R1,C1,C0,0

        ldr         sp,=SFE(BOOT_STACK)                 ; End of SVC_STACK
;start bootloader
        ldr         r0,=bootload
        bx          r0

                END
