import cv2
import numpy as np
import imutils
import math

def process_frame(frame):
    # BATAS MERAH
    lower_red = np.array([0, 134, 134])
    upper_red = np.array([179, 255, 255])

    # BATAS IJO
    lower_blue = np.array([49, 57, 118])
    upper_blue = np.array([90, 156, 255])

    # UBAH FRAME JADI HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    mask1 = cv2.inRange(hsv, lower_red, upper_red)
    mask2 = cv2.inRange(hsv, lower_blue, upper_blue)

    cnts1 = cv2.findContours(mask1, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    cnts1 = imutils.grab_contours(cnts1)

    cnts2 = cv2.findContours(mask2, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    cnts2 = imutils.grab_contours(cnts2)

    # LIST KOORDINAT PUSAT DAN WARNANYA
    b = []

    # CARI YANG MERAH
    for c in cnts1:
        area1 = cv2.contourArea(c)
        if area1 > 2000:
            # Draw the contour on the frame
            cv2.drawContours(frame, [c], -1, (0, 255, 0), 2)

            # Calculate the centroid
            M = cv2.moments(c)
            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])

            b.append(("r", cx, cy))

            # Draw the centroid on the frame
            cv2.circle(frame, (cx, cy), 4, (255, 255, 255), -1)

    # CARI YANG IJO
    for c in cnts2:
        area2 = cv2.contourArea(c)
        if area2 > 2000:
            cv2.drawContours(frame, [c], -1, (0, 255, 0), 2)

            M = cv2.moments(c)

            cx = int(M["m10"] / M["m00"])
            cy = int(M["m01"] / M["m00"])

            b.append(("g", cx, cy))

            cv2.circle(frame, (cx, cy), 4, (255, 255, 255), -1)

    # BUAT MASUKIN KOORDINAT DI SETIAP ROW
    row_one = []
    row_two = []
    row_three = []

    # KELUARIN 4 DI ROW 1
    numbers = [item[2] for item in b]
    sorted_numbers = sorted(numbers)
    four_smallest = sorted_numbers[:4]
    smallest_indices = [index for index, number in enumerate(numbers) if number in four_smallest]
    smallest_indices = sorted(smallest_indices)
    for index in reversed(smallest_indices):
        element = b.pop(index)
        row_one.append(element)

    # KELUARIN 4 DI ROW 2
    numbers = [item[2] for item in b]
    sorted_numbers = sorted(numbers)
    four_smallest = sorted_numbers[:4]
    smallest_indices = [index for index, number in enumerate(numbers) if number in four_smallest]
    smallest_indices = sorted(smallest_indices)
    for index in reversed(smallest_indices):
        element = b.pop(index)
        row_two.append(element)

    # SISA 4 KE ROW 3
    row_three = b

    rearranged = [row_one, row_two, row_three]

    # MENAMPILKAN ARRAY
    print(rearranged)

    # MENAMPILKAN MAZE
    maze = np.zeros((3, 4), dtype='U1')
    for i in range(3):
        for j in range(4):
            if (i, j) in [(0, 0), (1, 2), (2, 1)]:
                maze[i, j] = 'O'
            else:
                maze[i, j] = 'X'
    print(maze)

    # MENAMPILKAN FRAME
    cv2.imshow("Frame", frame)

# INISIALISASI KAMERA
cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

key_pressed = False
capture_frame = False

while True:
    # Read frame from the camera
    ret, frame = cap.read()

    # Display the frame
    cv2.imshow("Frame", frame)

    # Wait for key press event
    key = cv2.waitKey(1)

    # Key event handling
    if key == ord('q'):  # Press 'q' to exit
        break
    elif key == ord('c'):  # Press 'c' to capture frame and process
        capture_frame = True

    # Process the captured frame
    if capture_frame:
        process_frame(frame)
        capture_frame = False

# Release the camera and destroy windows
cap.release()
cv2.destroyAllWindows()