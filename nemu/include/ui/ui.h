#ifndef __UI_H__
#define __UI_H__

/*
nemu_state:
	END:
	STOP:
	RUNNING:
	INT:
*/
enum { END, STOP, RUNNING, INT };
extern int nemu_state;

#endif
