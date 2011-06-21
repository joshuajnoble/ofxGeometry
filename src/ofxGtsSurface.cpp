#include "ofxGtsSurface.h"

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

// level:  http://mathworld.wolfram.com/GeodesicDome.html
void ofxGtsSurface::createSphere(guint level) {
	gts_surface_generate_sphere(s, level);
	//gts_surface_write (s, stdout);
}

vector<GtsVertex*> ofxGtsSurface::getVertices() {
	//vector<GtsVertex*> vertices;
	gts_surface_foreach_vertex(s, (GtsFunc) build_vertex_vector, &surfaceVertices);
	return surfaceVertices;
}

vector<GtsEdge*> ofxGtsSurface::getEdges() {
	//vector<GtsEdge*> edges;
	gts_surface_foreach_edge(s, (GtsFunc) build_edge_vector, &surfaceEdges);
	return surfaceEdges;
}

vector<GtsTriangle*>& ofxGtsSurface::getTriangles() {
	//vector<GtsTriangle*> triangles;
	gts_surface_foreach_face(s, (GtsFunc) build_triangle_vector, &surfaceTriangles);
	return surfaceTriangles;
}

void ofxGtsSurface::addVertices(vector<ofVec3f> verts) {
	
	
	
}


bool ofxGtsSurface::intersects(ofxGtsSurface* surfaceToCheck) {
	
	return false;
}

void ofxGtsSurface::merge(ofxGtsSurface* surfaceToMerge) {
	
	gts_surface_merge (s, s1);
	
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
		GtsTriangle * t = i->data;
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
		GtsEdge * e = i->data;
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