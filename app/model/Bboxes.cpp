#include "Bboxes.hpp"
#include <iostream>
RTree *g_bboxtree=NULL;
bbox_map_info *cur_info=NULL;
//std::map<int,std::string> texture_file_names;
//std::vector<GtsBBox *> bboxes_all;
//std::map<int,bbox_map_info> gts_trans_map;

vector<pair<bbox_map_info, BoundingBox> > loadBBox (const char *str) {
    vector<pair<bbox_map_info, BoundingBox> > ret;
    char conf_name[255];
    FILE *bboxfp = fopen(str,"r");
    cout << "Opening "<< str<<endl;
    int count;
    if(bboxfp){

        char rname[255];
        char lname[255];
        double time;
        double x1,x2,y1,y2,z1,z2;
        double mtmp[4][4];
        int eof0=1, eof1=1,eof2=1,frame_count=0;
        while (eof0 != EOF && eof1 != EOF && eof2 != EOF){

            eof0 = fscanf(bboxfp,"%d %lf %s %s %lf %lf %lf %lf %lf %lf" ,&count, &time,lname,rname,
                          &x1,&y1,&z1,&x2,&y2,&z2);
            if(eof0 == EOF)
                break;
            for(int i=0; i < 4; i++)
                for(int j=0; j < 4; j++)
                    eof1 = fscanf(bboxfp," %lf",&mtmp[i][j]);
            eof2 = fscanf(bboxfp,"\n");
            bbox_map_info info;
            info.leftname=lname;
            info.rightname=rname;
            info.count=count;
            info.time=time;
            double zepi=2.0;
            BoundingBox bb;

            bb.edges[0].first  = x1;
            bb.edges[0].second = x2;

            bb.edges[1].first  = y1;
            bb.edges[1].second = y2;

            bb.edges[2].first  = z1;
            bb.edges[2].second = z2;

            ret.push_back (make_pair (info, bb));
            frame_count++;

        }

        fclose(bboxfp);
        printf("Loaded %d boxes\n",frame_count);
    }
    return ret;
}

bool find_closet_img_idx(const vector<pair<bbox_map_info, BoundingBox> > &bboxes,
                         osg::Vec3 pt,bbox_map_info &boxinfo, std::vector<QString> &files) {
    files.resize (0);
    BoundingBox bb;
    double epi=0.01;
    bb.edges[0].first  = pt[0];
    bb.edges[0].second = pt[0]+epi;
    bb.edges[1].first  = pt[1];
    bb.edges[1].second = pt[1]+epi;
    bb.edges[2].first  = pt[2];
    bb.edges[2].second = pt[2]+epi;

    for (vector<pair<bbox_map_info, BoundingBox> >::const_iterator it=bboxes.begin (); it!=bboxes.end (); ++it) {
        if (it->second.encloses (bb)) {
            boxinfo = it->first;
            files.push_back (QString (it->first.leftname.c_str ()));
        }
    }

    if (files.size ())
        return true;
    return false;
}
