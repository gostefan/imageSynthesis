/*! \file Array1D.h
    \brief Contains the definition of a resizable 1D array.
    \author Wojciech Jarosz
*/

#pragma once

#include <memory>

namespace Util {
	//! Generic, resizable, 1D array class.
	template <typename T>
	class Array1D {
		public:
			Array1D();          // empty array, 0 elements
			Array1D(int sizeX); // sizeX elements

			Array1D (const Array1D &) = delete;
			Array1D& operator= (const Array1D &) = delete;
			
			T &         operator()(int x);
			const T &   operator()(int x) const;
			T &         operator[](int i);
			const T &   operator[](int i) const;
			
			int getWidth()  const { return sizeX; }
    
			int getSize()  const { return sizeX; }
			int getSizeX() const { return sizeX; }
			
			std::unique_ptr<T>&& setData(std::unique_ptr<T>&& data, int sizeX);
			void resizeErase(int sizeX);
			void reset(const T& value = T(0));
			void operator=(const T&);

		private:
			std::unique_ptr<T[]> data;
			int sizeX;
	};

	template <typename T>
	inline Array1D<T>::Array1D():
			sizeX(0) { }


	template <typename T>
	inline Array1D<T>::Array1D(int sizeX):
			data(new T[sizeX]), sizeX(sizeX) { }
	

	template <typename T>
	inline T& Array1D<T>::operator()(int x) {
		return data[x];
	}


	template <typename T>
	inline const T& Array1D<T>::operator()(int x) const {
		return data[x];
	}


	template <typename T>
	inline T& Array1D<T>::operator[](int i) {
		return data[i];
	}


	template <typename T>
	inline const T& Array1D<T>::operator[](int i) const {
		return data[i];
	}


	template <typename T>
	inline std::unique_ptr<T>&& Array1D<T>::setData(std::unique_ptr<T>&& data, int sizeX) {
		auto oldData = std::move(this->data);
		this->data = data;
		this->sizeX = sizeX;
		return std::move(oldData);
	}


	template <typename T>
	inline void Array1D<T>::resizeErase(int sizeX) {
		if (sizeX == this->sizeX)
			return;

		this->data.reset(new T[sizeX]);
		this->sizeX = sizeX;
	}


	template <typename T>
	inline void Array1D<T>::reset(const T& value) {
		std::fill(data.get(), data.get() + size(), value);
	}


	template <typename T>
	inline void Array1D<T>::operator=(const T& value) {
		reset(value);
	}

} // namespace Util
