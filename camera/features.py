import os
import cv2
import csv

import numpy as np

def featureExtraction(sample):

    mean, std = cv2.meanStdDev(sample)
    mean = mean.flatten()[0]/255
    std = std.flatten()[0]/255

    laplacian_var = cv2.Laplacian(sample, cv2.CV_64F).var()
    laplacian_var = laplacian_var/(255**2)

    gx = cv2.Sobel(sample, cv2.CV_32F, 1, 0, ksize=3)
    gy = cv2.Sobel(sample, cv2.CV_32F, 0, 1, ksize=3)
    mag = cv2.magnitude(gx, gy)
    mag = cv2.normalize(mag, None, 0, 255, cv2.NORM_MINMAX)
    hist = cv2.calcHist([mag.astype(np.uint8)], 
                        [0], 
                        None, 
                        [8], 
                        [0, 256])
    hist = cv2.normalize(hist, hist).flatten()

    return [mean, std, laplacian_var, *hist]

def writeToCSV(src, dest, headers, featureFn):
    with open(dest, "w", newline="") as f:
        
        writer = csv.writer(f)

        writer.writerow(headers)

        for filename in sorted(os.listdir(src)):
            filepath = os.path.join(src, filename)

            sample = cv2.imread(filepath, cv2.IMREAD_GRAYSCALE)
            
            features = featureFn(sample)

            writer.writerow([
                filename,
                *[f"{feature:.4f}" for feature in features],
                "" # Manually apply classification labels 
            ])

def main():

    headers = [
        "filename",
        "mean",
        "std",
        "laplacian_variance",
        "hist_1",
        "hist_2",
        "hist_3",
        "hist_4",
        "hist_5",
        "hist_6",
        "hist_7",
        "hist_8",
        "label"]

    writeToCSV("model", "dataset_new.csv", headers, featureExtraction)

if __name__ == "__main__":
    main()