# Controlling the Motor From the Command Line #

If you have completed the previous tutorials, you should have successfully launch the `EposManager` and initialized an EPOS with your specific parameters. Now, you are ready to receive information from and control the EPOS.

## Receiving Motor Information ##
The easiest way to check to make sure `EposManager` is successfully running is to listen to the motor information it returns. Open up a terminal and run:
```
rostopic list
```

This should show a list of all available topics and, assuming you ran the launch file in the previous tutorial, should look something like this:
```
...
...
...
...
```
Note the two motor information topics available: "`Motor_Info`" and "`Group_Motor_Info`". Both of these publish the same information, just in a different way. The "`Motor_Info`" topic will publish each `MotorInfo` message separately, while "`Group_Motor_Info`" adds each `MotorInfo` message to a variable length array and publishes them together. The "`Group_Motor_Info`" is most useful with Odometry, when a subscribing callback needs to know each motors position or velocity at a single call rather than in multiple ones.

Because we are only dealing with a single motor though, they are functionally the same topic, and we will listen to the "`Motor_Info`" topic. To do this, run the following command in a terminal:
```
rostopic echo /motors/Motor_Info
```

If you are getting an output, then the EPOS is successfully communicating with the computer and returning information to it. Try moving the motor and seeing if any of the values change.

## Driving the Motor ##
Now that you are able to read information from the EPOS, it is time to send it some commands. Recall from the `rostopic list` command that there were some motor control topics: "`Motor_Control`" and "`Group_Motor_Control`". Just like with the motor information topics, these are for sending singular and batch commands to motors. "`Motor_Control`" is best used when dealing with single motor control, while "`Group_Motor_Control`" should be used if running multiple motors in the same chain. Again, because one motor is being used, we will focus on the "`Motor_Control`" topic.

To drive the motor, we are going to use rostopic pub. The syntax for sending a single motor command is as follows:
```
rostopic pub -1 /topic_name /message_type [node_id] [control_mode] [setpoint]
```

This command will publish 1 message of `[message_type]` to `[topic_name]` telling `[node_id]` to move to either move with `[setpoint]` velocity or to `[setpoint]` position depending upon `[control_mode]`. The valid control modes and there corresponding value are:

  1. **`Velocity:`** Sets the motor to `[setpoint]` velocity in rpm at the motor.
  1. **`Absolute Position:`** Tells the motor to go to `[setpoint]` encoder counts after completing any previous position command.
  1. **`Absolute Position Immediate:`** Tells the motor to go to `[setpoint]` encoder counts immediately.
  1. **`Relative Position:`** Tells the motor to move `[setpoint]` encoder counts, relative to its current position, after completing any previous position command.
  1. **`Relative Position Immediate:`** Tells the motor to move `[setpoint]` encoder counts, relative to its current position, immediately.

Suppose we want to run your motor at 4000 rpm. Open up a terminal and run:
```
rostopic pub -1 /motors/Motor_Control EposManager/EPOSControl 1 1 4000
```

If everything worked correctly, your motor should be moving at 4000 rpm. Note, that this is the motor velocity, not the output velocity. `EposManager` does not take into account the gearing of the motor.