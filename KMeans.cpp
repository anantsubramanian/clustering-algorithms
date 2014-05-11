#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

typedef pair<int, int> ii;
typedef pair<double, double> dd;

vector<ii> points;
vector<int> cluster;
vector<int> bestcluster;
vector<dd> clusters;
vector<int> clustercount;

int minpts;
int k, klow, khigh;

double eu_distance(ii p1, ii p2)
{
  double answer = (p1.first - p2.first) * (p1.first - p2.first);
  answer += (p1.second - p2.second) * (p1.second - p2.second);
  return sqrt(answer);
}

double eu_distance(ii p1, dd p2)
{
  double answer = (p1.first - p2.first) * (p1.first - p2.first);
  answer += (p1.second - p2.second) * (p1.second - p2.second);
  return sqrt(answer);
}

double eu_distance(dd p1, ii p2)
{
  double answer = (p1.first - p2.first) * (p1.first - p2.first);
  answer += (p1.second - p2.second) * (p1.second - p2.second);
  return sqrt(answer);
}
 
void reassign_clusters()
{
  for(int i = 0; i < clusters.size(); i++)
    clustercount[i] = 0;
  for(int i = 0; i < points.size(); i++)
  {
    double min = eu_distance(clusters[1], points[i]);  
    int pos = 1;
    for(int j = 2; j <= k; j++)
    {
      if(eu_distance(points[i], clusters[j]) < min)
      {
        min = eu_distance(points[i], clusters[j]);
        pos = j;
      }
    }
    cluster[i] = pos;
    clustercount[pos]++;
  }
}

void update_clusters()
{
  for(int i = 0; i <= k; i++)
    clusters[i].first = clusters[i].second = 0;
  for(int i = 0; i < points.size(); i++)
  {
    clusters[cluster[i]].first += points[i].first;
    clusters[cluster[i]].second += points[i].second;
  }
  int erasedcount = 0;
  for(int i = 1; i < clusters.size(); i++)
  {
    if(clustercount[i] == 0)
    {
      clusters.erase(clusters.begin()+i);
      clustercount.erase(clustercount.begin()+i);
      erasedcount++;
      if(i == clusters.size())
        break;
    }
    clusters[i].first /= (double) clustercount[i];
    clusters[i].second /= (double) clustercount[i];
  }
  for(int i = 0; i < erasedcount; i++)
  {
    clusters.push_back(points[rand()%points.size()]);
    clustercount.push_back(0);
  }
}

int calculate_change(vector<int> &oldcounts, vector<int> &newcounts)
{
  int change = 0;
  for(int i = 1; i < oldcounts.size(); i++)
    change += abs(oldcounts[i] - newcounts[i]);
  return change;
}

double compute_sse()
{
  double answer = 0.0;
  for(int i = 0; i < points.size(); i++)
    answer += eu_distance(points[i], clusters[cluster[i]]);
  return answer;
}

void initialize_clusters()
{
  clusters = vector<dd>();
  clustercount = vector<int>();
  clusters.push_back(dd(0.0,0.0));
  clustercount.push_back(0);
  ii newcluster = points[rand()%points.size()];
  clusters.push_back(dd((double) newcluster.first, (double) newcluster.second));
  clustercount.push_back(0);
  for(int i = 1; i < k; i++)
  {
    double maxd = 0;
    int chosenpoint = 0;
    for(int j = 0; j < points.size(); j++)
    {
      double d = 0.0;
      for(int l = 1; l < clusters.size(); l++)
        d += eu_distance(clusters[l], points[j]);
      d /= (double) clusters.size();
      if(d > maxd)
      {
        maxd = d;
        chosenpoint = j;
      }
    }
    newcluster = points[chosenpoint];
    clusters.push_back(dd((double) newcluster.first, (double) newcluster.second));
    clustercount.push_back(0);
  }
}

int main()
{
  int x, y;
  srand(time(NULL));
  cout<<"Input lowerlimit for K: ";
  cin>>klow;
  cout<<"Input upper limit for K: ";
  cin>>khigh;
  ifstream fin;
  ofstream fout;
  ofstream sseout;
  fin.open("points.dat");
  while(!fin.eof())
  {
    fin>>x>>y;
    points.push_back(ii(x, y));
    cluster.push_back(0);
  }
  fin.close();
  
  sseout.open("sse.dat");

  double minsse;
  int iterationscount = 0;
  int numclusters = 0;
  bool changed;

  for(k = max(klow, 1); k <= khigh; k++)
  {
    changed = false;
    initialize_clusters();

    int change;
    int iterations = 0;
    
    do
    {
      vector<int> oldcounts = clustercount;
      reassign_clusters();
      update_clusters();
      change = calculate_change(oldcounts, clustercount);
      iterations++;
    } while(change > 0);
    
    double sse = compute_sse();
    sseout<<k<<" "<<sse<<"\n";
    
    if(k == klow)
    {
      minsse = sse;
      iterationscount = iterations;
      changed = true;
    }
    else if(minsse > sse)
    {
      minsse = sse;
      iterationscount = iterations;
      changed = true;
    }

    if(changed)
      bestcluster = vector<int>(cluster);
  }
  sseout.close();

  fin.open("points.dat"); 
  fout.open("clustered_points_kmeans.dat");
  int i = 0;
  int max = bestcluster[0];

  while(!fin.eof())
  {
    fin>>x>>y;
    fout<<x<<" "<<y<<" "<<bestcluster[i]<<"\n";
    if(bestcluster[i] > max)
      max = bestcluster[i];
    i++;
  }

  numclusters = max;
  fin.close();
  fout.close();

  cout<<"K-Means found "<<numclusters<<" clusters and terminated with "<<iterationscount<<" iterations for the same.\n";
  return 0;  
}
