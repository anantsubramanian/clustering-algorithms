#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

int main()
{
  char equal;
  cin>>equal;
  srand(time(NULL));
  int delta = 1 + rand()%750;
  for(int i = 0; i < 40; i++)
  {
    int randomcordx = 750 + rand()%9250;
    int randomcordy = 750 + rand()%9250;
    int randomdelta;
    if(equal == 'y')
      randomdelta = delta;
    else
      randomdelta = 1 + rand()%750;
    for(int j = 0; j < 400; j++)
    {
      int x = rand()%randomdelta;
      int y = 1 + ceil(sqrt(randomdelta*randomdelta - x*x));
      y = rand()%y;
      x = rand()%2 >= 1 ? x : -x;
      y = rand()%2 >= 1 ? y : -y;
      y += randomcordy;
      x += randomcordx;
      cout<<x<<" " <<y<<"\n";
    }
  }
  return 0;  
}
