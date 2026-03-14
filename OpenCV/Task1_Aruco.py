import cv2
import numpy as np


dict_4x4 = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
dict_5x5 = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_5X5_50)
dict_6x6 = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_50)

dictionaries = [dict_4x4, dict_5x5, dict_6x6]

parameters = cv2.aruco.DetectorParameters()


camera_matrix = np.array([
    [800, 0, 320],
    [0, 800, 240],
    [0, 0, 1]
], dtype=np.float32)

dist_coeffs = np.zeros((5, 1))



marker_length = 0.05

cap = cv2.VideoCapture(0)


while True:

    ret, frame = cap.read()

    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    for dictionary in dictionaries:

        detector = cv2.aruco.ArucoDetector(dictionary, parameters)

        corners, ids, rejected = detector.detectMarkers(gray)

        if ids is not None:

            cv2.aruco.drawDetectedMarkers(frame, corners, ids)

            rvecs, tvecs, _ = cv2.aruco.estimatePoseSingleMarkers(
                corners,
                marker_length,
                camera_matrix,
                dist_coeffs
            )


            for i in range(len(ids)):

                cv2.drawFrameAxes(
                    frame,
                    camera_matrix,
                    dist_coeffs,
                    rvecs[i],
                    tvecs[i],
                    0.03
                )

                depth = tvecs[i][0][2]

                cv2.putText(
    frame,
    f"Depth:{depth:.2f}m tvec:{tvecs[i][0][0]:.2f},{tvecs[i][0][1]:.2f},{tvecs[i][0][2]:.2f} "
    f"rvec:{rvecs[i][0][0]:.2f},{rvecs[i][0][1]:.2f},{rvecs[i][0][2]:.2f}",
    (10, 40 + i * 30),
    cv2.FONT_HERSHEY_SIMPLEX,
    0.7,
    (0, 255, 0),
    2
)



    cv2.imshow("Aruco Pose Estimation", frame)


    # Press q to exit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


cap.release()
cv2.destroyAllWindows()
