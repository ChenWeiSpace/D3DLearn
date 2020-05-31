#include "GlobalInstance.h"
#include <algorithm>
GlobalInstance::GlobalInstance()
{

}

GlobalInstance::~GlobalInstance()
{

}

GlobalInstance* GlobalInstance::GetInstance()
{
	static GlobalInstance s_ins;
	return &s_ins;
}

void GlobalInstance::addObjcet(AbstractVisualObjectPtr obj)
{
	if (std::find(m_addObject.begin(), m_addObject.end(), obj) == m_addObject.end())
	{
		m_addObject.push_back(obj);
	}
}

std::vector<AbstractVisualObjectPtr> GlobalInstance::getObjs()
{
	return m_addObject;
}
