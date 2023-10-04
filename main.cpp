#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

void FillData(vector<vector<double>> &realData, int height, int width);

void PrintData(vector<vector<double>> &realData, int height, int width);

void Prediction(vector<vector<double>> &vectorParameters, int height, int width);

double Determinant(vector<vector<double>> &matrix);

vector<vector<double>> InverseMatrix(vector<vector<double>> &matrix);

void FillMatrix(vector<vector<double>> &matrix, int height, int width);

void Transpose(vector<vector<double>> &matrix, vector<vector<double>> &matrixTransposed, int height, int width);

void ProductMatrixTransposeAndMatrix(vector<vector<double>> &matrixSixSix, vector<vector<double>> &matrix, vector<vector<double>> &matrixTransposed, int size);

void ProductVectorDataAndMatrixT(vector<vector<double>> &buff, vector<vector<double>> &matrixTransposed, vector<vector<double>> &vectorData, int size);

void ProductMatrixSixSixAndBuff( vector<vector<double>> &vectorParameters ,vector<vector<double>> &buff,vector<vector<double>> &matrixSixSix);

void MakeVectorFromMatrix(vector<vector<double>> &realData, vector<vector<double>> &vectorData, int height, int width);

int main() {
    //Aw = b -> w = (A^T * A)^-1 * A^T * f
    int height, width, size;
    cin >> height >> width;
    size = height * width;
    vector<vector<double>> realData(height, vector<double>(width));
    vector<vector<double>> vectorData(size, vector<double>(1));
    vector<vector<double>> matrix(size, vector<double>(6));
    vector<vector<double>> matrixTransposed(6, vector<double>(size));
    vector<vector<double>> matrixSixSix(6, vector<double>(6));
    vector<vector<double>> vectorParameters(6, vector<double>(1));
    vector<vector<double>> buff(6, vector<double>(1));
    FillData(realData, height, width);
    FillMatrix(matrix, height, width);
    MakeVectorFromMatrix(realData, vectorData, height, width);
    Transpose(matrix, matrixTransposed, 6, size);
    ProductMatrixTransposeAndMatrix(matrixSixSix, matrix, matrixTransposed, size);
    ProductVectorDataAndMatrixT(buff, matrixTransposed, vectorData, size);
    matrixSixSix = InverseMatrix(matrixSixSix);
    ProductMatrixSixSixAndBuff(vectorParameters, buff, matrixSixSix);
    PrintData(realData, height, width);
    Prediction(vectorParameters, height, width);
    return 0;
}

//Запонлняем массив верными данными
void FillData(vector<vector<double>> &realData, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cin >> realData[i][j];
        }
    }
    std::cout << "\n";
}

//Выводим значения массива
void PrintData(vector<vector<double>> &realData, int height, int width) {
    cout << "Real data:\n";
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            cout << realData[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

//Считаем детерминант матрицы
double Determinant(vector<vector<double>> &matrix) {
    int n = matrix.size();
    double det = 0;

    if (n == 1) {
        return matrix[0][0];
    } else if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    } else {
        vector<vector<double>> submatrix(n - 1, vector<double>(n - 1));
        int i, l;
        for (int k = 0; k < n; k++) {
            i = 0;
            for (int j = 1; j < n; j++) {
                l = 0;
                for (int m = 0; m < n; m++) {
                    if (m != k) {
                        submatrix[i][l] = matrix[j][m];
                        l++;
                    }
                }
                i++;
            }

            det += pow(-1, k) * matrix[0][k] * Determinant(submatrix);
        }
        return det;
    }
}

//Ищем обратную матрицу
vector<vector<double>> InverseMatrix(vector<vector<double>> &matrix) {
    int n = matrix.size();
    double det = Determinant(matrix);

    if (det == 0) {
        cout << "Матрица вырожденная, обратной матрицы не существует.\n";
        return {};
    }
    vector<vector<double>> inverse(n, vector<double>(n));
    vector<vector<double>> submatrix(n - 1, vector<double>(n - 1));
    int l, m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            l = 0;
            for (int k = 0; k < n; k++) {
                if (k != i) {
                    m = 0;
                    for (int p = 0; p < n; p++) {
                        if (p != j) {
                            submatrix[l][m] = matrix[k][p];
                            m++;
                        }
                    }
                    l++;
                }
            }

            inverse[j][i] = pow(-1, i + j) * Determinant(submatrix) / det;
        }
    }
    return inverse;
}

// Заполняем матрицу, где на каждой строке будет (x^2, y^2, xy, x, y, 1)
void FillMatrix(vector<vector<double>> &matrix, int height, int width) {
    double x, y;
    int count = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            x = static_cast<double>(i) / height;
            y = static_cast<double>(j) / width;
            matrix[count][0] = x * x;
            matrix[count][1] = y * y;
            matrix[count][2] = x * y;
            matrix[count][3] = x;
            matrix[count][4] = y;
            matrix[count][5] = 1;
            count++;
        }
    }
}

// Находим A^T
void Transpose(vector<vector<double>> &matrix, vector<vector<double>> &matrixTransposed, int height, int width) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            matrixTransposed[i][j] = matrix[j][i];
        }
    }
}

// A^-1 * A
void ProductMatrixTransposeAndMatrix(vector<vector<double>> &matrixSixSix, vector<vector<double>> &matrix, vector<vector<double>> &matrixTransposed, int size) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            for (int k = 0; k < size; ++k) {
                matrixSixSix[i][j] += matrixTransposed[j][k] * matrix[k][i];
            }
        }
    }
}

// A^T * f
void ProductVectorDataAndMatrixT(vector<vector<double>> &buff, vector<vector<double>> &matrixTransposed, vector<vector<double>> &vectorData, int size) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < size; ++j) {
            buff[i][0] += matrixTransposed[i][j] * vectorData[j][0];
        }
    }
}

//Умножаем обратную матрицу (A^T * A)^-1 и buff, где buff = A^T * f
void ProductMatrixSixSixAndBuff( vector<vector<double>> &vectorParameters ,vector<vector<double>> &buff,vector<vector<double>> &matrixSixSix) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            vectorParameters[i][0] += matrixSixSix[i][j] * buff[j][0];
        }
    }
}

//Вытягиваем матрицу в столбец
void MakeVectorFromMatrix(vector<vector<double>> &realData, vector<vector<double>> &vectorData, int height, int width) {
    int count = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            vectorData[count][0] = realData[i][j];
            count++;
        }
    }
}

//Выводим предсказанные данные и коэффициенты функции
void Prediction(vector<vector<double>> &vectorParameters, int height, int width) {
    double x, y;
    double A, B, C, D, E, F;
    A = vectorParameters[0][0];
    B = vectorParameters[1][0];
    C = vectorParameters[2][0];
    D = vectorParameters[3][0];
    E = vectorParameters[4][0];
    F = vectorParameters[5][0];
    cout << "Predicted Data:\n";
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            x = static_cast<double>(i) / height;
            y = static_cast<double>(j) / width;
            cout << fixed << setprecision(4) << A * x * x + B * y * y + C * x * y + D * x + E * y + F << " ";
        }
        cout << endl;
    }
    cout << fixed << setprecision(5)
              << "Parameters:"
              << "\nA: " << A
              << "\nB: " << B
              << "\nC: " << C
              << "\nD: " << D
              << "\nE: " << E
              << "\nF: " << F
              << "\n";
}
