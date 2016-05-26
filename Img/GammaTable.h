/*! \file GammaTable.h
    \author Wojciech Jarosz
*/

#pragma once

#include <math.h>

namespace Img {
	class GammaTable {
		public:
			GammaTable(float gamma = 1.0f, float imageScale = 1.0f) :
					m_gamma(0.0f), m_scale(0.0f) {
				initTable(gamma, imageScale);
			}
    
			void initTable(float gamma, float imageScale) {
				m_gamma = gamma;
				m_scale = imageScale;

				float tval;
				float invGamma = 1.0f / m_gamma;
				for (int x = 0; x < 65536; x++) {
					tval = ::powf(short2float(x) * m_scale, invGamma) * 255.0f;
					m_table[x] = (tval > 255.0 ? 255 : (unsigned char) tval);
				}
        
				for (int x = 0; x < 256; ++x)
					m_reverseTable[x] = ::powf(x / 255.0f, invGamma);
			}

			unsigned char lookup(float f) const {
				return m_table[float2short(f)];
			}

			float lookup(unsigned char uc) const {
				return m_reverseTable[uc];
			}
    
			//private:

			inline static unsigned short float2short(float f) {
				union { float f; unsigned int i; } p;
				p.f = f;
				return (p.i >> 15);
			}
    
			inline static float short2float(unsigned int s) {
				union { float f; unsigned int i; } p;
				p.i = s << 15;
				return p.f;
			}

			unsigned char m_table[65536];
			float m_reverseTable[256];
			float m_gamma, m_scale;
	};
} // namespace Img