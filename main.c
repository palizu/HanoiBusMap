//
//  main.c
//  miniProject1
//
//  Created by Van Hoang on 12/11/19.
//  Copyright © 2019 Hoang Tuan Anh Van. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "weightedGraph.h"
#include "jrb.h"
#include "jval.h"

const int oo = 100000000 ;

int numStop = 0;


void inputBusStop(graph g) {
    
    FILE *fStop = fopen("./data/denotedStop.txt", "r") ;
    fscanf(fStop, "%d%*c", &numStop) ;
    char stop[1000][256] ;
    
    for (int i = 0; i < numStop; i++) {
        fgets(stop[i], 256, fStop) ; // read bus stop
        stop[i][strlen(stop[i]) - 1] = 0x0 ; // delete '\n'
        addVertex(g, stop[i], i) ;
        encodeVertex(g, i, stop[i]) ;
    }
}

void inputEdges(graph g) {
    
    FILE *fEdges = fopen("./data/Edges.txt", "r") ;
    int v[10000], d[10000] ;
    int n ;
    
    for (int i = 0; i < numStop; i++) {
        fscanf(fEdges, "%d", &n) ; // read number of edges
        for (int j = 0; j < n; j++)
            fscanf(fEdges, "%d", &v[j]) ; // read relatives
        for (int j = 0; j < n; j++)
            fscanf(fEdges, "%d", &d[j]) ; // read distance
        for (int j = 0; j < n; j++) {
            addEdge(g, i, v[j], d[j]) ;
        }
    }
}

void inputBusinStop(graph g) {
    
    FILE *fList = fopen("./data/ListBusinStop.txt", "r") ;
    int n ;
    char s[numStop][numStop][5] ;
    
    for (int i = 0; i < numStop; i++) {
        fscanf(fList, "%d", &n) ; // read number of bus
        for (int j = 0; j < n; j++) {
            //char s[5];
            fscanf(fList, "%s", s[i][j]) ; // read bus
            addBustoStop(g, i, s[i][j]) ;
        }
    }
}

void busToCatch(graph g, int *path, int n) {

    int cur = n ; // vi tri hien tai
    int trace[numStop]; // mang truy vet theo so hieu bus
    char busList[numStop][256] ; // mang tuyen bus theo truy vet
    trace[0] = -1; // danh dau dieu kien dung khi truy vet
    
    while (cur > 0) {
        
        JRB node = jrb_find_int(g.busInStop, path[cur]) ;
        JRB p = make_jrb() ;
        JRB tree = (JRB) jval_v(node -> val) ;
        char bus[5] ;
        
        int max_reach = cur ; // path id xa nhat di duoc voi 1 tuyen bus
        jrb_traverse(p , tree) {
            char *tempBus ;
            tempBus = jval_s(p -> key) ;
            
            for (int i = cur - 1; i >= 0 ; i--) {
                JRB List = jrb_find_int(g.busInStop, path[i]) ;
                JRB treeBus = (JRB) jval_v(List -> val) ;
                
                // tim path id xa nhat di den duoc
                if (jrb_find_str(treeBus, tempBus) == NULL) {
                    if (i + 1 < max_reach) {
                        max_reach = i ;
                        strcpy(bus, tempBus) ;
                    }
                    break ;
                }
                // dung vong lap neu da den destination
                if (i == 0) {
                    max_reach = 0 ;
                    strcpy(bus, tempBus) ;
                }
            }
        }
        // danh dau vet
        trace[cur] = max_reach;
        strcpy(busList[cur], bus) ;
        cur = max_reach ;
    }
    
    // in cac diem dung can di den va so hieu tuyen bus can di
    while (trace[n] != -1) {
        printf("%s ", busList[n]) ;
        JRB node = jrb_find_int(g.encodedVertices, path[n]) ;
        printf("%s\n", jval_s(node -> val)) ;
        n = trace[n] ;
    }
}

void shortestPath(graph g, char *start, char *des) {
   
    if (jrb_find_str(g.vertices, start) == NULL || jrb_find_str(g.vertices, des) == NULL) {
        printf("Location does not exist\n") ;
        return ;
    }

    int u = jval_i(jrb_find_str(g.vertices, start) -> val) ;
    int v = jval_i(jrb_find_str(g.vertices, des) -> val) ;
    
    int d[numStop + 3] ;
    int flag[numStop + 3] ;
    int trace[numStop + 3] ;
    int path[numStop + 3] ;
    
    for (int i = 0; i < numStop; i++) {
        d[i] = oo ;
        flag[i] = 0 ;
    }
    d[u] = 0;
    trace[u] = u ;
    
    while (1) {
        int vertex_min = -1, min = oo ;
        for (int i = 0; i < numStop; i++) {
            if (flag[i] == 0 && d[i] < min) {
                min = d[i] ;
                vertex_min = i ;
            }
        }
        if (vertex_min == -1) break ;
        if (vertex_min == v) break ;
        flag[vertex_min] = 1 ;
        int out[numStop + 3], l[numStop + 3] ;
        int n = outdegree(g, vertex_min, out, l) ;
        for (int i = 0; i < n; ++i) {
            if (!flag[out[i]] && d[out[i]] > d[vertex_min] + l[i]) {
                d[out[i]] = d[vertex_min] + l[i] ;
                trace[out[i]] = vertex_min ;
            }
        }
    }
    int n = 0 ;
    if (d[v] == oo) {
        printf("no path\n") ;
        return ;
    }
    printf("%d\n", d[v]) ;
    while (1) {
        path[n] = v ;
        v = trace[v] ;
        n++ ;
        if (v == trace[v]) break ;
    }
    path[n] = u ;
    busToCatch(g, path, n) ;
}

int main() {
    
    graph g = createGraph() ;
    inputBusStop(g) ;
    inputEdges(g) ;
    inputBusinStop(g) ;

    char s[100], f[100] ;
    char c ;
    int i = 0;
    printf("Enter starting stop: ") ;
    while ((c = getc(stdin)) != '\n') {
        s[i] = c;
        i++ ;
    }
    s[i] = '\0' ;
    i = 0;
    printf("Enter destination: ") ;
    while ((c = getc(stdin)) != '\n') {
        f[i] = c;
        i++ ;
    }
    f[i] = '\0' ;
    
    shortestPath(g, s, f) ;
    
    dropGraph(g) ;
    return 0 ;
}
