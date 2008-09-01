#ifndef H_3DS_DEFS
#define H_3DS_DEFS

#include <vector>
#include <map>
using namespace std;

enum CHs{
	cMAIN					=		0x4D4D,
	cM3D_VERSION			=		0x0002,
	c3D_EDITOR				=		0x3D3D,
	cMESH_VERSION			=		0x3D3E,
	cOBJECT_BLOCK			=		0x4000,
	cOBJECT_HIDDEN			=		0x4010,
	cTRIANGULAR_MESH		=		0x4100,
	cVERTICES_LIST			=		0x4110,
	cFACES_DESCRIPTION		=		0x4120,
	cFACES_MATERIAL			=		0x4130,
	cFACES_COORDS			=		0x4140,
	cSMOOTHING_GROUP		=		0x4150,
	cLOCAL_COORDS_SYTEM		=		0x4160,
	cLIGHT					=		0x4600,
	cSPOTLIGHT				=		0x4610,
	cCAMERA					=		0x4700,
	cMATERIAL				=		0xAFFF,
	cTEXTURE_MAP			=		0xA200,
	cMAPPING_FILENAME		=		0xA300,
	cMAPPING_PARAMETERS		=		0xA351,
	cU_SCALE				=		0xA354,
	cV_SCALE				=		0xA356,
	cU_OFFSET				=		0xA358,
	cV_OFFSET				=		0xA35A,
	cKEYFRAMER				=		0xB000,
	cMESH_INFORMATION		=		0xB002,
	cSPOTLIGHT_INFORMATION	=		0xB007,
	cFRAMES					=		0xB008,
	cOBJECT_NAME			=		0xB010,
	cOBJECT_PIVOT_POINT		=		0xB013,
	cPOSITION_TRACK			=		0xB020,
	cROTATION_TRACK			=		0xB021,
	cSCALE_TRACK			=		0xB022,
	cHIERARCHY_POSITION		=		0xB030,
	
	cMATERIAL_NAME			=		0xA000,
	cAMBIENT_COLOR			=		0xA010,
	cDIFFUSE_COLOR			=		0xA020,
	cSPECULAR_COLOR			=		0xA030,
	cSHININESS				=		0xA040,
    cSHININESS_STRENGTH		=		0xA041,
	cTRANSPARENCY			=		0xA050,
	cTRANSPARENCY_FALLOFF	=		0xA052,
	cREFLECTION_BLUR		=		0xA053,
	cTWO_SIDED				=		0xA081,
	cTEX_MAP1				=		0xA200,
	cTEX_MAP2				=		0xA33A,
	cOPACITY_MAP			=		0xA210,
	cBUMP_MAP				=		0xA230,
	cSHININESS_MAP			=		0xA33C,
	cSPECULAR_MAP			=		0xA204,
	cSELF_ILLUM_MAP			=		0xA33D,
	cREFLECTION_MAP			=		0xA220,

	cRGBf					=		0x0010,
	cRGBb					=		0x0011,
	cRGBGb					=		0x0012,
	cRGBGf					=		0x0013
};
struct xVertex {
	float x,y,z;
	float nx,ny,nz;
};

struct xMaterial;

struct xFace {
	unsigned short a,b,c;
	unsigned short flags;
	float x,y,z;
	xMaterial * material;
};

struct xCoord {
	float u,v;
};

struct xColor {
	unsigned char b[4];
	float f[4];
};

struct xRGB {
	xColor c,g;
};

struct xMap {
	char fname[512];
	xCoord offset;
	xCoord scale;
	int tex_num;
};

struct xFaceMat {
	char name[366];
	vector<unsigned short> face;
};

struct xMaterial {
	char name[255];
	xRGB	ambient_color;
	xRGB	diffuse_color;
	xRGB	specular_color;

	unsigned char shininess;
	unsigned char shininess_strength;

	unsigned char transparent;
	unsigned char transparent_falloff;
	unsigned char reflection_blur;

	char two_sided;

	xMap tex1_map;
	xMap tex2_map;
	xMap opacity_map;
	xMap shininess_map;
	xMap specular_map;
	xMap self_illum_map;
	xMap reflection_map;

};

class c3ds;

class xObject {
public:
	xObject();
	~xObject();
	char name[255];
	unsigned short vertices_count;
	unsigned short faces_count;
	unsigned short uvmap_count;
	xVertex * vertex;
	xFace * face;
	xCoord * uvmap;
	int hidden;
	int hasTexture;
	float translation_matrix[4][4];
	vector<xFaceMat *> face_mat;
	void assignFaceMaterials(c3ds * model);
	void computeNormals();
};

#endif
