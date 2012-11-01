#ifndef __BLOCK_SPECIAL__
#define __BLOCK_SPECIAL__

#include "PowerUpBase.h"
// block special execute the anihilation of the blocks
class BlockSpecial : public PowerUpBase
{
public:
	// create method, copied from CCNode class
	static BlockSpecial* create(int kind);
	// contructor
	BlockSpecial(int kind);
	// 'dealloc'
	~BlockSpecial(void);
private:
	void Execute();
	int kind;
};


#endif // __BLOCK_SPECIAL__