/*! \file Array3D.h
    \brief Contains the definition of a resizable 3D array.
    \author Wojciech Jarosz
*/

#pragma once

namespace Util {

	//! Generic, resizable, 3D array class.
	template <typename T>
	class Array3D {
		public:
			Array3D();
			Array3D(int sizeX, int sizeY, int sizeZ);
			~Array3D();

			Array3D(const Array3D&) = delete;
			Array3D & operator=(const Array3D&) = delete;

			T&         operator()(int x, int y, int z);
			const  &   operator()(int x, int y, int z) const;
			T&         operator[](int i);
			const T&   operator[](int i) const;

			int getWidth()  const { return sizeX; }
			int getHeight() const { return sizeY; }
			int getDepth()  const { return sizeZ; }
    
			int getSize()  const { return sizeX * sizeY * sizeZ; }
			int getSizeX() const { return sizeX; }
			int getSizeY() const { return sizeY; }
			int getSizeZ() const { return sizeZ; }

			void resizeErase(int sizeX, int sizeY, int sizeZ);
			void reset(const T& value = T(0));
			void operator=(const T&);

		private:
			std::unique_ptr<T[]> data;
			int sizeX;
			int sizeY;
			int sizeZ;
	};

	template <typename T>
	inline Array3D<T>::Array3D():
			data(nullptr), sizeX(0), sizeY(0), sizeZ(0) { }


	template <typename T>
	inline Array3D<T>::Array3D(int sizeX, int sizeY, int sizeZ) :
			data(new T[sizeX * sizeY * sizeZ]), sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) { }


	template <typename T>
	inline Array3D<T>::~Array3D() { }


	template <typename T>
	inline T& Array3D<T>::operator()(int x, int y, int z) {
		return data[z*sizeX*sizeY + y*sizeX + x];
	}


	template <typename T>
	inline const T& Array3D<T>::operator()(int x, int y, int z) const {
		return data[(z * sizeY + y) * sizeX + x];
	}


	template <typename T>
	inline T& Array3D<T>::operator[](int i) {
		return data[i];
	}


	template <typename T>
	inline const T& Array3D<T>::operator[](int i) const {
		return data[i];
	}


	template <typename T>
	inline void Array3D<T>::resizeErase(int sizeX, int sizeY, int sizeZ) {
		if (sizeX == this->sizeX && sizeY == this->sizeY && sizeZ == this->sizeZ)
			return;

		data.reset(new T[sizeX * sizeY * sizeZ]);
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		this->sizeZ = sizeZ;
	}


	template <typename T>
	inline void Array3D<T>::reset(const T& value) {
		std::fill(data.get(), data.get() + getSize(), value);
	}

	template <typename T>
	inline void Array3D<T>::operator=(const T& value) {
		reset(value);
	}

} // namespace Util
