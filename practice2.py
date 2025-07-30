# 실시간 영상 기반 각도 추출

import cv2
import mediapipe as mp
import numpy as np

# Calculate Angles
def Calculate_angle(a,b,c):
    a = np.array(a)
    b = np.array(b)
    c = np.array(c)

    radians = np.arctan2(c[1] - b[1], c[0] - b[0]) - np.arctan2(a[1] - b[1], a[0] - b[0])
    angle = np.abs(radians * 180.0 / np.pi)

    if angle > 180.0:
        angle = 360 - angle

    return angle


mp_drawing = mp.solutions.drawing_utils
mp_pose = mp.solutions.pose

# 1. Make Detection
# video feed
cap = cv2.VideoCapture(0)

# Setup Mediapipe instance
with mp_pose.Pose(min_detection_confidence = 0.5, min_tracking_confidence = 0.5) as pose:  
    while cap.isOpened():
        ret, frame = cap.read()

        # Detect stuff and render
        image = cv2.cvtColor(frame,cv2.COLOR_BGR2RGB)   # recolor image to rgb
        image.flags.writeable = False

        # Make detection
        results = pose.process(image)

        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)  # recolor image to bgr

        # 2. Extract landmarks
        try:
            landmarks = results.pose_landmarks.landmark

            shoulder = [landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].x, landmarks[mp_pose.PoseLandmark.LEFT_SHOULDER.value].y]
            elbow = [landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].x, landmarks[mp_pose.PoseLandmark.LEFT_ELBOW.value].y]
            wrist = [landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].x, landmarks[mp_pose.PoseLandmark.LEFT_WRIST.value].y]

            angle =  Calculate_angle(shoulder, elbow, wrist)

            # Visualize
            cv2.putText(image, str(angle),
                        tuple(np.multiply(elbow, [640,480]).astype(int),),
                         cv2.FONT_HERSHEY_SIMPLEX,0.5, (255, 255, 255), 2, cv2.LINE_AA
                        )
            
        except:
            pass

        # Render detection
        mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)  # image에 landmark와 connection을 그린다

        cv2.imshow('Mediapipe feed',image)

        if cv2.waitKey(10) & 0xFF == ord('q'):      # q를 입력하면 화면 종료
            break

    cap.release()
    cv2.destroyAllWindows()