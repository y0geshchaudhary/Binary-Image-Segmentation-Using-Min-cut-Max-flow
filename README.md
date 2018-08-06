# Binary-Image-Segmentation-Using-Min-cut-Max-flow

https://en.wikipedia.org/wiki/Max-flow_min-cut_theorem

Introduction

Binary Image segmentation is the process of classifying the pixels of an image into two categories: pixels
belonging to the foreground objects of an image and pixels belonging to the background objects of an image.
Figure 1 shows an example of image segmentation where the pixels belonging to the foreground objects are
highlighted in red and the pixels for the background are highlighted in blue. (Image I taken from the web). Image
segmentation is an important problem in image processing and computer vision with many application ranging
from background substraction and removal to object tracking, etc.

While there are many ways to model this problem mathematically, the one we will use for this homework in as a
min-cut finding problem with multiple sources and sinks [1] as described below. In this project, you are asked to
implement a simple binary image segmentation technique using min-cut and the OpenCV library.
Figure 1. Image segmentation examples from eth web.

The Min Graph-cut problem

Given a connected graph G(V, E), and two vertices s (source vertex) and t (sink vertex), a cut is a subset of
edges E’ that disconnects any path from s to t. A minimum cut E’’ is a cut where the sum of the weights of all
its edges is not larger than any other cut E’. The problem of minimum cut can be generalized to the case where
more than one source or sink exist as shown in the textbook. It is easy to observe that any cut of G classifies the
vertices in V into two disjoint sets: vertices connected to s and vertices connected to t.
As illustrated in class the min-cut problem and max-flow are dual to each other, in other words, if we solve for
one we also get the solution for the other. In class the Ford-Fulkerson algorithm for finding the maximum flow
was presented. However, for this homework you can choose any algorithm you wish as long as you submit your 
own original implementation that is not take in full or in parts from internet resources or other people either
students in the class or anyone else.

Image Segmentation using Graph-cuts

The binary image segmentation problem can be reduced to finding a minimum cut in the graph induced by the
image graph: the pixels are the vertices or nodes in the graph and we have edges between any neighboring
pixels in the horizontal and vertical direction (i.e. any vertex has maximum 4 neighbours).
Given an initial set of user specified foreground and background pixels (displayed as red and blue strokes in
Figure 1) you can use these as sources and targets (or sources and sinks depending on the terminology) and find
a minimum cut in this graph. After removing the edges in this cut, the pixels in the image will be disconnected
such that each pixel in the image will be connected to either the source the target. If a pixel is connected to the
source than it is a foreground pixel and if it is connected to the target than it is a background pixel.
For further resources including how to set up the weights in such a graph we recommend the following
resources: [1, 2,3];
