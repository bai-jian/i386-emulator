/* VMM: Virtual Memory Management, Segment/Page Memory Management */

#ifndef __VMM_H__
#define __VMM_H__

/*************** Segment Selector ***************/
/*                                              */
/*             +-------------+--+---+           */
/*             |    INDEX    |TI|RPL|           */
/*             +-------------+--+---+           */
/*             15           3  2 1 0            */
/*                                              */
/*   TI  - Table Indicator                      */
/*   RPL - Requtor's Privilege Level            */
/*                                              */
	typedef struct SegmentSelector_t
	{
		uint16_t RPL	: 2;
		uint16_t TI		: 1;
		uint16_t INDEX	: 13;
	} SegSel_t;
/*                                              */
/************************************************/


/********************************* Segment Descriptor ******************************************/
/*                                                                                             */
/*                                                                                             */
/*  |63            56|55  54  53  52  51         48|47  46 45 44  43   40|39            32     */
/*  +----------------+---+---+---+---+-------------+---+-----+---+-------+----------------+    */
/*  |                |   |   |   |   |             |   |     |   |       |                |    */
/*  |  base[31:24]   | G | B | 0 |AVL| limit[19:16]| P | DPL | S | TYPE  |   base[23:16]  |    */
/*  |                |   |   |   |   |             |   |     |   |0|E|W|A|                |    */
/*  +----------------+---+---+---+---+-------------+---+-----+---+-------+----------------+    */
/*  |                                              |                                      |    */
/*  |                base[15:0]                    |           limit[15:0]                |    */
/*  |                                              |                                      |    */
/*  +----------------------------------------------+--------------------------------------+    */
/*  |31                                          16|15                                   0|    */
/*	                                                                                           */
/*	    G   - 1   - Granularity: 0, byte(1 MB); 1, page(4 GB)                                  */
/*	    B   - 1   - bit Broadth of offset in a segment: 0, 32; 1, 16;                          */
/*	    AVL - 0   - AVaiLable for use by system programmers                                    */
/*      P   - 1   - segment Present: 0, not present; 1, present                                */
/*      DPL - 00  - Descriptor Privilege Level                                                 */
/*      S   - 1   - Segment type: 0, system; 1, user.                                          */
/*      E   - 0                                                                                */
/*      W   - 0                                                                                */
/*      A   - 0   - Accessed                                                                   */
/*      (1) limit = 0x0FFFF, thus, Physical Memory Size = 4KB * 2^16 = 256M                    */
/*                                                                                             */
	typedef struct SegmentDescriptor
	{
		uint64_t limit_15_0			: 16;
		uint64_t base_15_0			: 16;
		uint64_t base_23_16			: 8;
		uint64_t type				: 4;
		uint64_t S                  : 1;
		uint64_t DPL                : 2;
		uint64_t P					: 1;
		uint64_t limit_19_16		: 4;
		uint64_t AVL                : 1;
		uint64_t                    : 1;
		uint64_t D					: 1;
		uint64_t G					: 1;
		uint64_t base_31_24			: 8;
	} SegDesc;
/*                                                                                             */
/***********************************************************************************************/
	


#endif
