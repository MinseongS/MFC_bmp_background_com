from tensorflow.keras.models import load_model
import matplotlib.pyplot as plt
import cv2
import tensorflow as tf
import numpy as np

gpus = tf.config.experimental.list_physical_devices('GPU')
if gpus:
  try:
    # Currently, memory growth needs to be the same across GPUs
    for gpu in gpus:
      tf.config.experimental.set_memory_growth(gpu, True)
    logical_gpus = tf.config.experimental.list_logical_devices('GPU')
    print(len(gpus), "Physical GPUs,", len(logical_gpus), "Logical GPUs")
  except RuntimeError as e:
    # Memory growth must be set before GPUs have been initialized
    print(e)
IMG_WIDTH, IMG_HEIGHT = 256, 256


def preprocess(img):
    im = np.zeros((IMG_WIDTH, IMG_HEIGHT, 3), dtype=np.uint8)

    if img.shape[0] >= img.shape[1]:
        scale = img.shape[0] / IMG_HEIGHT
        new_width = int(img.shape[1] / scale)
        diff = (IMG_WIDTH - new_width) // 2
        img = cv2.resize(img, (new_width, IMG_HEIGHT))

        im[:, diff:diff + new_width, :] = img
    else:
        scale = img.shape[1] / IMG_WIDTH
        new_height = int(img.shape[0] / scale)
        diff = (IMG_HEIGHT - new_height) // 2
        img = cv2.resize(img, (IMG_WIDTH, new_height))

        im[diff:diff + new_height, :, :] = img

    return im


def maskimg(m_InImg):
    model = load_model('C:\\Users\\subak\\PycharmProjects\\project20210517\\masking\\unet.h5')
    img_ori = cv2.cvtColor(m_InImg.copy(), cv2.COLOR_BGR2RGB)
    # input_img = m_InImg.reshape((1, 256, 256, 3)).astype(np.float32) / 255.
    img = preprocess(m_InImg)
    input_img = img.reshape((1, IMG_WIDTH, IMG_HEIGHT, 3)).astype(np.float32) / 255.
    pred = model.predict(input_img)
    THRESHOLD = 0.5
    EROSION = 1

    def postprocess(img_ori, pred):
        h, w = img_ori.shape[:2]

        mask_ori = (pred.squeeze()[:, :, 1] > THRESHOLD).astype(np.uint8)
        max_size = max(h, w)
        result_mask = cv2.resize(mask_ori, dsize=(max_size, max_size))

        if h >= w:
            diff = (max_size - w) // 2
            if diff > 0:
                result_mask = result_mask[:, diff:-diff]
        else:
            diff = (max_size - h) // 2
            if diff > 0:
                result_mask = result_mask[diff:-diff, :]

        result_mask = cv2.resize(result_mask, dsize=(w, h))

        # fill holes
        #     cv2.floodFill(result_mask, mask=np.zeros((h+2, w+2), np.uint8), seedPoint=(0, 0), newVal=255)
        #     result_mask = cv2.bitwise_not(result_mask)
        result_mask *= 255

        #     # erode image
        #     element = cv2.getStructuringElement(cv2.MORPH_RECT, (2*EROSION + 1, 2*EROSION+1), (EROSION, EROSION))
        #     result_mask = cv2.erode(result_mask, element)

        # smoothen edges
        result_mask = cv2.GaussianBlur(result_mask, ksize=(9, 9), sigmaX=5, sigmaY=5)

        return result_mask

    mask = postprocess(img_ori, pred)

    plt.figure(figsize=(8, 8))
    plt.subplot(1, 2, 1)
    plt.imshow(img_ori)
    plt.subplot(1, 2, 2)
    plt.imshow(mask)
    plt.show()
    return mask

IMG_PATH = 'C:\\Users\\subak\\PycharmProjects\\project20210517\\masking\\lebmp.bmp'
save_PATH = 'C:\\Users\\subak\\PycharmProjects\\project20210517\\masking\\lenamask.bmp'
img = cv2.imread(IMG_PATH, cv2.IMREAD_COLOR)
tempmask = maskimg(img)
cv2.imwrite(save_PATH, tempmask)
print(tempmask)

