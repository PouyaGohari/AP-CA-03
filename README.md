# MyProject
My project is the third CA(Computer Assignment) of AP(Advance programming) course in [Univesity Of Tehran](https://ece.ut.ac.ir/en) which help us code in top down design and learn CSV files.
## Table of Contents

-[Installation](#installations)
-[Usage](#usage)

## Installations
In order to use my repo make sure you follow this steps:

1. Clone the repo to your local machine:

   git clone https://github.com/PouyaGohari/AP-CA-03.git

## Usage
In order to use this project:

1. Make sure you run the code in Linux terminal:

    g++ -std=c++11 main.cpp -o main.out

2. Use [Command line arguments](https://www.learncpp.com/cpp-tutorial/command-line-arguments/) for getting input and output

    ./main.out tests/1/1.csv outputs/1/1.txt

3. Differentiate the outputs with test's outputs and your outputs:

    diff outputs/1/1.txt tests/1/1.txt > differences/1/diff.txt


