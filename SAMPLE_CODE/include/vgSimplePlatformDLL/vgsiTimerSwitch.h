


#ifndef __VGSITIMERSWITCH_H__
#define __VGSITIMERSWITCH_H__

#include <vgKernel/vgkForward.h>

	

/**
	���ڽ��dialog��������������
*/
class TimerSwitch
{
public:
	TimerSwitch();
	~TimerSwitch();


private:

	CView* _activeView;
};

#define VGSI_TIMER_SWITCH()  TimerSwitch __timerswitch


#endif // end of __VGSITIMERSWITCH_H__