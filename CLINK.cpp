// TODO: Reduce complexity from O(n^3) to atleast O((n^2)logn), if not O(n^2)

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>

#define INF 1000000000

using namespace std;

typedef pair<int, int> ii;

vector<ii> points;
vector<int> cluster;
vector<int> newcluster;

double eu_distance(ii p1, ii p2)
{
  double answer = (p1.first - p2.first) * (p1.first - p2.first);
  answer += (p1.second - p2.second) * (p1.second - p2.second);
  return sqrt(answer);
}

int relax(int u)
{
  if(u == cluster[u])
    return u;
  else return cluster[u] = relax(cluster[u]);
}

ii getAndSetMin(vector< vector<int> > &distances)
{
  int mini = distances[0][0];
  int posi = 0, posj = 0;
  for(int i = 0; i < distances.size(); i++)
    for(int j = 0; j < distances.size(); j++)
    {
      if(mini > distances[i][j])
      {
        mini = distances[i][j];
        posi = i;
        posj = j;
      }
    }
  distances[posi][posj] = distances[posj][posi] = INF;
  return ii(posi, posj);
}

void updateValues(int u, int v, vector< vector<int> > &distances)
{
  for(int i = 0; i < distances.size(); i++)
    distances[u][i] = distances[v][i] = distances[i][u] = distances[i][v]
      = max(distances[u][i], distances[v][i]);
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
  vector< vector<int> > distances(numclusters);
  for(int i = 0; i < points.size(); i++)
    for(int j = 0; j < points.size(); j++)
    {
      if(i != j)
        distances[i].push_back(eu_distance(points[i], points[j]));
      else
        distances[i].push_back(INF);
    }

  while(numclusters > expectednum)
  {
    ii minpos = getAndSetMin(distances);
    int u = minpos.first;
    int v = minpos.second;

    cluster[u] = relax(u);
    cluster[v] = relax(v);
    
    if(cluster[u] == cluster[v])
      continue;
    
    cluster[cluster[v]] = cluster[u];
    updateValues(u, v, distances);
    numclusters--;
  }
  
  int i = 0;
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
  fout.open("clustered_points_clink.dat");
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

  cout<<"CLINK terminated with "<<numclusters<<" clusters.\n";
  return 0;  
}
