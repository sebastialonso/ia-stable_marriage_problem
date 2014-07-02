#Compile
~~~
g++ -std=c11++ gale_shapley.cpp -o gs
~~~
#Run
##Run single instance
~~~
./gs INPUT_FILE
~~~

where ```INPUT_FILE``` is one of the Instances files.

##Run several instances
~~~
./smp_runner.sh
~~~

to run instances of size 5, 10, 20, 40, 80, 160, 320, 640
