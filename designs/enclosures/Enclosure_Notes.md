OpenWing Enclosures
===================

![Enclosure Parts](http://i.imgur.com/bCEd3l.jpg)

Enclosures design files are named based on the features they can accomodate.  
  
  The OpenWing V0.1 device provides three hardware options for control/connectivity. Every configuration provides a USB connection used for programming the hardware and sending/receiving control data. There is also a second serial connection used only for sending/receiving control data, which may be connected to a computer via XBEE radios. Both serial connections interface with the OpenWing python library. The third connectivity option is an ethernet connection used for sending control signals via OSC. The enclosure design files are designed for four different configurations of these connection options:

  + "Full" version, with all possible connections (USB, Wireless, and ethernet), with a LiPo battery and charging hardware.
  + A version supporting USB and Wireless serial connections, with a LiPo battery and charging hardware.
  + A version supporting USB serial and ethernet (OSC) connections, with a LiPo battery and charging hardware.
  + A basic "USB Only" version, with no battery or charger.

