#include <vector>
#include <map>


class cubeData{
    public:
        std::vector<int> indexes;
        std::vector<int> front;
        std::vector<int> back;
        std::vector<int> left;
        std::vector<int> right;
        std::vector<int> top;
        std::vector<int> bottom;
        void leftCW();
        void rightCW();
        void bottomCW();
        void topCW();
        void inPlaceRotate(int face, int direction);
        int getIndexAfter(int index, int direction);
        void printcube();
        cubeData();
        ~cubeData(void);
};