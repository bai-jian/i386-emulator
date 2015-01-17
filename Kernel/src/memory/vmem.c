#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)

/* Use the function to get the start address of user page directory. */
inline PDE* get_updir();

// Mapping 
// from virtual  memory area [0xa0000, 0xa0000 + SCR_SIZE)
// to   physical memory area [0xa0000, 0xa0000 + SCR_SIZE)
/** Video Memory ******************************************/
/*                                                        */
/*   ** Starting Address: 0x000A0000 ****************     */
/*   *  +------------+------------+--------------+  *     */
/*   *  |0000 0000 00|00 1010 0000|0000 0000 0000|  *     */
/*   *  +------------+------------+--------------+  *     */
/*   *    PDI = 000H   PTI = 0A0H     PO = 000H     *     */
/*   ************************************************     */
/*                                                        */
/*   ** Ending Address  : 0x000AFA00 ****************     */
/*   *  +------------+------------+--------------+  *     */
/*   *  |0000 0000 00|00 1010 1111|1010 0000 0000|  *     */
/*   *  +------------+------------+--------------+  *     */
/*   *    PDI = 000H   PTI = 0AFH     PO = A00H     *     */
/*   ************************************************     */
/*                                                        */
/**********************************************************/
void create_video_mapping( )
{
	// fill some PDEs and PTEs
/*
	pdir[0].val = make_pde

	uint32_t pdir_idx, ptable_idx, pframe_idx = 0;
	for (
*/
	return ;

}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	} 
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	} 
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

