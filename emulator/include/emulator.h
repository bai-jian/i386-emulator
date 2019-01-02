#ifndef __EMULATOR_H__
#define __EMULATOR_H__

typedef uint32_t swaddr_t;
typedef uint16_t ioaddr_t;
typedef uint32_t phyaddr_t;
typedef uint32_t offset_t;

enum { END, STOP, RUNNING, INT };
int nemu_state;

#endif
