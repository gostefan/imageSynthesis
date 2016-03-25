/*! \file Array3D.h
    \brief Contains the definition of a resizable 3D array.
    \author Wojciech Jarosz
*/
#ifndef UTIL_ARRAY3D_H
#define UTIL_ARRAY3D_H

namespace Util
{

//! Generic, resizable, 3D array class.
template <typename T>
class Array3D
{
public:
    //@{ \name Constructors and destructors
    Array3D();
    Array3D(int sizeX, int sizeY, int sizeZ);
    ~Array3D();
    //@}

    //@{ \name Element access
    T &         operator()(int x, int y, int z);
    const T &   operator()(int x, int y, int z) const;
    T &         operator[](int i);
    const T &   operator[](int i) const;
    //@}
    
    //@{ \name Dimension sizes
    int width()  const { return m_sizeX; }
    int height() const { return m_sizeY; }
    int depth()  const { return m_sizeZ; }
    
    int size()  const { return m_sizeX*m_sizeY*m_sizeZ; }
    int sizeX() const { return m_sizeX; }
    int sizeY() const { return m_sizeY; }
    int sizeZ() const { return m_sizeZ; }
    //@}

    void resizeErase(int sizeX, int sizeY, int sizeZ);
    void reset(const T& value = T(0));
    void operator=(const T&);

protected:
    T * m_data;
    int m_sizeX;
    int m_sizeY;
    int m_sizeZ;

private:
    Array3D(const Array3D &);              // Copying and assignment
    Array3D & operator=(const Array3D &); // are not implemented
};

template <typename T>
inline
Array3D<T>::Array3D():
    m_data(0), m_sizeX(0), m_sizeY(0), m_sizeZ(0)
{
    // empty
}


template <typename T>
inline
Array3D<T>::Array3D(int sizeX, int sizeY, int sizeZ) :
    m_data(new T[sizeX * sizeY * sizeZ]),
    m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ)
{
    // empty
}


template <typename T>
inline
Array3D<T>::~Array3D()
{
    delete [] m_data;
}


template <typename T>
inline T &
Array3D<T>::operator()(int x, int y, int z)
{
    return m_data[z*m_sizeX*m_sizeY + y*m_sizeX + x];
}


template <typename T>
inline const T &
Array3D<T>::operator()(int x, int y, int z) const
{
    return m_data[z*m_sizeX*m_sizeY + y*m_sizeX + x];
}


template <typename T>
inline T &
Array3D<T>::operator[](int i)
{
    return m_data[i];
}


template <typename T>
inline const T &
Array3D<T>::operator[](int i) const
{
    return m_data[i];
}


template <typename T>
inline void
Array3D<T>::resizeErase(int sizeX, int sizeY, int sizeZ)
{
    if (sizeX == m_sizeX && sizeY == m_sizeY && sizeZ == m_sizeZ)
        return;

    delete [] m_data;
    m_data = new T[sizeX * sizeY * sizeZ];
    m_sizeX = sizeX;
    m_sizeY = sizeY;
    m_sizeZ = sizeZ;
}


template <typename T>
inline void
Array3D<T>::reset(const T& value)
{
    int size = m_sizeX*m_sizeY*m_sizeZ;
    for (int i = 0; i < size; i++)
        m_data[i] = value;
}


template <typename T>
inline void
Array3D<T>::operator=(const T& value)
{
    reset(value);
}

} // namespace Util

#endif // UTIL_ARRAY3D_H


