#ifndef HyacinthUntity_H
#define HyacinthUntity_H

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
#endif // !HyacinthUntity_H


