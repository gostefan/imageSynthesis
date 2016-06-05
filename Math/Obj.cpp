/*! \file Obj.cpp
    \author Wojciech Jarosz
*/

#include <Math/Obj.h>
#include <Math/Color.h>
#include <string.h>
#include <map>
#include <vector>
#include <errno.h>
#include <stdlib.h>

namespace Math {

	using namespace std;
	using namespace Math;

	namespace {

		class ObjMesh : public MeshBase {
			public:
				ObjMesh() : MeshBase() {}
				virtual ~ObjMesh() {
					delete [] vertices;
					delete [] normals;
					delete [] vertexColors;
					delete [] texCoords;
					delete [] materials;
        
					delete [] vertexIndices;
					delete [] materialIndices;
				}
		};

		// reads a line from fp, treating '\' at the end of a line as a
		// continuation character
		char* readLine(char *buf, int bufsize, FILE *fp, int *lineNum = 0) {
			if (!fgets(buf, bufsize, fp))
				return 0;

			if (lineNum)
			   (*lineNum)++;

			int len = strlen(buf) - 1;
			if (len <= 0)
				return buf;

			while((len < bufsize) &&(buf[len - 1] == '\\')) {
				if (!fgets(buf+len-1, bufsize-len, fp)) break;
				if (lineNum)
				   (*lineNum)++;
				len = strlen(buf) - 1;
			}
			return buf;
		}

		int getIndices(char *word, int nverts) {
			char *ptr;

			// replace slashes with null characters and cause tp and np to point
			// to character immediately following the first or second slash

			for (ptr = word; *ptr != '\0'; ptr++) {
				if (*ptr == '/')
					*ptr = '\0';
			}

			int v = atoi(word);
    
			v = (v < 0) ? v + nverts : v - 1;
			if (v >= nverts || v < 0)
				return -1;
			return v;
		}


		bool getInt(int* intVal, const char* str) {
			char *endPtr;
    
			errno = 0;
			long sl = strtol(str, &endPtr, 0);
    
			if (errno == ERANGE)
				// number out of range
				return true;
			else if (sl > INT_MAX)
				// value too large!
				return true;
			else if (sl < INT_MIN)
				// value too small
				return true;
			else if (endPtr == str)
				// invalid numeric input
				return true;
			else if ('\0' != *endPtr)
				// extra characters on input line
				return true;

			*intVal = int(sl);
			return false;
		}


		void getIndices(char *word,
				   int *vindex, int *tindex, int *nindex,
				   int numV, int numT, int numN)
		{
			char *null = " ";
			char *ptr;
			char *tp;
			char *np;

			// by default, the texture and normal pointers are set to the null string
			tp = null;
			np = null;

			// replace slashes with null characters and cause tp and np to point
			// to character immediately following the first or second slash
			for (ptr = word; *ptr != '\0'; ptr++) {
				if (*ptr == '/') {
					if (tp == null)
						tp = ptr + 1;
					else
						np = ptr + 1;

					*ptr = '\0';
				}
			}

			int v, t, n;
			*vindex = getInt(&v, word) ? -1 : ((v < 0) ? v + numV : v - 1);
			*tindex = getInt(&t,   tp) ? -1 : ((t < 0) ? t + numT : t - 1);
			*nindex = getInt(&n,   np) ? -1 : ((n < 0) ? n + numN : n - 1);
		}
	} // namespace


	MeshBase* readObjMesh(const string& filename) {
		#define DELIMS " \t\n\r\f"
		printf("Reading OBJ mesh...\n");

		FILE* fp = fopen(filename.c_str(), "rb");
    
		if (!fp) {
			std::cerr << "Cannot open \"" << filename.c_str() << "\" for reading" << std::endl;
			return 0;
		}
    
    
		map<string, uint32_t> materialMap;
  
		int nv = 0, nt = 0, nn = 0, nf = 0, nm = 0;
		char line[1024];
		char tk[12][32];
		int lineNum = 0;
		while(readLine(line, 1024, fp) != 0) {
			++lineNum;
			if (line[0] == 'v') {
				if (line[1] == 'n')
					nn++;
				else if (line[1] == 't')
					nt++;
				else
					nv++;
			} else if (line[0]  == 'f') {
				int num = sscanf(&line[1], "%32s %32s %32s %32s",
								  tk[ 0], tk[ 1], tk[ 2], tk[ 3]);
				if (num != 3) {
					fprintf(stderr,"Obj: %s: file contains polygons with > 3 vertices (line: %d, %s)\n", filename.c_str(), lineNum, line);
					return 0;
				}
				nf += num - 2;
			} else if (strncmp("usemtl", line, 6) == 0) {
				char* tok = strtok(line, DELIMS);
				tok = strtok(0, DELIMS);
				if (materialMap.find(tok) == materialMap.end()) {
					// insert a new material
					materialMap.insert(pair<string, uint32_t>(string(tok), nm));
					nm++;
				}
			}
		}
		fseek(fp, 0, 0);
    
		printf("OBJ:\n"
			   "   faces: %d\n"
			   "   vertices: %d\n"
			   "   normals: %d\n"
			   "   textures: %d\n"
			   "   materials: %d",
			   nf, nv, nn, nt, nm);
    
		MeshBase *mesh = new ObjMesh();

		mesh->numVertices = nv;
		mesh->vertices = new Vec3f[mesh->numVertices];
		mesh->vertexIndices = new MeshBase::TupleI3[nf]; // always have vertices
    
		bool fixTextures = false;
		if (nt && nt != nv) {
			printf("Obj: %s: file has non-indexed tex coords, will attempt remapping...\n", filename.c_str());
			fixTextures = true;
		}
    
		bool fixNormals = false;
		if (nn && nn != nv) {
			printf("Obj: %s: file has non-indexed normals, will attempt remapping...\n", filename.c_str());
			fixNormals = true;
		}
    
		if (nt)
			mesh->texCoords = new Vec2f[nt];
    
		if (nn)
			mesh->normals = new Vec3f[nn];
    
		mesh->numMaterials = materialMap.size() + 1;
		mesh->materials = new string[mesh->numMaterials];
		mesh->materialIndices = new uint32_t[nf];

		mesh->numTris = 0;
		int nvertices = 0;
		int nnormals = 0;
		int ntextures = 0;
	//     int nmaterials = 1;
		uint32_t currentShader = 0;
		lineNum = 0;
    
		Math::Vec3i* normalIndices = 0;
		Math::Vec3i* textureIndices = 0;
		if (fixNormals) {
			normalIndices = new Math::Vec3i[nf];
			for (int i = 0; i < nf; ++i)
				normalIndices[i].set(-1);
		}
		if (fixTextures) {
			textureIndices = new Math::Vec3i[nf];
			for (int i = 0; i < nf; ++i)
				textureIndices[i].set(-1);
		}

		while(readLine(line, 1024, fp, &lineNum) != 0) {
			if (line[0] == 'v') {
				if (line[1] == 'n') {
					if (mesh->normals) {
						float x, y, z;
						if (sscanf(&line[2], "%f %f %f\n", &x, &y, &z) != 3)
							fprintf(stderr, "Obj: %s:%d: error reading normal. "
											"Expecting 3 floats.",
											filename.c_str(), lineNum);
						mesh->normals[nnormals].set(x, y, z);
						mesh->normals[nnormals].normalize();
						nnormals++;
					}
				} else if (line[1] == 't') {
					if (mesh->texCoords) {
						float x, y;
						if (sscanf(&line[2], "%f %f\n", &x, &y) != 2)
							fprintf(stderr, "Obj: %s:%d: error reading texture "
											"coordinates. Expecting 2 floats.",
											filename.c_str(), lineNum);
						mesh->texCoords[ntextures].set(x, y);
						ntextures++;
					}
				} else {
					float x, y, z;
					if (sscanf(&line[1], "%f %f %f\n", &x, &y, &z) != 3)
							fprintf(stderr, "Obj: %s:%d: error reading vertex. "
											"Expecting 3 floats.",
											filename.c_str(), lineNum);
					mesh->vertices[nvertices].set(x, y, z);
					mesh->bbox.enclose(Vec4f(mesh->vertices[nvertices], 0.f));
					nvertices++;
				}
			} else if (line[0] == 'f') {
				int v1, t1, n1;
				int v2, t2, n2;
				int v3, t3, n3;
				int num = sscanf(&line[1], "%32s %32s %32s %32s",
								 tk[ 0], tk[ 1], tk[ 2], tk[ 3]);
				if (num != 3) {
					fprintf(stderr, "Obj: %s:%d: Error reading face. "
									"Expecting at least 3 indices. Skipping face.",
									filename.c_str(), lineNum);
					continue;
				}
            
				// FIXME, need to check bounds of normals and textures too
				getIndices(tk[0], &v1, &t1, &n1, nvertices, nnormals, ntextures);
				if (v1 >= nvertices || v1 < 0) {
					fprintf(stderr, "Obj: %s:%d: Invalid vertex index. "
									"Skipping face",
									filename.c_str(), lineNum);
					continue;
				} else
					mesh->vertexIndices[mesh->numTris][0] = v1;
                
				getIndices(tk[1], &v2, &t2, &n2, nvertices, nnormals, ntextures);
				if (v2 >= nvertices || v2 < 0) {
					fprintf(stderr, "Obj: %s:%d: Invalid vertex index. "
									"Skipping face",
									filename.c_str(), lineNum);
					continue;
				} else
					mesh->vertexIndices[mesh->numTris][1] = v2;
                
				getIndices(tk[2], &v3, &t3, &n3, nvertices, nnormals, ntextures);
				if (v3 >= nvertices || v3 < 0) {
					fprintf(stderr, "Obj: %s:%d: Invalid vertex index. "
									"Skipping face",
									filename.c_str(), lineNum);
					continue;
				} else
					mesh->vertexIndices[mesh->numTris][2] = v3;
                
				// we may have to remap normals and tex coords if they are not indexed
				if (fixNormals) {
					if (n1 < nnormals && n1 >= 0 &&
							n2 < nnormals && n2 >= 0 &&
							n3 < nnormals && n3 >= 0) {
						normalIndices[mesh->numTris][0] = n1;
						normalIndices[mesh->numTris][1] = n2;
						normalIndices[mesh->numTris][2] = n3;
					}
				}
				if (fixTextures) {
					if (t1 < ntextures && t1 >= 0 &&
							t2 < ntextures && t2 >= 0 &&
							t3 < ntextures && t3 >= 0) {
						textureIndices[mesh->numTris][0] = t1;
						textureIndices[mesh->numTris][1] = t2;
						textureIndices[mesh->numTris][2] = t3;
					}
				}
        
				mesh->materialIndices[mesh->numTris] = currentShader;
            
				mesh->numTris++;
			} else if (strncmp("usemtl", line, 6) == 0) {
				// set current material
				char* tok = strtok(line, DELIMS);
				tok = strtok(0, DELIMS);
            
				// find the material in the map
				currentShader = materialMap[tok]+1;
				mesh->materials[currentShader] = string(tok);
			}
			//  else ignore line
		}
		fclose(fp);
    
		if (fixTextures) {
			Vec2f* newTexCoords = new Vec2f[nv];
			for (uint32_t i = 0; i < mesh->numTris; i++) {
				if (textureIndices[i].x >= 0 &&
						textureIndices[i].y >= 0 &&
						textureIndices[i].z >= 0) {
					newTexCoords[mesh->vertexIndices[i].x] = mesh->texCoords[textureIndices[i].x];
					newTexCoords[mesh->vertexIndices[i].y] = mesh->texCoords[textureIndices[i].y];
					newTexCoords[mesh->vertexIndices[i].z] = mesh->texCoords[textureIndices[i].z];
				}
			}
			delete [] mesh->texCoords;
			mesh->texCoords = newTexCoords;
			delete [] textureIndices;
		}
    
		if (fixNormals) {
			Vec3f* newNormals = new Vec3f[nv];
			for (uint32_t i = 0; i < mesh->numTris; i++) {
				if (normalIndices[i].x >= 0 &&
						normalIndices[i].y >= 0 &&
						normalIndices[i].z >= 0) {
					newNormals[mesh->vertexIndices[i].x] = mesh->normals[normalIndices[i].x];
					newNormals[mesh->vertexIndices[i].y] = mesh->normals[normalIndices[i].y];
					newNormals[mesh->vertexIndices[i].z] = mesh->normals[normalIndices[i].z];
				}
			}
			delete [] mesh->normals;
			mesh->normals = newNormals;
			delete [] normalIndices;
		}
    
		printf("Loaded %d vertices and %d triangles from \"%s\".",
			   nvertices, mesh->numTris, filename.c_str());
                
		return mesh;
	}

} // namespace Math
