#ifndef VECTOR_HPP_INCLUDED
#define	VECTOR_HPP_INCLUDED

#include "vector.h"

	class Vector_base
	{
	public:
		explicit Vector_base() {}
		virtual ~Vector_base() {}

	};

	template<typename T, int DIM>
	class Vector : public Vector_base
	{
		typedef Vector<T, DIM> ME;
	public:
		explicit Vector()
		{
			memset(&v, 0, sizeof(v));
		}

		explicit Vector(T v[DIM])
		{
			memcpy(this->v, v, sizeof(this->v));
		}

		explicit Vector(T, T, T);

		explicit Vector(Vector3 rhs)
		{
			v[0] = rhs.x;
			v[1] = rhs.y;
			v[2] = rhs.z;
		}

		virtual ~Vector()
		{

		}

		// Operators

		T &operator [](size_t n)
		{
			return v[n];
		}

		T operator [](size_t n) const
		{
			return v[n];
		}

		ME &operator += (const ME& o)
		{
			for (size_t i = 0; i<DIM; ++i)
			{
				v[i] += o[i];
			}
			return *this;
		}

		ME &operator -= (const ME& o)
		{
			for (size_t i = 0; i<DIM; ++i)
			{
				v[i] -= o[i];
			}
			return *this;
		}

		ME &operator *= (const ME& o)
		{
			for (size_t i = 0; i<DIM; ++i)
			{
				v[i] *= o[i];
			}
			return *this;
		}

		ME &operator /= (const ME& o)
		{
			for (size_t i = 0; i<DIM; ++i)
			{
				v[i] /= o[i];
			}
			return *this;
		}


		ME &operator *= (float scalar)
		{
			for (size_t i = 0; i<DIM; ++i)
			{
				v[i] *= scalar;
			}
			return *this;
		}

		ME &operator /= (float scalar)
		{
			for (size_t i = 0; i<DIM; ++i)
			{
				v[i] /= scalar;
			}
			return *this;
		}


		const ME operator-(const ME &o) const
		{
			ME result = *this;
			result -= o;
			return result;
		}

		const ME operator+(const ME &o) const
		{
			ME result = *this;
			result += o;
			return result;
		}

		const ME operator*(const ME &o) const
		{
			ME result = *this;
			result *= o;
			return result;
		}

		const ME operator*(float scalar) const
		{
			ME result = *this;
			result *= scalar;
			return result;
		}

		const ME operator/(const ME &o) const
		{
			ME result = *this;
			result /= o;
			return result;
		}

		const ME operator/(float scalar) const
		{
			ME result = *this;
			result /= scalar;
			return result;
		}



		// Misc stuff
		ME &get()
		{
			return v;
		}

		float length()
		{
			float temp = 0.0f;

			for (size_t i = 0; i<DIM; ++i)
			{
				temp += v[i] * v[i];
			}

			return sqrtf(temp);
		}

		void normalize()
		{
			float len = length();

			for (size_t i = 0; i<DIM; ++i)
			{
				v[i] /= len;
			}

		}

		// Other than Vector3 gives linker errors
		float dot(const ME &v) const;


	private:
		T v[DIM];
	};


	typedef Vector<float, 3> KVector3;



	

#endif // VECTOR_HPP_INCLUDED


