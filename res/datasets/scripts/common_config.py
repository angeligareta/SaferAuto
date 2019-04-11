import cv2
import os
import numpy as np
import random as rand
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from PIL import Image

FALSE_NEGATIVE_CLASS = 5
MAX_WIDTH = 512
MAX_HEIGHT = 512

TRAIN_PROB = 0.7  # REAL 0.85 ADDING FALSE DATA
TEST_PROB = 0.3  # REAL 0.15

SHOW_IMG = False
COLOR_MODE = -1
OUTPUT_IMG_EXTENSION = ".jpg"

# Superclasses BTSDB
traffic_sign_classes = {}
classes_counter_train = [0, 0, 0, 0, 0, 0]
classes_counter_test = [0, 0, 0, 0, 0]

DB_PREFIX = 'BTSDB'


def initialize_classes_counter():
    for i in range(0, len(classes_counter_train)):
        classes_counter_train[i] = 0

    for i in range(0, len(classes_counter_test)):
        classes_counter_test[i] = 0


def update_db_prefix(db_prefix):
    global DB_PREFIX
    DB_PREFIX = db_prefix


def read_img(input_img_file_path):
    return cv2.imread(input_img_file_path, COLOR_MODE)


def read_img_plt(input_img_file_path):
    return Image.open(input_img_file_path)


def get_img_dim(input_img):
    img_height, img_width, channels = input_img.shape
    return img_width, img_height


def get_img_dim_plt(input_img):
    return input_img.size


def write_img(output_file_path, output_img):
    return cv2.imwrite(output_file_path + OUTPUT_IMG_EXTENSION, output_img)


def resize_img(input_img):
    return cv2.resize(input_img, (MAX_WIDTH, MAX_HEIGHT))


def resize_img_plt(input_img):
    return np.asarray(input_img.resize((MAX_WIDTH, MAX_HEIGHT)))


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

    return FALSE_NEGATIVE_CLASS


def parse_darknet_format(object_class, img_width, img_height, left_x, bottom_y, right_x, top_y):
    object_width = right_x - left_x
    object_height = top_y - bottom_y
    object_mid_x = (left_x + right_x) / 2.0
    object_mid_y = (bottom_y + top_y) / 2.0

    object_width_rel = object_width / img_width
    object_height_rel = object_height / img_height
    object_mid_x_rel = object_mid_x / img_width
    object_mid_y_rel = object_mid_y / img_height

    dark_net_label = "{} {} {} {} {}". \
        format(object_class, object_mid_x_rel, object_mid_y_rel, object_width_rel, object_height_rel)

    return dark_net_label


def write_data(filename, input_img, input_img_labels, text_file, output_dir, train_file):
    output_file_path = output_dir + filename

    # Save file in general training/testing file
    text_file.write(output_file_path + OUTPUT_IMG_EXTENSION + "\n")
    # Save file in correct folder
    write_img(output_file_path, input_img)

    # SAVE TXT FILE WITH THE IMG
    f = open(output_file_path + '.txt', "a")
    for input_img_label in input_img_labels:
        f.write(input_img_label + "\n")

        object_class = int(input_img_label.split()[0])
        if train_file:
            classes_counter_train[object_class] += 1
        else:
            classes_counter_test[object_class] += 1


# Function for reading the images
def add_bg_data(total_bg_files, background_img_path, output_train_dir_path, train_text_file):
    print("Adding " + str(total_bg_files) + " background images...")
    bg_files = os.listdir(background_img_path)
    # Return a k length list of unique elements chosen from the population sequence. (population, k)
    bg_files_rand_list = rand.sample(bg_files, total_bg_files)

    for bg_file in bg_files_rand_list:
        input_img = read_img(background_img_path + bg_file)
        input_img = resize_img(input_img)

        if DB_PREFIX == 'btsdb-':
            output_filename = DB_PREFIX + "fn-" + bg_file[3:-4]
        else:
            output_filename = DB_PREFIX + "fn-" + bg_file[:-4]

        write_data(output_filename, input_img, [], train_text_file, output_train_dir_path, True)


def add_false_negatives(total_false_negatives_count, total_false_negatives_dir, output_train_dir_path, train_text_file):
    print("Adding " + str(total_false_negatives_count) + " false images...")
    false_negative_sublist = rand.sample(total_false_negatives_dir.keys(), total_false_negatives_count)

    for fn_filename in false_negative_sublist:
        fn_file_path = total_false_negatives_dir[fn_filename][0]
        input_img = read_img(fn_file_path)
        input_img = resize_img(input_img)

        if DB_PREFIX == 'btsdb-':
            output_filename = DB_PREFIX + "fn-" + fn_filename[3:-4]
        else:
            output_filename = DB_PREFIX + "fn-" + fn_filename[:-4]

        write_data(output_filename, input_img, [], train_text_file, output_train_dir_path, True)


def add_false_data(total_false_data, total_false_negatives_dir, background_img_path, output_train_dir_path, train_text_file):
    add_bg_data(round(total_false_data / 2), background_img_path, output_train_dir_path, train_text_file)
    add_false_negatives(round(total_false_data / 2), total_false_negatives_dir, output_train_dir_path, train_text_file)
