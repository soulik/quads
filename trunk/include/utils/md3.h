#ifndef H_MD3
#define H_MD3

#include "common.h"
#include "qDebug.h"
#include "utils/qModel.h"
#include <vector>
#include <hash_map>
#include <string>
#include <stdio.h>
#include "objects/qVector.h"

using namespace std;
using namespace stdext;

#define MD3_ID	"IDP3"

typedef enum{
	BOTH_DEATH1 = 0,		// The first twirling death animation
	BOTH_DEAD1,				// The end of the first twirling death animation
	BOTH_DEATH2,			// The second twirling death animation
	BOTH_DEAD2,				// The end of the second twirling death animation
	BOTH_DEATH3,			// The back flip death animation
	BOTH_DEAD3,				// The end of the back flip death animation

	TORSO_GESTURE,			// The torso's gesturing animation

	TORSO_ATTACK,			// The torso's attack1 animation
	TORSO_ATTACK2,			// The torso's attack2 animation

	TORSO_DROP,				// The torso's weapon drop animation
	TORSO_RAISE,			// The torso's weapon pickup animation

	TORSO_STAND,			// The torso's idle stand animation
	TORSO_STAND2,			// The torso's idle stand2 animation

	LEGS_WALKCR,			// The legs's crouching walk animation
	LEGS_WALK,				// The legs's walk animation
	LEGS_RUN,				// The legs's run animation
	LEGS_BACK,				// The legs's running backwards animation
	LEGS_SWIM,				// The legs's swimming animation

	LEGS_JUMP,				// The legs's jumping animation
	LEGS_LAND,				// The legs's landing animation

	LEGS_JUMPB,				// The legs's jumping back animation
	LEGS_LANDB,				// The legs's landing back animation

	LEGS_IDLE,				// The legs's idle stand animation
	LEGS_IDLECR,			// The legs's idle crouching animation

	LEGS_TURN,				// The legs's turn animation

	MAX_ANIMATIONS			// The define for the maximum amount of animations
} nAnimations;

struct cMD3Header{
	char	fileID[4];		//file ID - Must be "IDP3"
	int		version;		//file version - Must be 15
	char	strFile[68];	//name of the file
	int		numFrames;		//number of animation frames
	int		numTags;		//tag count
	int		numMeshes;		//number of sub-objects in the mesh
	int		numMaxSkins;	//number of skins for the mesh
	int		headerSize;		//mesh header size
	int		tagStart;		//offset into the file for tags
	int		tagEnd;			//end offset into the file for tags
	int		fileSize;		//file size
};

struct cMD3Mesh{
	char	meshID[4];		//mesh ID (We don't care)
	char	strName[68];	//mesh name (We do care)
	int		numMeshFrames;	//mesh animation frame count
	int		numSkins;		//mesh skin count
	int		numVertices;	//mesh vertex count
	int		numTriangles;	//mesh face count
	int		triStart;		//starting offset for the triangles
	int		headerSize;		//header size for the mesh
	int		uvStart;		//starting offset for the UV coordinates
	int		vertexStart;	//starting offset for the vertex indices
	int		meshSize;		//total mesh size
};

// This is our tag structure for the .MD3 file format.  These are used link other
// models to and the rotate and translate the child models of that model.
class cMD3Model;

struct cMD3Tag{
	char		strName[64];	//name of the tag (I.E. "tag_torso")
	qVector		vPosition;		//translation that should be performed
	float		rotation[3][3];	//3x3 rotation matrix for this frame
	cMD3Model	* link;
};

//bone information
struct cMD3Bone{
	float	mins[3];		//This is the min (x, y, z) value for the bone
	float	maxs[3];		//This is the max (x, y, z) value for the bone
	float	position[3];	//This supposedly stores the bone position???
	float	scale;			//scale of the bone
	char	creator[16];	//The modeler used to create the model (I.E. "3DS Max")
};

struct cMD3Triangle{
	signed short  vertex[3];
	unsigned char normal[2];
};

struct cMD3Face{
	int vertexIndices[3];                
};

struct cMD3TexCoord {
	float textureCoord[2];
};

struct cMD3Skin{
	char strName[68];
};

struct cMD3Vertex{
	float vertex[3];
	float normal[3];
};

struct cMD3Animation {
	int start_frame;
	int end_frame;
	int looping_frames;
	int fps;
};

class cMD3VertexBuffer {
public:
	cMD3VertexBuffer();
	~cMD3VertexBuffer();
	void prepare(int num_xyz,int num_uv,int num_norm, int num_indices);
	void free(unsigned char what=0xFF);	
	float * xyz; //vertex coords
	float * uv; //UV texture maps
	float * norm; //normals
	unsigned int * index;
	int xyzs, uvs, norms, indices;
};

class cMD3Object {
private:
	hash_map<string,cMD3Skin*> skins;
	vector<cMD3TexCoord*> texCoords;
	vector<cMD3Face*> faces;
	vector<cMD3Vertex*> vertices;
	cMD3Mesh mesh;
	int texture;
	cMD3VertexBuffer buffer;
	unsigned int glBuffers[4];
public:
	cMD3Object();
	~cMD3Object();
	void draw(int index,int index2, float t, int mode = 0);
	void drawBuffers(int index,int index2, float t);
	void readVertices(FILE * f);
	void readFaces(FILE * f);
	void readTexCoords(FILE * f);
	void readSkins(FILE * f);
	const cMD3Mesh * readObject(FILE * f,int mesh_offset);
	void process();
	inline const cMD3Mesh * getMeshInfo(){
		return &mesh;
	}
	inline void setTexture(int id){
		texture = id;
	};
};

class cMD3Model{
private:
	//vector<cMD3Model*> links;
	vector<cMD3Tag*> tags;
	hash_map<string,cMD3Object*> objects;
	hash_map<string,cMD3Animation*> animations;
	cMD3Animation blank_animation;
	vector<cMD3Bone*> bones;

	cMD3Header header;
	FILE * f;

	void readBones();
	void readTags();
	void readMesh();
	void readShaders();
	void loadShader(const char * fname);
	void setTime();
	int current_frame, next_frame,start_frame,end_frame;
	string current_animation;

public:
	cMD3Model();
	~cMD3Model();
	void loadModel(const char * fname);
	void loadSkin(const char * fname);
	void prepareMaterials();
	void updateMaterial(const char * material_name);
	void drawModel(int index, int index2, float t, int mode = 0);
	void linkTo(cMD3Model * model, const char * name);
	void drawLinkedModel(int mode=0);
	void setAnimation(const char * name, cMD3Animation * anim);
	void setAnimation(const char * name);
	void setFrame(int num);
	cMD3Animation * getAnimation(const char * name);
	void update();
	inline const char * getCurrentAnimation(){
		return current_animation.c_str();
	}
	float time;
	int lastTime;
};

class cMD3: public qModel{
private:
	hash_map<string,cMD3Model*> models;
	string name;
	qDebug * debug;
public:
	cMD3();
	~cMD3();
	void setDebug(qDebug * debug);
	void loadModel(const char * name, const char * style=NULL);
	void prepareMaterials();
	void updateMaterial(const char * material_name);
	void drawModel(int mode = 0);
	void loadAnimation(const char * fname);
	void setFrame(const char * obj_name,int num);
	void setAnimation(const char * obj_name,const char * name);
	void update();
};

#endif