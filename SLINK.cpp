// TODO: Convert O((n^2)logn) to O(n^2)

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<int, int> ii;
typedef pair<ii, double> iid;

vector<ii> points;
vector<int> cluster;
vector<int> newcluster;
vector<iid> distances;

double eu_distance(ii p1, ii p2)
{
  double answer = (p1.first - p2.first) * (p1.first - p2.first);
  answer += (p1.second - p2.second) * (p1.second - p2.second);
  return sqrt(answer);
}

bool comp(iid d1, iid d2)
{
  return d1.second < d2.second;  
}

int relax(int u)
{
  if(u == cluster[u])
    return u;
  else return cluster[u] = relax(cluster[u]);
}

int main()
{
  int x, y;
  int expectednum;
  cout<<"Input expected number of clusters: ";
  cin>>expectednum;
  ifstream fin;
  ofstream fout;
  fin.open("points.dat");
  int j = 0;
  while(!fin.eof())
  {
    fin>>x>>y;
    points.push_back(ii(x, y));
    cluster.push_back(j);
    newcluster.push_back(-1);
    j++;
  }
  fin.close();
  
  int numclusters = points.size();
  for(int i = 0; i < points.size(); i++)
    for(int j = i+1; j < points.size(); j++)
    {
      ii temp = ii(i, j);
      distances.push_back(iid(temp, eu_distance(points[i], points[j])));
    }
  
  sort(distances.begin(), distances.end(), comp);
  int i = 0;

  while(numclusters > expectednum)
  {
    iid next = distances[i++];
    int u = next.first.first;
    int v = next.first.second;

    cluster[u] = relax(u);
    cluster[v] = relax(v);
    
    if(cluster[u] == cluster[v])
      continue;
    
    cluster[cluster[v]] = cluster[u];
    numclusters--;
  }
  
  for(i = 0; i < points.size(); i++)
    cluster[i] = relax(i);

  int curcluster = 1;
  for(i = 0; i < points.size(); i++)
  {
    if(newcluster[cluster[i]] == -1)
    {
      newcluster[cluster[i]] = curcluster++;
      cluster[i] = newcluster[cluster[i]];
    }
    else cluster[i] = newcluster[cluster[i]];
  }
  
  fin.open("points.dat"); 
  fout.open("clustered_points_slink.dat");
  i = 0;
  int max = cluster[0];

  while(!fin.eof())
  {
    fin>>x>>y;
    fout<<x<<" "<<y<<" "<<cluster[i]<<"\n";
    if(cluster[i] > max)
      max = cluster[i];
    i++;
  }
  numclusters = max;
  fin.close();
  fout.close();

  cout<<"SLINK terminated with "<<numclusters<<" clusters.\n";
  return 0;  
}
