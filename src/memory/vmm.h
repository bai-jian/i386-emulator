/* VMM: Virtual Memory Management, Segment/Page Memory Management */

#ifndef __VMM_H__
#define __VMM_H__

/** Segment Selector ****************************/
/*                                              */
/*             +-------------+--+---+           */
/*             |    INDEX    |TI|RPL|           */
/*             +-------------+--+---+           */
/*             15           3  2 1 0            */
/*                                              */
/*   TI  - Table Indicator                      */
/*   RPL - Requtor's Privilege Level            */
/*                                              */
/*                                              */
/************************************************/
	typedef struct SegmentSelector_t
	{
		uint16_t RPL	: 2;
		uint16_t TI		: 1;
		uint16_t INDEX	: 13;
	} SegSel_t;


/** Segment Descriptor *************************************************************************/
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
/***********************************************************************************************/
	typedef struct SegmentDescriptor_t
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
	} SegDesc_t;



/** Gate Descriptor ****************************************************************************/
/*                                                                                             */
/*                                                                                             */
/*            |63                                  48|47  46 45 44  43    40|39            32  */
/*            +--------------------------------------+---+-----+---+--------+----------------+ */
/*            |                                      |   |     |   |        |                | */
/*            |           offset[31:16]              | P | DPL | S |  TYPE  |                | */
/*            |                                      |   |     |   |        |                | */
/*            +--------------------------------------+---+-----+---+--------+----------------+ */
/*            |                                      |                                       | */
/*            |              selector                |           offset[15:0]                | */
/*            |                                      |                                       | */
/*            +--------------------------------------+---------------------------------------+ */
/*            |31                                  16|15                                    0| */
/*	                                                                                           */
/*              P   - 1   - Present: 0, not present; 1, present                                */
/*              DPL - 00  - Descriptor Privilege Level                                         */
/*              S   - 1   - System: 0, system; 1, user.                                        */
/*              TYPE:                                                                          */
/*                  0101    Task      Gate                                                     */
/*                  1110    Interrupt Gate                                                     */
/*                  1111    Trap      Gate                                                     */
/*                                                                                             */
/***********************************************************************************************/
	typedef struct GateDescriptor_t
	{
		uint64_t off_l		: 16;	// offset[0...15]
		uint64_t selector	: 16;	// segment selector
		uint64_t pad0		: 8;
		uint64_t type		: 4;
		uint64_t S			: 1;	// system
		uint64_t DPL		: 2;	// descriptor privilege level
		uint64_t P			: 1;	// present
		uint64_t off_h		: 16;	// offset[16...31]
	};


/** Paging *************************************************************************************/
/*                                                                                             */
/*     ** Virtual Memory Address *************************************************             */
/*     *                                                                         *             */
/*     *    +----------+----------+------------+                                 *             */
/*     *    |   VPDI   |   VPTI   |    VPO     |                                 *             */
/*     *    +----------+----------+------------+                                 *             */
/*     *      31      22 21      12 11         0                                 *             */
/*     *                                                                         *             */
/*     *        VPDI: Virtual Page Directory Index                               *             */
/*     *        VPTI: Virtual Page Table     Index                               *             */
/*     *        VPO : Virtual Page Offset                                        *             */ 
/*     *                                                                         *             */
/*     ***************************************************************************             */
/*                                                                                             */
/*     ** Page Directory Entry ***************************************************             */
/*     *                                                                         *             */
/*     *    +--------------------+------------+---+---+---+---+---+---+          *             */
/*     *    |                    |            |   |   |   |   |   |   |          *             */
/*     *    |      page frme     |            | A |PCD|PWT|U/S|R/W| P |          *             */
/*     *    |                    |            |   |   |   |   |   |   |          *             */
/*     *    +--------------------+------------+---+---+---+---+---+---+          *             */
/*     *    |31                12|11         6| 5   4   3   2   1   0 |          *             */
/*	   *                                                                         *             */
/*     *        A - Accessed                                                     *             */
/*     *        PCD - Page Cache Disable                                         *             */ 
/*     *        PWT - Page Write Through                                         *             */
/*     *        U/S - User/Supervisor                                            *             */
/*     *        R/W - Read/Write                                                 *             */
/*     *        P - Present                                                      *             */`
/*     *                                                                         *             */
/*     ***************************************************************************             */
/*                                                                                             */
/*     ** Page Table Entry *******************************************************             */
/*     *                                                                         *             */
/*     *    +--------------------+-----+---+---+---+---+---+---+---+---+---+     *             */
/*     *    |                    |     |   |   |   |   |   |   |   |   |   |     *             */
/*     *    |      page frme     |     | G |   | D | A |PCD|PWT|U/S|R/W| P |     *             */
/*     *    |                    |     |   |   |   |   |   |   |   |   |   |     *             */
/*     *    +--------------------+-----+---+---+---+---+---+---+---+---+---+     *             */
/*     *    |31                12|11  9| 8   7   6   5   4   3   2   1   0 |     *             */
/*	   *                                                                         *             */
/*     *        G - Gloal                                                        *             */
/*     *        D - Dirty                                                        *             */
/*     *        A - Accessed                                                     *             */
/*     *        PCD - Page Cache Disable                                         *             */ 
/*     *        PWT - Page Write Through                                         *             */
/*     *        U/S - User/Supervisor                                            *             */
/*     *        R/W - Read/Write                                                 *             */
/*     *        P - Present                                                      *             */`
/*     *                                                                         *             */
/*     ***************************************************************************             */
/*                                                                                             */
/***********************************************************************************************/
	typedef struct PageDirectoryEntry_t
	{
		uint32_t P		: 1;
		uint32_t RW		: 1;
		uint32_t US		: 1;
		uint32_t PWT	: 1;
		uint32_t PCD	: 1;
		uint32_t A		: 1;
		uint32_t     	: 6;
		uint32_t frame  : 20;
	} PDE_t;
	typedef struct PageTableEntry_t
	{
		uint32_t P		: 1;
		uint32_t RW		: 1;
		uint32_t US		: 1;
		uint32_t PWT	: 1;
		uint32_t PCD	: 1;
		uint32_t A		: 1;
		uint32_t D		: 1;
		uint32_t     	: 1;
		uint32_t G		: 1;
		uint32_t 		: 3;
		uint32_t frame  : 20;
	} PTE_t;


#endif
