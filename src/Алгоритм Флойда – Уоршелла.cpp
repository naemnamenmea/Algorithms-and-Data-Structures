#include <iostream> 

using namespace std;

const int maxV = 1000;
int i, j, n;
int GR[maxV][maxV];

void FU(int D[][maxV], int V) {
  int k;
  for (i = 0; i < V; i++) 
    D[i][i] = 0;
  
  for (k = 0; k < V; k++)
    for (i = 0; i < V; i++)
      for (j = 0; j < V; j++)
        if (D[i][k] && D[k][j] && i != j)
          if (D[i][k] + D[k][j] < D[i][j] || D[i][j] == 0) 
            D[i][j] = D[i][k] + D[k][j];
            
  for (i = 0; i < V; i++) {
    for (j = 0; j < V; j++) 
        cout << D[i][j] << "\t";
        
    cout << endl;
  }
}

int main() {
  setlocale(LC_ALL, "Rus");
  cout << "Количество вершин в графе > ";
  cin >> n;
  cout << "Введите матрицу весов ребер:\n";
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      cout << "GR[" << i + 1 << "][" << j + 1 << "] > ";
      cin >> GR[i][j];
    }
  cout << "Матрица кратчайших путей:" << endl;
  FU(GR, n);
  system("pause>>void");
  return 0;
}