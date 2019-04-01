# Modification of the GTSRB script [http://benchmark.ini.rub.de/?section=gtsrb&subsection=dataset#Structure]

# Python program for converting the ppm files from The German Traffic Sign Recognition Benchmark (GTSRB) to jpg files
# in order to use them in YOLO. Besides, it generate a txt with all the paths to the converted images in darknet format.
# By Ángel Igareta for SaferAuto [https://github.com/angeligareta/SaferAuto]

import matplotlib.pyplot as plt
import matplotlib.patches as patches
import csv
import os

input_train_path = "../GTSRB/input-img-train/"  # Path to the ppm images of the GTSRB dataset used to train the network.
input_test_path = "../GTSRB/input-img-test/"  # Path to the ppm images of the GTSRB dataset used to test the network.
output_train_path = "../GTSRB/output-img-train/"  # Path of the resulting training images of this script and labels.
output_test_path = "../GTSRB/output-img-test/"  # Path of the resulting testing images of this script and their labels.
train_text_file_path = "../GTSRB/sl-train.txt"  # Path of the training txt used as input for darknet.
test_text_file_path = "../GTSRB/sl-test.txt"  # Path of the testing txt used as input for darknet.


# Function for reading the images
def read_traffic_signs(input_path, output_path, text_file_path, train_mode):
    text_file = open(text_file_path, "w")
    number_dir = 9 if train_mode else 1  # Originally 43
    maximum_class = 9  # Originally 43

    for dir_index in range(0, number_dir):
        if train_mode:
            input_prefix = input_path + format(dir_index, '05d') + '/'  # Subdirectory for class
            gt_file = open(input_prefix + 'GT-' + format(dir_index, '05d') + '.csv')  # Annotations file
        else:
            input_prefix = input_path  # Subdirectory for class
            gt_file = open(input_prefix + 'GT-final_test.csv')  # Annotations file

        gt_reader = csv.reader(gt_file, delimiter=';')  # CSV parser for annotations file
        next(gt_reader, None)  # Skip the headers

        index = 0
        for row in gt_reader:
            object_class = int(row[7])
            if (object_class != 6) & (object_class < maximum_class):
                if os.path.isfile(input_prefix + row[0]): # CHECK FILE EXISTS
                    read_img(input_prefix, output_path, text_file, row, index)
            index += 1

        gt_file.close()


def read_img(input_prefix, output_path, text_file, row, index):
    img_width = float(row[1])
    img_height = float(row[2])
    object_lb_x1 = float(row[3])
    object_lb_y1 = float(row[4])
    object_rt_x2 = float(row[5])
    object_rt_y2 = float(row[6])

    object_class_real = int(row[7])
    if object_class_real < 6:
        object_class = str(object_class_real)
    else:
        object_class = str(object_class_real - 1)

    object_width = object_rt_x2 - object_lb_x1
    object_height = object_rt_y2 - object_lb_y1
    object_mid_x = (object_lb_x1 + object_rt_x2) / 2
    object_mid_y = (object_lb_y1 + object_rt_y2) / 2

    object_width_rel = object_width / img_width
    object_height_rel = object_height / img_height
    object_mid_x_rel = object_mid_x / img_width
    object_mid_y_rel = object_mid_y / img_height

    dark_net_label = "{} {} {} {} {}". \
        format(object_class, object_mid_x_rel, object_mid_y_rel, object_width_rel, object_height_rel)
    # print(dark_net_label)

    output_prefix = output_path + object_class + "-" + str(index)
    output_img = plt.imread(input_prefix + row[0])
    # show_img(img,  object_lb_x1, object_lb_y1, object_height, object_width)

    # WRITE THE FILE IN GENERAL FILE (TEST.TXT OR TRAIN.TXT)
    text_file.write(os.getcwd() + output_prefix[1:] + ".jpg\n")

    # SAVE IMG IN JPG FORMAT
    plt.imsave('{0}.jpg'.format(output_prefix), output_img)

    # SAVE TXT FILE WITH THE IMG
    f = open('{0}.txt'.format(output_prefix), "w")
    f.write(dark_net_label + "\n")


def show_img(img,  object_lb_x1, object_lb_y1, object_height, object_width):
    fig, ax = plt.subplots(1)
    ax.imshow(img)
    rect = patches.Rectangle(
        (object_lb_x1, object_lb_y1),
        object_width,
        object_height,
        linewidth=6,
        edgecolor='r',
        facecolor='none'
    )
    ax.add_patch(rect)
    plt.show()


read_traffic_signs(input_train_path, output_train_path, train_text_file_path, True)
read_traffic_signs(input_test_path, output_test_path, test_text_file_path, False)
