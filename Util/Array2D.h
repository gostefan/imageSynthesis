/*! \file Array2D.h
    \brief Contains the definition of a resizable 2D array.
    \author Wojciech Jarosz
*/
#ifndef UTIL_ARRAY2D_H
#define UTIL_ARRAY2D_H

namespace Util
{

//! Generic, resizable, 2D array class.
template <typename T>
class Array2D
{
public:
    //@{ \name Constructors and destructors
    Array2D();                     // empty array, 0 by 0 elements
    Array2D(int sizeX, int sizeY); // sizeX by sizeY elements
    ~Array2D ();
    //@}

    //@{ \name Element access
    T &         operator()(int x, int y);
    const T &   operator()(int x, int y) const;
    T &         operator[](int i);
    const T &   operator[](int i) const;
    //@}
    
    //@{ \name Dimension sizes
    int width()  const { return m_sizeX; }
    int height() const { return m_sizeY; }
    
    int size()  const { return m_sizeX*m_sizeY; }
    int sizeX() const { return m_sizeX; }
    int sizeY() const { return m_sizeY; }
    //@}

    T* setData(T * data, int sizeX, int sizeY);
    void resizeErase(int sizeX, int sizeY);
    void reset(const T& value = T(0));
    void operator=(const T&);

// protected:
    T * m_data;
    int m_sizeX;
    int m_sizeY;

private:
    Array2D (const Array2D &);              // Copying and assignment
    Array2D & operator = (const Array2D &); // are not implemented
};

template <typename T>
inline
Array2D<T>::Array2D():
    m_data(0), m_sizeX(0), m_sizeY(0)
{
    // empty
}


template <typename T>
inline
Array2D<T>::Array2D(int sizeX, int sizeY):
    m_data(new T[sizeX * sizeY]), m_sizeX(sizeX), m_sizeY(sizeY)
{
    // empty
}


template <typename T>
inline
Array2D<T>::~Array2D ()
{
    delete [] m_data;
}


template <typename T>
inline T &
Array2D<T>::operator()(int x, int y)
{
    return m_data[y*m_sizeX + x];
}


template <typename T>
inline const T &
Array2D<T>::operator()(int x, int y) const
{
    return m_data[y*m_sizeX + x];
}


template <typename T>
inline T &
Array2D<T>::operator[](int i)
{
    return m_data[i];
}


template <typename T>
inline const T &
Array2D<T>::operator[](int i) const
{
    return m_data[i];
}


template <typename T>
inline T*
Array2D<T>::setData(T * data, int sizeX, int sizeY)
{
    T* oldData = m_data;
    m_data = data;
    m_sizeX = sizeX;
    m_sizeY = sizeY;
    return oldData;
}


template <typename T>
inline void
Array2D<T>::resizeErase(int sizeX, int sizeY)
{
    if (sizeX == m_sizeX && sizeY == m_sizeY)
        return;

    delete [] m_data;
    m_data = new T[sizeX * sizeY];
    m_sizeX = sizeX;
    m_sizeY = sizeY;
}


template <typename T>
inline void
Array2D<T>::reset(const T& value)
{
    int size = m_sizeX*m_sizeY;
    for (int i = 0; i < size; i++)
        m_data[i] = value;
}


template <typename T>
inline void
Array2D<T>::operator=(const T& value)
{
    reset(value);
}

} // namespace Util

#endif // UTIL_ARRAY2D_H


