# Prezans - A call list assistant
![Render](https://github.com/MarceloFariaz/Prezans/blob/master/Mechanics/ProjectRender.png)
<p align="center"> A render of the 3D printed case </p>

# The Idea
Assist the teacher in filling out the presence list worksheet and saving some eucalyptus trees.

# The How To
<ol>
  <li>A portable device capable of reading the student's ID card and sending the ID, date and time to the internet.</li>
  <li>A script able to read the data sent by the device and turn them into presence or absence on the teacher's attendance worksheet.</li>
</ol>

# The Device
An RFID reader, an SD card adapter, an RTC module, an Oled display and an ESP8266 NodeMCU as the main controller, all powered by a small battery, was enough to create a functional prototype capable of making the idea real.

![Board](https://github.com/MarceloFariaz/Prezans/blob/master/Electronics/PrezansBoardDiagram.png)
<p align="center";>The eletronic board diagram</p>

# The Script
The data read by the devices is then published in a google worksheet in the teacher's drive.

![Registers](https://github.com/MarceloFariaz/Prezans/blob/master/Software/Call_List_Register_Image.PNG)

This data is then processed by a google script running behind the google sheets, which associates the ID with each of the students marking their presence or not.

![Script](https://github.com/MarceloFariaz/Prezans/blob/master/Software/Call_List_Image.PNG)

# The First Real Prototype
![Prototype](https://github.com/MarceloFariaz/Prezans/blob/master/Media/First_Real_Prototype.png)
<p align="center">The first real prototype</p>
