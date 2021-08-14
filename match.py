#_*_coding:utf-8_*_
import cv2
import numpy as np

  
img_path = 'target.png'
img_rgb = cv2.imread(img_path)
img_gray = cv2.cvtColor(img_rgb, cv2.COLOR_BGR2GRAY)
template = cv2.imread('mark.png', 0)

template_h, template_w = template.shape[:2]
# print(img_gray.shape)   # (207, 225)
# print(template.shape)  # (27, 16)


res = cv2.matchTemplate(img_gray, template, cv2.TM_CCOEFF_NORMED)
threshold = 0.7
# 取匹配程度大于 80% 的坐标
loc = np.where(res >= threshold)
count=0
for pt in zip(*loc[::-1]):  # *表示可选参数
    bottom_right = (pt[0] + template_w, pt[1] + template_h)
    cv2.rectangle(img_rgb, pt, bottom_right, (0, 0, 255), 2)
    count+=1

print(count)
cv2.imshow('img_rgb', img_rgb)
cv2.waitKey(0)
cv2.destroyAllWindows()