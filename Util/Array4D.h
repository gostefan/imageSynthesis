/*! \file Array4D.h
    \brief Contains the definition of a resizable 4D array.
    \author Wojciech Jarosz
*/
#pragma once

namespace Util {
	//! Generic, resizable, 4D array class.
	template <typename T>
	class Array4D {
		public:
			Array4D();
			Array4D(int sizeX, int sizeY, int sizeZ, int sizeW);
			~Array4D();

			Array4D(const Array4D&) = delete;
			Array4D & operator=(const Array4D&) = delete;

			T&         operator()(int x, int y, int z, int w);
			const T&   operator()(int x, int y, int z, int w) const;
			T&         operator[](int i);
			const T&   operator[](int i) const;

			int width()  const { return sizeX; }
			int height() const { return sizeY; }
			int depth()  const { return sizeZ; }
			int length() const { return sizeW; }
    
			int size()  const { return sizeX * sizeY * sizeZ * sizeW; }
			int sizeX() const { return sizeX; }
			int sizeY() const { return sizeY; }
			int sizeZ() const { return sizeZ; }
			int sizeW() const { return sizeW; }

			void resizeErase(int sizeX, int sizeY, int sizeZ, int sizeW);
			void reset(const T& value = T(0));
			void operator=(const T&);

		protected:
			std::unique_ptr<T[]> data;
			int sizeX;
			int sizeY;
			int sizeZ;
			int sizeW;

		private:
	};

	template <typename T>
	inline Array4D<T>::Array4D():
			data(nullptr), sizeX(0), sizeY(0), sizeZ(0), sizeW(0) { }

	template <typename T>
	inline Array4D<T>::Array4D(int sizeX, int sizeY, int sizeZ, int sizeW) :
			m_data(new T[sizeX * sizeY * sizeZ * sizeW]), m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ), m_sizeW(sizeW) { }

	template <typename T>
	inline Array4D<T>::~Array4D() { }

	template <typename T>
	inline T& Array4D<T>::operator()(int x, int y, int z, int w) {
		return data[w*m_sizeX*m_sizeY*m_sizeZ +
					z*m_sizeX*m_sizeY + y*m_sizeX + x];
	}

	template <typename T>
	inline const T& Array4D<T>::operator()(int x, int y, int z, int w) const {
		return data[w*m_sizeX*m_sizeY*m_sizeZ +
					z*m_sizeX*m_sizeY + y*m_sizeX + x];
	}

	template <typename T>
	inline T& Array4D<T>::operator[](int i) {
		return data[i];
	}

	template <typename T>
	inline const T& Array4D<T>::operator[](int i) const {
		return data[i];
	}

	template <typename T>
	inline void Array4D<T>::resizeErase(int sizeX, int sizeY, int sizeZ, int sizeW) {
		if (sizeX == this->sizeX && sizeY == this->sizeY &&
			sizeZ == this->sizeZ && sizeW == this->sizeW)
			return;

		data.reset(new T[sizeX * sizeY * sizeZ * sizeW]);
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		this->sizeZ = sizeZ;
		this->sizeW = sizeW;
	}

	template <typename T>
	inline void Array4D<T>::reset(const T& value) {
		std::fill(data.get(), data.get() + getSize(), value);
	}

	template <typename T>
	inline void Array4D<T>::operator=(const T& value) {
		reset(value);
	}
} // namespace Util


