# How To

## Introduction
This document is the How To for the IRSYS irrigation system.
It describes the user experience for this product.

## Tenets
- The user experience should be very simple.
Even non-technical people should be able to understand the steps to set up the system, and the data of the interfaces
- End-users should be able to diagnose and recover from failures easily, thus reducing the need to send back the item or send it for repair.
- Hardware should be safe and durable. We're working in outside conditions with water and electricity. Corosion and short circuiting should be prevented with high priority.

## Hardware
The user receives a package containing 2 electronic devices and some materials to connect it to the water supply and electricy outlets.

The devices are the controller and a moisture sensor.
The ports / cables on these devices are as follows:

#### Controller
- Power cable (outgoing): needed to start up the device and keep it running.
<!-- - Network port and small network cable: needed to setup the initial configuration, like configuring the Wifi. -->
- Water lines in/out. There will be different versions of the controller, with different amounts of in/out lines. It should be easy to connect by using "quick connect" water hose connectors. Provided are also a small hose and some quick connectors (female / male).

#### Moisture sensor box
- Power cable (outgoing): needed to start up the device and keep it running.
- The end of the moisture sensor: this needs to be sticked in the soil.


Both these devices need to be linked up to main power (240V in EU) in order to turn them on.
Once this is done, they should start by itself, only needing some configuration.

## User experience
Once the controller is plugged into the main power, it will boot up.
It will automatically spin up a wifi access point, which a laptop or phone can connect to. Once connected, the user should be able to go to a web page where it can configure the wifi of the controller so it has access to the internet.

Once the wifi is set up, more parts of the configuration interface becomes available: it can show a dashboard with the measured moisture values (once available), and it can connect new moisture sensors.

#### Set up new moisture sensor
Sticking a moisture sensor in the soil and connecting it to power is not enough to let the system know who owns the sensor and it's data.

Through the admin interface of the controller, we can trigger a search for new sensors. If it has found sensors in the area, we can associate the sensor to the controller.

Now that this is done, the moisture sensor can submit it's sensor values to the controller, which stores it in the cloud.

> To reset the wifi, simply press the reset button and follow the steps for adding a new sensor through the admin interface again.

#### Viewing the data
The moisture values can be viewed directly through the interface.
Go to the dashboard, there you will see the values in a graph. The last 24 hours are visible, with a datapoint for each sensor every 15 minutes showing the average value for that interval.

#### Turning water valves on
The controller can open the valves and also decides itself when to open them.
When the moisture sensor is below the threshold, it will automatically open the valve and giving water to the plants.

> Make sure the sensor is placed nearby plants that gets water, in order to accurately determine if the plants need more water

The threshold per zone can be configured in the admin panel.
