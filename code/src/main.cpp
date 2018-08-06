#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>
#include <queue>

using namespace cv;
using namespace std;

//vector operations
void setEdge(vector<vector<pair<int, int> > > &graph, int row, int col, int value) {
    vector<pair<int,int> >  &vec = graph.at(row);
    vec.push_back(make_pair(col,value));
}

int readEdge(vector<vector<pair<int,int> > > &graph,int row, int col){
        vector<pair<int,int> > &v = graph.at(row);
        for(int j=0; j<v.size();j++){
            pair<int,int> &p = v.at(j);
            if(p.first == col){
                return p.second;
            }
        }
    return 0;
}

void updateEdge(vector<vector<pair<int, int> > > &graph, int row, int col, int updateValue) {
        vector<pair<int,int> > &v = graph.at(row);
        for(int j=0; j<v.size();j++){
            pair<int,int> &p = v.at(j);
            if(p.first == col){
                v.at(j) = make_pair(col,updateValue);
            }
        }
}

bool breadthFirstSearchWithVector(vector<vector<pair<int,int> > > &rGraph, int source, int target, int *parent, int V) {
    bool *visited= new bool[V]();
    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;
    int f,qFront;
    vector<pair<int,int> > vectorPairs;
    pair<int,int> p;
    while (!q.empty()) {
        qFront = q.front();
        q.pop();
        vectorPairs = rGraph.at(qFront);
        for (int i=0; i<vectorPairs.size(); i++) {
            p = vectorPairs.at(i);
            f= p.first;
            if (!visited[f] && readEdge(rGraph,qFront,f) > 0) {
                parent[f] = qFront;
                visited[f] = true;
                q.push(f);
            }
        }
        if(visited[target])
            break;
    }

    return visited[target];
}

void modifiedBFS(vector<vector<pair<int,int> > > &rGraph, int s, bool *visited, int V){
    queue<int> q;
    q.push(s);
    visited[s] = true;
    int qFirst,f;
    vector<pair<int,int> > vectorPairs;
    pair<int,int> p;
    while (!q.empty()) {
        qFirst = q.front();
        q.pop();
        vectorPairs = rGraph.at(qFirst);
        for (int i=0; i<vectorPairs.size(); i++) {
            p = vectorPairs.at(i);
            f= p.first;
            if (!visited[f] && readEdge(rGraph,qFirst,f) > 0) {
                visited[f] = true;
                q.push(f);
            }
        }
    }
}

void giveMeMinCut(vector<vector<pair<int,int> > > &graph, int s, int t, int V, Mat &out) {
    int u, v;
    int count = 0;
    vector<vector<pair<int,int> > > rGraph(graph);
    int *path = new int[V];
    int updatedValue=0;
    while (breadthFirstSearchWithVector(rGraph, s, t, path, V)) {
        int capacity = INT_MAX;
        for (v = t; v != s; v = path[v]) {
            u = path[v];
            capacity = min(capacity, readEdge(graph,u,v));
        }

        for (v = t; v != s; v = path[v]) {
            u = path[v];
            updatedValue = readEdge(rGraph,u,v) - capacity;
            updateEdge(rGraph,u,v,updatedValue);
            updatedValue = readEdge(rGraph,v,u) + capacity;
            updateEdge(rGraph,v,u,updatedValue);
        }
    }

    bool *visited = new bool[V]();
    modifiedBFS(rGraph, s, visited, V);

    Vec3b backgroundPixel;
    backgroundPixel[0] = 255; //b
    backgroundPixel[1] = 0; //g
    backgroundPixel[2] = 0; //r

    Vec3b foregroundPixel;
    foregroundPixel[0] = 0; //b
    foregroundPixel[1] = 0; //g
    foregroundPixel[2] = 255; //r
    int x, y, l, N = out.cols;
    for (l = 1; l < V; ++l) {
        x = (l-1) / N;
        y = (l-1) % N;
        if (visited[l] == true) {
            out.at<Vec3b>(x, y) = backgroundPixel;
        } else out.at<Vec3b>(x, y) = foregroundPixel;
    }

    delete path;
    delete visited;
    return;
}

void createAdjacencyList(vector<vector<pair<int,int> > > &graph, int rows, int cols, Mat &gray) {
    int N = cols, current, mapping;
    int samePixels = 1000000, diffPixels = 1;
    //fill adjacency list.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            current = 1 + N * i + j;
            if (i == 0) {
                if (j == 0) {
                    mapping = 1 + N * i + (j + 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j + 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i + 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i + 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);
                } else if (j == (cols - 1)) {
                    mapping = 1 + N * i + (j - 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j - 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i + 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i + 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);
                } else {
                    mapping = 1 + N * i + (j - 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j - 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * i + (j + 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j + 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i + 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i + 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);
                }
            } else if (i == (rows - 1)) {
                if (j == 0) {
                    mapping = 1 + N * i + (j + 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j + 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i - 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i - 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);
                } else if (j == (cols - 1)) {
                    mapping = 1 + N * i + (j - 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j - 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i - 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i - 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);
                } else {
                    mapping = 1 + N * i + (j - 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j - 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * i + (j + 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j + 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i - 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i - 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);
                }
            } else {
                if (j == 0) {
                    mapping = 1 + N * i + (j + 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j + 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i - 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i - 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i + 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i + 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);
                } else if (j == (cols - 1)) {
                    mapping = 1 + N * i + (j - 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j - 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i - 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i - 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i + 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i + 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);
                } else {
                    mapping = 1 + N * i + (j - 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j - 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * i + (j + 1);
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i, j + 1)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i - 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i - 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);

                    mapping = 1 + N * (i + 1) + j;
                    if ((int) gray.at<uchar>(i, j) == (int) gray.at<uchar>(i + 1, j)) {
                        setEdge(graph,current,mapping,samePixels);
                    } else setEdge(graph,current,mapping,diffPixels);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        cout << "Usage: ../seg input_image initialization_file output_mask" << endl;
        return -1;
    }

    // Load the input image
    // the image should be a 3 channel image by default but we will double check that in teh seam_carving
    Mat in_image;
    in_image = imread(argv[1]/*, CV_LOAD_IMAGE_COLOR*/);

    if (!in_image.data) {
        cout << "Could not load input image!!!" << endl;
        return -1;
    }

    if (in_image.channels() != 3) {
        cout << "Image does not have 3 channels!!! " << in_image.depth() << endl;
        return -1;
    }

    // the output image
    Mat out_image = in_image.clone();

    ifstream f(argv[2]);
    if (!f) {
        cout << "Could not load initial mask file!!!" << endl;
        return -1;
    }

    int cols = in_image.cols;
    int rows = in_image.rows;
    vector<Point> fgPixels;
    vector<Point> bgPixels;
    Point p;

    int n;

    f>>n;

    // get the initil pixels
    for(int i=0;i<n;++i){
        int x, y, t;
        f>>x>>y>>t;

        if(x<0 || x>=cols || y<0 || y>=rows){
            cout<<"Invalid pixel mask!"<<endl;
            return -1;
        }

        p.x=x;
        p.y=y;
        if(t==1){
            fgPixels.push_back(p);
        } else {
            bgPixels.push_back(p);
        }
    }

    Mat gray;
    cvtColor(in_image, gray, CV_BGR2GRAY);

    int numOfVer = (rows * cols)+2;
    vector< vector<pair<int,int> > > graph(numOfVer);
    createAdjacencyList(graph, rows, cols, gray);
    int pix;
    for (int k = 0; k < fgPixels.size() ; ++k) {
        pix=fgPixels.at(k).y*cols+fgPixels.at(k).x+1;//+1 as we added 0 as out S pixel
        setEdge(graph,pix,numOfVer-1,1000000);
    }
    for (int k = 0; k < bgPixels.size() ; ++k) {
        pix=bgPixels.at(k).y*cols+bgPixels.at(k).x+1;//+1 as we added 0 as out S pixel
        setEdge(graph,0,pix,1000000);
    }

    int s =0, t =numOfVer-1;
    Mat out(in_image.rows, in_image.cols, in_image.type());
    giveMeMinCut(graph, s, t, numOfVer, out);

    out_image = out.clone();
    // write it on disk
    imwrite(argv[3], out_image);

    namedWindow( "Original image", WINDOW_AUTOSIZE );
    namedWindow( "Show Marked Pixels", WINDOW_AUTOSIZE );
    imshow( "Original image", in_image );
    imshow( "Show Marked Pixels", out_image );
    waitKey(0);
    return 0;
}