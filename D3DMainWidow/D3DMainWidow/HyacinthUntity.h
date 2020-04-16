#ifndef HyacinthUntity_H
#define HyacinthUntity_H
#include <d3dx9.h>
struct EulerAngle
{
	EulerAngle() {
		heading = 0;
		pitch = 0;
		roll = 0;
	}

	EulerAngle(float h, float p, float r) {
		heading = h;
		pitch = p;
		roll = r;
	}

	union 
	{
		struct 
		{
			float heading;
			float pitch;
			float roll;
		};
		struct
		{
			float x;
			float y;
			float z;
		};
	};
	bool operator==(const EulerAngle& that)const
	{
		return z == that.z && y == that.y && x == that.x;
	}
	bool operator!=(const EulerAngle& that)const
	{
		return !operator==(that);
	}
};
static  const 	D3DXVECTOR3 s_xAxis{ 1,0,0 };
static  const 	D3DXVECTOR3 s_yAxis{ 0,1,0 };
static  const 	D3DXVECTOR3 s_zAxis{ 0,0,1 };
#endif // !HyacinthUntity_H


