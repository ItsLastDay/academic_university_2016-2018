#! /usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt
from scipy.misc import face as rakoon_face
from scipy.signal import gaussian


def convolve(image, kernel):
    image_convoluted = np.fft.fft2(image)
    kernel_convoluted = np.fft.fft2(kernel, s=image.shape)

    result_convoluted = image_convoluted * kernel_convoluted

    return np.fft.ifft2(result_convoluted).astype(int)


def draw_images(image, kernel, convolved):
    plt.subplot(131)
    plt.imshow(image, cmap='gray')
    plt.xticks([])
    plt.yticks([])
    plt.subplot(132)
    plt.imshow(kernel, cmap='gray')
    plt.xticks([])
    plt.yticks([])
    plt.subplot(133)
    plt.imshow(convolved, cmap='gray')
    plt.xticks([])
    plt.yticks([])
    plt.show()


if __name__ == '__main__':
    image = rakoon_face(True)

    kernel = np.outer(gaussian(10, 3), gaussian(10, 3))
    kernel /= kernel.sum()

    blurred = convolve(image, kernel)

    draw_images(image, kernel, blurred)
