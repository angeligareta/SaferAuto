# Modification of the GTSRB script [http://benchmark.ini.rub.de/?section=gtsrb&subsection=dataset#Structure]

# Python program for converting the ppm files from The German Traffic Sign Recognition Benchmark (GTSRB) to jpg files
# in order to use them in YOLO. Besides, it generate a txt with all the paths to the converted images in darknet format.
# By Ángel Igareta for SaferAuto [https://github.com/angeligareta/SaferAuto]
from random import choices

import matplotlib.pyplot as plt
import matplotlib.patches as patches
import csv
import os

from PIL import Image

ANNOTATIONS_FILE_PATH = "../GTSDB/gt.txt"
INPUT_PATH = "../GTSDB/input-img/"  # Path to the ppm images of the GTSRB dataset.
GTSDB_ROOT_PATH = "/home/angeliton/Desktop/SaferAuto/res/datasets/GTSDB/"

# Path of the resulting training images of this script and labels.
OUTPUT_TRAIN_PATH = GTSDB_ROOT_PATH + "output-img-train/"
# Path of the resulting testing images of this script and their labels.
OUTPUT_TEST_PATH = GTSDB_ROOT_PATH + "output-img-test/"

TRAIN_TEXT_FILE_PATH = GTSDB_ROOT_PATH + "gtsdb-train.txt"  # Path of the training txt used as input for darknet.
TEST_TEXT_FILE_PATH = GTSDB_ROOT_PATH + "gtsdb-test.txt"  # Path of the testing txt used as input for darknet.

TRAIN_PROB = 0.75
TEST_PROB = 0.25


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


def write_data(input_img, text_file, dark_net_label, output_file_path):
    # WRITE THE FILE IN GENERAL FILE (TEST.TXT OR TRAIN.TXT)
    # SAVE IMG IN JPG FORMAT IF HAS NOT ALREADY BEEN SAVED
    if not os.path.isfile(output_file_path):
        text_file.write(output_file_path + ".jpg\n")
        plt.imsave(output_file_path + '.jpg', input_img)

    # SAVE TXT FILE WITH THE IMG
    f = open(output_file_path + '.txt', "a")
    f.write(dark_net_label + "\n")


# Function for reading the images
def read_traffic_signs(input_path, annotations_file_path, output_train_path, output_test_path):
    train_text_file = open(TRAIN_TEXT_FILE_PATH, "w")
    test_text_file = open(TEST_TEXT_FILE_PATH, "w")
    maximum_class = 9  # Originally 45

    gt_file = open(annotations_file_path)  # Annotations file
    gt_reader = csv.reader(gt_file, delimiter=';')  # CSV parser for annotations file

    # processed_files = []
    classes_count = {}
    for class_count in range(0, maximum_class):
        classes_count[str(class_count)] = 0  # ADD ONE TO CLASSES

    for row in gt_reader:
        filename = row[0]
        file_path = input_path + filename
        left_x = int(row[1])
        top_y = int(row[2])
        right_x = int(row[3])
        bottom_y = int(row[4])
        object_class = int(row[5])

        if (object_class != 6) & (object_class < maximum_class) & os.path.isfile(file_path):
            im = Image.open(file_path)
            width, height = im.size

            input_img = plt.imread(file_path)
            img_width, img_height = im.size
            # show_img(input_img, left_x, bottom_y, (right_x - left_x), (top_y - bottom_y))

            if object_class < 6:
                object_class_adjusted = str(object_class)
            else:
                object_class_adjusted = str(object_class - 1)

            dark_net_label = calculate_darknet_dimensions(object_class_adjusted, img_width, img_height, left_x, top_y, right_x, bottom_y)

            # Only if file has not been processed increment classes count.
            # if filename not in processed_files:
            #    processed_files.append(filename)
            #    classes_count[object_class] = int(classes_count[object_class]) + 1

            # incremented_object_class = int(object_class) + 1
            # output_prefix = 'class' + str(incremented_object_class) + '-' + str(classes_count[object_class])
            output_filename = filename[:-4]

            # Get percentage for train and another for testing
            train_file = choices([True, False], [TRAIN_PROB, TEST_PROB])[0]
            if train_file:
                write_data(input_img, train_text_file, dark_net_label, output_train_path + output_filename)
            else:
                write_data(input_img, test_text_file, dark_net_label, output_test_path + output_filename)

    gt_file.close()


read_traffic_signs(INPUT_PATH, ANNOTATIONS_FILE_PATH, OUTPUT_TRAIN_PATH, OUTPUT_TEST_PATH)
