#include <iostream>
#include <queue>
#include <fstream>
#include <cmath>

using namespace std;

typedef pair<int, int> ii;
vector<ii> points;
vector< vector<int> > neighbours;
vector<int> corePoints;
vector<bool> clustered;
vector<int> cluster;

int curcluster = 1;
double eps;
int minpts;

double eu_distance(ii p1, ii p2)
{
  double answer = (p1.first - p2.first) * (p1.first - p2.first);
  answer += (p1.second - p2.second) * (p1.second - p2.second);
  return sqrt(answer);
}

void expand(int i)
{
  queue<int> Q;
  Q.push(i);
  clustered[i] = true;
  while(!Q.empty())
  {
    int curpoint = Q.front();
    Q.pop();
    for(int k = 0; k < neighbours[curpoint].size(); k++)
    {
      int p = neighbours[curpoint][k];
      if(!clustered[p])
      {
        cluster[p] = curcluster;
        clustered[p] = true;
        if(neighbours[p].size() > minpts)
          Q.push(p);
      }
    }
  }
  curcluster++;
}

int main()
{
  int x, y;
  cout<<"Input EPS value: ";
  cin>>eps;
  cout<<"Input MinPts value: ";
  cin>>minpts;
  ifstream fin;
  ofstream fout;
  fin.open("points.dat");
  while(!fin.eof())
  {
    fin>>x>>y;
    points.push_back(ii(x, y));
    clustered.push_back(false);
    cluster.push_back(0);
    neighbours.push_back(vector<int>());
  }
  fin.close();
  for(int i = 0; i < points.size(); i++)
    for(int j = 0; j < points.size(); j++)
    {
      if(i != j && eu_distance(points[i], points[j]) <= eps)
        neighbours[i].push_back(j);
    }
  for(int i = 0; i < neighbours.size(); i++)
    if(neighbours[i].size() >= minpts)
      corePoints.push_back(i);
  for(int i = 0; i < corePoints.size(); i++)
    if(!clustered[i])
      expand(i);
  fin.open("points.dat"); 
  fout.open("clustered_points_dbscan.dat");
  int i = 0;
  int max = cluster[1];
  while(!fin.eof())
  {
    fin>>x>>y;
    if(cluster[i] != 0)
      fout<<x<<" "<<y<<" "<<cluster[i]<<"\n";
    if(cluster[i] > max)
      max = cluster[i];
    i++;
  }
  fin.close();
  fout.close();
  cout<<"DBSCAN discovered "<<max<<" clusters with given constraints\n";
  return 0;  
}
