import cv2
import serial
import pyrebase
import time

# Initialize serial communication with the Arduino
arduino = serial.Serial('/dev/ttyACM0', 9600)

# Load class names for object detection
classFile = '/home/youssef/Desktop/ObjectDetector1/coco.names'
with open(classFile, 'rt') as f:
    classNames = f.read().rstrip('\n').split('\n')

# Paths to object detection model
configPath = '/home/youssef/Desktop/ObjectDetector1/ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt'
weightsPath = '/home/youssef/Desktop/ObjectDetector1/frozen_inference_graph.pb'

# Initialize the object detection model
net = cv2.dnn_DetectionModel(weightsPath, configPath)
net.setInputSize(164, 164)
net.setInputScale(1.0 / 127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)

# Initialize variables
thres = 0.272
numbers = 0
plant_detected = False

# Open the camera
cap = cv2.VideoCapture(0)

while True:
    success, img = cap.read()
    classIds, confs, bbox = net.detect(img, thres)

    if len(classIds) != 0:
        for classId, confidence, box in zip(classIds.flatten(), confs.flatten(), bbox):
            if classNames[classId - 1] == 'potted plant':
                cv2.rectangle(img, box, color=(0, 100, 0), thickness=2)
                cv2.putText(img, classNames[classId - 1], (box[0] + 10, box[1] + 40),
                            cv2.FONT_HERSHEY_PLAIN, fontScale=3, color=(0, 100, 0), thickness=3)
                arduino.write(b'1')
                numbers += 1

                # Create data dictionary
                data = {"numbers": numbers}

                # TODO: Uncomment the following lines if you intend to use D-Bus
                # dbus.update(data)
                # time.sleep(2)

    cv2.imshow('Output', img)
    cv2.waitKey(1)
