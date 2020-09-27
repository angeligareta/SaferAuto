<p align="center">
  <img height="150" width="150" src="https://saferautohome.files.wordpress.com/2019/03/logo-saferauto.jpg?w=1400&h=9999" alt="Safer Auto">  
</p>
<h1 align="center">Safer Auto</h1>
<h4 align="center">Real-time detection system to assist during driving. </h4>

<p align="center">
  <a href="https://github.com/angeligareta/SaferAuto/blob/master/LICENSE">
    <img alt="GitHub" src="https://img.shields.io/github/license/angeligareta/SaferAuto.svg?style=for-the-badge">
  </a>
  <a href="https://saferauto.home.blog/">
    <img alt="Website" src="https://img.shields.io/website/https/saferauto.home.blog.svg?style=for-the-badge">
  </a>
  <a href="https://twitter.com/SaferAuto">
    <img alt="Twitter Follow" src="https://img.shields.io/twitter/follow/SaferAuto.svg?style=for-the-badge">
  </a>
  <a href="https://github.com/ellerbrock/open-source-badges/">
    <img alt="Website" src="https://badges.frapsoft.com/os/v1/open-source-175x29.png?v=103">
  </a>
</p>


<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#author">Author</a> •
  <a href="#license">License</a>
</p>

Detect, locate and track road elements from a video stream using **[YOLOv3](https://pjreddie.com/darknet/yolo/) real-time object detection system**. Among the elements to classify, the model will be trained to detect traffic lights, vehicles and some types of road signs.

## Key Features
* Detection of speed limit road signs and speed identification. ✔️
* Detection of traffic lights and classification depending on its state (green, amber, red). ✔️
* Detect the previous elements in real-time. ✔️
* Track the previous elements in real-time. ✔️
* UI that displays the current detection and tracking of the road elements, showing bounding boxes with the different classes. ✔️
* Detection of cars and classification of the type. *[work in progress]*
* Implementation of a chatbot that notifies the driver of possible road threats and is able to repeat last notifications. *[work in progress]*

## How to use
<!--- Command line disabled for now.
There are two options of trying the traffic sign detector:
### Command-line program
The first one is a command-line program that receives a video stream and plays the video while it indicates the bounding boxes of the different classes that the YOLO model detects.
For running this program script you need to have [python3 installed](https://www.python.org/downloads/). Besides, you need to download the [weights file](https://drive.google.com/uc?export=download&confirm=Yp2w&id=1Ddvmwn9WcB__jocW70piDeWzyvGbqUP5) that represents the trained model. Finally, you only have to follow these steps:
```
git clone https://github.com/angeligareta/SaferAuto.git
mkdir -p models/weights/edb
mv <downloaded-weights> models/weights/edb/yolov3-spp_4000.weights
python3 ./darknet/darknet_video.py
```
The output would be similar to this one:
<p align="center" >
  <img src="https://github.com/angeligareta/SaferAuto/blob/master/res/media/first-detection-saferauto.gif" />
</p>*/
-->

### GUI for Safer Auto [beta]
The second way of testing the detector is a Graphical UI that shows the detection in a more user-friendly way. The process now is a bit tedious but I will try to imporve it in the future with Docker. 

#### Requirements
- Clone this repository (git clone https://github.com/angeligareta/SaferAuto.git).
- Nvidia GPU and correct driver.
- CUDA and OPENCV. I have written a [tutorial](https://saferauto.home.blog/2019/04/06/4o-how-to-install-yolo-darknet-with-cuda-and-opencv-in-ubuntu/) about it. You would need to follow the steps **CUDA INSTALLATION**(Nvidia GPU required) and **Installing OPENCV**.
- [QT Creator 5.12](https://www.qt.io/download). It is important to install this version.
- [Weights file](https://drive.google.com/uc?export=download&confirm=Yp2w&id=12ZeXoDsPeNnk7X66LEtOBeDCYNbBbj4C) that represents the trained model.
THe next step would be to set the weights file in the correct folder:
```
cd SaferAuto/
mkdir -p models/weights/
mv <downloaded-weights> models/weights/yolov3-saferauto.weights
```

#### Compiling
Once you satisfy all the requirements, you have to compile the darknet program. I am using a fork by [AlexeyAB](https://github.com/AlexeyAB/darknet). For compiling it with GPU follow these steps (from SaferAuto folder):
```
cd darknet/
sed -i s/GPU=0/GPU=1/g Makefile
sed -i s/CUDNN=0/CUDNN=1/g Makefile
sed -i s/OPENCV=0/OPENCV=1/g Makefile
sed -i s/LIBSO=0/LIBSO=1/g Makefile
make
cd ..
mkdir lib
cp libdarknet.so lib
```

#### Executing
After that, you need to open the project [SaferAutoGUI.pro](SaferAutoGUI.pro) with QT Creator. Finally you only need to execute the program.

The main window will open. In this first window, there are 4 options you can change, the configuration, names, weights and input file paths for testing the SaferAuto detector. Once you have selected the correct configuration (there is one selected by default), you can press the start button to start detecting!

You can see the interface windows here:
<p align="center" >
  <img height="400" width="600" src="https://github.com/angeligareta/SaferAuto/blob/master/res/media/main-window.png" />
</p>
<hr style="margin-top: "10px" margin-bottom: "10px" height: 1px; color: red; background-color: red;"/>
<p align="center" >
  <img height="400" width="600" src="https://github.com/angeligareta/SaferAuto/blob/master/res/media/detection-window.png" />
</p>

## Bibliography
Here I show some of the latest documentation I have found related with Advanced Driver Assistance Systems (ADAS). You can find a more detailed version of the project in my [final degree project report](https://riull.ull.es/xmlui/handle/915/14512)(only in spanish).

### Papers
* [Modular safe driving assistant](https://patentscope.wipo.int/search/docs2/pct/WO2018124987/pdf/K4bXp_8mgXtOiimgIsN83Emx_3SlgM63trIXmYiXShc3sc5bSGQpykJpqYXYsALG2x00ch9LZypLgGfTzTIj4oGFb6RtcRM_q04_bdU_f7yXE0CK5Hmkou_2h0Hq9FwU?docId=id00000043118723&psAuth=MeGp5WgtSXuL59RSkE9FUUGQxkHVGlDiqGcXTc0_28w)
* [The Architectural Implications of Autonomous Driving: Constraints and Acceleration](https://web.eecs.umich.edu/~shihclin/papers/AutonomousCar-ASPLOS18.pdf)
* [YOLO Object Detector for Onboard Driving Images](https://ddd.uab.cat/pub/tfg/2017/tfg_71066/paper.pdf)
* [Real-time object detection for autonomous driving-based on deep learning](https://tamucc-ir.tdl.org/handle/1969.6/5637)
* [Convolutional Neural Network for Person and Car Detection using YOLO Framework](http://journal.utem.edu.my/index.php/jtec/article/view/3599/2491)
* [Evaluating State-of-the-art Object Detector on Challenging Traffic Light Data](http://openaccess.thecvf.com/content_cvpr_2017_workshops/w9/papers/Jensen_Evaluating_State-Of-The-Art_Object_CVPR_2017_paper.pdf)
* [Yolov3](https://pjreddie.com/media/files/papers/YOLOv3.pdf)

### Example of final products
* [EyeSight](https://www.subaru.com/engineering/eyesight.html)
* [MobileEye](https://www.mobileye.com/)

### Tutorials
* [Yolo Algorithm](https://www.coursera.org/lecture/convolutional-neural-networks/yolo-algorithm-fF3O0)

## Author
[**Ángel Igareta**](https://github.com/angeligareta) - Computer Engineering Student

## License
This project is licensed under the **[MIT License](LICENSE)**
