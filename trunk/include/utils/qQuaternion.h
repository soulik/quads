#ifndef H_QUATERNION
#define H_QUATERNION

class qQuaternion {
private:
	float x, y, z, w;
public:
	qQuaternion() { x = y = z = 0.0f;   w = 1.0f;  }

	qQuaternion(float X, float Y, float Z, float W){ 
		x = X;		y = Y;		z = Z;		w = W;
	}

	void CreateMatrix(float *pMatrix);
	void CreateFromMatrix(float *pTheMatrix, int rowColumnCount);
	qQuaternion Slerp(qQuaternion &q1, qQuaternion &q2, float t);
};

#endif