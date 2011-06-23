#include "ofxGtsSurface.h"

void build_list (gpointer data, GSList ** list)
{
	*list = g_slist_prepend (*list, data);
}

static void build_vertex_vector(gpointer data, vector<GtsVertex*>* vertices) {
	vertices->push_back((GtsVertex*)data);
}

static void build_edge_vector(gpointer data, vector<GtsEdge*>* edges) {
	edges->push_back((GtsEdge*)data);
}

static void build_triangle_vector(gpointer data, vector<GtsTriangle*>* triangles) {
	triangles->push_back((GtsTriangle*)data);
}

ofxGtsSurface::ofxGtsSurface() {
	s = gts_surface_new(
			 gts_surface_class()
			,gts_face_class()
			,gts_edge_class()
			,gts_vertex_class()
	);
}

ofxGtsSurface::ofxGtsSurface(GtsSurface* passIn) {
	gts_surface_copy (s, passIn);
}


ofxGtsSurface::~ofxGtsSurface() {
	{
		vector<GtsVertex*>::iterator it = vertices.begin();
		while(it != vertices.end()) {
			delete *it;
			++it;
		}
	}
	{
		vector<GtsEdge*>::iterator it = edges.begin();
		while(it != edges.end()) {
			delete *it;
			++it;
		}
	}
	{
		vector<GtsFace*>::iterator it = faces.begin();
		while(it != faces.end()) {
			delete *it;
			++it;
		}
	}
}

// level:  http://mathworld.wolfram.com/GeodesicDome.html
void ofxGtsSurface::createSphere(guint level) {
	gts_surface_generate_sphere(s, level);
}

GtsVertex* ofxGtsSurface::createVertex(float x, float y, float z) {
	GtsVertex* v = gts_vertex_new(s->vertex_class, x, y, z);
	vertices.push_back(v);
	return v;
}

GtsEdge* ofxGtsSurface::createEdge(GtsVertex* v1, GtsVertex* v2) {
	GtsEdge* edge = gts_edge_new(s->edge_class, v1, v2);
	edges.push_back(edge);
	return edge;
}

GtsFace* ofxGtsSurface::createFace(GtsEdge* e1, GtsEdge* e2, GtsEdge* e3) {
	GtsFace* face = gts_face_new(s->face_class, e1, e2, e3);
	faces.push_back(face);
	return face;
}

vector<GtsVertex*> ofxGtsSurface::getFaceVertices(GtsFace* face) {
	// THIS IS NOT CORRECT!!!!
	cout << "GETFACEVERTICES IS NOT YET READY!!! " << endl;
	GtsVertex* v1 = face->triangle.e1->segment.v1;
	GtsVertex* v2 = face->triangle.e2->segment.v2;
	GtsVertex* v3 = face->triangle.e3->segment.v1;
	vector<GtsVertex*> result;
	result.push_back(v1);
	result.push_back(v2);
	result.push_back(v3);
	return result;
}

vector<GtsVertex*>& ofxGtsSurface::getVertices() {
	//vector<GtsVertex*> vertices;
	gts_surface_foreach_vertex(s, (GtsFunc) build_vertex_vector, &vertices);
	return vertices;
}

vector<GtsEdge*>& ofxGtsSurface::getEdges() {
	//vector<GtsEdge*> edges;
	gts_surface_foreach_edge(s, (GtsFunc) build_edge_vector, &edges);
	return edges;
}

vector<GtsTriangle*>& ofxGtsSurface::getTriangles() {
	//vector<GtsTriangle*> triangles;
	gts_surface_foreach_face(s, (GtsFunc) build_triangle_vector, &triangles);
	return triangles;
}

// FACES v TRIANGLES???
void ofxGtsSurface::addVertices(vector<ofVec3f> verts) {
	
	vector<ofVec3f>::iterator vit = verts.begin();
	//vector<GtsEdge*> edges;
	
	GtsVertex *pt, *pt2;
	while(vit != verts.end()) {
		pt = gts_vertex_new(s->vertex_class, vit->x, vit->y, vit->z);
		if(pt2 != NULL) {
			GtsEdge* e = gts_edge_new(s->edge_class, pt, pt2);
			edges.push_back(e);
		}
		pt2 = pt;
		++vit;
	}
	
	vector<GtsEdge*>::iterator eit = edges.begin();
	//vector<GtsFace*> faces;
	
	while(eit != edges.end()) {
		GtsFace* f = gts_face_new(s->face_class, *eit, *(++eit), *(++eit));
		faces.push_back(f);
		++eit;
	}
	
	vector<GtsFace*>::iterator fit = faces.begin();
	
	while(fit != faces.end()) {
		gts_surface_add_face(s, *fit);
		++fit;
	}
	
	
	
}


bool ofxGtsSurface::intersects(ofxGtsSurface* surfaceToCheck) {
	
	return false;
}

void ofxGtsSurface::merge(ofxGtsSurface* surfaceToMerge) {
	
	gts_surface_merge (s, surfaceToMerge->getSurface());
	
	// now delete the other surface?
}

void ofxGtsSurface::cleanupTriangles()
{
	
	
	GSList * triangles = NULL;
	GSList * i;
	
	g_return_if_fail (s != NULL);
	
	/* build list of triangles */
	gts_surface_foreach_face (s, (GtsFunc) build_list, &triangles);
	
	/* remove duplicate triangles */
	i = triangles;
	while (i) {
		GtsTriangle * t = (GtsTriangle*) i->data ;
		if (gts_triangle_is_duplicate (t))
		/* destroy t, its edges (if not used by any other triangle)
		 and its corners (if not used by any other edge) */
			gts_object_destroy (GTS_OBJECT (t));
		i = i->next;
	}
	
	/* free list of triangles */
	g_slist_free (triangles);
}

void ofxGtsSurface::cleanupVertices()
{
	
	GSList * edges = NULL;
	GSList * i;
	
	g_return_if_fail (s != NULL);
	
	/* build list of edges */
	gts_surface_foreach_edge (s, (GtsFunc) build_list, &edges);
	
	/* remove degenerate and duplicate edges.
     Note: we could use gts_edges_merge() to remove the duplicates and then
     remove the degenerate edges but it is more efficient to do everything 
     at once (and it's more pedagogical too ...) */
	
	/* We want to control manually the destruction of edges */
	gts_allow_floating_edges = TRUE;
	
	i = edges;
	while (i) {
		GtsEdge * e = (GtsEdge*) i->data;
		GtsEdge * duplicate;
		if (GTS_SEGMENT (e)->v1 == GTS_SEGMENT (e)->v2) /* edge is degenerate */
		/* destroy e */
			gts_object_destroy (GTS_OBJECT (e));
		else if ((duplicate = gts_edge_is_duplicate (e))) {
			/* replace e with its duplicate */
			gts_edge_replace (e, duplicate);
			/* destroy e */
			gts_object_destroy (GTS_OBJECT (e));
		}
		i = i->next;
	}
	
	/* don't forget to reset to default */
	gts_allow_floating_edges = FALSE;
	
	/* free list of edges */
	g_slist_free (edges);
	
	
}

pair<ofVec3f, ofVec3f> ofxGtsSurface::getBoundingBox()
{
	GtsBBox * bb = gts_bbox_surface (gts_bbox_class (), s);
	ofVec3f ll(bb->x1, bb->y1, bb->z1);
	ofVec3f ur(bb->x2, bb->y2, bb->z2);
	pair<ofVec3f, ofVec3f> p(ll, ur);
	return p;
}

bool ofxGtsSurface::checkIsPointInside(ofVec3f point) {
	GtsBBox * bb = gts_bbox_surface (gts_bbox_class (), s);
	GtsPoint p;
	p.x = point.x;
	p.y = point.y;
	p.z = point.z;
	return gts_bbox_point_is_inside ( bb, &p );
}


bool ofxGtsSurface::areOverlapping( ofxGtsSurface* other)
{
	pair<ofVec3f, ofVec3f> theirPair = other->getBoundingBox();
	GtsBBox theirBB;
	bbFromPair(&theirBB, &theirPair);
	GtsBBox * ourBB = gts_bbox_surface (gts_bbox_class (), s);
	return gts_bboxes_are_overlapping ( ourBB, &theirBB );
}

void ofxGtsSurface::bbFromPair(GtsBBox* box, pair<ofVec3f, ofVec3f>* p)
{
	box->x1 = p->first.x;
	box->y1 = p->first.y;
	box->z1 = p->first.z;
	box->x2 = p->second.x;
	box->y2 = p->second.y;
	box->z2 = p->second.z;
}

void ofxGtsSurface::drawBounding() 
{

	GtsBBox *bb = gts_bbox_surface (gts_bbox_class (), s);
	drawNode.setPosition(bb->x1, bb->y1, bb->z1);
	drawNode.setScale(bb->x2 - bb->x1, bb->y2 - bb->y1, bb->z2 - bb->z1);
	drawNode.draw();
}