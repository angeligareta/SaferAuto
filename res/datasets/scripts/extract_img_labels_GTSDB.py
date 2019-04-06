# Modification of the GTSRB script [http://benchmark.ini.rub.de/?section=gtsrb&subsection=dataset#Structure]

# Python program for converting the ppm files from The German Traffic Sign Recognition Benchmark (GTSRB) to jpg files
# in order to use them in YOLO. Besides, it generate a txt with all the paths to the converted images in darknet format.
# By Ángel Igareta for SaferAuto [https://github.com/angeligareta/SaferAuto]
from random import choices

import cv2
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import csv
import os

import numpy as np
from PIL import Image

ANNOTATIONS_FILE_PATH = "../GTSDB/gt.txt"
INPUT_PATH = "../GTSDB/input-img/"  # Path to the ppm images of the GTSRB dataset.
GTSDB_ROOT_PATH = "/home/angeliton/Desktop/SaferAuto/res/datasets/GTSDB/"

# Path of the resulting training images of this script and labels.
OUTPUT_TRAIN_PATH = GTSDB_ROOT_PATH + "output-img-train/"
# Path of the resulting testing images of this script and their labels.
OUTPUT_TEST_PATH = GTSDB_ROOT_PATH + "output-img-test1/"

TRAIN_TEXT_FILE_PATH = GTSDB_ROOT_PATH + "gtsdb-train.txt"  # Path of the training txt used as input for darknet.
TEST_TEXT_FILE_PATH = GTSDB_ROOT_PATH + "gtsdb-test.txt"  # Path of the testing txt used as input for darknet.

TRAIN_PROB = 0.9
TEST_PROB = 0.1


traffic_sign_classes = {
    "0-prohibitory": [0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 15, 16, 17],
    "1-danger": [11, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31],
    "2-mandatory": [33, 34, 35, 36, 37, 38, 39, 40],
    "3-stop": [14],
    "4-yield": [13],
    "5-false_negatives": [6, 12, 32, 41, 42]
}


classes_counter = [0, 0, 0, 0, 0, 0]


def calculate_darknet_dimensions(object_class, img_width, img_height, left_x, top_y, right_x, bottom_y):
    # print("1: ", img_width, img_height, left_x, top_y, right_x, bottom_y)

    object_width = right_x - left_x
    object_height = bottom_y - top_y  # Reversed order of y!!!
    object_mid_x = (left_x + right_x) / 2
    object_mid_y = (bottom_y + top_y) / 2

    # print("2: ", object_width, object_height, object_mid_x, object_mid_y)

    object_width_rel = object_width / img_width
    object_height_rel = object_height / img_height
    object_mid_x_rel = object_mid_x / img_width
    object_mid_y_rel = object_mid_y / img_height

    dark_net_label = "{} {} {} {} {}". \
        format(object_class, object_mid_x_rel, object_mid_y_rel, object_width_rel, object_height_rel)

    return dark_net_label


def show_img(img,  object_lb_x1, object_lb_y1, object_width, object_height):
    fig, ax = plt.subplots(1)
    ax.imshow(img)
    rect = patches.Rectangle(
        (object_lb_x1, object_lb_y1),
        object_width,
        object_height,
        linewidth=1,
        edgecolor='r',
        facecolor='none'
    )
    ax.add_patch(rect)
    plt.show()


def write_data(object_class_adjusted, input_img, text_file, dark_net_label, output_file_path):
    # WRITE THE FILE IN GENERAL FILE (TEST.TXT OR TRAIN.TXT)
    # SAVE IMG IN JPG FORMAT IF HAS NOT ALREADY BEEN SAVED
    if not os.path.isfile(output_file_path):
        text_file.write(output_file_path + ".jpg\n")
        cv2.imwrite(output_file_path + '.jpg', input_img)

    # SAVE TXT FILE WITH THE IMG
    f = open(output_file_path + '.txt', "a")
    if (object_class_adjusted != 5): # NotFalse negative
        f.write(dark_net_label + "\n")


def adjust_object_class(obj_class):
    for classes in traffic_sign_classes.items():
        if (obj_class in classes[1]):
            object_class_adjusted = int(classes[0].split("-")[0])
            classes_counter[object_class_adjusted] += 1
            return object_class_adjusted


def rgb2gray(rgb):
    return np.dot(rgb[...,:3], [0.2989, 0.5870, 0.1140])

# Function for reading the images
def read_traffic_signs(input_path, annotations_file_path, output_train_path, output_test_path):
    train_text_file = open(TRAIN_TEXT_FILE_PATH, "w")
    test_text_file = open(TEST_TEXT_FILE_PATH, "w")

    gt_file = open(annotations_file_path)  # Annotations file
    gt_reader = csv.reader(gt_file, delimiter=';')  # CSV parser for annotations file

    for row in gt_reader:
        filename = row[0]
        file_path = input_path + filename
        left_x = int(row[1])
        top_y = int(row[2])
        right_x = int(row[3])
        bottom_y = int(row[4])
        object_class = int(row[5])

        if os.path.isfile(file_path):
            img = Image.open(file_path)
            img_width, img_height = img.size

            input_img = cv2.imread(file_path, 0)

            # show_img(input_img, left_x, bottom_y, (right_x - left_x), (top_y - bottom_y))

            # Join classes and adjust the rest
            object_class_adjusted = adjust_object_class(object_class)

            dark_net_label = calculate_darknet_dimensions(object_class_adjusted, img_width, img_height, left_x, top_y, right_x, bottom_y)
            output_filename = filename[:-4]

            # Get percentage for train and another for testing
            train_file = choices([True, False], [TRAIN_PROB, TEST_PROB])[0]
            if train_file:
                write_data(object_class_adjusted, input_img, train_text_file, dark_net_label, output_train_path + output_filename)
            else:
                write_data(object_class_adjusted, input_img, test_text_file, dark_net_label, output_test_path + output_filename)

    for i in range(0, len(classes_counter)):
        print('CLASS ' + str(i) + ' : ' + str(classes_counter[i]))
    gt_file.close()


read_traffic_signs(INPUT_PATH, ANNOTATIONS_FILE_PATH, OUTPUT_TRAIN_PATH, OUTPUT_TEST_PATH)
