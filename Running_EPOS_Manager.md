# Making an EposManager compatible package #
Once you have successfully built EposManager, it is time to start using it with your own packages. Because EposManager is able to accept a user-configurable number of EPOS controller, a little bit more preparation is required. Here, you will learn how to create a package that uses EposManager to run motors with parameters that you specify.

## Making a New Package ##
To begin create a package called epos\_tutorials that depends upon EposManager and roscpp:

```
roscreate-pkg epos_tutorials EposManager roscpp
```





