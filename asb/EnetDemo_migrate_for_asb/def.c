
typedef unsigned long u_int32;

extern void    board_disable_intrs(void);
extern void    WPI_SimulateInterrupts(void);
extern void    board_enable_intrs(void);
#if 0
void WT_lxFwd_SimulateInterrupts_ISR(u_int32 dev_num);

void WT_lxFwd_SimulateInterrupts_ISR(u_int32 dev_num)
{
	u_int32 iii = 0;
     iii = dev_num;

    board_disable_intrs();
    WPI_SimulateInterrupts();
    board_enable_intrs();
}
#endif

