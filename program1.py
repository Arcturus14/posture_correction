# 저장된 영상 기반 각도 추출

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

# load the input video
video = r"C:\Programming\video\Usain Bolt Running Posture_1.mp4"
cap = cv2.VideoCapture(video)
fps = cap.get(cv2.CAP_PROP_FPS)

# 2. 저장할 비디오 설정 (코덱, FPS, 해상도 등)
fourcc = cv2.VideoWriter_fourcc(*'mp4v')  # 또는 'MP4V' → .mp4 파일
out = cv2.VideoWriter(r"C:\Programming\video\output2_video.mp4", fourcc, 20.0, (360, 640))  # 파일명, 코덱, FPS, 해상도

frame_idx = 0
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

            shoulder = [landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].x, landmarks[mp_pose.PoseLandmark.RIGHT_SHOULDER.value].y]
            elbow = [landmarks[mp_pose.PoseLandmark.RIGHT_ELBOW.value].x, landmarks[mp_pose.PoseLandmark.RIGHT_ELBOW.value].y]
            wrist = [landmarks[mp_pose.PoseLandmark.RIGHT_WRIST.value].x, landmarks[mp_pose.PoseLandmark.RIGHT_WRIST.value].y]

            angle =  Calculate_angle(shoulder, elbow, wrist)

            # Visualize
            cv2.putText(image, str(angle),
                        tuple(np.multiply(elbow, [360,640]).astype(int),),
                        cv2.FONT_HERSHEY_SIMPLEX,0.5, (0, 0, 0), 2, cv2.LINE_AA
                        )
        except:
            pass

        # Render detection
        mp_drawing.draw_landmarks(image, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)  # image에 landmark와 connection을 그린다

        out.write(image)
        cv2.imshow('Mediapipe feed',image)

        if not ret:     # 영상이 끝나면 자동 종료
            break

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        frame_idx += 1

cap.release()
out.release()
cv2.destroyAllWindows()
