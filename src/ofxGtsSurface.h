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
	
	void ofxGtsSurface::addVertices(vector<ofVec3f> verts);
	bool ofxGtsSurface::intersects(ofxGtsSurface* surfaceToCheck);
	void ofxGtsSurface::merge(ofxGtsSurface* surfaceToMerge);
	
	void ofxGtsSurface::cleanupTriangles();
	
	void ofxGtsSurface::cleanupVertices();
	
	
private:
	GtsSurface* s;
	
	vector<GtsTriangle*> surfaceTriangles;
	vector<GtsVertex*> surfaceVertexes;
	vector<GtsEdge*> surfaceEdges;
	
};