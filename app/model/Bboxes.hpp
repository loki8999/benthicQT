#ifndef BBOXES_H
#define BBOXES_H
#include <stdlib.h>
#include <stdio.h>
#include <RStarTree.h>
#include <map>
#include <vector>
#include <string>
#include <osg/Vec3>
#include <QString>
using namespace std;

typedef struct _bbox_map_info{
  string leftname;
  string rightname;
  double time;
  int count;
}bbox_map_info;
typedef RStarTree<bbox_map_info, 3, 8, 64> 	RTree;
typedef RTree::BoundingBox			BoundingBox;

bool find_closet_img_idx(const std::vector<std::pair<bbox_map_info, RTree::BoundingBox> > &bboxes,
                         osg::Vec3 pt,bbox_map_info &boxinfo, std::vector<QString> &files);
std::vector<std::pair<bbox_map_info, RTree::BoundingBox> > loadBBox (const char *str);

#endif
