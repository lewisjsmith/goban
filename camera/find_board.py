import cv2
import numpy as np

from math import atan2

original_img = cv2.imread("training/board_20260618-121132.jpg")

# Write permissions
img = original_img.copy()

# Grayscale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

cv2.namedWindow('img')

cv2.createTrackbar('blur_kernel_size', 'img', 0, 5, lambda x: None)
def map_blur_kernel_size(pos):
    return pos * 2 + 1

cv2.createTrackbar('dp_epsilon', 'img', 0, 100, lambda x: None)
def map_dp_epsilon(pos, perimeter):
    return (pos / 1000)*perimeter

board_points = None

while True:
    img_copy = img.copy()

    # Blur
    blur_pos = cv2.getTrackbarPos('blur_kernel_size', 'img')
    blur_pos = map_blur_kernel_size(blur_pos)
    blur = cv2.GaussianBlur(gray, (blur_pos, blur_pos), 0)

    # Edge detection, apertureSize 7 is also sufficient
    edges = cv2.Canny(blur, 100, 200, apertureSize=5)
    
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

    if k == 115 and len(simple_polygon) == 4:
        cv2.destroyAllWindows()
        board_points = simple_polygon
        cv2.destroyAllWindows()
        break

board_points = board_points.squeeze()

def order_points(points):
    top_left = min(points, key=lambda p: p[0] + p[1])
    bottom_right = max(points, key=lambda p: p[0] + p[1])
    top_right = min(points, key=lambda p: p[0] - p[1])
    bottom_left = max(points, key=lambda p: p[0] - p[1])

    return [top_left, top_right, bottom_right, bottom_left]

ordered_points = np.array(order_points(board_points), dtype="float32")
new_points = np.array(([0,0], [0,500], [500,500], [500,0]), dtype="float32")

M = cv2.getPerspectiveTransform(ordered_points, new_points)
board = cv2.warpPerspective(img, M, (500,500))



