Step 1: Define every potential stop.

In a city, this would be the center of each block. Are you happy to have users cross roads? If not, two stops in the center of each block, one on each side of the road. In rural areas, maybe every 250 meters along each road. Suburbs are more complex. Possibly identify main roads and place potential stops along those, every 250 meters.

Step 2:

Calculate the need at every potential stop.

Step 3:

Use K-Means to identify the N stops that will become actual stops ( see example implementation below ). N is an input. You might have to iterate over N values, calculating the worst served need distance to the nearest actual stop, until you find the N that gives an acceptable worst served distance.

Step 4:

Construct a graph of vertices ( actual stops ) and edges ( roads )

Step 5:

Use the travelling salesman ( Euclidean ) algorithm to calculate the bus route.

![screenshot](https://github.com/JamesBremner/bussttop/assets/2046227/6cc7f8d8-0b72-4a4d-9bbb-1a2bd4689799)
