# **CP386 - Assignment 4**

The official repository to our CP386 (Operating Systems) Assignment 4. The following README file gives an overview about this Assignment. This program implements the Banker's Algorithm, which is used for *Deadlock Avoidance* and *Resource Allocation*.



 

## License

This project is licensed under MIT License. Please review the *LICENSE.md* file for more details.



 

## Authors

- Kaish Panjwani
- Ravish Virani



 

## Clone

- Clone this repository to your local machine using the following command:

  `git clone https://github.com/kaishp/CP386_A04.git`



 

## Installation

1. Open Terminal of your Linux Machine and change directory to the one where the files are cloned

2. Run the `make` command to compile the file(s)

3. Run the compiled program using the following command:

   `$ ./a.out 10 5 7 8`  here the numbers can be replaced by Command Line Arguments



 

## Screenshots

A sample execution of the program:

![Screenshot](https://imgur.com/JtucW6G)

 

## Commit History

#### Initial Commit:

1. Repository Created and License Added

 

#### Second Commit:

1. Data Structures for Available and Maximum created
2. Command Line Input is now accepted and is stored in Available array
3. Number of Customers is now read and stored
4. Program now reads the file and stores the data into Maximum 2D array
5. Currently Available array is printed as per requirements
6. Maximum 2D array is printed as per requirements
7. Sample input file added

  

#### Third Commit:

1. *README.md* file updated

  

#### Fourth Commit:

1. <u>Command</u>: `RQ` is now supported
2. <u>Command</u>: `RL` is now supported
3. <u>Command</u>: `exit` is now supported

  

#### Fifth Commit:

1. <u>Command</u>: `*` is now supported
2. <u>Function</u>: calcNeeded() is added

  

#### Sixth Commit:

1. <u>Function</u>: *threadRun() is added
2. <u>Command</u>: `run` is now supported
3. *README.md* file updated



#### Seventh Commit:

1. Compilation Error fixed
2. Minor Bugs removed



#### Eighth Commit:

1. <u>Function</u>: bankerAlgorithm() is added
2. <u>Function</u>: getCustCount() is added
3. <u>Function</u>: reCalcCurrent() is added
4. Minor Bugs were fixed
5. Comments added



#### Ninth Commit:

1. Available, Needed, Allocated arrays are now Globally Declared and Dynamically Allocated
2. <u>Function</u>: threadRun() now works correctly


#### Tenth Commit:
1. *README.md* file updated
