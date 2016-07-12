/*! \file Array2D.h
    \brief Contains the definition of a resizable 2D array.
    \author Wojciech Jarosz
*/

#pragma once

#include <algorithm>
#include "util/memory.h"

namespace Util {

	//! Generic, resizable, 2D array class.
	template <typename T>
	class Array2D {
		public:
			Array2D();                     // empty array, 0 by 0 elements
			Array2D(int sizeX, int sizeY); // sizeX by sizeY elements
			~Array2D ();

			Array2D(const Array2D &) = delete;
			Array2D & operator = (const Array2D &) = delete;
			
			T&         operator()(int x, int y);
			const T&   operator()(int x, int y) const;
			T&         operator[](int i);
			const T&   operator[](int i) const;
			
			int getWidth()  const { return sizeX; }
			int getHeight() const { return sizeY; }
    
			int getSize()  const { return sizeX * sizeY; }
			int getSizeX() const { return sizeX; }
			int getSizeY() const { return sizeY; }
			
			std::unique_ptr<T>&& setData(std::unique_ptr<T>&& data, int sizeX, int sizeY);
			void resizeErase(int sizeX, int sizeY);
			void reset(const T& value = T(0));
			void operator=(const T&);

		private:
			std::unique_ptr<T[]> data;
			int sizeX;
			int sizeY;
	};

	template <typename T>
	inline Array2D<T>::Array2D():
			data(nullptr), sizeX(0), sizeY(0) { }


	template <typename T>
	inline Array2D<T>::Array2D(int sizeX, int sizeY):
		data(new T[sizeX * sizeY]), sizeX(sizeX), sizeY(sizeY) { }


	template <typename T>
	inline Array2D<T>::~Array2D () { } // Needs to be here because of destructor of unique_ptr


	template <typename T>
	inline T& Array2D<T>::operator()(int x, int y) {
		return data[y*sizeX + x];
	}


	template <typename T>
	inline const T& Array2D<T>::operator()(int x, int y) const {
		return data[y*sizeX + x];
	}


	template <typename T>
	inline T& Array2D<T>::operator[](int i) {
		return data[i];
	}


	template <typename T>
	inline const T& Array2D<T>::operator[](int i) const {
		return data[i];
	}


	template <typename T>
	inline std::unique_ptr<T>&& Array2D<T>::setData(std::unique_ptr<T>&& data, int sizeX, int sizeY) {
		auto oldData = std::move(this->data);
		this->data = data;
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		return std::move(oldData);
	}


	template <typename T>
	inline void Array2D<T>::resizeErase(int sizeX, int sizeY) {
		if (sizeX == this->sizeX && sizeY == this->sizeY)
			return;

		data.reset(new T[sizeX * sizeY]);
		this->sizeX = sizeX;
		this->sizeY = sizeY;
	}


	template <typename T>
	inline void Array2D<T>::reset(const T& value) {
		std::fill(data.get(), data.get() + getSize(), value);
	}


	template <typename T>
	inline void Array2D<T>::operator=(const T& value) {
		reset(value);
	}

} // namespace Util
