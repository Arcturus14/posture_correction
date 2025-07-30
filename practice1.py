from mediapipe import solutions
from mediapipe.framework.formats import landmark_pb2
import numpy as np

# 관절 그리는 함수
def draw_landmarks_on_image(rgb_image, detection_result):
    pose_landmarks_list = detection_result.pose_landmarks

    # MediaPipe는 BGR 이미지가 필요하므로 변환
    annotated_image = cv2.cvtColor(np.copy(rgb_image), cv2.COLOR_RGB2BGR)

    for idx in range(len(pose_landmarks_list)):         # 인식되는 사람 수만큼 루프
        pose_landmarks = pose_landmarks_list[idx]

        pose_landmarks_proto = landmark_pb2.NormalizedLandmarkList()    # mediapipe 내부포맷으로 변환
        pose_landmarks_proto.landmark.extend([
            landmark_pb2.NormalizedLandmark(
                x=landmark.x, y=landmark.y, z=landmark.z
            ) for landmark in pose_landmarks
        ])

        solutions.drawing_utils.draw_landmarks(                 # 관절 시각화 (관절점과 연결선을 그려줌)
            annotated_image,
            pose_landmarks_proto,
            solutions.pose.POSE_CONNECTIONS,
            solutions.drawing_styles.get_default_pose_landmarks_style()
        )

    return annotated_image


# 1. Import modules
import cv2
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision

# 2: Create PoseLandmarker object
base_options = python.BaseOptions(model_asset_path=r"C:\Programming\py_language\project\pose_landmarker_lite.task")
options = vision.PoseLandmarkerOptions(
    base_options=base_options,
    output_segmentation_masks=True,
    running_mode = vision.RunningMode.VIDEO
    )

detector = vision.PoseLandmarker.create_from_options(options)

# # 3: Load the input image
# test1 = r"C:\Programming\image\running_posture.png"
# test2 = r"C:\Programming\image\kick_posture.png"
# image = mp.Image.create_from_file(test2)

# 3-1 : load the input video
video = r"C:\Programming\video\Usain Bolt Running Posture_1.mp4"
cap = cv2.VideoCapture(video)
fps = cap.get(cv2.CAP_PROP_FPS)

# 3-2  Store the video -----
# output_path = r"C:\Programming\video\output_video.mp4"     # 저장할 파일 경로
# fourcc = cv2.VideoWriter_fourcc(*'mp4v')  # mp4 코덱
# fps = cap.get(cv2.CAP_PROP_FPS)  # 원본 영상의 FPS 유지
# frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
# frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

# out = cv2.VideoWriter(output_path, fourcc, fps, (frame_width, frame_height))
# ------------------------

frame_idx = 0
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:     # ret이 false면  영상 끝
        break

    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  
    mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=frame_rgb)
    detection_result = detector.detect_for_video(mp_image, timestamp_ms= int(frame_idx * 1000 / fps))

    annotated = draw_landmarks_on_image(frame_rgb, detection_result)

    cv2.imshow("Pose Tracking", annotated)
    # out.write(annotated)     # 현재 프레임 저장
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

    frame_idx += 1

cap.release()
# out.release()  # 파일 저장 완료
cv2.destroyAllWindows()