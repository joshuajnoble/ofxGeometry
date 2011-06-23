#pragma once
#include <gts.h>
#include <vector>
#include "ofMain.h"

using namespace std;

class ofxGtsSurface {
	
public: 
	
	ofxGtsSurface();
	~ofxGtsSurface();
	GtsVertex* createVertex(float x, float y, float z);
	GtsEdge* createEdge(GtsVertex* v1, GtsVertex* v2);
	GtsFace* createFace(GtsEdge* e1, GtsEdge* e2, GtsEdge* e3);

	vector<GtsVertex*> getFaceVertices(GtsFace* face);
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
	vector<GtsFace*> surfaceFaces;
};