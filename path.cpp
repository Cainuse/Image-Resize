#include "path.h"
#include <queue>
#include <stack>
using namespace std;

path::path(const PNG & im, pair<int,int> s, pair<int,int> e)
   :start(s),end(e),image(im){
    BFS();
}
    // used to initialize member variable pathPts.
    // called by constructor to create path if it 
    // exists. 
    //
    // requires: neighbors, good, assemble helpers
    //
    // See the interfaces for good and assemble
    // to get a hint on two auxiliary structures
    // you will want to build: predecessor table,
    // and visited table.

void path::BFS(){
	// initialize working vectors
	vector<vector<bool>> V(image.height(), vector<bool> (image.width(),false)); //Builds 2D vector resembling png file with all falses
	vector<vector<pair<int,int>>> P(image.height(), vector<pair<int,int>> (image.width(),end));

    queue<pair<int, int>> queue;
    //Traverse the Graph
    V[start.second][start.first] = true;
    queue.push(start);
    /* your code here */

    while(!queue.empty()){
        pair<int, int> vertex = queue.front();
        queue.pop();
        vector<pair<int,int>> adj = neighbors(vertex);
        for(unsigned int i = 0; i<adj.size(); i++){
            if(good(V, vertex, adj[i])){
                V[adj[i].second][adj[i].first]=true;
                P[adj[i].second][adj[i].first]=vertex;
                queue.push(adj[i]);
            }
        }
    }
	pathPts = assemble(P,start,end);
    cout <<length()<<endl;
}

    //draws path points in red on a copy of the image and returns it
PNG path::render(){
    PNG ret = PNG(image);
    RGBAPixel red = RGBAPixel(255,0,0);
    for(unsigned int i = 0; i<pathPts.size(); i++){
        *ret.getPixel(pathPts[i].first, pathPts[i].second) = red;
    }
    return ret;
}

vector<pair<int,int>> path::getPath() { return pathPts;}

    //returns length of shortest path of points
int path::length() { return pathPts.size();}

    // tests a neighbor (adjacent vertex) to see if it is 
    // 1. within the image, 2. unvisited, and 3. close in color to curr.
    // An entry in table V is true if a cell has previously been 
    // visited.
bool path::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
    if(next.first>=0 && next.first<image.width() && next.second>=0 && next.second<image.height()){//Test within image
        if(!v[next.second][next.first]){ //Test unvisited
            RGBAPixel* currPix = image.getPixel(curr.first, curr.second);
            RGBAPixel* nextPix = image.getPixel(next.first, next.second);
            if(closeEnough(*currPix, *nextPix)){
                return true;
            }
        } 
    }
    
    return false;
}

    // builds a vector containing the locations of the 
    // four vertices adjacent to curr:
    // above, left, right, below (no particular order). 
    // does not pay any attention to whether or not the neighbors are 
    // valid (in the image, previously visited, or the right color).
vector<pair<int,int>> path::neighbors(pair<int,int> curr) {
	vector<pair<int,int>> n;
    n.push_back(pair<int,int>(curr.first-1, curr.second));// LEFT
    n.push_back(pair<int,int>(curr.first, curr.second+1));// BELOW
    n.push_back(pair<int,int>(curr.first+1, curr.second));// RIGHT
    n.push_back(pair<int,int>(curr.first, curr.second-1));// ABOVE
    
    
    
    return n;
}

    // Assumes the predecessor table, built in the BFS as follows: For each 
    // location in the image reachable from the start vertex, "loc", the 
    // table contains the location "pred" from which "loc" was first seen.
    // ("pred", "loc") is thus an edge in the shortest path from s to 
    // "loc".
    //
    // returns the set of points on the shortest path from s to e, if 
    // it exists. Call this vector P.
    //
    // if there is a shortest path: position 0 should contain s, 
    // and for all 0 < i < size, P[0] to P[i] is the set of points
    // on the shortest path from s to point P[i]. P[size-1] == e.

    // if no path from s to e exists, then just return a single element
    // vector P with P[0] == s.
vector<pair<int,int>> path::assemble(vector<vector<pair<int,int>>> & p,pair<int,int> s, pair<int,int> e) {
    
    vector<pair<int,int>> P;
    /* hint, gold code contains the following line:*/
	stack<pair<int,int>> S; 
   

    pair<int,int> curr = e;
    while(p[curr.second][curr.first]!=e){
        S.push(curr);
        curr = p[curr.second][curr.first];
            
    }

    S.push(s);
    
    while(!S.empty()){
        pair<int,int> curr = S.top();
        S.pop();
        P.push_back(curr);
    }
    
    return P;
}
    // tests whether p1 and p2 are near in color. returns 
    // true if the sum of squared difference over color channels
    // is less than or equal to 80. 
bool path::closeEnough(RGBAPixel p1, RGBAPixel p2){
   int dist = (p1.r-p2.r)*(p1.r-p2.r) + (p1.g-p2.g)*(p1.g-p2.g) +
               (p1.b-p2.b)*(p1.b-p2.b);

   return (dist <= 80);
}
