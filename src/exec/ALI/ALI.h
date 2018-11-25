#ifndef __ALI_H__
#define __ALI_H__


/*Arithmetic and Logical Instructions:*/

/*  ALI_1: INC   DEC                  */  #include "ALI_1/ALI_1.h"
/*         (+) JMP  <- CTI/CTI_1      */
/*         (+) CALL <- CTI/CTI_1      */
/*         (+) PUSH <- DMI            */

/* 	ALI_2: NEG   NOT   MUL   IMUL     */  #include "ALI_2/ALI_2.h"
/*  	   DIV   IDIV  TEST           */

/* 	ALI_3: ADD   SUB   ADC   SBB      */  #include "ALI_3/ALI_3.h"              
/*  	   CMP   AND   OR    XOR      */

/* 	ALI_4: SAL   SHL   SAR   SHR      */  #include "ALI_4/ALI_4.h"

/* 	ALI_5: SETcc                      */  #include "ALI_5/setcc.h"

/*  ALI_6: BT                         */  #include "ALI_6/ALI_6.h"


#endif
