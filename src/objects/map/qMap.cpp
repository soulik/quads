#include "objects/map/qMap.h"
#include "errors/exception.h"
#include <math.h>

qMap::qMap(){
	map = NULL;
	width = 0;
	height = 0;
	flags = 0;
	setName("qMap");
}

qMap::~qMap(){
	ulong size = (uint)worldMap.size();
	for (unsigned int i=0; i < worldMap.size();i++){
		delete worldMap[i];
	}
	dprintf("%d map items freed\n",size);
	delete[] map;
}

void qMap::LoadTestMap(int width, int height){
	dprintf("LoadTestMap() old stub");
}



void qMap::LoadMapV2(const char * fname){
	try{
		qMapHeader hdr;
		qMapTile tile;
		
		memset(hdr.unused ,0,32);
		FILE * f = NULL;
		f = ::fopen(fname,"rb");
		if (!f) throw cException("Can't open map file");
		
		if (!fread(&hdr,sizeof(qMapHeader),1,f)){
			throw cException ("Invalid map file");
		}
		
		dprintf("Map size: %ldx%ld\n",hdr.width,hdr.height);
		this->width = hdr.width;
		this->height = hdr.height;
		if (map) delete[] map;
		map = new qTile[width*height];
		memset(tile.unused ,0,32);

		ulong temp=0;
		for (ulong y=0;y < height;y++){
			temp = y*width;
			tile.y = y;
			for (ulong x=0;x<width;x++){
				//map[temp+x].x = x;
				//map[temp+x].y = y;
				//map[temp+x].z = (int)((sin(float(x)/float(width)*M_PI*2)*cos(float(y)/float(height)*M_PI*2)+1)*50);
				if (fread(&tile,sizeof(qMapTile),1,f)){
					map[temp+x].x = tile.x;
					map[temp+x].y = tile.y;
					map[temp+x].z = tile.z;
				}
			}
		}
		fclose(f);
	}catch (cException e){
		dprintf("Exception: %s\n",e.what());
		fflush(stderr);
	}
}

void qMap::SaveMapV2(const char * fname){
	try{
		qMapHeader hdr;
		qMapTile tile;
		
		memset(hdr.unused ,0,32);
		hdr.width = width;
		hdr.height = height;
		FILE * f = NULL;
		f = ::fopen(fname,"wb");
		if (!f) throw cException("Can't open map file");
		
		if (!fwrite(&hdr,sizeof(qMapHeader),1,f)){
			throw cException ("Can't write header");
		}
		
		dprintf("Saving Map size: %ldx%ld\n",hdr.width,hdr.height);
		memset(tile.unused ,0,32);

		ulong temp=0;
		for (ulong y=0;y < height;y++){
			temp = y*width;
			tile.y = y;
			for (ulong x=0;x<width;x++){
				tile.x = x;
				tile.z = map[temp+x].z;
				fwrite(&tile,sizeof(qMapTile),1,f);
			}
		}
		fclose(f);
	}catch (cException e){
		dprintf("Exception: %s\n",e.what());
		fflush(stderr);
	}
}

void qMap::GenerateMapV2(long width, long height, float t){
	if ((this->width!=width) || (this->height!=height)){
		this->width = width;
		this->height = height;
		if (map) delete[] map;
		map = new qTile[width*height];
	}

	ulong temp=0;
	for (ulong y=0;y < (ulong)height;y++){
	temp = y*width;
			for (ulong x=0;x<(ulong)width;x++){
				map[temp+x].x = x;
				map[temp+x].y = y;
				map[temp+x].z = (int)((sin(float(x)/float(width)*M_PI*2+t) *cos(float(y)/float(height)*M_PI*2+t)+1) *50);
			}
		}
}

void qMap::ClearMap(long width, long height){
	if ((width!=0) || (height!=0))
	if ((this->width!=width) || (this->height!=height)){
		this->width = width;
		this->height = height;
		if (map) delete[] map;
		map = new qTile[width*height];
	}

	ulong temp=0;
	for (ulong y=0;y < (ulong)height;y++){
	temp = y*width;
			for (ulong x=0;x<(ulong)width;x++){
				map[temp+x].x = x;
				map[temp+x].y = y;
				map[temp+x].z = 0;
			}
		}
}

void qMap::LoadMap(const char * fname){
	/*
	dprintf("Trying to open %s\n",fname);
	char * buf=NULL;
	
	try{
		FILE * f = NULL;
		f = ::fopen(fname,"rb");
		if (!f) throw cException("Can't open map file");
		
		if (fscanf(f,"%d %d\n",&width,&height)!=2){
			width = 0;
			height = 0;
			throw cException ("Invalid map file");
		}
		
		dprintf("Map size: %dx%d\n",width,height);

		map = new int[width*height];
		map_flag = new unsigned int[width*height];
		buf = new char[width+3]; // 2 chars for endline + 1 for NULL ending
		int y=0;
		do{
			fgets(buf,width+3,f);
			if (feof(f)) break;
			//printf("%X ",(unsigned int)buf); //DEBUG!
			
			for (int x=0; x<width; x++){
				int position = x+y*width;
				if (position<(width*height)){
					map[position] = buf[x];
					fputc(buf[x],STREAM_STDDBG);

					if (map[position]=='1'){
						worldMap.push_back(new qQuad(qVector((float)x*16,(float)y*16,0),16,16));
					}
				}else{
					dprintf("Overflow at [%d,%d]!\n",x,y);
				}
			}
			y++;
			dprintf("\n");
		}while(!feof(f));
		dprintf("Map : \"%s\" loaded\n",fname);
		
		fclose(f);
	}catch (cException e){
		dprintf("Exception: %s\n",e.what());
		fflush(stderr);
	}
	delete[] buf; 
	*/
	dprintf("LoadMap() old stub");
}

qTile * qMap::GetMap(ulong x, ulong y){
	if ((map)&&(x>=0)&&(x<width)&&(y>=0)&&(y<height)){
		return &(map[x + y*width]);
	}else{
		return &temp;
	}
}
/*
unsigned int qMap::GetMapFlag(int x, int y){
	if ((x>=0)&&(x<width)&&(y>=0)&&(y<height)){
		return map_flag[x + y*width];
	}else{
		return -1;
	}
}

void qMap::SetMapFlag(int x, int y, unsigned int flag){
	if ((x>=0)&&(x<width)&&(y>=0)&&(y<height)){
		map_flag[x + y*width];
	}
}
*/