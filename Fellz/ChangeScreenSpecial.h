

#ifndef __CHANGE_SCREEN_SPECIAL__
#define __CHANGE_SCREEN_SPECIAL__


#include "PowerUpBase.h"
#include "RaknetGlobals.h"
// change screen special implements the functionality of the "swap level"  feature
class ChangeScreenSpecial : public PowerUpBase
{
public:
	// create method, copied from CCNode class
	static ChangeScreenSpecial* create();

	//constructor 
	ChangeScreenSpecial();
	// delloc
	~ChangeScreenSpecial(void);

private:
	void Execute();
};

#endif //__CHANGE_SCREEN_SPECIAL__