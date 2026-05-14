#ifndef LCSMATRIX_H
#define LCSMATRIX_H

#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>

class LCSMatrix {
private:
   int rowCount;
   int columnCount;
   std::string str1;
   std::string str2;

   // based on the unit tests, assume that a verifiable vector is required (no space optimization?)
   std::vector<std::vector<int>> matrix;
   std::unordered_set<std::string> LCSSet;

   // backtracks to rebuild subsequences from the constructed matrix
   void Backtrack(int i, int j, std::string currSeq, std::string& str1, std::string& str2) {
      if (i < 0 || j < 0) {
        std::reverse(currSeq.begin(), currSeq.end());
        LCSSet.insert(currSeq);
        return;
      }

      if (str1[i] == str2[j]) {
        Backtrack(i - 1, j - 1, currSeq + str1[i], str1, str2);
      }
      else {
        bool branched = false;
        if (j > 0 && matrix[i][j-1] == matrix[i][j]) {
            Backtrack(i, j-1, currSeq, str1, str2);
            branched = true;
        }
        if (i > 0 && matrix[i-1][j] == matrix[i][j]) {
            Backtrack(i-1, j, currSeq, str1, str2);
            branched = true;
        }

        // additional base case handles where LCS length != current LCS length and or iterators <= 0
        if (!branched) {
            Backtrack(-1, -1, currSeq, str1, str2);
        }
      }
    }

public:
   LCSMatrix(std::string& str1, std::string& str2) {
      this->rowCount = (int) str1.length();
      this->columnCount = (int) str2.length();
      matrix.assign(rowCount, std::vector<int>(columnCount, 0));
      if (rowCount == 0 || columnCount == 0) return;

      matrix[0][0] = (str1[0] == str2[0]) ? 1 : 0;
      for (int j = 1; j < columnCount; ++j) {
        matrix[0][j] = (str1[0] == str2[j]) ? 1 : matrix[0][j-1];
      }

      for (int i = 1; i < rowCount; ++i) {
        matrix[i][0] = (str1[i] == str2[0]) ? 1 : matrix[i-1][0];
        for (int j = 1; j < columnCount; ++j) {
            if (str1[i] == str2[j]) {
                matrix[i][j] = matrix[i-1][j-1] + 1;
            }
            else {
                matrix[i][j] = std::max(matrix[i][j-1], matrix[i-1][j]);
            }
        }
      }

      // case if LCS doesn't exist
      if (matrix[rowCount - 1][columnCount - 1] == 0) {
        return;
      }

      // proceed to backtrack the matrix and add subsequences to the set
      Backtrack(rowCount-1, columnCount-1, "", str1, str2);
    }

   int GetColumnCount() {
      return columnCount;
   }
   
   int GetEntry(int rowIndex, int columnIndex) {
      if (rowIndex >= 0 && columnIndex >= 0) {
        if (rowIndex <= rowCount && columnIndex <= columnCount) {
            return matrix[rowIndex][columnIndex];
        }
      }
      return 0;
   }
   
   int GetRowCount() {
      return rowCount;
   }
   
   std::unordered_set<std::string> GetLongestCommonSubsequences() {
      return LCSSet;
   }
};

#endif