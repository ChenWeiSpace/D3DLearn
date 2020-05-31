#ifndef SphereObject_h__
#define SphereObject_h__
#include "..\AbstractVisualObject.h"
#include <vector>
#include "..\HyacinthUntity.h"

class UniformGridPlane : public std::enable_shared_from_this<UniformGridPlane>
{
public:
	UniformGridPlane();
	~UniformGridPlane();
	void setPosList(std::vector<VertexP_S_T> & posVec);
	std::vector<VertexP_S_T>& getPosList();
	inline void setSize(Size size) { m_size = size; }
	void buildIndexList(std::vector<unsigned int> & indexList);
private:
	std::vector<VertexP_S_T> posVector;
	Size m_size{0,0};
	std::vector<unsigned int> m_indexList;
};
typedef std::shared_ptr<UniformGridPlane> UniformGridPlanePtr;


class SphereObject : public StaticAbstractVisualObject
{
public:
	SphereObject();
	~SphereObject();

	virtual void inits(Context& con) override;
	virtual void frameMove(std::uint64_t frameNumber, std::uint64_t elapsed) override;
private:
	int m_indexSize{0};
};

#endif // SphereObject_h__
