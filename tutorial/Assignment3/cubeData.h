#include <vector>
#include <map>


class cubeData{
    private:
        std::vector<int> indexes;
        std::vector<int> front;
        std::vector<int> back;
        std::vector<int> left;
        std::vector<int> right;
        std::vector<int> top;
        std::vector<int> bottom;
    public:
        std::vector<int> getIndexes(int);
        void leftCW();
        void rightCW();
        void bottomCW();
        void topCW();
        void frontCW();
        void backCW();
        void inPlaceRotate(int face, int direction);
        int getIndexAfter(int index, int direction);
        void printcube();
        cubeData();
        ~cubeData(void);
};