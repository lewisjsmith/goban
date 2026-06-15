from dotenv import load_dotenv
import os

import paramiko
import cv2
import time

import threading

load_dotenv()
hostname = os.getenv("HOSTNAME")
username = os.getenv("USERNAME")
password = os.getenv("PASSWORD")
port = int(os.getenv("PORT", "22"))
streamAddress = os.getenv("STREAM_ADDRESS")

required = [
    hostname, 
    username, 
    password, 
    streamAddress
]

if not all(required):
    raise ValueError("Missing required environment variables")


# Start streaming from rpicam 
cmd = f'rpicam-vid -t 0 --width 640 --height 480 --inline --framerate 10 -o - | ffmpeg -f h264 -re -i - -f mpegts {streamAddress}'

# Create client
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

action = ""

# Thread to register input, needs to loop or will just run once
def user_input():
    global action
    while True:
        action = input("> ")

# Create the thread and then start it
thread = threading.Thread(target=user_input, daemon=True)
thread.start()

cam = None

try:
    # Make SSH connection
    ssh.connect(
        hostname=hostname,
        username=username,
        password=password,
        port=port,
        look_for_keys=False,
        allow_agent=False
    )

    stdin, stdout, stderr = ssh.exec_command(cmd)

    # Start receiving udp stream 
    cam = cv2.VideoCapture(streamAddress)

    if not cam.isOpened():
        raise RuntimeError("Failed to open video stream")

    # If successful
    while True:
        if(action == "quit"):
            break

        # Read frame
        ret, frame = cam.read()

        if ret:
            if(action == "capture"):
                # Save photo
                timestr = time.strftime("%Y%m%d-%H%M%S")
                cv2.imwrite(f'board_{timestr}.jpg', frame)
                action = ""

            # Update video stream
            cv2.imshow("test_cam", frame)
            cv2.waitKey(1)

# Clean-up
finally:
    if cam is not None:
        cam.release()
    cv2.destroyAllWindows()
    ssh.close()

# vlc udp://@:port :demux=h264
