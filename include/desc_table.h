// desc_table:  (Globl / Local / Interrupt) Descriptor Table

#ifndef __DESC_TABLE__
#define __DESC_TABLE__


	/* Gate Descriptor */
	//     63...48   47  46 45  44  43...40  39...32  31...16    15...0
	//    off[31-16] P    DPL   S    type     PAD0    selector  off[15-0]
	//
	//    type:  1110B  Interrupt Gate
	//           1111B  Trap Gate
	//           0101B  Task Gate
	typedef struct
	{
		uint64_t off_l		: 16;	// offset[0...15]
		uint64_t selector	: 16;	// segment selector
		uint64_t pad0		: 8;
		uint64_t type		: 4;
		uint64_t S			: 1;	// system
		uint64_t DPL		: 2;	// descriptor privilege level
		uint64_t P			: 1;	// present
		uint64_t off_h		: 16;	// offset[16...31]
	} GateDesc_t;


#endif
