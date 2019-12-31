########################################################
DATA PROCESSING
- data_raw is fed to processData.py to get 3 files: denotedStop.txt, Edges.txt, ListBusinStop.txt.
- Those 3 files will be the program's data.
- Distance between 2 bus stop is randomly generated.

########################################################
STRUCTURE OF DATA FILE
1. denotedBus.txt
- 1st line: number of bus stop.
- line i from line 2nd to line bus-stop-th: name of the bus stop i-th.

2. Edges.txt
- information of bus stop i stored in 3 lines:
	+ first line: number of vertices that connected to bus stop i.
	+ 2nd line: encoded vertices that connected to bus stop i.
	+ 3rd line: distance between bus top i and the corresponding vertices in the 2nd line.

3.ListBusinStop.txt
- information of the list of bus in bus stop i stored in 2 lines:
	+ 1st line: number of bus in bus stop I.
	+ 2nd line: list of bus in bus stop I.