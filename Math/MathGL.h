/*! \file MathGL.h
    \author Wojciech Jarosz
*/
#ifndef MATH_MATHGL_H_INCLUDED
#define MATH_MATHGL_H_INCLUDED

#include <OGL/Core.h>

#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <Math/Vec4.h>
#include <Math/Mat44.h>

inline void glVertex   (const Math::Vec2f &v) {glVertex2f(v.x,v.y);          }
inline void glVertex   (const Math::Vec2d &v) {glVertex2d(v.x,v.y);          }
inline void glVertex   (const Math::Vec2i &v) {glVertex2i(v.x,v.y);          }
inline void glVertex   (const Math::Vec2s &v) {glVertex2s(v.x,v.y);          }

inline void glVertex   (const Math::Vec3f &v) {glVertex3f(v.x,v.y,v.z);      }
inline void glVertex   (const Math::Vec3d &v) {glVertex3d(v.x,v.y,v.z);      }
inline void glVertex   (const Math::Vec3i &v) {glVertex3i(v.x,v.y,v.z);      }
inline void glVertex   (const Math::Vec3s &v) {glVertex3s(v.x,v.y,v.z);      }

inline void glVertex   (const Math::Vec4f &v) {glVertex4f(v.x,v.y,v.z,v.w);  }
inline void glVertex   (const Math::Vec4d &v) {glVertex4d(v.x,v.y,v.z,v.w);  }
inline void glVertex   (const Math::Vec4i &v) {glVertex4i(v.x,v.y,v.z,v.w);  }
inline void glVertex   (const Math::Vec4s &v) {glVertex4s(v.x,v.y,v.z,v.w);  }

inline void glNormal   (const Math::Vec3f &n) {glNormal3f(n.x,n.y,n.z);      }
inline void glNormal   (const Math::Vec3d &n) {glNormal3d(n.x,n.y,n.z);      }
inline void glNormal   (const Math::Vec3i &n) {glNormal3i(n.x,n.y,n.z);      }
inline void glNormal   (const Math::Vec3s &n) {glNormal3s(n.x,n.y,n.z);      }

inline void glColor    (const Math::Vec3f &c) {glColor3f(c.x,c.y,c.z);       }
inline void glColor    (const Math::Vec3d &c) {glColor3d(c.x,c.y,c.z);       }
inline void glColor    (const Math::Vec3i &c) {glColor3i(c.x,c.y,c.z);       }
inline void glColor    (const Math::Vec3s &c) {glColor3s(c.x,c.y,c.z);       }
inline void glColor    (const Math::Vec3uc &c){glColor3ub(c.x,c.y,c.z);      }
inline void glColor    (const Math::Vec3c &c) {glColor3b(c.x,c.y,c.z);       }

inline void glColor    (const Math::Vec4f &c) {glColor4f(c.x,c.y,c.z,c.w);   }
inline void glColor    (const Math::Vec4d &c) {glColor4d(c.x,c.y,c.z,c.w);   }
inline void glColor    (const Math::Vec4i &c) {glColor4i(c.x,c.y,c.z,c.w);   }
inline void glColor    (const Math::Vec4s &c) {glColor4s(c.x,c.y,c.z,c.w);   }
inline void glColor    (const Math::Vec4uc &c){glColor4ub(c.x,c.y,c.z,c.w);  }
inline void glColor    (const Math::Vec4c &c) {glColor4b(c.x,c.y,c.z,c.w);   }

inline void glTexCoord (const Math::Vec2f &t) {glTexCoord2f(t.x,t.y);        }
inline void glTexCoord (const Math::Vec2d &t) {glTexCoord2d(t.x,t.y);        }
inline void glTexCoord (const Math::Vec2i &t) {glTexCoord2i(t.x,t.y);        }
inline void glTexCoord (const Math::Vec2s &t) {glTexCoord2s(t.x,t.y);        }

inline void glTexCoord (const Math::Vec3f &t) {glTexCoord3f(t.x,t.y,t.z);    }
inline void glTexCoord (const Math::Vec3d &t) {glTexCoord3d(t.x,t.y,t.z);    }
inline void glTexCoord (const Math::Vec3i &t) {glTexCoord3i(t.x,t.y,t.z);    }
inline void glTexCoord (const Math::Vec3s &t) {glTexCoord3s(t.x,t.y,t.z);    }

inline void glTexCoord (const Math::Vec4f &t) {glTexCoord4f(t.x,t.y,t.z,t.w);}
inline void glTexCoord (const Math::Vec4d &t) {glTexCoord4d(t.x,t.y,t.z,t.w);}
inline void glTexCoord (const Math::Vec4i &t) {glTexCoord4i(t.x,t.y,t.z,t.w);}
inline void glTexCoord (const Math::Vec4s &t) {glTexCoord4s(t.x,t.y,t.z,t.w);}

inline void glTranslate(const Math::Vec3f &t) {glTranslatef(t.x,t.y,t.z);    }
inline void glTranslate(const Math::Vec3d &t) {glTranslated(t.x,t.y,t.z);    }

inline void glLightModel(GLenum p, const Math::Vec2f &v) {glLightModelfv(p, (const GLfloat*)v.toArray());}
inline void glLightModel(GLenum p, const Math::Vec2i &v) {glLightModeliv(p, (const GLint*)v.toArray());}
inline void glLightModel(GLenum p, const Math::Vec3f &v) {glLightModelfv(p, (const GLfloat*)v.toArray());}
inline void glLightModel(GLenum p, const Math::Vec3i &v) {glLightModeliv(p, (const GLint*)v.toArray());}
inline void glLightModel(GLenum p, const Math::Vec4f &v) {glLightModelfv(p, (const GLfloat*)v.toArray());}
inline void glLightModel(GLenum p, const Math::Vec4i &v) {glLightModeliv(p, (const GLint*)v.toArray());}

inline void glLight(GLenum l, GLenum p, const Math::Vec2f &v) {glLightfv(l, p, (const GLfloat*)v.toArray());}
inline void glLight(GLenum l, GLenum p, const Math::Vec2i &v) {glLightiv(l, p, (const GLint*)v.toArray());}
inline void glLight(GLenum l, GLenum p, const Math::Vec3f &v) {glLightfv(l, p, (const GLfloat*)v.toArray());}
inline void glLight(GLenum l, GLenum p, const Math::Vec3i &v) {glLightiv(l, p, (const GLint*)v.toArray());}
inline void glLight(GLenum l, GLenum p, const Math::Vec4f &v) {glLightfv(l, p, (const GLfloat*)v.toArray());}
inline void glLight(GLenum l, GLenum p, const Math::Vec4i &v) {glLightiv(l, p, (const GLint*)v.toArray());}

inline void glGetLight(GLenum l, GLenum p, Math::Vec2f *v) {glGetLightfv(l, p, (GLfloat*)v->toArray());}
inline void glGetLight(GLenum l, GLenum p, Math::Vec2i *v) {glGetLightiv(l, p, (GLint*)v->toArray());}
inline void glGetLight(GLenum l, GLenum p, Math::Vec3f *v) {glGetLightfv(l, p, (GLfloat*)v->toArray());}
inline void glGetLight(GLenum l, GLenum p, Math::Vec3i *v) {glGetLightiv(l, p, (GLint*)v->toArray());}
inline void glGetLight(GLenum l, GLenum p, Math::Vec4f *v) {glGetLightfv(l, p, (GLfloat*)v->toArray());}
inline void glGetLight(GLenum l, GLenum p, Math::Vec4i *v) {glGetLightiv(l, p, (GLint*)v->toArray());}

inline void glMaterial(GLenum f, GLenum p, const Math::Vec2f &v) {glMaterialfv(f, p, (const GLfloat*)v.toArray());}
inline void glMaterial(GLenum f, GLenum p, const Math::Vec2i &v) {glMaterialiv(f, p, (const GLint*)v.toArray());}
inline void glMaterial(GLenum f, GLenum p, const Math::Vec3f &v) {glMaterialfv(f, p, (const GLfloat*)v.toArray());}
inline void glMaterial(GLenum f, GLenum p, const Math::Vec3i &v) {glMaterialiv(f, p, (const GLint*)v.toArray());}
inline void glMaterial(GLenum f, GLenum p, const Math::Vec4f &v) {glMaterialfv(f, p, (const GLfloat*)v.toArray());}
inline void glMaterial(GLenum f, GLenum p, const Math::Vec4i &v) {glMaterialiv(f, p, (const GLint*)v.toArray());}

inline void glGetMaterial(GLenum f, GLenum p, Math::Vec2f *v) {glMaterialfv(f, p, (GLfloat*)v->toArray());}
inline void glGetMaterial(GLenum f, GLenum p, Math::Vec2i *v) {glMaterialiv(f, p, (GLint*)v->toArray());}
inline void glGetMaterial(GLenum f, GLenum p, Math::Vec3f *v) {glMaterialfv(f, p, (GLfloat*)v->toArray());}
inline void glGetMaterial(GLenum f, GLenum p, Math::Vec3i *v) {glMaterialiv(f, p, (GLint*)v->toArray());}
inline void glGetMaterial(GLenum f, GLenum p, Math::Vec4f *v) {glMaterialfv(f, p, (GLfloat*)v->toArray());}
inline void glGetMaterial(GLenum f, GLenum p, Math::Vec4i *v) {glMaterialiv(f, p, (GLint*)v->toArray());}


inline void 
glMultMatrix(const Math::Mat44f& m) 
{
    glMultMatrixf((GLfloat*)m[0]);
}

inline void 
glMultMatrix(const Math::Mat44f* m)
{
    glMultMatrixf((GLfloat*)(*m)[0]);
}

inline void 
glMultMatrix(const Math::Mat44d& m) 
{
    glMultMatrixd((GLdouble*)m[0]);
}

inline void 
glMultMatrix(const Math::Mat44d* m)
{
    glMultMatrixd((GLdouble*)(*m)[0]);
}

inline void 
glLoadMatrix(const Math::Mat44f& m) 
{
    glLoadMatrixf((GLfloat*)m[0]);
}

inline void 
glLoadMatrix(const Math::Mat44f* m) 
{
    glLoadMatrixf((GLfloat*)(*m)[0]);
}

inline void 
glLoadMatrix(const Math::Mat44d& m) 
{
    glLoadMatrixd((GLdouble*)m[0]);
}

inline void 
glLoadMatrix(const Math::Mat44d* m) 
{
    glLoadMatrixd((GLdouble*)(*m)[0]);
}


#endif // MATH_MATHGL_H_INCLUDED
