/*! \file Array4D.h
    \brief Contains the definition of a resizable 4D array.
    \author Wojciech Jarosz
*/
#ifndef UTIL_ARRAY4D_H
#define UTIL_ARRAY4D_H

namespace Util
{

//! Generic, resizable, 4D array class.
template <typename T>
class Array4D
{
public:
    //@{ \name Constructors and destructors
    Array4D();
    Array4D(int sizeX, int sizeY, int sizeZ, int sizeW);
    ~Array4D();
    //@}

    //@{ \name Element access
    T &         operator()(int x, int y, int z, int w);
    const T &   operator()(int x, int y, int z, int w) const;
    T &         operator[](int i);
    const T &   operator[](int i) const;
    //@}
    
    //@{ \name Dimension sizes
    int width()  const { return m_sizeX; }
    int height() const { return m_sizeY; }
    int depth()  const { return m_sizeZ; }
    int length() const { return m_sizeW; }
    
    int size()  const { return m_sizeX*m_sizeY*m_sizeZ*m_sizeW; }
    int sizeX() const { return m_sizeX; }
    int sizeY() const { return m_sizeY; }
    int sizeZ() const { return m_sizeZ; }
    int sizeW() const { return m_sizeW; }
    //@}

    void resizeErase(int sizeX, int sizeY, int sizeZ, int sizeW);
    void reset(const T& value = T(0));
    void operator=(const T&);

protected:
    T * m_data;
    int m_sizeX;
    int m_sizeY;
    int m_sizeZ;
    int m_sizeW;

private:
    Array4D(const Array4D &);              // Copying and assignment
    Array4D & operator=(const Array4D &); // are not implemented
};

template <typename T>
inline
Array4D<T>::Array4D():
    m_data(0), m_sizeX(0), m_sizeY(0), m_sizeZ(0), m_sizeW(0)
{
    // empty
}


template <typename T>
inline
Array4D<T>::Array4D(int sizeX, int sizeY, int sizeZ, int sizeW) :
    m_data(new T[sizeX * sizeY * sizeZ * sizeW]),
    m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ), m_sizeW(sizeW)
{
    // empty
}


template <typename T>
inline
Array4D<T>::~Array4D()
{
    delete [] m_data;
}


template <typename T>
inline T &
Array4D<T>::operator()(int x, int y, int z, int w)
{
    return m_data[w*m_sizeX*m_sizeY*m_sizeZ +
                  z*m_sizeX*m_sizeY + y*m_sizeX + x];
}


template <typename T>
inline const T &
Array4D<T>::operator()(int x, int y, int z, int w) const
{
    return m_data[w*m_sizeX*m_sizeY*m_sizeZ +
                  z*m_sizeX*m_sizeY + y*m_sizeX + x];
}


template <typename T>
inline T &
Array4D<T>::operator[](int i)
{
    return m_data[i];
}


template <typename T>
inline const T &
Array4D<T>::operator[](int i) const
{
    return m_data[i];
}


template <typename T>
inline void
Array4D<T>::resizeErase(int sizeX, int sizeY, int sizeZ, int sizeW)
{
    if (sizeX == m_sizeX && sizeY == m_sizeY &&
        sizeZ == m_sizeZ && sizeW == m_sizeW)
        return;

    delete [] m_data;
    m_data = new T[sizeX * sizeY * sizeZ * sizeW];
    m_sizeX = sizeX;
    m_sizeY = sizeY;
    m_sizeZ = sizeZ;
    m_sizeW = sizeW;
}


template <typename T>
inline void
Array4D<T>::reset(const T& value)
{
    int size = m_sizeX*m_sizeY*m_sizeZ*m_sizeW;
    for (int i = 0; i < size; i++)
        m_data[i] = value;
}


template <typename T>
inline void
Array4D<T>::operator=(const T& value)
{
    reset(value);
}

} // namespace Util

#endif // UTIL_ARRAY4D_H


