/*! \file Array1D.h
    \brief Contains the definition of a resizable 1D array.
    \author Wojciech Jarosz
*/
#ifndef UTIL_ARRAY1D_H
#define UTIL_ARRAY1D_H

namespace Util
{

//! Generic, resizable, 1D array class.
template <typename T>
class Array1D
{
public:
    //@{ \name Constructors and destructors
    Array1D();          // empty array, 0 elements
    Array1D(int sizeX); // sizeX elements
    ~Array1D ();
    //@}

    //@{ \name Element access
    T &         operator()(int x);
    const T &   operator()(int x) const;
    T &         operator[](int i);
    const T &   operator[](int i) const;
    //@}
    
    //@{ \name Dimension sizes
    int width()  const { return m_sizeX; }
    
    int size()  const { return m_sizeX; }
    int sizeX() const { return m_sizeX; }
    //@}

    T* setData(T * data, int sizeX);
    void resizeErase(int sizeX);
    void reset(const T& value = T(0));
    void operator=(const T&);

// protected:
    T * m_data;
    int m_sizeX;

private:
    Array1D (const Array1D &);              // Copying and assignment
    Array1D & operator = (const Array1D &); // are not implemented
};

template <typename T>
inline
Array1D<T>::Array1D():
    m_data(0), m_sizeX(0)
{
    // empty
}


template <typename T>
inline
Array1D<T>::Array1D(int sizeX):
    m_data(new T[sizeX]), m_sizeX(sizeX)
{
    // empty
}


template <typename T>
inline
Array1D<T>::~Array1D ()
{
    delete [] m_data;
}


template <typename T>
inline T &
Array1D<T>::operator()(int x)
{
    return m_data[x];
}


template <typename T>
inline const T &
Array1D<T>::operator()(int x) const
{
    return m_data[x];
}


template <typename T>
inline T &
Array1D<T>::operator[](int i)
{
    return m_data[i];
}


template <typename T>
inline const T &
Array1D<T>::operator[](int i) const
{
    return m_data[i];
}


template <typename T>
inline T*
Array1D<T>::setData(T * data, int sizeX)
{
    T* oldData = m_data;
    m_data = data;
    m_sizeX = sizeX;
    return oldData;
}


template <typename T>
inline void
Array1D<T>::resizeErase(int sizeX)
{
    if (sizeX == m_sizeX)
        return;

    delete [] m_data;
    m_data = new T[sizeX];
    m_sizeX = sizeX;
}


template <typename T>
inline void
Array1D<T>::reset(const T& value)
{
    for (int i = 0; i < m_sizeX; i++)
        m_data[i] = value;
}


template <typename T>
inline void
Array1D<T>::operator=(const T& value)
{
    reset(value);
}

} // namespace Util

#endif // UTIL_ARRAY1D_H


