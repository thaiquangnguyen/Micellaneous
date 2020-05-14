//
//  main.cpp
//  Sudoku
//
//  Created by NguyenQ.Thai on 5/13/20.
//  Copyright © 2020 NguyenQ.Thai. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#define maxSize 9

using namespace std;

class zeroCells{
public:
    zeroCells(){};
    zeroCells(int Row, int Column, vector <int> possibleValues): Row(Row), Column(Column), possibleValues(possibleValues){};
    int Row;
    int Column;
    vector <int> possibleValues;
};

void fillVector(vector<zeroCells>& newVector, int Row, int Column, vector <int> possibleValues){
    zeroCells newCell(Row, Column, possibleValues);
    newVector.push_back(newCell);
}

vector <int> checkCellValues(vector <vector<int>> array, int rowNum, int columnNum){
    vector <int> arrayCellValue;
    int size = 0;
    while((maxSize+1) != size){
        arrayCellValue.push_back(size);
        size++;
    }
    int columnBigCell=0;
    int rowBigCell=0;
    int sqrtmaxSize = sqrt(maxSize);
    
    for (int squareRoot=0; squareRoot<sqrtmaxSize; squareRoot++){
        if ((columnNum >= (sqrtmaxSize*squareRoot)) && (columnNum < (sqrtmaxSize*squareRoot+sqrtmaxSize)))
            columnBigCell = squareRoot;
        if ((rowNum >= (sqrtmaxSize*squareRoot)) && (rowNum < (sqrtmaxSize*squareRoot+sqrtmaxSize)))
            rowBigCell = squareRoot;
    }
    for (int rowCell=rowBigCell*sqrtmaxSize; rowCell< (rowBigCell*sqrtmaxSize+sqrtmaxSize); rowCell++)
        for (int columnCell=columnBigCell*sqrtmaxSize; columnCell< (columnBigCell*sqrtmaxSize+sqrtmaxSize); columnCell++)
            arrayCellValue[array[rowCell][columnCell]] = (array[rowCell][columnCell] != 0)? 0:arrayCellValue[array[rowCell][columnCell]];
    
    for (int column=0; column<maxSize; column++){
        arrayCellValue[array[rowNum][column]] = (array[rowNum][column] != 0)? 0:arrayCellValue[array[rowNum][column]];
    }
    
    for (int row=0; row<maxSize; row++){
        arrayCellValue[array[row][columnNum]] = (array[row][columnNum] != 0)? 0:arrayCellValue[array[row][columnNum]];
    }
    
    vector <int> subArrayCellValue;
    for (int index=0; index< arrayCellValue.size(); index++)
        if (arrayCellValue[index] != 0){
            subArrayCellValue.push_back(arrayCellValue[index]);
        }
/*    cout << rowNum << '\t' << columnNum;
    for (int i=0; i<subArrayCellValue.size(); i++)
        cout << "value: " << subArrayCellValue[i] << '\t';
    cout << endl;*/
    return subArrayCellValue;
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
    for (int i=0; i<maxSize; i++)
            if ((zerocells[i].Row==rowNum) && (zerocells[i].Column==columnNum)){
                return i;
                break;
            }
    return NULL;
}

void remainVectorValue (vector<zeroCells>& zerocells, int cellOrder, int remainIndex){
    for (int k=0; k< zerocells[cellOrder].possibleValues.size(); k++)
        if (zerocells[cellOrder].possibleValues[k] != zerocells[cellOrder].possibleValues[remainIndex]){
            zerocells[cellOrder].possibleValues[k]=0;
        }
}

void eliminateCellValues(vector <vector<int>> array, vector<zeroCells>& zerocells, int rowNum, int columnNum){
    int columnBigCell=0;
    int rowBigCell=0;
    int sqrtmaxSize = sqrt(maxSize);
    int mainCellOrder = getCellOrder(array, zerocells, rowNum, columnNum);
    for (int squareRoot=0; squareRoot<sqrtmaxSize; squareRoot++){
        if ((columnNum >= (sqrtmaxSize*squareRoot)) && (columnNum < (sqrtmaxSize*squareRoot+sqrtmaxSize)))
            columnBigCell = squareRoot;
        if ((rowNum >= (sqrtmaxSize*squareRoot)) && (rowNum < (sqrtmaxSize*squareRoot+sqrtmaxSize)))
            rowBigCell = squareRoot;
    }
    for (int checkingIndex=0; checkingIndex< zerocells[mainCellOrder].possibleValues.size(); checkingIndex++){
        int count = 0;
        for (int rowCell=rowBigCell*sqrtmaxSize; rowCell< (rowBigCell*sqrtmaxSize+sqrtmaxSize); rowCell++)
            for (int columnCell=columnBigCell*sqrtmaxSize; columnCell< (columnBigCell*sqrtmaxSize+sqrtmaxSize); columnCell++)
                if ((array[rowCell][columnCell] == 0) && (rowCell != rowNum) && (columnCell != columnNum)){
                    int cellorder = getCellOrder(array, zerocells, rowCell, columnCell);
                    for (int j=0; j< zerocells[cellorder].possibleValues.size(); j++)
                        if (zerocells[mainCellOrder].possibleValues[checkingIndex] == zerocells[cellorder].possibleValues[j])
                            count++;
                }
        if (count == 0)
            remainVectorValue(zerocells, mainCellOrder, checkingIndex);
    }
    
    for (int index=0; index< zerocells[mainCellOrder].possibleValues.size(); index++)
        if (zerocells[mainCellOrder].possibleValues[index] == 0){
            zerocells[mainCellOrder].possibleValues.erase(zerocells[mainCellOrder].possibleValues.begin() + index);
            index--;
        }
}




int main(int argc, const char * argv[]) {
    vector <vector<int>> puzzleArray;
    cout << "Insert matrix:" << endl;
    puzzleArray = arrayInsert();
    vector <zeroCells> zerocells;
    while (noZeroIn(puzzleArray)==false){
        for (int row=0; row< maxSize; row++)
            for (int column=0; column< maxSize; column++)
                if (puzzleArray[row][column] == 0)
                    fillVector(zerocells,row,column,checkCellValues(puzzleArray, row, column));
        for (int row=0; row< maxSize; row++)
            for (int column=0; column< maxSize; column++)
                if (puzzleArray[row][column] == 0)
                    eliminateCellValues(puzzleArray, zerocells, row, column);
        for (int row=0; row< maxSize; row++)
            for (int column=0; column< maxSize; column++)
                if (puzzleArray[row][column] == 0)
                    if (zerocells[getCellOrder(puzzleArray, zerocells, row, column)].possibleValues.size() == 1){
                        puzzleArray[row][column] = zerocells[getCellOrder(puzzleArray, zerocells, row, column)].possibleValues.front();
                        zerocells.clear();
                    }
  /*      for (int r=0; r<maxSize; r++){
            for (int c=0;c<maxSize; c++)
                cout << puzzleArray[r][c] << '\t';
            cout << endl;
        }*/
    }
    cout << "Solved matrix:" << endl;
    for (int row=0; row<maxSize; row++){
        for (int column=0; column< maxSize; column++)
            cout << puzzleArray[row][column] << '\t';
        cout << endl;
    }
    
    return 0;
}
/*
0 1 0 0 0 0 4 3 0
7 0 0 0 0 0 0 0 0
0 0 0 2 5 4 9 0 0
1 7 0 0 4 0 2 0 6
0 0 0 0 9 0 0 0 3
0 0 3 0 0 6 0 8 0
0 0 1 4 7 0 0 6 0
0 0 0 5 0 8 1 2 0
0 9 0 0 6 0 3 0 4

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
 
 Easy2:
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
