g++ -std=c++11 gale_shapley.cpp -o gs
for PEOPLE in 3 4 5 6 7 8 9 10 11 12
do
  ARG="InstanceSmti-"$PEOPLE"-0-0.txt"
  echo "${ARG}"
  ./gs ${ARG}
done
