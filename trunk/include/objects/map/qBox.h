#ifndef H_QBOX
#define H_QBOX

#include <vector>
#include "objects/qVector.h"
#include "objects/map/qTile.h"

using namespace std;

class qBox;
class qTile;

class qBox {
public:
	qVector pos;
	qVector size;
	vector<qBox *> obj;
	vector<qTile *> tiles;
};

#endif