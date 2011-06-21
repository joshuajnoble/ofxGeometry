#pragma once
#include <gts.h>
#include <vector>
#include "ofMain.h"

using namespace std;

class ofxGtsSurface {
	
public: 
	
	ofxGtsSurface();
	void createSphere(guint level);
	vector<GtsVertex*>& getVertices();
	vector<GtsEdge*>& getEdges();
	vector<GtsTriangle*>& getTriangles();
	
	void addVertices(vector<ofVec3f> verts);
	bool intersects(ofxGtsSurface* surfaceToCheck);
	void merge(ofxGtsSurface* surfaceToMerge);
	
	void cleanupTriangles();
	void cleanupVertices();
	
	GtsSurface* getSurface() { return s; }
	
private:
	
	GtsSurface* s;
	
	vector<GtsTriangle*> surfaceTriangles;
	vector<GtsVertex*> surfaceVertices;
	vector<GtsEdge*> surfaceEdges;
	
};