# Making a Parameter File #
Rather than rewrite code every time a new or different motor is to be used, `EposManager` loads motor parameters from .YAML files. This allows the user to change the motor or EPOS parameters without ever having to recompile any code. Furthermore, based off of which parameters are given to `EposManager`, it can determine whether or not it needs to loading missing information from the EPOS. The user only needs to input the parameters they want to change.

To create a parameter file for your motor, make a new directory called "params" and copy the sample file below into `MyEpos.yaml`.

## Sample Parameter File ##

```
# IDENTITY PARAMETERS
Node_ID: 1
Motor_Name: Test_Motor

# MOTOR PARAMETERS
Motor_Type: 10 # DC (1), EC_SINUS (10), EC_BLOCK (11)
Motor_Nominal_Current: 9650        # MILLIAMPS
Motor_Max_Current: 23600           # MILLIAMPS
Motor_Thermal_Time_Constant: 21.1
Motor_Number_Pole_Pairs: 2         # EC_MAX (1), EC_4POLE (2)

# SENSOR PARAMETERS
Sensor_Type: 2                     # NONE (0), INC_3CH (1), INC_2CH (2)
                                   # HALL (3), SSI_BINARY (4), SSI_GRAY (5)
Sensor_Inversion:   0              # Encoder Opposite Hall (1), Same (0)
Encoder_Resolution: 500            # INC ENCODER CPR
Encoder_Data_Rate: 0               # ABS ENCODER SSI DATA RATE
Encoder_Number_Multiturn_Bits: 0   # ABS ENCODER SSI MULTI TURN BITS
Encoder_Number_Single_Turn_Bits: 0 # ABS ENCODER SSI SINGLE TURN BITS

# SAFETY PARAMETERS
Max_Following_Error: 20000   # MAX POS FOLLOWING ERROR, ENC COUNTS
Max_Profile_Velocity: 12500  # MAX PROFILE VELOCITY
Max_Acceleration: 15000      # MAX ACCELERATION

# PID PARAMETERS
PositionPGain : 0  # P GAIN FOR POSITION CONTROL
PositionIGain : 0  # I GAIN FOR POSITION CONTROL
PositionDGain : 0  # D GAIN FOR POSITION CONTROL
VelocityPGain : 0  # P GAIN FOR VELOCITY CONTROL
VelocityIGain : 0  # I GAIN FOR VELOCITY CONTROL
CurrentPGain  : 0  # P GAIN FOR CURRENT CONTROL
CurrentIGain  : 0  # I GAIN FOR CURRENT CONTROL

# MODE SEPECIFIC PARAMETERS
Position_Profile_Velocity: 12500     # VELOCITY FOR POS PROFILE MODE
Position_Profile_Acceleration: 8000  # ACCELERATION FOR POS PROFILE MODE
Position_Profile_Deceleration: 14000 # DECELERATION FOR POS PROFILE MODE
Velocity_Profile_Acceleration: 8000  # ACCELERATION FOR VEL PROFILE MODE
Velocity_Profile_Deceleration: 9000  # DECELERATION FOR VEL PROFILE MODE

```

It is unlikely that you will use the same parameter values for your specific motor, but you will use the same parameters names. The only parameter required for `EposManager` to run is Node\_ID, but it is best to make sure that the motor, safety, sensor, and PID parameters for your motor are properly configured before you run it.

If you do not know any of the specific parameters, you can either consult its datasheet for that specific motor, or leave it blank. If it is left blank, it
will use the corresponding parameter that is already on the EPOS. A warning though, this may result in a mismatch if parameters are incompatible with each other, so it is best to load as many parameters as possible each time. The only exception to this is PID parameters, which we will discuss later.

## Parameter Descriptions ##

### Identity Parameters ###
Identity parameters are used to identify what motor is what. The `EposManager` itself does this through `Node_ID`, which is why it is a required input. The `Motor_Name` simply makes it easier for the user to identify what motor is what.

**`Node_ID:`**

**`Motor_Name:`**

### Motor Parameters ###

**`Motor_Type:`**

**`Motor_Nominal_Current:`**

**`Motor_Max_Current:`**

**`Motor_Thermal_Time_Constant:`**

**`Motor_Number_Pole_Pairs:`**

### Sensor Parameters ###

**`Sensor_Type:`**

**`Sensor_Inversion:`**

**`Encoder_Resolution:`**

**`Encoder_Data_Rate:`**

**`Encoder_Number_Multiturn_Bits:`**

**`Encoder_Number_Single_Turn_Bits:`**

### Safety Parameters ###

**`Max_Following_Error:`**

**`Max_Profile_Velocity:`**

**`Max_Acceleration:`**

### PID Parameters ###

**`PositionPGain:`**

**`PositionIGain:`**

**`PositionDGain:`**

**`VelocityPGain:`**

**`VelocityIGain:`**

**`CurrentPGain:`**

**`CurrentIGain:`**

### Mode Specific Parameters ###

**`Position_Profile_Velocity:`**

**`Position_Profile_Acceleration:`**

**`Position_Profile_Deceleration:`**

**`Velocity_Profile_Acceleration:`**

**`Velocity_Profile_Deceleration:`**

## Configuring the Parameter File for Your Motor ##
Now that you know what each parameter is, you need to modify it to use your motor's parameters.