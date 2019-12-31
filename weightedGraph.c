//
//  weightedGraph.c
//  miniProject1
//
//  Created by Van Hoang on 12/11/19.
//  Copyright © 2019 Hoang Tuan Anh Van. All rights reserved.
//

#include "jrb.h"
#include "jval.h"
#include "string.h"
#include "stdlib.h"
#include "weightedGraph.h"

graph createGraph() {
    
    graph g;
    g.busInStop = make_jrb() ;
    g.edges = make_jrb() ;
    g.encodedVertices = make_jrb() ;
    g.vertices = make_jrb() ;
    
    return g ;
}

void addVertex(graph g, char *v, int i) {
    
    if (jrb_find_str(g.vertices, v) == NULL) {
        jrb_insert_str(g.vertices, v, new_jval_i(i)) ;
    }
}

void encodeVertex(graph g, int v, char* name) {
    
    if (jrb_find_int(g.encodedVertices, v) == NULL) {
        jrb_insert_int(g.encodedVertices, v, new_jval_s(name)) ;
    }
}

int hasEdge(graph g, char *u, char *v) {
    
    if (jrb_find_str(g.vertices, u) == NULL || jrb_find_str(g.vertices, v) == NULL) return 0 ;
    
    int encoded_u = jval_i(jrb_find_str(g.vertices, u) -> val) ;
    int encoded_v = jval_i(jrb_find_str(g.vertices, v) -> val) ;
    
    JRB node = jrb_find_int(g.edges, encoded_u) ;
    JRB tree = (JRB) jval_v(node -> val) ;
    if (tree == NULL) return 0 ;
    if (jrb_find_int(tree, encoded_v) == NULL) return 0 ;
    return 1;
}

void addEdge(graph g, int u, int v, int l) {

    JRB tree = make_jrb() ;
    if (jrb_find_int(g.edges, u) == NULL) {
        jrb_insert_int(g.edges, u, new_jval_v(tree)) ;
        jrb_insert_int(tree, v, new_jval_i(l)) ;
    } else {
        JRB node = jrb_find_int(g.edges, u) ;
        tree = (JRB) jval_v(node -> val) ;
        jrb_insert_int(tree, v, new_jval_i(l)) ;
    }
    
    tree = make_jrb() ;
    if (jrb_find_int(g.edges, v) == NULL) {
        jrb_insert_int(g.edges, v, new_jval_v(tree)) ;
        jrb_insert_int(tree, u, new_jval_i(l)) ;
    } else {
        JRB node = jrb_find_int(g.edges, v) ;
        tree = (JRB) jval_v(node -> val) ;
        jrb_insert_int(tree, v, new_jval_i(l)) ;
    }
}

void addBustoStop(graph g, int stop, char *bus) {
    
    JRB tree = make_jrb() ;
    if (jrb_find_int(g.busInStop, stop) == NULL) {
        jrb_insert_int(g.busInStop, stop, new_jval_v(tree)) ;
        jrb_insert_str(tree, bus, new_jval_i(1)) ;
    } else {
        JRB node = jrb_find_int(g.busInStop, stop) ;
        JRB tree = (JRB) jval_v(node -> val) ;
        jrb_insert_str(tree, bus, new_jval_i(1)) ;
    }
}

int indegree(graph g, int v, int *out) {
    
    JRB node = make_jrb() ;
    int i = 0;
    
    jrb_traverse(node, g.encodedVertices) {
        
        int u = jval_i(node -> key) ;
        if (jrb_find_int(g.edges, u) == NULL) continue ;
        
        JRB nodeE = jrb_find_int(g.edges, u) ;
        JRB tree = (JRB) jval_v(nodeE -> val) ;
        if (tree == NULL) continue ;
        if (jrb_find_int(tree, v) != NULL) out[i++] = u;
    }
    return i;
}

int outdegree(graph g, int v, int *out, int *l) {
    
    if (jrb_find_int(g.encodedVertices, v) == NULL) return 0 ;
    
    JRB node = jrb_find_int(g.edges, v) ;
    JRB tree = (JRB) jval_v(node -> val) ;
    if (tree == NULL) return 0 ;
    
    int i = 0 ;
    jrb_traverse(node, tree) {
        out[i] = jval_i(node -> key) ;
        l[i] = jval_i(node -> val) ;
        i++ ;
    }
    return i ;
}

void dropGraph(graph g) {
    jrb_free_tree(g.vertices) ;
    jrb_free_tree(g.busInStop) ;
    jrb_free_tree(g.edges);
    jrb_free_tree(g.encodedVertices) ;
}
