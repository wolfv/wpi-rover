# Writing a Launch File #
While it is possible to run the `EposManager` from the command line through rosrun, the easiest way by far is from a launch file. Here, you will learn how to write a launch file to control any number of EPOS in a chain.

Assuming you have already followed the previous tutorials and created your parameter files, create a new directory called "launch". In the directory, create a new launch file named "`MyMotorTest.launch`" and copy the sample launch file below into it.

## Sample Launch File ##
```
<launch>
	<rosparam command="load" file="$(find epos_tutorials)/params/MyEpos.yaml" ns="motors/MyEpos" />

	<node name="NewEpos" pkg="EposManager" type="EposManager"  ns="motors" output="screen" args="MyEpos" >
		<param name="Protocol" value="USB" />
		<param name="Port" value="USB0" />
		<param name="Publish_Rate" value="10" />
	</node>
</launch>
```

## The Launch File Explained ##
While this launch file may look complicated, it is actually very simple and straightforward. Below is a breakdown of the code.

### Loading EPOS Parameters ###
```
<rosparam command="load" file="$(find epos_tutorials)/params/MyEpos.yaml" ns="motors/MyEpos" /> 
```

This line loads the parameters from the given parameter file into a specific namespace. The

```
file="$(find epos_tutorials)/params/MyEpos.yaml"
```

portion of the line uses the ROS file-system to find the absolute path of the given package, in this case "`epos_tutorials`". From there, you specify the relative path to the parameter file you are loading into the namespace.

The namespace is specified by:
```
ns="motors/MyEpos
```
The "motors" before the "`MyEpos`" is solely for organizational purposes, it can be removed or expanded. The "`MyEpos`" (or whatever name you wish to belong to that specific EPOS) is mandatory and must be unique. You cannot have two EPOS's in the same namespace or their parameters will overwrite each other.

### EposManager Node ###
```
<node name="NewEpos" pkg="EposManager" type="EposManager" ns="motors" output="screen" args="MyEpos"  >
```

This next portion runs an instance of the `EposManager`. Like every ROS node, the name must be unique. In this case, we are only running one `EposManager`, so that is not an issue. We also give the `EposManager` a namespace through "ns". Just like the parameter loading above, this namespace can be lengthened or removed completely. The only requirement is that it be the exact same as the namespace used in the parameter loading portion of the file, minus the name of the specific EPOS (in this case `MyEpos`). The name of the specific EPOS needs to be given in the "args=" argument. This is done so `EposManager` knows where to search for the parameters for the EPOS it is using. While we are only using one EPOS in this example, you can pass in as many EPOS as you wish, so long as each one has an associated parameter file. We will discuss using multiple EPOS in a later tutorial.

### EposManager Parameters ###
```
<param name="Protocol" value="USB" />
<param name="Port" value="USB0" />
<param name="Publish_Rate" value="10" />
```

While each EPOS has its parameters, the `EposManager` has parameters of its own. Shown above are the default values for each parameter; if you remove them it will work identically. It is still best that you specify them every time to make it easy to modify them in the future. Below, we will discuss what each parameter means.

**`Protocol`:** This is the communications protocol used by the EPOS. The possible values are: USB , RS232, and CAN. All of these are case specific.

**`Port`:** This is the specific port that the EPOS is plugged in on. For USB, is is USB0, USB1, etc. ; for RS232 is it COM1, COM2, etc. ; and for CAN it is the name of the CAN method you are using.

**`Publish_Rate`:** The rate in Hz that the `EposManager` will publish motor information. This information includes, among other things, position, velocity, current, and motor state. While this value can be set as high as the user wishes, in reality, a single EPOS can only communicate at about 50 Hz. As more are added to a chain, this decreases linearly (2 EPOS controllers in a chain have a limit of 25 Hz, 3 a limit of around 17 Hz, etc).

## Running the Launch File ##
Once you have written the launch file and modified the `EposManager` Parameters to suit your needs, you can now launch it. Open up a terminal and type:
```
roslaunch epos_tutorials MyMotorTest.launch
```

Assuming everything went well, you should see the following output:
```
...
...
...
```