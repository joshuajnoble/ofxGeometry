#include "ofxGts.h"
ofxGts::ofxGts() {
}

ofxGts::~ofxGts() {	
	// cleanup surfaces.
	vector<ofxGtsSurface*>::iterator it = surfaces.begin();
	while(it != surfaces.end()) {
		delete (*it);
		it = surfaces.erase(it);
	}
}

ofxGtsSurface& ofxGts::createSurface() {
	ofxGtsSurface* s = new ofxGtsSurface();
	surfaces.push_back(s);
	return *s;
}

vector<ofxGtsSurface*> ofxGts::splitSurface(ofxGtsSurface* toBeSplit) {

	splitSurfs.clear();
	
	GSList* list = gts_surface_split (toBeSplit->getSurface());
	while (list) {
		splitSurfs.push_back(new ofxGtsSurface( ( GtsSurface* ) list->data));
		list = list->next;
	}
	return splitSurfs;
	
}

void ofxGts::splitSurface(ofxGtsSurface* toBeSplit, GSList* list) {
	
	list = gts_surface_split (toBeSplit->getSurface());
	
}