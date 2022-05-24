#include "cubeData.h"
#include <iostream>

cubeData::cubeData ()
{
    for (int i = 0; i< 27; i++){
        indexes.push_back(i);
    }
    left = {0,1,2,3,4,5,6,7,8};
    right = {18,19,20,21,22,23,24,25,26};
    top = {6,7,8,15,16,17,24,25,26};
    bottom = {0,1,2,9,10,11,18,19,20};
    front = {2,5,8,11,14,17,20,23,26};
    back = {0,3,6,9,12,15,18,21,24};
}

void cubeData :: printcube(){
    // inPlaceRotate(1,2);
    for (int i = 0; i<27; i++){
            std::cout << indexes[i] << "  ";    
            if (indexes[i]<10)
                std::cout << " ";
    }   
    std::cout << std::endl;
}

std::vector<int> cubeData :: getIndexes(int face)
{
    std::vector<int> temp;
    switch(face){
        case 0: 
            temp = front;
            break;
        case 1:
            temp = back;
            break;
        case 2:
            temp = right;
            break;
        case 3:
            temp = left;
            break;
        case 4:
            temp = top;
            break;
        case 5:
            temp = bottom;
            break;
    }
    return {indexes[temp[0]],indexes[temp[1]],indexes[temp[2]],
            indexes[temp[3]],indexes[temp[4]],indexes[temp[5]],
            indexes[temp[6]],indexes[temp[7]],indexes[temp[8]]};

}

void cubeData::leftCW(){
    int temp = indexes[2];
    indexes[2] = indexes[0];
    int temp2 = indexes[8];
    indexes[8] = temp;
    temp = indexes[6];
    indexes[6] = temp2;
    indexes[0] = temp;
    temp = indexes[5];
    indexes[5] = indexes[1];
    temp2 = indexes[7];
    indexes[7] = temp;
    temp = indexes[3];
    indexes[3] = temp2;
    indexes[1] = temp;
}

void cubeData::rightCW(){
    int temp = indexes[24];
    indexes[24] = indexes[18];
    int temp2 = indexes[26];
    indexes[26] = temp;
    temp = indexes[20];
    indexes[20] = temp2;
    indexes[18] = temp;
    temp = indexes[25];
    indexes[25] = indexes[21];
    temp2 = indexes[23];
    indexes[23] = temp;
    temp = indexes[19];
    indexes[19] = temp2;
    indexes[21] = temp;
}

void cubeData::bottomCW(){
    int temp = indexes[0];
    indexes[0] = indexes[2];
    int temp2 = indexes[18];
    indexes[18] = temp;
    temp = indexes[20];
    indexes[20] = temp2;
    indexes[2] = temp;
    temp = indexes[1];
    indexes[1] = indexes[11];
    temp2 = indexes[9];
    indexes[9] = temp;
    temp = indexes[19];
    indexes[19] = temp2;
    indexes[11] = temp;
}

void cubeData::topCW(){
    int temp = indexes[6];
    indexes[6] = indexes[8];
    int temp2 = indexes[24];
    indexes[24] = temp;
    temp = indexes[26];
    indexes[26] = temp2;
    indexes[8] = temp;
    temp = indexes[15];
    indexes[15] = indexes[7];
    temp2 = indexes[25];
    indexes[25] = temp;
    temp = indexes[17];
    indexes[17] = temp2;
    indexes[7] = temp;
}

void cubeData::frontCW(){
    int temp = indexes[8];
    indexes[8] = indexes[2];
    int temp2 = indexes[26];
    indexes[26] = temp;
    temp = indexes[20];
    indexes[20] = temp2;
    indexes[2] = temp;
    temp = indexes[17];
    indexes[17] = indexes[5];
    temp2 = indexes[23];
    indexes[23] = temp;
    temp = indexes[11];
    indexes[11] = temp2;
    indexes[5] = temp;
}




void cubeData::inPlaceRotate(int face, int direction){
    std::vector<std::vector<int>> temp2;
    std::vector<std::vector<int>> temp = {{0,0,0}, 
                                        {0, 0, 0 }, 
                                        {0,0,0}};
    // switch(face){
    //     case 0: 
    //         temp2 = front;
    //         break;
    //     case 1:
    //         temp2 = back;
    //         break;
    //     case 2:
    //         temp2 = right;
    //         break;
    //     case 3:
    //         temp2 = left;
    //         break;
    //     case 4:
    //         temp2 = top;
    //         break;
    //     case 5:
    //         temp2 = bottom;
    //         break;
    // }
    if(direction){
        for (int i = 0; i< 3; i++){//counter clockwise rotate
            for (int j = 0; j< 3; j++){
                temp[i][j] = temp2[j][2-i];
            }
        }
    }
    else{
        for (int i = 0; i< 3; i++){// clockwise rotate
            for (int j = 0; j< 3; j++){
                temp[i][j] = temp2[2-j][i];
            }
        }
    }
    // switch(face){
    //     case 0: 
    //         front = temp;
    //         break;
    //     case 1:
    //         back = temp;
    //         break;
    //     case 2:
    //         right = temp; 
    //         break;
    //     case 3:
    //         left = temp;
    //         break;
    //     case 4:
    //         top = temp;
    //         break;
    //     case 5:
    //         bottom = temp;
    //         break;
    // }
}