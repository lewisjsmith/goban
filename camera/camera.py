from dotenv import load_dotenv
import os
import paramiko
import cv2
import time

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

cmd = f'rpicam-vid -t 0 --width 1920 --height 1080 --inline --framerate 10 --nopreview -o - | ffmpeg -f h264 -re -i - -f mpegts udp://{streamAddress}'

ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

cam = None

try:
    ssh.connect(
        hostname=hostname,
        username=username,
        password=password,
        port=port,
        look_for_keys=False,
        allow_agent=False
    )

    stdin, stdout, stderr = ssh.exec_command(cmd)

    cam = cv2.VideoCapture(f"udp://{streamAddress}")

    if not cam.isOpened():
        raise RuntimeError("Failed to open video stream")

    while True:
        ret, frame = cam.read()

        if not ret:
            print("Failed to read frame")
            time.sleep(0.1)
            continue

        cv2.imshow("test_cam", frame)

        key = cv2.waitKey(1) & 0xFF

        if key == ord(' '):
            timestr = time.strftime("%Y%m%d-%H%M%S")
            filename = f"board_{timestr}.jpg"
            cv2.imwrite(filename, frame)
            print(f"Captured: {filename}")

        elif key == ord('q'):
            break

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

# Currently need to kill Raspberry Pi process manually
