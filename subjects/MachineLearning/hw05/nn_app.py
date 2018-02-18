import keras
from keras.preprocessing import image
from keras import optimizers
from keras.models import Sequential
from keras.layers import Dense, Activation, Flatten, Conv2D, MaxPooling2D

import time
import os
import os.path
import shutil

NUM_CLASSES = 10
IMG_SIZE = 28
INPUT_SHAPE = (IMG_SIZE, IMG_SIZE)

BATCH_SIZE = 64

IMG_DIR = './notMNIST_small'
IMG_TRAIN_DIR = './img_train'
IMG_TEST_DIR = './img_test'
IMG_VALIDATE_DIR = './img_valid'


def prepare_data():
    labels = 'A  B  C  D  E  F  G  H    I  J'.replace(' ', '')

    files = dict()
    min_len = 10 ** 5

    for label in labels:
        mk_subdir = lambda x: os.path.join(x, label)
        os.makedirs(mk_subdir(IMG_TRAIN_DIR), exist_ok=True)
        os.makedirs(mk_subdir(IMG_TEST_DIR), exist_ok=True)
        os.makedirs(mk_subdir(IMG_VALIDATE_DIR), exist_ok=True)

        files_with_label = os.listdir(mk_subdir(IMG_DIR))
        files_with_label = list(filter(lambda x: os.path.getsize(
            os.path.join(mk_subdir(IMG_DIR), x)) != 0, files_with_label))
        
        files[label] = files_with_label
        min_len = min(min_len, len(files_with_label))

    print('Minimal length is {}'.format(min_len))
    train_len = int(0.8 * min_len)
    valid_len = int(0.1 * min_len)
    test_len = min_len - train_len - valid_len
    print('Training set {}, validation set {}, test set {}'.format(
        train_len, valid_len, test_len))

    for label in labels:
        files_with_label = files[label][:min_len]
        mk_subdir = lambda x, f: os.path.join(os.path.join(x, label), f)
        
        for fname in files_with_label[:train_len]:
            shutil.copyfile(mk_subdir(IMG_DIR, fname), mk_subdir(IMG_TRAIN_DIR, fname))

        for fname in files_with_label[train_len:train_len+valid_len]:
            shutil.copyfile(mk_subdir(IMG_DIR, fname), mk_subdir(IMG_VALIDATE_DIR, fname))

        for fname in files_with_label[train_len+valid_len:]:
            shutil.copyfile(mk_subdir(IMG_DIR, fname), mk_subdir(IMG_TEST_DIR, fname))


def get_data_generators():
    # https://keras.io/preprocessing/image/
    generator = image.ImageDataGenerator()
    tsize = INPUT_SHAPE

    train_gen = generator.flow_from_directory(
        IMG_TRAIN_DIR,
        target_size=tsize,
        color_mode='grayscale',
        batch_size=BATCH_SIZE)

    valid_gen = generator.flow_from_directory(
        IMG_VALIDATE_DIR,
        target_size=tsize,
        color_mode='grayscale',
        batch_size=BATCH_SIZE)

    test_gen = generator.flow_from_directory(
        IMG_TEST_DIR,
        target_size=tsize,
        color_mode='grayscale',
        batch_size=BATCH_SIZE)

    return (train_gen, valid_gen, test_gen)


def experiment_nn_1():
    train, valid, test = get_data_generators()
    
    num_steps = 100 #1500 // BATCH_SIZE + 20
    val_steps = 20 #190 // BATCH_SIZE + 4
    test_steps = 190 // BATCH_SIZE + 1

    sample_shape = (IMG_SIZE, IMG_SIZE, 1)

    for act in ('relu', 'tanh', 'sigmoid'):
        print('Activation function:', act)
        model = Sequential()
        model.add(Flatten(input_shape=sample_shape))
        model.add(Dense(4092, activation=act))
        model.add(Dense(2048, activation=act))
        model.add(Dense(1024, activation=act))
        model.add(Dense(NUM_CLASSES, activation='softmax'))

        sgd = optimizers.SGD(lr=1e-5, decay=0.01, momentum=0.9, nesterov=True)
        model.compile(optimizer=sgd,
                loss='categorical_crossentropy',
                metrics=['accuracy'])

        hist_callback = model.fit_generator(train, steps_per_epoch=num_steps,
                epochs=10, workers=4, validation_data=valid, 
                validation_steps=val_steps)

        res = model.evaluate_generator(test, steps=test_steps, workers=1)
        print('\n'.join(map(str, hist_callback.history['val_acc'])))
        print('\t', model.metrics_names)
        print('result: ', res)
        print('\n\n')


def experiment_nn_2():
    train, valid, test = get_data_generators()
    
    num_steps = 100 # 1500 // BATCH_SIZE + 1
    val_steps = 30 # 190 // BATCH_SIZE + 1
    test_steps = 190 // BATCH_SIZE + 1

    sample_shape = (IMG_SIZE, IMG_SIZE, 1)

    for act in ('relu',):
        print('Activation function:', act)
        model = Sequential()
        model.add(Conv2D(3, (3, 3), input_shape=sample_shape))
        model.add(Flatten())
        model.add(Dense(4092, activation=act))
        model.add(Dense(2048, activation=act))
        model.add(Dense(1024, activation=act))
        model.add(Dense(NUM_CLASSES, activation='softmax'))

        sgd = optimizers.SGD(lr=1e-4, decay=0.01, momentum=0.9, nesterov=True)
        model.compile(optimizer=sgd,
                loss='categorical_crossentropy',
                metrics=['accuracy'])

        hist_callback = model.fit_generator(train, steps_per_epoch=num_steps,
                epochs=10, workers=3, validation_data=valid, 
                validation_steps=val_steps)

        res = model.evaluate_generator(test, steps=test_steps, workers=1)
        print('\n'.join(map(str, hist_callback.history['val_acc'])))
        print('\t', model.metrics_names)
        print('result: ', res)
        print('\n\n')


def experiment_nn_3():
    train, valid, test = get_data_generators()
    
    num_steps = 1500 // BATCH_SIZE + 1
    val_steps = 190 // BATCH_SIZE + 1
    test_steps = 190 // BATCH_SIZE + 1

    sample_shape = (IMG_SIZE, IMG_SIZE, 1)

    act = 'relu'
    print('Activation function:', act)
    model = Sequential()
    model.add(Conv2D(3, (3, 3), input_shape=sample_shape))
    model.add(MaxPooling2D((2, 2)))
    model.add(Conv2D(3, (3, 3)))
    model.add(MaxPooling2D((2, 2)))
    model.add(Flatten())
    model.add(Dense(4092, activation=act))
    model.add(Dense(2048, activation=act))
    model.add(Dense(1024, activation=act))
    model.add(Dense(NUM_CLASSES, activation='softmax'))

    sgd = optimizers.SGD(lr=1e-5, decay=0.0, momentum=0.9, nesterov=True)
    model.compile(optimizer=sgd,
            loss='categorical_crossentropy',
            metrics=['accuracy'])

    hist_callback = model.fit_generator(train, steps_per_epoch=num_steps,
            epochs=8, workers=3, validation_data=valid, 
            validation_steps=val_steps)

    res = model.evaluate_generator(test, steps=test_steps, workers=1)
    print('\n'.join(map(str, hist_callback.history['val_acc'])))
    print('\t', model.metrics_names)
    print('result: ', res)
    print('\n\n')


def main():
    #experiment_nn_1()
    experiment_nn_2()
    #experiment_nn_3()

if __name__ == '__main__':
    #prepare_data()
    main()
