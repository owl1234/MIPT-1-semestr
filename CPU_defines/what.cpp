#include <iostream>
#include <cstdlib>
#include <cmath>

int main(){
  unsigned n;
  size_t R;
  size_t N;
  double di;
  std::cout << "Enter R: \n";
  std::cin >> R;
  N = 2*R;
  di = 1.0/R;
  for( int i=int(R);i>=-int(R);--i ){
    n = static_cast<int>(cos(asin(di*abs(int(i))))*R);
    for( unsigned j=0;j<=R-n;++j ) std::cout << " ";
    std::cout << "*";
    for( unsigned j=0;j<2*n;++j ) std::cout << " ";
    std::cout << "*\n";
  }
}
