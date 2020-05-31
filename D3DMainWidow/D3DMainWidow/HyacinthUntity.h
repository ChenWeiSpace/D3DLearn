#ifndef HyacinthUntity_H
#define HyacinthUntity_H
#include <d3dx9.h>

template<typename T>
inline void SafeRelease(T* p)
{
	if (0 != p)
	{
		p->Release();
	}
}

template<typename T>
inline void SafeAddRef(T* p)
{
	if (0 != p)
	{
		p->AddRef();
	}
}

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

template <typename T> 
struct Vector2
{
	Vector2() {
		x = 0;
		y = 0;
		
	}

	Vector2(T w, T h) {
		width = w;
		height = h;
	}

	union
	{
		struct
		{
			T x;
			T y;
		};
		struct
		{
			T width;
			T height;
		};
	};

	bool operator==(const Vector2& that)const
	{
		return y == that.y && x == that.x;
	}
	bool operator!=(const Vector2& that)const
	{
		return !operator==(that);
	}
	T size() { return x * y; }
};
typedef Vector2<int> Size;
typedef Vector2<float> SizeF;

static  const 	D3DXVECTOR3 s_xAxis{ 1,0,0 };
static  const 	D3DXVECTOR3 s_yAxis{ 0,1,0 };
static  const 	D3DXVECTOR3 s_zAxis{ 0,0,1 };
#endif // !HyacinthUntity_H


