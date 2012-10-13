#ifndef __ORYXAPP_H__
#define __ORYXAPP_H__

class OryxApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
};


#endif