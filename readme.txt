/************************************************************************************
  SIMULATOR	:	DISCRETE EVENT SIMULATOR for Distributed Sorting on a line network
  PROBLEMS	: 	SASAKI AND MODULUS 3 ALGORITHMS
  CODE WRITTEN BY : SHOBHIT MALARYA
  					UG-3 CSE
*************************************************************************************/




HOW TO RUN 
-------------------------------------------------------------------------------------------------------

ON LINUX
_______________________________________________________________________________________________________

Run ./sasaki (and) ./mod3 for respective algorithms

ON WINDOWS
_______________________________________________________________________________________________________

Run sasaki.exe (and) mod3.exe for respective algorithms


SASAKI ALGO SIMULATOR - compile and run " sasaki.c " 

MODULUS 3 ALGO SIMULATOR - compile and run " mod3.c "


INPUT
--------------------------------------------------------------------------------------------------------

By default the code runs on 5 nodes

>> ./a.out                                   ----------- THIS WILL RUN FOR 5 NODES by default--This can be changed by changing N_DEFAULT

Both the codes can take number of nodes as the input from command line argument

>> ./a.out NUMBER_OF_NODES

ex:

>> ./a.out 10                                ---------- THIS WILL RUN FOR 10 NODES


OUTPUT
---------------------------------------------------------------------------------------------------------

By default the code shows initial values of nodes , the nodes after running the algo and elements after flag selection in case of sasaki.


Uncomment all the printf() statements in the code to get the detailed printing of whats happening in the code (or) running of algo


For sasaki

	 PRINTING CONVENTION
         -| Left Value | Right Value |( area )|-

For Mod3
 	 
 	 PRINTING CONVENTION
         -| id | value | (flag) |-



VALUES
----------------------------------------------------------------------------------------------------------

The codes fills the nodes with random number between 1 and n*n (n=number of nodes)

___________________________________________________________________________________________________________

The values for eventType is 1 for a SEND event and 0 for a RECIEVE event 