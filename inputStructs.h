#ifndef _INPUTSTRUCTS_H_
#define _INPUTSTRUCTS_H_

#include <XInput.h>

struct Thumb
{
	float x;
	float y;
	float magnitude;
};

struct Controller
{
	XINPUT_STATE state;
	Thumb leftThumb;
	Thumb rightThumb;
	float leftTrigger;
	float rightTrigger;
	bool enabled;
};

#endif//_INPUTSTRUCTS_H_

/*
Change Log:

*/