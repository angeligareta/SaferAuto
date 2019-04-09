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

OUTPUT_TRAIN_DIR_PATH = BTSDB_ROOT_PATH + "output-img-train/"
OUTPUT_TEST_DIR_PATH = BTSDB_ROOT_PATH + "output-img-test/"

# Path of the training and testing txt used as input for darknet.
OUTPUT_TRAIN_TEXT_PATH = BTSDB_ROOT_PATH + "btsdb-train.txt"
OUTPUT_TEST_TEXT_PATH = BTSDB_ROOT_PATH + "btsdb-test.txt"
OUTPUT_TRAIN_TENSOR_TEXT_PATH = BTSDB_ROOT_PATH + "train.txt"

FALSE_NEGATIVE_CLASS = 5
MAX_WIDTH = 512
MAX_HEIGHT = 512


# Superclasses BTSDB
traffic_sign_classes = {
    "0-prohibitory": [2, 8],
    "1-danger": [1],
    "2-mandatory": [3],
    "3-stop": [7],
    "4-yield": [6],
    "5-false_negatives": [-1, 0, 4, 5, 9, 10, 11]  # undefined, other, redbluecircles, diamonds
}

classes_counter_train = [0, 0, 0, 0, 0, 0]
classes_counter_test = [0, 0, 0, 0, 0]


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


def adjust_object_class(obj_class):
    for classes in traffic_sign_classes.items():
        if obj_class in classes[1]:
            object_class_adjusted = int(classes[0].split("-")[0])
            return object_class_adjusted


def parse_darknet_format(object_class, img_width, img_height, left_x, bottom_y, right_x, top_y):
    dark_net_label = "{} {} {} {} {}". \
        format(left_x, bottom_y, right_x, top_y, object_class)

    return dark_net_label


def calculate_darknet_format(input_img, image_width, image_height, row):
    real_img_width, real_img_height = input_img.size
    width_proportion = (real_img_width / MAX_WIDTH)
    height_proportion = (real_img_height / MAX_HEIGHT)

    left_x = float(row[1]) / width_proportion
    bottom_y = float(row[2]) / height_proportion
    right_x = float(row[3]) / width_proportion
    top_y = float(row[4]) / height_proportion

    object_class = int(row[6])
    object_class_adjusted = adjust_object_class(object_class)  # Adjust class category

    # show_img(input_img, left_x, bottom_y, (right_x - left_x), (top_y - bottom_y))

    return parse_darknet_format(object_class_adjusted, image_width, image_height, left_x, bottom_y, right_x, top_y)


def write_data(filename, input_img, input_img_labels, output_dir, train_tensor_file):
    output_filename = filename[3:-4]
    output_file_path = output_dir + output_filename

    # Save file in correct folder
    # plt.imsave(output_file_path + '.jpg', input_img)

    train_tensor_file.write(output_file_path + ".jpg ")

    # SAVE TXT FILE WITH THE IMG
    for input_img_label in input_img_labels:
        train_tensor_file.write(input_img_label + " ")

        object_class = int(input_img_label.split()[4])
        classes_counter_train[object_class] += 1

    train_tensor_file.write("\n")


# Function for reading the images
def add_bg_data(total_bg_files, train_tensor_file):
    print("Adding " + str(total_bg_files) + " background images...")
    bg_files = os.listdir(BACKGROUND_IMG_PATH)
    # Return a k length list of unique elements chosen from the population sequence. (population, k)
    bg_files_rand_list = rand.sample(bg_files, total_bg_files)

    for bg_file in bg_files_rand_list:
        input_img = Image.open(BACKGROUND_IMG_PATH + bg_file)
        input_img = np.asarray(input_img.resize((MAX_WIDTH, MAX_HEIGHT)))

        filename = "bg-" + bg_file[:-4]
        write_data(filename, input_img, [], OUTPUT_TRAIN_DIR_PATH, train_tensor_file)


def add_false_negatives(total_false_negatives_count, total_false_negatives_dir, train_tensor_file):
    print("Adding " + str(total_false_negatives_count) + " false images...")
    false_negative_sublist = rand.sample(total_false_negatives_dir.keys(), total_false_negatives_count)

    for fn_filename in false_negative_sublist:
        fn_file_path = total_false_negatives_dir[fn_filename][0]
        input_img = Image.open(fn_file_path)
        input_img = np.asarray(input_img.resize((MAX_WIDTH, MAX_HEIGHT)))

        write_data(fn_filename, input_img, [], OUTPUT_TRAIN_DIR_PATH, train_tensor_file)


def add_false_data(total_false_data, total_false_negatives_dir, train_tensor_file):
    add_bg_data(round(total_false_data / 2), train_tensor_file)
    add_false_negatives(round(total_false_data / 2), total_false_negatives_dir, train_tensor_file)


def read_traffic_signs():
    img_labels = {}  # Set of images and its labels [filename]: [()]
    train_tensor_file = open(OUTPUT_TRAIN_TENSOR_TEXT_PATH, "w")

    gt_file = open(COMBINED_ANNOTATIONS_FILE_PATH)  # Annotations file
    gt_reader = csv.reader(gt_file, delimiter=';')  # CSV parser for annotations file

    # WRITE ALL THE DATA IN A DICTIONARY (TO GROUP LABELS ON SAME IMG)
    for row in gt_reader:
        filename = row[0][:-4] + ".jpg"  # Replace jp2 with jpg
        file_path = INPUT_PATH + filename

        if os.path.isfile(file_path):
            input_img = Image.open(file_path)
            darknet_label = calculate_darknet_format(input_img, MAX_WIDTH, MAX_HEIGHT, row)
            object_class_adjusted = int(darknet_label.split()[4])

            if filename not in img_labels.keys():  # If it is the first label for that img
                img_labels[filename] = [file_path]

            if object_class_adjusted != FALSE_NEGATIVE_CLASS:  # Add only useful labels (not false negatives)
                img_labels[filename].append(darknet_label)

    # COUNT FALSE NEGATIVES (IMG WITHOUT LABELS)
    total_false_negatives_dir = {}
    total_annotated_images_dir = {}
    for filename in img_labels.keys():
        img_label_subset = img_labels[filename]
        if len(img_label_subset) == 1:
            total_false_negatives_dir[filename] = img_label_subset
        else:
            total_annotated_images_dir[filename] = img_label_subset

    total_annotated_images = len(img_labels.keys()) - len(total_false_negatives_dir.keys())
    total_false_data = round(total_annotated_images)  # False data: False negative + background

    print("total_false_negatives: " + str(len(total_false_negatives_dir.keys())))
    print("total_annotated_images: " + str(total_annotated_images) + " == "
          + str(len(total_annotated_images_dir.keys())))
    print("total_false_data: " + str(total_false_data))

    # ADD FALSE IMAGES TO TRAIN
    # add_false_data(total_false_data, total_false_negatives_dir, train_tensor_file)

    # ADJUST PROBABILITIES TO MAINTAIN OLD PROPORTIONS (Counting false negatives from training)
    # orig_test_img_size = TEST_PROB * len(img_labels.keys())
    # new_test_prob = orig_test_img_size / total_annotated_images
    # new_train_prob = 1.0 - new_test_prob

    # SET ANNOTATED IMAGES IN TRAIN OR TEST DIRECTORIES
    # max_imgs = 1000
    for filename in total_annotated_images_dir.keys():
        input_img_file_path = img_labels[filename][0]
        input_img = Image.open(input_img_file_path)  # Read image from image_file_path
        input_img = np.asarray(input_img.resize((MAX_WIDTH, MAX_HEIGHT)))  # Resize img
        input_img_labels = img_labels[filename][1:]

        write_data(filename, input_img, input_img_labels, OUTPUT_TRAIN_DIR_PATH, train_tensor_file)

        # max_imgs -= 1
        # if max_imgs == 0:
        #    break

    print("[TRAIN FILES]")
    print_class_info(classes_counter_train)

    print("\n[TEST FILES]")
    print_class_info(classes_counter_test)

    print("\n[PROPORTION]")
    for i in range(0, len(classes_counter_test)):
        total_classes = classes_counter_train[i] + classes_counter_test[i]
        print('\t-CLASS: ' + str(i) + ' : ' + "{:.2f}%".format(classes_counter_test[i]/total_classes * 100.0))

    gt_file.close()
    train_tensor_file.close()


def print_class_info(classes_counter):
    for i in range(0, len(classes_counter)):
        print('\t-CLASS: ' + str(i) + ' : ' + str(classes_counter[i]))
    print('TOTAL: ' + str(sum(classes_counter)))


read_traffic_signs()
