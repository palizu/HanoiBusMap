//
//  weightedGraph.h
//  miniProject1
//
//  Created by Van Hoang on 12/11/19.
//  Copyright © 2019 Hoang Tuan Anh Van. All rights reserved.
//

#ifndef weightedGraph_h
#define weightedGraph_h

#include <stdio.h>
#include "jrb.h"
#include "jval.h"

typedef struct graph{
    JRB vertices ;
    JRB encodedVertices ;
    JRB edges;
    JRB busInStop ;
} graph;

graph createGraph() ;
void addVertex(graph g, char *v, int i) ;
void encodeVertex(graph g, int v, char* name) ;
int hasEdge(graph g, char *u, char *v) ;
void addEdge(graph g, int u, int v, int l) ;
void addBustoStop(graph g, int stop, char *bus) ;
int indegree(graph g, int v, int *out) ;
int outdegree(graph g, int v, int *out, int *l) ;
void dropGraph(graph g) ;


#endif /* weightedGraph_h */
