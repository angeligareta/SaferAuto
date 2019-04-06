# Program to extract img and labels from BTSDB and converting them to darknet format.

# Python program for converting the ppm files from The Belgium Traffic Sign dataset to jpg files
# in order to use them in YOLO. Besides, it generate a txt with all the paths to the converted images in darknet format.
# By Angel Igareta for SaferAuto [https://github.com/angeligareta/SaferAuto]
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import csv
import os
import cv2

import glymur

from PIL import Image
from pgmagick import Image as Magic

BTSDB_ROOT_PATH = "/home/angeliton/Desktop/SaferAuto/res/datasets/BTSDB/"

TRAIN_ANNOTATIONS_FILE_PATH = BTSDB_ROOT_PATH + "BTSD_training_GTclear.txt"
TEST_ANNOTATIONS_FILE_PATH = BTSDB_ROOT_PATH + "BTSD_testing_GTclear.txt"

# Path to the ppm images of the BTSDB dataset.
INPUT_PATH = BTSDB_ROOT_PATH + "input-img/"

# Path of the resulting training and testing images of this script and labels.

OUTPUT_TRAIN_PATH = BTSDB_ROOT_PATH + "output-img-train/"
OUTPUT_TEST_PATH = BTSDB_ROOT_PATH + "output-img-test1/"

# Path of the training and testing txt used as input for darknet.
OUTPUT_TRAIN_TEXT_PATH = BTSDB_ROOT_PATH + "btsdb-test.txt"
OUTPUT_TEST_TEXT_PATH = BTSDB_ROOT_PATH + "btsdb-train.txt"

# Superclasses BTSDB
traffic_sign_classes = {
    "0-prohibitory": [2, 8],
    "1-danger": [1],
    "2-mandatory": [3],
    "3-redbluecircles": [4],
    "4-diamonds": [5],
    "5-yield": [6],
    "6-stop": [7],
    "7-other": [9, 10, 11],
    "8-false_negatives": [-1, 0]
}

FALSE_NEGATIVE_CLASS = 8
classes_counter = [0, 0, 0, 0, 0, 0, 0, 0, 0]


def calculate_darknet_dimensions(object_class, img_width, img_height, left_x, top_y, right_x, bottom_y):
    # print("1: ", img_width, img_height, left_x, top_y, right_x, bottom_y)

    object_width = right_x - left_x
    object_height = bottom_y - top_y  # Reversed order of y!!!
    object_mid_x = (left_x + right_x) / 2.0
    object_mid_y = (bottom_y + top_y) / 2.0

    # print("2: ", object_width, object_height, object_mid_x, object_mid_y)

    object_width_rel = object_width / img_width
    object_height_rel = object_height / img_height
    object_mid_x_rel = object_mid_x / img_width
    object_mid_y_rel = object_mid_y / img_height

    dark_net_label = "{} {} {} {} {}". \
        format(object_class, object_mid_x_rel, object_mid_y_rel, object_width_rel, object_height_rel)

    return dark_net_label


def show_img(img, object_lb_x1, object_lb_y1, object_width, object_height):
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


def write_data(object_class_adjusted, input_img, text_file, dark_net_label, output_file_path, output_filename):
    # WRITE THE FILE IN GENERAL FILE (TEST.TXT OR TRAIN.TXT)
    # SAVE IMG IN JPG FORMAT IF HAS NOT ALREADY BEEN SAVED
    output_filename = output_filename[3:]
    output_file_path += output_filename
    if not os.path.isfile(output_file_path):
        text_file.write(output_file_path + ".jpg\n")
        # input_img.save(output_file_path + '.jpg')
        plt.imsave(output_file_path + '.jpg', input_img)

    # SAVE TXT FILE WITH THE IMG
    f = open(output_file_path + '.txt', "a")
    if object_class_adjusted != FALSE_NEGATIVE_CLASS:  # NotFalse negative
        f.write(dark_net_label + "\n")


def adjust_object_class(obj_class):
    for classes in traffic_sign_classes.items():
        if obj_class in classes[1]:
            object_class_adjusted = int(classes[0].split("-")[0])
            classes_counter[object_class_adjusted] += 1
            return object_class_adjusted


# Function for reading the images
def read_traffic_signs(input_path, annotations_file_path, output_text_path, output_path):
    output_text_file = open(output_text_path, "w")

    gt_file = open(annotations_file_path)  # Annotations file
    gt_reader = csv.reader(gt_file, delimiter=';')  # CSV parser for annotations file

    for row in gt_reader:
        filename = row[0][:-4] + ".jpg"
        file_path = input_path + filename
        left_x = float(row[1])
        bottom_y = float(row[2])
        right_x = float(row[3])
        top_y = float(row[4])
        # object_real_class = int(row[5])
        object_class = int(row[6])

        if os.path.isfile(file_path):
            im = Image.open(file_path)

            width, height = im.size

            input_img = plt.imread(file_path)

            img_width, img_height = im.size
            # show_img(input_img, left_x, bottom_y, (right_x - left_x), (top_y - bottom_y))

            # Join classes and adjust the rest
            object_class_adjusted = adjust_object_class(object_class)

            dark_net_label = calculate_darknet_dimensions(object_class_adjusted, img_width, img_height, left_x, top_y,
                                                          right_x, bottom_y)
            output_filename = filename[:-4]

            write_data(object_class_adjusted, input_img, output_text_file, dark_net_label,
                       output_path, output_filename)

    for i in range(0, len(classes_counter)):
        print('CLASS ' + str(i) + ' : ' + str(classes_counter[i]))

    print('TOTAL: ' + str(sum(classes_counter)))
    gt_file.close()


read_traffic_signs(INPUT_PATH, TRAIN_ANNOTATIONS_FILE_PATH, OUTPUT_TRAIN_TEXT_PATH, OUTPUT_TRAIN_PATH)
read_traffic_signs(INPUT_PATH, TEST_ANNOTATIONS_FILE_PATH, OUTPUT_TEST_TEXT_PATH, OUTPUT_TEST_PATH)
