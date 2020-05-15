//
//  main.cpp
//  Sudoku2
//
//  Created by NguyenQ.Thai on 5/14/20.
//  Copyright © 2020 NguyenQ.Thai. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#define maxSize 9

using namespace std;

class zeroCells{
public:
    zeroCells(){};
    zeroCells(int Value, int Row, int Column, int flatIndex): Value(Value), Row(Row), Column(Column), flatIndex(flatIndex){};
    int Value = 0;
    int Row;
    int Column;
    int flatIndex;
};

void fillVector(vector<zeroCells>& newVector, int Value, int Row, int Column, int flatIndex){
    zeroCells newCell(Value, Row, Column, flatIndex);
    newVector.push_back(newCell);
}

vector <vector<int>> arrayInsert(){
    vector <vector<int>> puzzleArray;
    puzzleArray.resize(maxSize);
    for(int row = 0; row < maxSize; ++row)
        puzzleArray[row].resize(maxSize);
    for (int row = 0; row < maxSize; row++) {
        for (int column = 0; column < maxSize; column++)
            cin >> puzzleArray[row][column];
    }
    return puzzleArray;
}

bool noZeroIn (vector <vector<int>> puzzleArray){
    for (int row=0; row<maxSize; row++)
        for (int column=0; column< maxSize; column++)
            if (puzzleArray[row][column] == 0){
                return false;
                break;
            }
    return true;
}

int getCellOrder (vector <vector<int>> array, vector<zeroCells>& zerocells, int rowNum, int columnNum){
    for (int i=0; i<zerocells.size(); i++)
        if ((zerocells[i].Row==rowNum) && (zerocells[i].Column==columnNum)){
            return i;
            break;
        }
    return NULL;
}

int getCellRow (vector <vector<int>> array, vector<zeroCells>& zerocells, int order){
        return (zerocells[order].flatIndex-1)/maxSize;
}

int getCellColumn (vector <vector<int>> array, vector<zeroCells>& zerocells, int order){
        return (zerocells[order].flatIndex-1)%maxSize;
}

bool checkCellAlias (vector <vector<int>> array, int rowNum, int columnNum){
    int sqrtmaxSize = sqrt(maxSize);
    int columnBigCell = columnNum/sqrtmaxSize;
    int rowBigCell = rowNum/sqrtmaxSize;
    int count = 0;
    for (int rowCell=rowBigCell*sqrtmaxSize; rowCell< (rowBigCell*sqrtmaxSize+sqrtmaxSize); rowCell++)
        for (int columnCell=columnBigCell*sqrtmaxSize; columnCell< (columnBigCell*sqrtmaxSize+sqrtmaxSize); columnCell++)
            if (array[rowNum][columnNum] == array[rowCell][columnCell])
                count++;
    
    for (int column=0; column<maxSize; column++)
        if (array[rowNum][columnNum] == array[rowNum][column])
            count++;
    
    for (int row=0; row<maxSize; row++)
        if (array[rowNum][columnNum] == array[row][columnNum])
            count++;
    
    if (count == 3)
        return true;
    else
        return false;
}


int main(int argc, const char * argv[]) {
    vector <vector<int>> puzzleArray;
    cout << "Insert matrix:" << endl;
    puzzleArray = arrayInsert();
    auto start = chrono::high_resolution_clock::now();
    stack <int> zeroCellOrder;
    vector <zeroCells> zerocells;
    vector <int> subArray;
    for (int r=0; r<maxSize; r++)
        for (int c=0; c<maxSize; c++)
            subArray.push_back(puzzleArray[r][c]);
        
    while (noZeroIn(puzzleArray)==false){
        for (int row=0; row<maxSize; row++)
            for (int column=0; column<maxSize; column++)
                fillVector(zerocells, 0, row, column, row*maxSize+column);
        int flatindex = 0;
        zeroCellOrder.push(-1);
        while (flatindex < maxSize*maxSize){
            cout << 1 << endl;
            int row = flatindex /maxSize;
            int column = flatindex %maxSize;
            if ((puzzleArray[row][column] == 0) || (flatindex == zeroCellOrder.top())){
                zerocells[getCellOrder(puzzleArray, zerocells, row, column)].Value++;
                if (zerocells[getCellOrder(puzzleArray, zerocells, row, column)].Value == 1)
                    zeroCellOrder.push(getCellOrder(puzzleArray, zerocells, row, column));
                while (zerocells[getCellOrder(puzzleArray, zerocells, row, column)].Value <= maxSize){
                    puzzleArray[row][column] = zerocells[getCellOrder(puzzleArray, zerocells, row, column)].Value;
                    if (checkCellAlias(puzzleArray, row, column) == true)
                        break;
                    zerocells[getCellOrder(puzzleArray, zerocells, row, column)].Value++;
                }
                if (zerocells[getCellOrder(puzzleArray, zerocells, row, column)].Value > maxSize){
                    zerocells[getCellOrder(puzzleArray, zerocells, row, column)].Value = 0;
                    puzzleArray[row][column] = 0;
                    zeroCellOrder.pop();
                    flatindex = zeroCellOrder.top() -1;
                }
            }
            flatindex++;
        }
    }
    
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Solved matrix:" << endl;
    for (int row=0; row<maxSize; row++){
        for (int column=0; column< maxSize; column++)
            cout << puzzleArray[row][column] << '\t';
        cout << endl;
    }
    cout << "Time taken by execution: "<< duration.count() << " microseconds" << endl;
    return 0;
}
/*
0 5 0 0 0 0 0 0 9
3 4 0 0 0 0 0 6 0
8 0 0 0 2 0 0 0 0
0 0 0 6 0 0 0 0 0
0 9 0 0 3 0 1 0 0
0 0 0 0 0 9 5 0 3
4 0 0 0 0 8 0 0 1
0 0 0 3 0 2 0 7 8
6 0 0 0 0 0 0 4 0

 Expert: done
0 1 0 0 0 0 4 3 0
7 0 0 0 0 0 0 0 0
0 0 0 2 5 4 9 0 0
1 7 0 0 4 0 2 0 6
0 0 0 0 9 0 0 0 3
0 0 3 0 0 6 0 8 0
0 0 1 4 7 0 0 6 0
0 0 0 5 0 8 1 2 0
0 9 0 0 6 0 3 0 4
 
0 0 0 0 0 0 0 0 0
0 0 0 7 0 0 0 5 0
6 0 0 8 9 0 0 0 3
5 0 0 0 0 0 0 0 0
0 0 0 0 0 4 1 0 0
0 0 4 0 0 3 0 6 8
0 6 0 0 3 0 0 0 9
8 0 5 0 0 2 7 0 0
0 0 1 0 0 0 6 0 5
 
 Easy1: done
1 5 0 0 4 2 0 0 6
2 7 4 5 6 0 0 1 0
0 0 6 1 0 7 4 0 2
0 1 8 0 0 0 5 4 0
0 0 3 0 5 0 0 0 0
0 6 0 4 0 3 1 9 0
0 2 0 6 0 5 9 0 0
9 8 5 0 3 0 0 6 0
0 4 0 2 1 9 8 3 0
 
 Easy2: done
0 6 8 0 0 0 9 3 0
0 4 2 0 0 0 6 0 0
1 9 0 0 8 0 0 4 0
0 8 5 2 0 1 0 0 7
7 0 0 8 9 0 0 0 0
2 0 9 0 0 7 5 0 3
0 2 0 1 0 0 0 5 0
8 5 0 0 4 0 7 6 0
4 7 3 0 5 2 0 0 9
 
1 2 3 4
3 4 1 2
4 1 2 3
2 3 4 1
 
1 0 3 0
0 0 0 2
0 0 0 3
2 0 0 1
 
1 3 4 2
2 4 1 3
4 2 3 1
3 1 2 4
 
0 0 0 2
0 4 0 3
0 2 3 0
3 0 2 4
 */

