#include<fstream>
#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>


namespace boost{
    std::string check(std::pair<int,int> const& p, std::vector<std::pair<int,int>>& v, bool eof) {
        auto it = std::find_if(v.begin(),v.end(),[&](std::pair<int,int> const& pa){return ((p.first==pa.first)&&(p.second==pa.second))||((p.first==pa.second)&&(p.second==pa.first));});
        if (true){
            v.push_back(p);
            if(eof){
                return (std::to_string(p.first) + " "+ std::to_string(p.second));
            } else {
                return ('\n' + std::to_string(p.first) + " "+ std::to_string(p.second) );
            }
        } else {
            return "";
        }
    }

    void create_manhattan_data(const int SIZE){
        std::ofstream file_in("data/manhattan_grid.dat", std::ios::trunc);
        std::vector<std::pair<int,int>> v;
        for (int i = 0; i < SIZE*SIZE; i++)
        {
            int row = i/SIZE;
            int column = i-row*(SIZE);
            if(column==0){
                if(row==0){
                    std::pair<int,int> p1(row*SIZE+column,row*SIZE+column+1);
                    std::pair<int,int> p2(row*SIZE+column,(row+1)*SIZE+column);
                    file_in << check(p1,v,true) << check(p2,v,false);
                } else if (row ==SIZE-1){
                    std::pair<int,int> p1(row*SIZE+column,row*SIZE+column+1);
                    std::pair<int,int> p2(row*SIZE+column, (row-1)*SIZE+column);
                    file_in << check(p1,v,false)<< check(p2,v,false);
                } else {
                    std::pair<int,int> p1(row*SIZE+column,row*SIZE+column+1);
                    std::pair<int,int> p2(row*SIZE+column,(row-1)*SIZE+column);
                    std::pair<int,int> p3(row*SIZE+column, (row+1)*SIZE+column);
                    file_in << check(p1,v,false) << check(p2,v,false) << check(p3,v,false);
                }
            } else if (column == SIZE-1){
                if(row==0){
                    std::pair<int,int> p1(row*SIZE+column,row*SIZE+column-1);
                    std::pair<int,int> p2(row*SIZE+column,(row+1)*SIZE+column);
                    file_in << check(p1,v,false) << check(p2,v,false);
                } else if (row ==SIZE-1){
                    std::pair<int,int> p1(row*SIZE+column, row*SIZE+column-1);
                    std::pair<int,int> p2(row*SIZE+column , (row-1)*SIZE+column);
                    file_in << check(p1,v,false) << check(p2,v,false);
                } else {
                    std::pair<int,int> p1(row*SIZE+column , row*SIZE+column-1);
                    std::pair<int,int> p2(row*SIZE+column ,(row-1)*SIZE+column);
                    std::pair<int,int> p3(row*SIZE+column , (row+1)*SIZE+column);
                    file_in << check(p1,v,false) << check(p2,v,false) << check(p3,v,false);
                }
            } else {
                if(row==0){
                    std::pair<int,int> p1(row*SIZE+column ,row*SIZE+column-1);
                    std::pair<int,int> p2(row*SIZE+column ,(row+1)*SIZE+column);
                    std::pair<int,int> p3(row*SIZE+column ,row*SIZE+column+1);
                    file_in << check(p1,v,false) << check(p2,v,false) << check(p3,v,false);
                } else if (row ==SIZE-1){
                    std::pair<int,int> p1(row*SIZE+column , row*SIZE+column-1);
                    std::pair<int,int> p2(row*SIZE+column , (row-1)*SIZE+column);
                    std::pair<int,int> p3(row*SIZE+column , row*SIZE+column+1);
                    file_in << check(p1,v,false) << check(p2,v,false) << check(p3,v,false);
                } else {
                    std::pair<int,int> p1(row*SIZE+column , row*SIZE+column-1);
                    std::pair<int,int> p2( row*SIZE+column , (row+1)*SIZE+column);
                    std::pair<int,int> p3(row*SIZE+column , row*SIZE+column+1);
                    std::pair<int,int> p4(row*SIZE+column , (row-1)*SIZE+column);
                    file_in << check(p1,v,false) << check(p2,v,false) << check(p3,v,false) << check(p4,v,false);
                }
            }
        }    
}
}


