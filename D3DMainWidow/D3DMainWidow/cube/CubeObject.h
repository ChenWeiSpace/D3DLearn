#ifndef CubeObject_h__
#define CubeObject_h__

#include <memory>
#include "../AbstractVisualObject.h"

class CubeObject : public AbstractVisualObject
{
public:
	CubeObject();
	~CubeObject();
public:
	void inits(Context & con) override;
	void frameMove(std::uint64_t frameNumber, std::uint64_t elapsed) override;
};

#endif // CubeObject_h__
