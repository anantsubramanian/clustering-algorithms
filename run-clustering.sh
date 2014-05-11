# Remove previous leftover files
rm points.dat 2>/dev/null
rm clustered_points_dbscan.dat 2>/dev/null
rm clustered_points_slink.dat 2>/dev/null
rm clustered_points_kmeans.dat 2>/dev/null
rm sse.dat 2>/dev/null
rm a.out 2>/dev/null

echo "Globular or Rectangular clusters? (g = Globular, r = Rectangular): "
read input1
echo "Approx. equal size clusters? (y = Yes, n = No): "
read input2
if [[ "$input1" == "g" ]]; then
  echo "Globular clusters chosen"
  g++ generatedata.cpp
  if [[ "$input2" == "y" ]]; then
    echo "Equal size clusters chosen"
    echo "y" | ./a.out > points.dat
  else
    echo "Cluster sizes not necessarily equal"
    echo "n" | ./a.out > points.dat
  fi
else
  echo "Rectangular clusters chosen"
  if [[ "$input2" == "y" ]]; then
    echo "Equal size clusters chosen"
    echo "y" | ./generatedata.sh > points.dat
  else
    echo "Cluster sizes not necessarily equal"
    echo "n" | ./generatedata.sh > points.dat
  fi
fi

echo ""
echo "Running DBSCAN:-"
rm a.out 2>/dev/null
echo "plot \"points.dat\" title 'Unclustered Data' with points" | gnuplot --persist
g++ DBSCAN.cpp
./a.out
echo ""
echo "set palette model HSV defined (0 0 1 1, 1 1 1 1)" > plotcommand
echo "plot \"clustered_points_dbscan.dat\" u 1:2:3 title 'DBSCAN Clustering' with points palette" >> plotcommand
gnuplot < plotcommand --persist

# Remove DBSCAN data
rm plotcommand 2>/dev/null
rm a.out 2>/dev/null

echo "Running K-Means:-"
g++ KMeans.cpp
./a.out
echo""
echo "set palette model HSV defined (0 0 1 1, 1 1 1 1)" > plotcommand
echo "plot \"clustered_points_kmeans.dat\" u 1:2:3 title 'K-Means Clustering' with points palette" >> plotcommand
gnuplot < plotcommand --persist

# Remove K-Means data
rm plotcommand 2>/dev/null
rm a.out 2>/dev/null

# Plot SSE from K-Means
echo "plot \"sse.dat\" title 'SSE vs K for K-Means' with points" >> plotcommand
gnuplot < plotcommand --persist
rm plotcommand 2>/dev/null

# Run SLINK agglomerative clustering
echo ""
echo "Running SLINK:-"
g++ SLINK.cpp
./a.out
echo "set palette model HSV defined (0 0 1 1, 1 1 1 1)" > plotcommand
echo "plot \"clustered_points_slink.dat\" u 1:2:3 title 'SLINK Clustering' with points palette" >> plotcommand
gnuplot < plotcommand --persist
rm a.out 2>/dev/null
rm plotcommand 2>/dev/null
