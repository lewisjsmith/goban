import cv2
import numpy as np

original_img = cv2.imread("training/board_20260618-121132.jpg")

# Write permissions
img = original_img.copy()

# Grayscale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

cv2.namedWindow('img')

cv2.createTrackbar('blur_kernel_size', 'img', 0, 5, lambda x: None)
def map_blur_kernel_size(pos):
    return pos * 2 + 1

cv2.createTrackbar('adaptive_threshold_block_size', 'img', 0, 4,lambda x: None)
def map_adaptive_threshold_block_size(pos):
    return max(3, pos * 2 + 3)

cv2.createTrackbar('adaptive_threshold_constant', 'img', 0, 30,lambda x: None)
def map_adaptive_threshold_constant(pos):
    return pos - 5

cv2.createTrackbar('dp_epsilon', 'img', 0, 100, lambda x: None)
def map_dp_epsilon(pos, perimeter):
    return (pos / 1000)*perimeter

while True:
    img_copy = img.copy()

    # Blur
    blur_pos = cv2.getTrackbarPos('blur_kernel_size', 'img')
    blur_pos = map_blur_kernel_size(blur_pos)
    blur = cv2.GaussianBlur(gray, (blur_pos, blur_pos), 0)

    # Threshold
    tbl_pos = cv2.getTrackbarPos('adaptive_threshold_block_size', 'img')
    tbl_pos = map_adaptive_threshold_block_size(tbl_pos)

    tbc_pos = cv2.getTrackbarPos('adaptive_threshold_constant', 'img')
    tbc_pos = map_adaptive_threshold_constant(tbc_pos)

    threshold = cv2.adaptiveThreshold(blur, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, tbl_pos, tbc_pos)

    # Edge detection, apertureSize 7 is also sufficient
    edges = cv2.Canny(threshold, 100, 200, apertureSize=5)
    
    # Contours
    contours, hierarchy = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    if contours:
        max_contour = max(contours, key=cv2.contourArea)

        if cv2.contourArea(max_contour) > 0:

            # DP contour simplification
            perimeter = cv2.arcLength(max_contour, True)
            dpe_pos = cv2.getTrackbarPos('dp_epsilon', 'img')
            epsilon = map_dp_epsilon(dpe_pos, perimeter)
            simple_polygon = cv2.approxPolyDP(max_contour, epsilon, True)

            cv2.drawContours(img_copy, [simple_polygon], -1, (0, 0, 255), 3)

    cv2.imshow('img', img_copy)

    k = cv2.waitKey(30) & 0xFF
    if k == 27:
        cv2.destroyAllWindows()
        break