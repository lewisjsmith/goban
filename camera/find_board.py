import cv2
import numpy as np
from math import atan2

# TODO
# 1. Train and use classification model for stone and space detection, L=look into Lab colouring
# 2. Clean up matrix logic, ratios instead of hard-coded
# 3. Look into Canny aperture size choices
# 4. Add a threshold range to contour area rather than "nax" e.g. 60% of picture area
# 5. Make Pythonic e.g. camelCase
# 6. Split into modules

def blur_pos_to_kernel_size(pos):
    return pos * 2 + 1

def pos_to_epsilon(pos, perimeter):
    epsilon_range = [0.01, 0.1, 0.2, 0.3, 0.5, 0.7, 0.9]
    return epsilon_range[pos]*perimeter

def getMappedTrackBarPos(name, imageName, fn, *args):
    pos = cv2.getTrackbarPos(name, imageName)
    return fn(pos, *args)

def manual_boundary(image):
    cv2.namedWindow('img')

    cv2.createTrackbar('Gaussian Blur: kernel size', 'img', 0, 5, lambda x: None)
    cv2.createTrackbar('Canny Edge: minimum threshold', 'img', 0, 255, lambda x: None)
    cv2.createTrackbar('Canny Edge: maximum threshold', 'img', 0, 255, lambda x: None)
    cv2.createTrackbar('Douglas-Peucker: epsilon', 'img', 0, 6, lambda x: None)

    # Grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)    

    # Image processing step views
    show_image = "default"

    while True:
        image_copy = image.copy()

        simple_polygon = None
        hull = None

        # Blur
        blur_pos = getMappedTrackBarPos('Gaussian Blur: kernel size', 'img', blur_pos_to_kernel_size)
        blur = cv2.GaussianBlur(gray, (blur_pos, blur_pos), 0)

        # Edge detection, apertureSize 7 is also sufficient
        t_min = cv2.getTrackbarPos('Canny Edge: minimum threshold', 'img')
        t_max = cv2.getTrackbarPos('Canny Edge: maximum threshold', 'img')
        edges = cv2.Canny(blur, t_min, t_max, apertureSize=5)
        
        # Contours
        contours, hierarchy = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        if contours:
            max_contour = max(contours, key=cv2.contourArea)

            if cv2.contourArea(max_contour) > 0:
                hull = cv2.convexHull(max_contour)

                # DP contour simplification
                perimeter = cv2.arcLength(hull, True)
                epsilon = getMappedTrackBarPos('Douglas-Peucker: epsilon', 'img', pos_to_epsilon, perimeter)
                simple_polygon = cv2.approxPolyDP(hull, epsilon, True)

                colour = (0, 0, 255)
                if len(simple_polygon) == 4:
                    colour = (0, 255, 0)

                cv2.drawContours(image_copy, [simple_polygon], -1, colour, 3)

        match show_image:
            case "default":
                cv2.imshow('img', image_copy)
            case "blur":
                cv2.imshow('img', blur)
            case "edges":
                cv2.imshow('img', edges)
                
        k = cv2.waitKey(30) & 0xFF

        if k in (27, ord('q')):
            cv2.destroyAllWindows()
            break

        if k == ord('1'):
            show_image = "blur"
        elif k == ord('2'):
            show_image = "edges"
        elif k == ord('3'):
            show_image = "default"
        elif k == ord('s'):
            if simple_polygon is not None and len(simple_polygon) == 4:
                cv2.destroyAllWindows()
                return simple_polygon.reshape(4, 2)

    return []

def order_points(points):
    top_left = min(points, key=lambda p: p[0] + p[1])
    bottom_right = max(points, key=lambda p: p[0] + p[1])
    top_right = min(points, key=lambda p: p[0] - p[1])
    bottom_left = max(points, key=lambda p: p[0] - p[1])

    return [top_left, top_right, bottom_right, bottom_left]
    
def getAreaOfPlay(image, points):
    # A squares width off the board edge for stones at the extremes
    # 540 / 18 = 30
    # 60 extra width each way added to accomodate
    ordered_points = np.array(order_points(points), dtype="float32")
    new_points = np.array(([30,30], [30,570], [570,570], [570,30]), dtype="float32")

    M = cv2.getPerspectiveTransform(ordered_points, new_points)
    # N = M.copy()
    # N[0,2] = N[0,2] + 30
    # N[1,2] = N[1,2] + 30
    area_of_play = cv2.warpPerspective(image, M, (600, 600))

    return area_of_play

def scanBoard(boardImage):
    # Square width
    sq = 30

    board = cv2.cvtColor(boardImage, cv2.COLOR_BGR2GRAY)

    # 5 for testing, 18 in reality)
    for i in range(0, 5):
        for j in range(0, 5):

            # ------------------------------
            # cv2.imshow('board', board[
            #     (i*sq)+15:((i+1)*sq)+15, 
            #     (j*sq)+15:((j+1)*sq)+15
            #     ])
            # ------------------------------

            to_scan = board[(i*sq)+15:((i+1)*sq)+15, (j*sq)+15:((j+1)*sq)+15]
            hist = cv2.calcHist(to_scan, [0], None, [5], [0, 256])
            print(f"Position: {i+(i*18) + j}, hist: {hist}")

            cv2.imshow('slice', to_scan)
            k = cv2.waitKey(0) & 0xFF
            if k in (27, ord('q')):
                cv2.destroyAllWindows()
                break
            else:
                cv2.destroyAllWindows()


def main():
    # image = cv2.imread("training/board_20260618-121132.jpg")
    # image = cv2.imread("training/board_20260621-202226.jpg")
    # image = cv2.imread("training/board_20260621-202617.jpg")
    image = cv2.imread("training/board_20260621-204917.jpg")
    board_points = manual_boundary(image)

    if board_points is None or len(board_points) != 4:
        print("Invalid, board not found.")
        return

    board = getAreaOfPlay(image, board_points)
    # Preview for debugging
    cv2.imshow('board', board)
    k = cv2.waitKey(0) & 0xFF
    if k in (27, ord('q')):
        cv2.destroyAllWindows()

    scanBoard(board)

if __name__ == '__main__':
    main()




