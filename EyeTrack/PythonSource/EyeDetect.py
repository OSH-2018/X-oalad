import numpy as np
import cv2
import dlib

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor('./shape_predictor_68_face_landmarks.dat')


for a in range(120):
    # cv2读取图像
    img = cv2.imread("./dataset/picture/%d.bmp"%a)

    # 取灰度
    img_gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)


    # 人脸数rects
    rects = detector(img_gray, 0)
    for i in range(len(rects)):
        landmarks = np.matrix([[p.x, p.y] for p in predictor(img,rects[i]).parts()])
        for idx, point in enumerate(landmarks):
            # 68点的坐标
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


            # 利用cv2.circle给每个特征点画一个圈，共68个
            #cv2.circle(img, pos, 5, color=(0, 255, 0))
            # 利用cv2.putText输出1-68
            #font = cv2.FONT_HERSHEY_SIMPLEX
            #cv2.putText(img, str(idx+1), pos, font, 0.8, (0, 0, 255), 1,cv2.LINE_AA)


    lefteye = img[lefteyeup:lefteyedown,lefteyel:lefteyer]
    cv2.imshow("lefteye",lefteye)
    cv2.imwrite('./dataset/eyedetect/left%d.bmp'%a, lefteye)
    righteye = img[righteyeup:righteyedown,righteyel:righteyer]
    cv2.imwrite('./dataset/eyedetect/right%d.bmp'%a, righteye)
    cv2.imshow("righteye",righteye)
    cv2.namedWindow("img", 2)
    cv2.imshow("img", img)
    cv2.waitKey(10)
