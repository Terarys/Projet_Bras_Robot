# import the necessary packages
import numpy as np
import cv2
import subprocess
from subprocess import PIPE

def change(x):
    # print(x)
    pass

def reconnaissance(): #retourne la position de l'objet
    x = 0
    y = 0
    nbrMesureMax = 15
    nbrMesure = 0
    while nbrMesure < nbrMesureMax:
        # image 480*640*3
        ret1, img = cap.read()
        imgGry = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        #imgGry = cv2.bitwise_not(imgGry)

        max_val = cv2.getTrackbarPos('max_val', 'params')
        th = cv2.getTrackbarPos('th', 'params')
        start = cv2.getTrackbarPos('start', 'params')
        ret, thrash = cv2.threshold(imgGry, th , max_val, cv2.THRESH_BINARY_INV)
        contours , hierarchy = cv2.findContours(thrash, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        i = 0
        indexMaxArea = 0
        maxArea = 0
        for c in contours:
            area = cv2.contourArea(c)
            if(area > maxArea):
                indexMaxArea = i
                maxArea = area
            i = i+1


        if(len(contours)):
            contour = contours[indexMaxArea]
            rect = cv2.minAreaRect(contour)
            ((cX, cY), (w, h), angle) = rect


            box = cv2.boxPoints(rect)
            box = np.int0(box)
            cv2.drawContours(img, [box], 0, (0, 0, 0), 5)

            cv2.circle(img, (int(cX),int(cY)), 7, (0, 0, 255), -1)
            if(start):
                x = x + cX
                y = y + cY
                nbrMesure = nbrMesure + 1

        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break
        cv2.imshow("Shape", img)
        cv2.imshow("thrash", thrash)

    y_offset = 120
    x_offset = 320
    x = int(x/nbrMesureMax) + x_offset
    y = int(y/nbrMesureMax) + y_offset

    return (x, y, k)



# load the image and resize it to a smaller factor so that
# the shapes can be approximated better
cap = cv2.VideoCapture(6)

cv2.namedWindow("params")

cv2.createTrackbar('max_val', 'params', 255, 255, change)
cv2.createTrackbar('th', 'params', 60, 255, change) #réglage du threshold
cv2.createTrackbar('start', 'params', 0, 1, change) #Curseur pour démarrer la démo

#on place le bras dans sa position de repérage
subprocess.run(["./init-bras"])

k = 0
iteration = 0
iterationMax = 3
offsetZ = 0

#La première itération pour ajuster la position du robot pour prendre l'objet (le bras se positionne au dessus de l'objet)
#La deuxième itération pour ajuster la position du robot pour déposer l'objet (le bras se positionne au dessus de la croix)
for j in range(2):
    #On répète 3 ajustement de la position pour contrer l'effet de distortion de la lentille du capteur
    while iteration < iterationMax and k != 27:
        move = 1
        descente = 0
        depot = 0

	#On repère où est l'objet
        (posX, posY, k) = reconnaissance()

	#On lance le programme pour se calibrer (sauf si l'on a appuyer sur la touche "echap"
        if k!=27:
            ret = subprocess.run(["./calibration-bras", str(posX), str(posY), str(move), str(descente), str(depot), str(0)])

        iteration = iteration + 1

    iteration = 2
    move = 0
    descente = 1-j
    depot = j

    #On repère finalement l'objet pour venir le saisir dans la première itération
    #Pour la deuxième itération, on repère la croix pour poser l'objet dessus
    (posX, posY, k) = reconnaissance()
    if k!= 27:
        ret = subprocess.run(["./calibration-bras", str(posX - 50), str(posY + 10), str(move), str(descente), str(depot), str(offsetZ)], stderr=PIPE)
        offsetZ = int(ret.stderr)


cv2.destroyAllWindows()
