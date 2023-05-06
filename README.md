<h1 align="center">AI - A star fruit sorting</h1>


## Table of Contents

- [Build](#install)
- [Usage](#usage)
- [Implementation](#implementation)
- [Dependencies](#dependencies)

## Build
To build and run application execute the following commands.

```shell
g++ --std=c++11 main.cpp -o a_star.out
```

To run the input case

```shell
./a_star.out < in1.txt 
```


## Usage

## Implementation
I have tried two herusitics.

In the first one, i check i how close the given type of fruit is to its correct position when it is sorted. And i sum those numbers for the given state of the fruit. Let's say I have an apple with size 12 and now it is in the first row. When it is sorted it is position should be the last row. I substract those row numbers and add them up for each fruit type. <br>

Another heuristic i tried is manhattan distance. In this case, let's say largest apple fruit with size 10 is in position (0,0) but when it is sorted, it can be in one of last row and the three columns. So i take all possible combinations and calculate the manhattan distance for each fruit on grid for all 6 possible cases and the take the minimum. This one performed better than the previos on on input cases I ran. But the problem with this heuristic is that it does not work on all possible input cases.



## Dependencies
The project uses the gnu c++ compiler and no external dependencies is used. During testing i have compiled the code
with c++11 standards.