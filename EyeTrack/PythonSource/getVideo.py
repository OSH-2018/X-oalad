import numpy as np
import cv2
import dlib

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor('./shape_predictor_68_face_landmarks.dat')
#img = cv2.imread("/Users/ziweiwu/Downloads/dataset/picture/%d.bmp"%a)
cap = cv2.VideoCapture(0)
while(True):
		# Capture frame-by-frame
	ret, img = cap.read()
		# Our operations on the frame come here
		
	img_gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
	rects = detector(img_gray, 0)
	for i in range(len(rects)):
		landmarks = np.matrix([[p.x, p.y] for p in predictor(img,rects[i]).parts()])
		for idx, point in enumerate(landmarks):
			# 68
			pos = (point[0, 0], point[0, 1])
			print(idx,pos)
			if idx == 36:
				lefteyel = pos[0]-5
			if idx == 39:
				lefteyer = pos[0]+5
			if idx == 37:
				lefteyeup = pos[1]-5
			if idx == 41:
				lefteyedown = pos[1]+5
			if idx == 42:
				righteyel = pos[0]
			if idx == 45:
				righteyer = pos[0]+5
			if idx == 44:
				righteyeup = pos[1]-5
			if idx == 47:
				righteyedown = pos[1]


	lefteye = img[lefteyeup:lefteyedown,lefteyel:lefteyer]
	cv2.imshow("lefteye",lefteye)
	cv2.imwrite('./lefteye.bmp', lefteye)
	righteye = img[righteyeup:righteyedown,righteyel:righteyer]
	cv2.imwrite('./righteye.bmp', righteye)
	cv2.imshow("righteye",righteye)
	cv2.namedWindow("img", 2)
	cv2.imshow("img", img)
	cv2.waitKey(200)
