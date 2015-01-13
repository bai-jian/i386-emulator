#include "exec/helper.h"

#include "all-instr.h"


char suffix = 'l';  
make_helper(data_size);

make_helper(tran_mean);


typedef int (*helper_fun)(swaddr_t);
helper_fun opcode_table [256] = 
{
/* 0x00 */	add_r2rm_b,    add_r2rm_v,    add_rm2r_b,    add_rm2r_v, 
/* 0x04 */	add_i2r0_b,    add_i2r0_v,    inv, inv, 
/* 0x08 */	 or_r2rm_b,     or_r2rm_v,     or_rm2r_b,     or_rm2r_v, 
/* 0x0c */	 or_i2r0_b,     or_i2r0_v,    inv,           tran_mean, 
/* 0x10 */	adc_r2rm_b,    adc_r2rm_v,    adc_rm2r_b,    adc_rm2r_v, 
/* 0x14 */	adc_i2r0_b,    adc_i2r0_v,    inv, inv, 
/* 0x18 */	sbb_r2rm_b,    sbb_r2rm_v,    sbb_rm2r_b,    sbb_rm2r_v, 
/* 0x1c */	sbb_i2r0_b,    sbb_i2r0_v,    inv, inv, 
/* 0x20 */	and_r2rm_b,    and_r2rm_v,    and_rm2r_b,    and_rm2r_v, 
/* 0x24 */	and_i2r0_b,    and_i2r0_v,    inv,           inv,
/* 0x28 */	sub_r2rm_b,    sub_r2rm_v,    sub_rm2r_b,    sub_rm2r_v, 
/* 0x2c */	sub_i2r0_b,    sub_i2r0_v,    inv,           inv, 
/* 0x30 */	xor_r2rm_b,    xor_r2rm_v,    xor_rm2r_b,    xor_rm2r_v, 
/* 0x34 */	xor_i2r0_b,    xor_i2r0_v,    inv, inv,
/* 0x38 */	cmp_r2rm_b,    cmp_r2rm_v,    cmp_rm2r_b,    cmp_rm2r_v, 
/* 0x3c */	cmp_i2r0_b,    cmp_i2r0_v,    inv, inv, 
/* 0x40 */	inc_r_v,       inc_r_v,       inc_r_v,       inc_r_v, 
/* 0x44 */	inc_r_v,       inc_r_v,       inc_r_v,       inc_r_v,
/* 0x48 */	dec_r_v,       dec_r_v,       dec_r_v,       dec_r_v, 
/* 0x4c */	dec_r_v,       dec_r_v,       dec_r_v,       dec_r_v, 
/* 0x50 */	push_r_v,      push_r_v,      push_r_v,      push_r_v, 
/* 0x54 */	push_r_v,      push_r_v,      push_r_v,      push_r_v,
/* 0x58 */	pop_r_v,       pop_r_v,       pop_r_v,       pop_r_v, 
/* 0x5c */	pop_r_v,       pop_r_v,       pop_r_v,       pop_r_v, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv,                     data_size,     inv,
/* 0x68 */	push_i_v,      imul_i2r_v,    push_i_b,      imul_i2r_v, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv,                     jb_b,          jae_b,
/* 0x74 */	je_b,          jne_b,         jbe_b,         ja_b,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	jl_b,          jge_b,         jle_b,         jg_b, 
/* 0x80 */	ALI_3_b,       ALI_3_v,       nemu_trap,     ALI_3_v, 
/* 0x84 */	test_r2rm_b,   test_r2rm_v,   xchg_r2rm_b,   xchg_r2rm_v, 
/* 0x88 */	mov_r2rm_b,    mov_r2rm_v,    mov_rm2r_b,    mov_rm2r_v,
/* 0x8c */	mov_sreg2rm,   lea_m2r_v,     mov_rm2sreg,   DMI_3_v, 
/* 0x90 */	xchg_r2r0_v,   xchg_r2r0_v,   xchg_r2r0_v,   xchg_r2r0_v,
/* 0x94 */	xchg_r2r0_v,   xchg_r2r0_v,   xchg_r2r0_v,   xchg_r2r0_v,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	mov_moffs2a_b, mov_moffs2a_v, mov_a2moffs_b, mov_a2moffs_v,
/* 0xa4 */	movs_b,        movs_v,        inv, inv,
/* 0xa8 */	inv, inv, inv, inv,
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	mov_i2r_b,     mov_i2r_b,     mov_i2r_b,     mov_i2r_b,
/* 0xb4 */	mov_i2r_b,     mov_i2r_b,     mov_i2r_b,     mov_i2r_b,
/* 0xb8 */	mov_i2r_v,     mov_i2r_v,     mov_i2r_v,     mov_i2r_v, 
/* 0xbc */	mov_i2r_v,     mov_i2r_v,     mov_i2r_v,     mov_i2r_v, 
/* 0xc0 */	ALI_4_ib_b,    ALI_4_ib_v,    ret_iw,        ret,
/* 0xc4 */	inv, inv,                     mov_i2rm_b,    mov_i2rm_v,
/* 0xc8 */	inv,           leave, inv, inv,
/* 0xcc */	int3,          INT,           inv,           iret,
/* 0xd0 */	ALI_4_b,       ALI_4_v,       ALI_4_CL_b,    ALI_4_CL_v,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	call_i,        jmp_i_v,       jmp_seg,     jmp_i_b,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv,                               rep,
/* 0xf4 */	inv, inv,                     ALI_2_b,       ALI_2_v,
/* 0xf8 */	inv, inv,                     cli,           inv,
/* 0xfc */	cld, inv,                     ALI_1_b,       ALI_1_v
};


make_helper(exec) 
{ 	return  opcode_table[ instr_fetch(eip, 1) ](eip); }

make_helper(data_size)
{
	suffix = 'w';
	int instr_len = exec(eip + 1);
	suffix = 'l';
	return instr_len + 1;
}

make_helper(tran_mean)
{
	uint8_t inst = instr_fetch(eip+1, 1);
	if ( inst == 0x01 )  return  1 + SI_v(eip + 1);
	if ( inst == 0x20 )  return  1 + mov_cr2r(eip + 1);
	if ( inst == 0x22 )  return  1 + mov_r2cr(eip + 1);
	if ( inst == 0x82 )  return  1 + jb_v(eip + 1);
	if ( inst == 0x83 )  return  1 + jae_v(eip + 1);
	if ( inst == 0x84 )  return  1 + je_v(eip + 1);
	if ( inst == 0x86 )  return  1 + jbe_v(eip + 1);
	if ( inst == 0x87 )  return  1 + ja_v(eip + 1);
	if ( inst == 0x8c )  return  1 + jl_v(eip + 1);
	if ( inst == 0x8d )  return  1 + jge_v(eip + 1);
	if ( inst == 0x8e )  return  1 + jle_v(eip + 1);
	if ( inst == 0x8f )  return  1 + jg_v(eip + 1);
	if ( inst == 0x95 )  return  1 + setne(eip + 1);
	if ( inst == 0xaf )  return  1 + imul_rm2r_v(eip + 1);
	if ( inst == 0xb6 )  return  1 + movzbv(eip + 1);
	if ( inst == 0xb7 )  return  1 + movzwl(eip + 1);
	if ( inst == 0xbe )  return  1 + movsbv(eip + 1);
	if ( inst == 0xbf )  return  1 + movswl(eip + 1);
	assert(0);	return 0;
}
