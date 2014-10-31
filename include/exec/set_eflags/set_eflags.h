#ifndef __SET_EFLAGS_H__
#define __SET_EFLAGS_H__


void set_CF_b(uint8_t,  uint8_t,  uint8_t);
void set_CF_w(uint16_t, uint16_t, uint16_t);
void set_CF_l(uint32_t, uint32_t, uint32_t);

void set_PF_b(uint8_t);
void set_PF_w(uint16_t);
void set_PF_l(uint32_t);

void set_ZF_b(uint8_t);
void set_ZF_w(uint16_t);
void set_ZF_l(uint32_t);

void set_SF_b(uint8_t);
void set_SF_w(uint16_t);
void set_SF_l(uint32_t);

void set_OF_b(uint8_t,  uint8_t,  uint8_t);
void set_OF_w(uint16_t, uint16_t, uint16_t);
void set_OF_l(uint32_t, uint32_t, uint32_t);

#endif
