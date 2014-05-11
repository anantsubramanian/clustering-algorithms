for((i = 0; i < 40; i++)); do
  lowerx=$(($RANDOM%9250))
  upperx=$(($lowerx + $RANDOM%750))
  deltax=$(($upperx - $lowerx + 1))
  lowery=$(($RANDOM%9250))
  uppery=$(($lowery + $RANDOM%750))
  deltay=$(($uppery - $lowery + 1))
  for((j = 0; j < 400; j++)); do
    echo "$(($lowerx + $RANDOM%deltax)) $(($lowery + $RANDOM%deltay))"
  done
done
