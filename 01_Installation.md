# Installing EposManager #

`EposManager` is still actively being developed. While most of the changes are likely to be minor, we will add new features if they are requested. For this reason, `EposManager` must be installed from source. It is also important to note that **EposManager will only work on a 32 bit OS**. I understand that this may be a huge inconvenience for you, but there is nothing that we can do about it. Unfortunately, Maxon's driver only works on 32 bit machines. We are talking with Maxon to see if they can write a 64 bit version of their Linux driver. We will let everyone know and update the code until we hear about a fix.
## Checking out the Source Code ##

You can checkout `EposManager` into a directory in your ROS\_PACKAGE\_PATH through the following command:

`svn checkout http://wpi-rover.googlecode.com/svn/trunk/EposManager`

## Linking the required libraries ##

`EposManager` requires two external libraries in order to function: one for the FTDI chip in the EPOS, and the other for the Maxon's EPOS Library. Both of these can be found in the `EposManager` package under the "libs" directory. Below are the steps required to successfully install and link the required libraries.

### Linking the FTDI library ###
The EPOS uses an FTDI chip to communicate between the computer and the micro-controller. Unfortunately, the FTDI library is an older non-standard version library. Fortunately, we have not found any conflicts that occur from the installation of the FTDI driver. To install the FTDI drivers:

  1. `roscd EposManager/libs`
  1. `sudo cp libftd2xx.so.0.4.16 /usr/local/lib`
  1. `sudo ln -s /usr/local/lib/libftd2xx.so.0.4.16 /usr/local/lib/libftd2xx.so.0`
  1. `sudo ln -s /usr/local/lib/libftd2xx.so.0.4.16 /usr/lib/libftd2xx.so.0`
  1. `sudo cp 99-ftdi.rules /etc/udev/rules.d`
  1. `sudo /etc/init.d/udev restart`

### Linking the EPOS Library ###
The EPOS library is a modified version of Maxon's own libEposCmd. It is almost identical to libEposCmd. The only difference is the addition of a loop to resend a command if the first few tries fail. This is because we found that, on newer computer systems, it often takes multiple attempts to successfully send a command. While the user can implement this resending on a higher level (as we also do), relying on only this leads to a command delay of up to 500 milliseconds. Including a low level loop reduces this delay to less than a millisecond in most cases. If you do wish to use the unmodified EPOS library, it can be found HERE. The full documentation of the EPOS Command Library that we use can also be found HERE.

To install the EPOS Command Library:

  1. `roscd EposManager/libs`
  1. `sudo cp libEPOS.so /opt/lib`
  1. `sudo cp libEPOS.so /usr/lib`

### Building EposManager ###
Once the required libraries are installed, it can now be built. Simply open up a terminal and type:

`rosmake EposManager`

## Making Your Own Package ##
Navigate to your personal ros workspace and run the following:

`roscreate-pkg epos_tutorials roscpp EposManager`

This will be the package where you will place all of your launch and parameter files.

Now you should be ready to move on to our tutorials.