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
        void leftCCW();
        void rightCCW();
        void bottomCCW();
        void topCCW();
        void frontCCW();
        void backCCW();
        void rotate(int face, int direction);
        void frontR(int);
        void backR(int);
        void leftR(int);
        void rightR(int);
        void topR(int);
        void bottomR(int);
        void printcube();
        cubeData();
        ~cubeData(void);
};