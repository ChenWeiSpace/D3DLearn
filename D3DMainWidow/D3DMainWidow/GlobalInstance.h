#ifndef GlobalInstance_h__
#define GlobalInstance_h__
#include <vector>
#include "AbstractVisualObject.h"

class GlobalInstance
{
public:
	GlobalInstance();
	~GlobalInstance();

	static GlobalInstance* GetInstance();
	void addObjcet(AbstractVisualObjectPtr obj);
	std::vector<AbstractVisualObjectPtr> getObjs();
private:
	std::vector<AbstractVisualObjectPtr> m_addObject;
};


template <typename T>
class AddVisualObjct
{
public:
	AddVisualObjct()
	{
		GlobalInstance::GetInstance()->addObjcet(std::make_shared<T>());
	}
};
#define REGIST_VISUALOBJCT(OBJ) static AddVisualObjct<OBJ> s_##OBJ
#endif // GlobalInstance_h__
