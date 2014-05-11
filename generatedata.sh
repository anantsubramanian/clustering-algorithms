read equal
if [[ "$equal" == "y" ]]; then
  delta=$(($RANDOM%750))
fi
for((i = 0; i < 40; i++)); do
  lowerx=$(($RANDOM%9250))
  upperx=$(($lowerx + $RANDOM%750))
  if [[ "$equal" == "y" ]]; then
    deltax=$delta
  else
    deltax=$(($upperx - $lowerx + 1))
  fi
  lowery=$(($RANDOM%9250))
  uppery=$(($lowery + $RANDOM%750))
  if [[ "$equal" == "y" ]]; then
    deltay=$delta
  else
    deltay=$(($uppery - $lowery + 1))
  fi
  for((j = 0; j < 400; j++)); do
    echo "$(($lowerx + $RANDOM%deltax)) $(($lowery + $RANDOM%deltay))"
  done
done
