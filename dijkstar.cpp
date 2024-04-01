#include <bits/stdc++.h>
#include <fmt/core.h>

using namespace std;

void dijkstar() {
  // 定义最大值
  // 时间复杂度0(n^2)
  int M                               = INT_MAX;
  // 初始化邻接矩阵
  ::std::vector<::std::vector<int>> e = {
      {0, 1, 1, M, M,  M },
      {M, 0, 9, 3, M,  M },
      {M, M, 0, M, 5,  M },
      {M, M, 4, 0, 13, 15},
      {M, M, M, M, 0,  4 },
      {M, M, M, M, M,  0 },
  };

  int n = (int)e.size();
  ::std::vector<int> cost(e[0]);
  ::std::vector<int> used(n, 0);

  // 初始化cost

  used[0] = 1;  // 1代表已经更新完成
  int min = 0;
  int u   = 0;  // 记录离0最小距离的坐标
                //
  for (int i = 0; i < n; i++) {
    // 找到离1号顶点最近的顶点
    min = M;
    for (int j = 1; j < n; j++) {
      if (used[j] == 0 && cost[j] < min) {
        min = cost[j];
        u   = j;
      }
    }

    used[u] = 1;
    // 更新cost
    for (int v = 1; v < n; v++) {
      if (e[u][v] >= M) {
      }

      if (cost[v] > cost[u] + e[u][v])
        cost[v] = cost[u] + e[u][v];
    }
  }

  // 打印更新过的结果
  for (auto d : cost) {
    cout << d << endl;
  }
}

int main() {
  dijkstar();

  return 0;
}
