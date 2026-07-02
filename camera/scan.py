from math import atan2
from pathlib import Path

import cv2
import numpy as np
import joblib

from features import featureExtraction

class CannyStrategy:
    def setup_ui(self, window_name):
        cv2.createTrackbar('Canny Edge: minimum threshold', window_name, 0, 255, lambda x: None)
        cv2.createTrackbar('Canny Edge: maximum threshold', window_name, 0, 255, lambda x: None)

    def apply(self, image, window_name):
        t_min = cv2.getTrackbarPos('Canny Edge: minimum threshold', window_name)
        t_max = cv2.getTrackbarPos('Canny Edge: maximum threshold', window_name)
        edges = cv2.Canny(image, t_min, t_max, apertureSize=5)
        return edges

class AdaptiveStrategy:
    def __init__(self):
        self.block_range = [3, 5, 7, 9, 11, 15, 21, 31, 41, 51]
        self.c_range = [-10, -5, 0, 2, 5, 10]

    def setup_ui(self, window_name):
        cv2.createTrackbar('Adaptive: block size', window_name, 0, len(self.block_range) - 1, lambda x: None)
        cv2.createTrackbar('Adaptive: C', window_name, 0, len(self.c_range) - 1, lambda x: None)
        cv2.setTrackbarPos('Adaptive: block size', window_name, 5)
        cv2.setTrackbarPos('Adaptive: C', window_name, 4)

    def apply(self, image, window_name):
        block_pos = cv2.getTrackbarPos('Adaptive: block size', window_name)
        c_pos = cv2.getTrackbarPos('Adaptive: C', window_name)
        edges = cv2.adaptiveThreshold(image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, self.block_range[block_pos], self.c_range[c_pos])
        return edges

class BoundaryDetector:
    def __init__(self, image, edge_strategy):
        self.image = image
        self.grey = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)    
        self.edge_strategy = edge_strategy
        self.window_name = "boundary"

        self.epsilon_range = [0.01, 0.1, 0.2, 0.3, 0.5, 0.7, 0.9]

    def _setup_ui(self):
        cv2.namedWindow(self.window_name)
        cv2.createTrackbar('Gaussian Blur: kernel size', self.window_name, 0, 5, lambda x: None)
        self.edge_strategy.setup_ui(self.window_name)
        cv2.createTrackbar('Douglas-Peucker: epsilon', self.window_name, 0, len(self.epsilon_range) - 1, lambda x: None)

    def run(self):
        self._setup_ui()
        show_mode = "default"

        while True:
            display_image = self.image.copy()
            current_polygon = None
            hull = None

            blur_pos = (cv2.getTrackbarPos('Gaussian Blur: kernel size', self.window_name) * 2) + 1
            blur = cv2.GaussianBlur(self.grey, (blur_pos, blur_pos), 0)

            edges = self.edge_strategy.apply(blur, self.window_name)

            contours, hierarchy = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

            if contours:
                max_contour = max(contours, key=cv2.contourArea)

                if cv2.contourArea(max_contour) > 0:
                    hull = cv2.convexHull(max_contour)
                    perimeter = cv2.arcLength(hull, True)

                    epsilon_pos = cv2.getTrackbarPos('Douglas-Peucker: epsilon', self.window_name)
                    epsilon = self.epsilon_range[epsilon_pos]*perimeter

                    simple_polygon = cv2.approxPolyDP(hull, epsilon, True)

                    colour = (0, 255, 0) if len(simple_polygon) == 4 else (0, 0, 255)
                    cv2.drawContours(display_image, [simple_polygon], -1, colour, 3)

                    if len(simple_polygon) == 4:
                        current_polygon = simple_polygon.reshape(4, 2)

            if show_mode == "default": cv2.imshow(self.window_name, display_image)
            elif show_mode == "blur": cv2.imshow(self.window_name, blur)
            elif show_mode == "edges": cv2.imshow(self.window_name, edges)
                    
            k = cv2.waitKey(30) & 0xFF
            if k in (27, ord('q')):
                cv2.destroyAllWindows()
                return []

            elif k == ord('1'): show_mode = "blur"
            elif k == ord('2'): show_mode = "edges"
            elif k == ord('3'): show_mode = "default"
            elif k == ord('s') and current_polygon is not None:
                cv2.destroyAllWindows()
                return current_polygon

def order_points(points):
    top_left = min(points, key=lambda p: p[0] + p[1])
    bottom_right = max(points, key=lambda p: p[0] + p[1])
    top_right = min(points, key=lambda p: p[0] - p[1])
    bottom_left = max(points, key=lambda p: p[0] - p[1])

    return [top_left, top_right, bottom_right, bottom_left]

# 19x19 ONLY
GRID_AREA_WIDTH = 540
SQUARE_WIDTH = int(GRID_AREA_WIDTH / 18)
TRANSLATED_GRID_AREA_WIDTH = GRID_AREA_WIDTH + SQUARE_WIDTH
AREA_OF_PLAY_WIDTH = int(GRID_AREA_WIDTH + (2*SQUARE_WIDTH))

def getAreaOfPlay(image, points):
    ordered_points = np.array(order_points(points), dtype="float32")
    new_points = np.array(([SQUARE_WIDTH, SQUARE_WIDTH], [SQUARE_WIDTH, TRANSLATED_GRID_AREA_WIDTH], [TRANSLATED_GRID_AREA_WIDTH, TRANSLATED_GRID_AREA_WIDTH], [TRANSLATED_GRID_AREA_WIDTH, SQUARE_WIDTH]), dtype="float32")

    M = cv2.getPerspectiveTransform(ordered_points, new_points)

    area_of_play = cv2.warpPerspective(image, M, (AREA_OF_PLAY_WIDTH, AREA_OF_PLAY_WIDTH))

    return area_of_play

def scanBoard(boardImage, model, size):
    board = cv2.cvtColor(boardImage, cv2.COLOR_BGR2GRAY)
    board_state = []

    for i in range(0, size):
        for j in range(0, size):

            to_scan = board[
                (i*SQUARE_WIDTH)+ int(SQUARE_WIDTH/2) : ((i+1)*SQUARE_WIDTH)+ int(SQUARE_WIDTH/2), 
                (j*SQUARE_WIDTH)+ int(SQUARE_WIDTH/2) : ((j+1)*SQUARE_WIDTH)+ int(SQUARE_WIDTH/2)]

            X = featureExtraction(to_scan)
            X = np.hstack(X)
            X = X.reshape(1, -1)

            prediction = model.predict(X)

            # Automatically encoded values are: Black = 0, Clear = 1, White = 2
            if prediction[0] == 0: board_state.append(1)
            if prediction[0] == 1: board_state.append(0)
            if prediction[0] == 2: board_state.append(2)

    return board_state

def distance(contour, pt):
    cx, cy = pt
    c = contour.reshape(-1, 2)  # flatten contour points
    return ((c[:, 0] - cx)**2 + (c[:, 1] - cy)**2).min()

def selectPoint(action, x, y, flags, state):

    if action == cv2.EVENT_LBUTTONDOWN:

        if state["point_to_fix"] is None:
            state["index"] = min(
                range(len(state["board_points"])),
                key=lambda i: distance(state["board_points"][i], (x, y)))
            state["point_to_fix"] = state["board_points"][state["index"]]

    elif action == cv2.EVENT_LBUTTONUP:

        if state["point_to_fix"] is not None:
            if state["index"] != -1:
                state["board_points"][state["index"]] = np.array([x, y])
                state["point_to_fix"] = None
                state["index"] = -1

def manual_fix(image, board_points):

    state = {
        "board_points": board_points,
        "point_to_fix": None,
        "index": -1,
    }

    cv2.namedWindow("fix")
    cv2.setMouseCallback("fix", selectPoint, state)

    while True:
        copy = image.copy()    

        cv2.drawContours(copy, [state["board_points"]], -1, (0, 255, 0), 2)

        cv2.imshow('fix', copy)
        k = cv2.waitKey(30) & 0xFF
        if k in (27, ord('q')):
            cv2.destroyAllWindows()
            break
    
    return state["board_points"]

def run_board_scan():

    MODEL_PATH = Path(__file__).resolve().parent / "random_forest_model.pkl"
    model = joblib.load(MODEL_PATH)

    IMAGE_PATH = Path(__file__).resolve().parent / "training/board_20260622-202257.jpg"
    image = cv2.imread(IMAGE_PATH)
    
    strategy = AdaptiveStrategy()
    # strategy = CannyStrategy()

    tuner = BoundaryDetector(image, strategy)
    board_points = tuner.run()

    # 2. Temporary manual fix
    board_points = manual_fix(image, board_points)

    board = getAreaOfPlay(image, board_points)

    # Preview for debugging
    # cv2.imshow('board', board)
    # k = cv2.waitKey(0) & 0xFF
    # if k in (27, ord('q')):
    #     cv2.destroyAllWindows()

    board_state = scanBoard(board, model, 19)

    return board_state

if __name__ == '__main__':
    run_board_scan()