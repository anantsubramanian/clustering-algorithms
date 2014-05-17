// TODO: Optimize O((n^2)logn) algorithm to O(n^2)

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

typedef pair<int, int> ii;

vector<ii> points;
vector<int> cluster;
vector<int> newcluster;
vector< vector<double> > distances;

struct comp
{
  bool operator()(ii a, ii b)
  {
    if(distances[a.first][a.second] < distances[b.first][b.second])
      return true;
    else if(distances[a.first][a.second] > distances[b.first][b.second])
      return false;
    else
    {
      if(a.first < b.first)
        return true;
      else if(a.first > b.first)
        return false;
      else return a.second < b.second;
    }
  }
};

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

ii getAndSetMin(set<ii, comp> &Q)
{
  ii temp = *Q.begin();
  Q.erase(*Q.begin());
  return temp;
}

void updateValues(int u, int v, set<ii, comp> &Q)
{
  for(int i = 0; i < distances.size(); i++)
  {
    if(i < u)
      Q.erase(ii(u,i));
    else if(i > u)
      Q.erase(ii(i, u));
    if(i < v)
      Q.erase(ii(v,i));
    else if(i > v)
      Q.erase(ii(i,v));
    if(i > u)
    {
      if(i > v)
        distances[i][u] = distances[i][v] = max(distances[i][u], distances[i][v]);
      else if(v > i)
        distances[i][u] = distances[v][i] = max(distances[i][u], distances[v][i]);
    }
    else if(u > i)
    {
      if(i > v)
        distances[u][i] = distances[i][v] = max(distances[u][i], distances[i][v]);
      else if(v > i)
        distances[u][i] = distances[v][i] = max(distances[u][i], distances[v][i]);
    }
    if(u > i)
      Q.insert(ii(u, i));
    else if(i > u)
      Q.insert(ii(i, u));
  }
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
  
  set<ii, comp> Q;
  int numclusters = points.size();
  distances = vector< vector<double> >(numclusters);
  for(int i = 1; i < points.size(); i++)
    for(int j = 0; j < i; j++)
    {
        distances[i].push_back(eu_distance(points[i], points[j]));
        Q.insert(ii(i, j));
    }

  while(numclusters > expectednum)
  {
    ii minpos = getAndSetMin(Q);
    int u = minpos.first;
    int v = minpos.second;

    cluster[u] = relax(u);
    cluster[v] = relax(v);
    
    if(cluster[u] == cluster[v])
      continue;
    
    cluster[cluster[v]] = cluster[u];
    updateValues(u, v, Q);
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
