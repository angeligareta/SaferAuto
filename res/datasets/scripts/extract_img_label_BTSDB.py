# Program to extract img and labels from BTSDB and converting them to darknet format.

# Python program for converting the ppm files from The Belgium Traffic Sign dataset to jpg files
# in order to use them in YOLO. Besides, it generate a txt with all the paths to the converted images in darknet format.
# By Angel Igareta for SaferAuto [https://github.com/angeligareta/SaferAuto]
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import random as rand
import csv
import os

from PIL import Image


BTSDB_ROOT_PATH = "/home/angeliton/Desktop/SaferAuto/res/datasets/BTSDB/"

COMBINED_ANNOTATIONS_FILE_PATH = BTSDB_ROOT_PATH + "annotations-combined.txt"

# Path to the ppm images of the BTSDB dataset.
INPUT_PATH = BTSDB_ROOT_PATH + "input-img/"
BACKGROUND_IMG_PATH = BTSDB_ROOT_PATH + "input-img-bg/"

# Path of the resulting training and testing images of this script and labels.

OUTPUT_TRAIN_PATH = BTSDB_ROOT_PATH + "output-img-train/"
OUTPUT_TEST_PATH = BTSDB_ROOT_PATH + "output-img-test/"

# Path of the training and testing txt used as input for darknet.
OUTPUT_TRAIN_TEXT_PATH = BTSDB_ROOT_PATH + "btsdb-train.txt"
OUTPUT_TEST_TEXT_PATH = BTSDB_ROOT_PATH + "btsdb-test.txt"

TRAIN_PROB = 0.8
TEST_PROB = 0.2

# Superclasses BTSDB
traffic_sign_classes = {
    "0-prohibitory": [2, 8],
    "1-danger": [1],
    "2-mandatory": [3],
    "3-stop": [7],
    "4-yield": [6],
    "5-false_negatives": [-1, 0, 4, 5, 9, 10, 11]  # undefined, other, redbluecircles, diamonds
}

FALSE_NEGATIVE_CLASS = 5
classes_counter_train = [0, 0, 0, 0, 0, 0]
classes_counter_test = [0, 0, 0, 0, 0, 0]

MAX_WIDTH = 500
MAX_HEIGHT = 500


def calculate_darknet_dimensions(object_class, img_width, img_height, left_x, bottom_y, right_x, top_y):
    # print("1: ", img_width, img_height, left_x, top_y, right_x, bottom_y)

    object_width = right_x - left_x
    object_height = top_y - bottom_y  # Reversed order of y!!!
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


def write_data(object_class_adjusted, input_img, text_file, dark_net_label, output_file_path, train_file):
    # Do not generate empty txt in test file
    if (not train_file) & (object_class_adjusted == FALSE_NEGATIVE_CLASS):
        return

    # If the file is not already there (another class), create it
    if not os.path.isfile(output_file_path):
        text_file.write(output_file_path + ".jpg\n")
        plt.imsave(output_file_path + '.jpg', input_img)

    # SAVE TXT FILE WITH THE IMG
    f = open(output_file_path + '.txt', "a")
    if object_class_adjusted != FALSE_NEGATIVE_CLASS:  # NotFalse negative
        f.write(dark_net_label + "\n")


def adjust_object_class(obj_class):
    for classes in traffic_sign_classes.items():
        if obj_class in classes[1]:
            object_class_adjusted = int(classes[0].split("-")[0])
            return object_class_adjusted


# Function for reading the images
def add_background_data(total_bg_files, train_text_file):
    print("Adding " + str(total_bg_files) + " background images...")
    bg_files = os.listdir(BACKGROUND_IMG_PATH)
    # Return a k length list of unique elements chosen from the population sequence. (population, k)
    bg_files_rand_list = rand.sample(bg_files, total_bg_files)

    for bg_file in bg_files_rand_list:
        input_img = Image.open(BACKGROUND_IMG_PATH + bg_file)
        input_img = np.asarray(input_img.resize((MAX_WIDTH, MAX_HEIGHT)))

        filename = "bg-" + bg_file[:-4]
        write_data(FALSE_NEGATIVE_CLASS, input_img, train_text_file, "", OUTPUT_TRAIN_PATH + filename)



def read_traffic_signs():
    train_text_file = open(OUTPUT_TRAIN_TEXT_PATH, "w")
    test_text_file = open(OUTPUT_TEST_TEXT_PATH, "w")

    gt_file = open(COMBINED_ANNOTATIONS_FILE_PATH)  # Annotations file
    gt_reader = csv.reader(gt_file, delimiter=';')  # CSV parser for annotations file
    # test = 50

    for row in gt_reader:
        filename = row[0][:-4] + ".jpg"
        file_path = INPUT_PATH + filename

        if os.path.isfile(file_path):
            input_img = Image.open(file_path)
            img_width, img_height = input_img.size
            input_img = np.asarray(input_img.resize((MAX_WIDTH, MAX_HEIGHT)))

            width_proportion = (img_width / MAX_WIDTH)
            height_proportion = (img_height / MAX_HEIGHT)

            left_x = float(row[1]) / width_proportion
            bottom_y = float(row[2]) / height_proportion
            right_x = float(row[3]) / width_proportion
            top_y = float(row[4]) / height_proportion
            # object_real_class = int(row[5])
            object_class = int(row[6])

            # show_img(input_img, left_x, bottom_y, (right_x - left_x), (top_y - bottom_y))

            # Join classes and adjust the rest
            object_class_adjusted = adjust_object_class(object_class)
            dark_net_label = calculate_darknet_dimensions(object_class_adjusted, img_width, img_height, left_x, bottom_y,
                                                          right_x, top_y)
            # Remove jp2 extension
            output_filename = filename[3:-4]

            # Get percentage for train and another for testing
            train_file = rand.choices([True, False], [TRAIN_PROB, TEST_PROB])[0]
            if train_file:
                write_data(object_class_adjusted, input_img, train_text_file, dark_net_label,
                           OUTPUT_TRAIN_PATH + output_filename, train_file)
                classes_counter_train[object_class_adjusted] += 1
            else:
                write_data(object_class_adjusted, input_img, test_text_file, dark_net_label,
                           OUTPUT_TEST_PATH + output_filename, train_file)
                classes_counter_test[object_class_adjusted] += 1

            # test -= 1
            # if test == 0:
            #    break

    background_images = sum(classes_counter_train[:-1]) - classes_counter_train[FALSE_NEGATIVE_CLASS]

    if background_images > 0:
        add_background_data(background_images, train_text_file)

    print("[TRAIN FILES]")
    print_class_info(classes_counter_train)

    print("\n[TEST FILES]")
    print_class_info(classes_counter_test)

    print("\n[PROPORTION]")
    for i in range(0, len(classes_counter_train)):
        total_classes = classes_counter_train[i] + classes_counter_test[i]
        print('\t-CLASS: ' + str(i) + ' : ' + "{:.2f}%".format(classes_counter_test[i]/total_classes * 100.0))

    gt_file.close()


def print_class_info(classes_counter):
    for i in range(0, len(classes_counter)):
        print('\t-CLASS: ' + str(i) + ' : ' + str(classes_counter[i]))
    print('TOTAL: ' + str(sum(classes_counter)))


read_traffic_signs()
