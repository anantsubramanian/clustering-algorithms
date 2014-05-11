# Remove previous leftover files
rm points.dat 2>/dev/null
rm clustered_points_dbscan.dat 2>/dev/null
rm clustered_points_kmeans.dat 2>/dev/null
rm sse.dat 2>/dev/null
rm a.out 2>/dev/null

echo "Globular or Rectangular clusters? (g = Globular, r = Rectangular): "
read input
if [[ "$input" == "g" ]]; then
  echo "Globular clusters chosen"
  g++ generatedata.cpp
  ./a.out > points.dat
else
  echo "Rectangular clusters chosen"
  ./generatedata.sh > points.dat
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
