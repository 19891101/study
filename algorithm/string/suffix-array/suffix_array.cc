
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> UpdateRankFiner(const std::vector<int> &sa,
                                 const std::vector<int> &old_rank, int n, int w,
                                 int *last_p) {
  std::vector<int> rk(n);
  rk[sa[0]] = 0;
  int p = 0;
  for (int i = 1; i < n; ++i) {
    if (old_rank[sa[i]] == old_rank[sa[i - 1]] &&
        ((sa[i] + w >= n && sa[i - 1] + w >= n) ||
         (sa[i] + w < n && sa[i - 1] + w < n &&
          old_rank[sa[i] + w] == old_rank[sa[i - 1] + w]))) {
      rk[sa[i]] = p;
    } else {
      rk[sa[i]] = ++p;
    }
  }
  *last_p = p + 1;
  return rk;
}

template <typename Elem>
std::vector<Elem> CountingSortFiner(const std::vector<Elem> &eles, int C,
                                    std::function<int(const Elem &)> functor) {
  int n = static_cast<int>(eles.size());
  std::vector<int> bucket(C, 0);
  std::vector<int> bucket_result(n);
  for (int i = 0; i < n; ++i) {
    ++bucket[bucket_result[i] = functor(eles[i])];
  }
  for (int i = 1; i < C; ++i) {
    bucket[i] += bucket[i - 1];
  }
  std::vector<Elem> result(n);
  for (int i = n - 1; i >= 0; --i) {
    result[--bucket[bucket_result[i]]] = eles[i];
  }
  return result;
}

std::vector<int> SortSecondKeyWord(const std::vector<int> &sa, int n, int w) {
  std::vector<int> id(n);
  int p = 0;
  for (int i = n - 1; i >= n - w; --i) {
    id[p++] = i;
  }
  for (int i = 0; i < n; ++i) {
    if (sa[i] >= w) {
      id[p++] = sa[i] - w;
    }
  }
  return id;
}

std::vector<int> ComputeSA4(const std::string &s) {
  int n = static_cast<int>(s.size());
  int m = std::max(256, n) + 1;
  std::vector<int> sa(n);
  std::vector<int> rk(n);
  std::vector<int> cnt(m, 0);
  for (int i = 0; i < n; ++i) {
    ++cnt[rk[i] = s[i]];
  }
  for (int i = 1; i < m; ++i) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = n - 1; i >= 0; --i) {
    sa[--cnt[rk[i]]] = i;
  }
  for (int w = 1; w < n; w <<= 1) {
    sa = CountingSortFiner<int>(SortSecondKeyWord(sa, n, w), m,
                                [&](const int &x) { return rk[x]; });
    rk = UpdateRankFiner(sa, rk, n, w, &m);
  }
  return sa;
}

std::vector<int> ComputeHeight(const std::string &s,
                               const std::vector<int> &sa) {
  int n = static_cast<int>(s.size());
  std::vector<int> rk(n);
  for (int i = 0; i < n; ++i) {
    rk[sa[i]] = i;
  }
  std::vector<int> height(n, 0);
  for (int i = 0, k = 0; i < n; ++i) {
    std::cout << "XXX: " << i << "\n";
    if (rk[i] == 0) {
      continue;
    }
    if (k > 0) {
      --k;
    }
    while (i + k < n && sa[rk[i] - 1] + k < n &&
           s[i + k] == s[sa[rk[i] - 1] + k]) {
      ++k;
    }
    height[rk[i]] = k;
  }
  return height;
}

int main() {
  /*
    std::string s = "aabaaaab";
    auto sa = ComputeSA4(s);

    for (int x : sa) {
      printf("%d %s\n", x, s.substr(x).c_str());
    }
    auto h = ComputeHeight(s, sa);
    for (int x : h) {
      printf("%d ", x);
    }
    puts(""); */
  std::string s;
  std::cin >> s;
  auto sa = ComputeSA4(s);
  for (int x : sa) {
    std::cout << (x + 1) << " ";
  }
  std::cout << "\n";
  return 0;
}
