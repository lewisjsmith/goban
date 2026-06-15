from dotenv import load_dotenv
import os

import paramiko
import cv2
import time

# import threading
# import queue

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

# input_queue = queue.Queue()

# Thread to register input, needs to loop or will just run once
# def user_input():
#     while True:
#         # strip removes whitespaces
#         user_in = input("> ").strip().lower()
#         input_queue.put(user_in)

# Create the thread and then start it
# thread = threading.Thread(target=user_input, daemon=True)
# thread.start()

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

    # stdin.read() is blocking and isn't checked in this instance
    stdin, stdout, stderr = ssh.exec_command(cmd)

    # Start receiving udp stream 
    cam = cv2.VideoCapture(streamAddress)

    if not cam.isOpened():
        raise RuntimeError("Failed to open video stream")

    # action = None

    # # If successful
    # while True:
    #     try:
    #         action = input_queue.get_nowait()
    #         if action == "quit":
    #             break
    #     except queue.Empty:
    #         pass

    #     # Read frame
    #     ret, frame = cam.read()

    #     if ret:
    #         if(action == "capture"):
    #             # Save photo
    #             timestr = time.strftime("%Y%m%d-%H%M%S")
    #             filename = f'board_{timestr}.jpg'
    #             cv2.imwrite(filename, frame)
    #             print(f"Captured: {filename}")

    #             action = None

    #         # Update video stream
    #         cv2.imshow("test_cam", frame)

    #     cv2.waitKey(1)

    while True:
        ret, frame = cam.read()

        if not ret:
            print("Failed to read frame")
            # stops CPU spinning at max
            time.sleep(0.1)
            continue

        cv2.imshow("test_cam", frame)

        # this bitwise operator "&" applies the 0xFF mask to keep the lowest 8 digits (one hex being 4 digits 0-15)
        # 0xFF == 0x000000FF
        key = cv2.waitKey(1) & 0xFF

        if key == ord(' '):
            timestr = time.strftime("%Y%m%d-%H%M%S")
            filename = f"board_{timestr}.jpg"
            cv2.imwrite(filename, frame)
            print(f"Captured: {filename}")

        elif key == ord('q'):
            break

# Clean-up
finally:
    if cam is not None:
        cam.release()
    cv2.destroyAllWindows()
    try:
        stdin.close()
        stdout.close()
        stderr.close()
    except:
        pass
    ssh.close()
    print("Done.")

# vlc udp://@:port :demux=h264
