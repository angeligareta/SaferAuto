import res.datasets.scripts.btsdb_extractor as BTSDB
import res.datasets.scripts.gtsdb_extractor as GTSDB
import res.datasets.scripts.lisats_extractor as LISATS

ROOT_PATH = "/home/angeliton/Desktop/SaferAuto/res/datasets/GENERAL/"

# Path of the training and testing txt used as input for darknet.
OUTPUT_TRAIN_TEXT_PATH = ROOT_PATH + "train.txt"
OUTPUT_TEST_TEXT_PATH = ROOT_PATH + "test.txt"

# Path of the resulting training and testing images of this script and labels.
OUTPUT_TRAIN_DIR_PATH = ROOT_PATH + "output-img-train/"
OUTPUT_TEST_DIR_PATH = ROOT_PATH + "output-img-test/"


def print_class_info(classes_counter):
    for i in range(0, len(classes_counter)):
        print('\t-CLASS: ' + str(i) + ' : ' + str(classes_counter[i]))
    print('TOTAL: ' + str(sum(classes_counter)))


def print_db_info(classes_counter_train, classes_counter_test):
    print("[TRAIN FILES]")
    print_class_info(classes_counter_train)

    print("\n[TEST FILES]")
    print_class_info(classes_counter_test)

    print("\n[PROPORTION]")
    for i in range(0, len(classes_counter_test)):
        total_classes = classes_counter_train[i] + classes_counter_test[i]
        if total_classes == 0:
            total_classes = 1
        print('\t-CLASS: ' + str(i) + ' : ' + "{:.2f}%".format(classes_counter_test[i] / total_classes * 100.0))


def add_arrays(array_1, array_2):
    for i in range(0, len(array_1)):
        array_2[i] += array_1[i]

    return array_2

def main():
    classes_counter_train_total, classes_counter_test_total = \
        GTSDB.read_traffic_signs(OUTPUT_TRAIN_TEXT_PATH, OUTPUT_TEST_TEXT_PATH, OUTPUT_TRAIN_DIR_PATH, OUTPUT_TEST_DIR_PATH)
    print_db_info(classes_counter_train_total, classes_counter_test_total)

    classes_counter_train, classes_counter_test = \
          BTSDB.read_traffic_signs(OUTPUT_TRAIN_TEXT_PATH, OUTPUT_TEST_TEXT_PATH, OUTPUT_TRAIN_DIR_PATH, OUTPUT_TEST_DIR_PATH)
    print_db_info(classes_counter_train, classes_counter_test)

    classes_counter_train_total = add_arrays(classes_counter_train_total, classes_counter_train)
    classes_counter_test_total = add_arrays(classes_counter_test_total, classes_counter_test)

    classes_counter_train, classes_counter_test = \
        LISATS.read_traffic_signs(OUTPUT_TRAIN_TEXT_PATH, OUTPUT_TEST_TEXT_PATH, OUTPUT_TRAIN_DIR_PATH, OUTPUT_TEST_DIR_PATH)
    print_db_info(classes_counter_train, classes_counter_test)

    classes_counter_train_total = add_arrays(classes_counter_train_total, classes_counter_train)
    classes_counter_test_total = add_arrays(classes_counter_test_total, classes_counter_test)

    print('TOTAL DATASET: ')
    print_db_info(classes_counter_train_total, classes_counter_test_total)




main()
