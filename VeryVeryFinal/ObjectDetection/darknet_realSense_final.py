
import ctypes
import random
import os
import cv2
import time
import darknet
import argparse
from threading import Thread, enumerate
#from threading import Thread
import threading
from queue import Queue
import numpy as np
import pandas as pd
import copy
import math


# 추가
import pyrealsense2 as rs
import pyzbar.pyzbar as pyzbar
DEPTH_WIDTH=848
DEPTH_HEIGHT=480

# default값 수정
def parser():
    parser = argparse.ArgumentParser(description="YOLO Object Detection")
    parser.add_argument("--input", type=str, default=0,
                        help="video source. If empty, uses webcam 0 stream")
    parser.add_argument("--out_filename", type=str, default="",
                        help="inference video name. Not saved if empty")
    parser.add_argument("--weights", default="yolov4-custom-bit_final.weights",
                        help="yolo weights path")
    parser.add_argument("--dont_show", action='store_true',
                        help="windown inference display. For headless systems")
    parser.add_argument("--ext_output", action='store_true',
                        help="display bbox coordinates of detected objects")
    parser.add_argument("--config_file", default="./cfg/yolov4-custom-bit.cfg",
                        help="path to config file")
    parser.add_argument("--data_file", default="./cfg/bit.data",
                        help="path to data file")
    parser.add_argument("--thresh", type=float, default=.25,
                        help="remove detections with confidence below this value")
    return parser.parse_args()

###############################################3
class thread_with_exception(threading.Thread):
    def __init__(self, name):
        threading.Thread.__init__(self)
        self.name = name

    def run(self):

        # target function of the thread class
        try:
            while True:
                print('running ' + self.name)
        finally:
            print('ended')

    def get_id(self):

        # returns id of the respective thread
        if hasattr(self, '_thread_id'):
            return self._thread_id
        for id, thread in threading._active.items():
            if thread is self:
                return id

    def raise_exception(self):
        thread_id = self.get_id()
        res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id,
                                                         ctypes.py_object(SystemExit))
        if res > 1:
            ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 0)
            print('Exception raise failure')

###############################################3


def str2int(video_path):
    """
    argparse returns and string althout webcam uses int (0, 1 ...)
    Cast to int if needed
    """
    try:
        return int(video_path)
    except ValueError:
        return video_path


def check_arguments_errors(args):
    assert 0 < args.thresh < 1, "Threshold should be a float between zero and one (non-inclusive)"
    if not os.path.exists(args.config_file):
        raise(ValueError("Invalid config path {}".format(os.path.abspath(args.config_file))))
    if not os.path.exists(args.weights):
        raise(ValueError("Invalid weight path {}".format(os.path.abspath(args.weights))))
    if not os.path.exists(args.data_file):
        raise(ValueError("Invalid data file path {}".format(os.path.abspath(args.data_file))))
    if str2int(args.input) == str and not os.path.exists(args.input):
        raise(ValueError("Invalid video path {}".format(os.path.abspath(args.input))))


def set_saved_video(output_video, size):
    fourcc = cv2.VideoWriter_fourcc(*"MJPG")
    #fps = int(input_video.get(cv2.CAP_PROP_FPS))
    fps = 30
    video = cv2.VideoWriter(output_video, fourcc, fps, size)
    return video


X_OFFSET = 35
Y_OFFSET = 15


F_SIZE = 45
def SearchArea_MakeMask(srcImg):
    global erosionColor

    # 침식과 팽창 연산을 통해 노이즈를 제거한다.
    k = cv2.getStructuringElement(cv2.MORPH_RECT, (F_SIZE, F_SIZE))
    erosion = cv2.erode(srcImg, k)
    dst = cv2.dilate(erosion, k)

    ret, thr = cv2.threshold(dst, 0, 255, cv2.THRESH_OTSU)

    contours, _ = cv2.findContours(thr, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    erosionColor = np.dstack((dst, dst, dst))  # depth image is 1 channel, color is 3 channels
    newMask = np.full((480, 848, 3), (0, 0, 0), dtype=np.uint8)

    x = y = w = h = 0
    for cont in contours:
        approx = cv2.approxPolyDP(cont, cv2.arcLength(cont, True) * 0.02, True)
        vtc = len(approx)

        # contour를 둘러싼 박스의 정보를 얻어와서 박스를 그리고 마스크를 생성한다.
        x, y, w, h = cv2.boundingRect(cont)
        x = x - X_OFFSET
        y = y - Y_OFFSET
        w = w + X_OFFSET * 2
        h = h + Y_OFFSET * 2
        pt1 = (x, y)
        pt2 = (x + w, y + h)

        if (4 < vtc <10):
            cv2.rectangle(erosionColor, pt1, pt2, (0, 255, 0), 2)
            cv2.rectangle(newMask, pt1, pt2, (255, 255, 255), -1)

        # elif vtc == 6:
        #     cv2.rectangle(erosionColor, pt1, pt2, (0, 255, 0), 2)
        #     cv2.rectangle(newMask, pt1, pt2, (255, 255, 255), -1)

    return erosionColor, newMask


def excuteMask(srcImg, maskImg):
    maskInv = cv2.bitwise_not(maskImg)

    # 삽입할 이미지의 row, col, channel정보
    rows, cols, channels = maskInv.shape

    # 대상 이미지에서 삽입할 이미지의 영역을 추출
    roi = srcImg[0:rows, 0:cols]

    #mask를 만들기 위해서 img1을 gray로 변경후 binary image로 전환
    #mask는 logo부분이 흰색(255), 바탕은 검은색(0)
    #mask_inv는 logo부분이 검은색(0), 바탕은 흰색(255)

    img2gray = cv2.cvtColor(maskInv, cv2.COLOR_BGR2GRAY)
    ret, mask = cv2.threshold(img2gray, 128, 255, cv2.THRESH_BINARY)
    mask_inv = cv2.bitwise_not(mask)

    #bitwise_and 연산자는 둘다 0이 아닌 경우만 값을 통과 시킴.
    #즉 mask가 검정색이 아닌 경우만 통과가 되기때문에 mask영역 이외는 모두 제거됨.
    #아래 img1_fg의 경우는 bg가 제거 되고 fg(logo부분)만 남게 됨.
    #img2_bg는 roi영역에서 logo부분이 제거되고 bg만 남게 됨.
    img1_fg = cv2.bitwise_and(maskInv, maskInv, mask=mask)
    img2_bg = cv2.bitwise_and(roi, roi, mask=mask_inv)

    return img2_bg


# def video_capture(frame_queue, darknet_image_queue):
#     while cap.isOpened():
#         ret, frame = cap.read()
#         if not ret:
#             break
#         frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
#         frame_resized = cv2.resize(frame_rgb, (width, height),
#                                    interpolation=cv2.INTER_LINEAR)
#         frame_queue.put(frame_resized)
#         img_for_detect = darknet.make_image(width, height, 3)
#         darknet.copy_image_from_bytes(img_for_detect, frame_resized.tobytes())
#         darknet_image_queue.put(img_for_detect)
#     cap.release()

def drawing(frame_queue, detections_queue, fps_queue):
    global resultImage, resultFlag, limitTime,place
    #resultFlag=False
    random.seed(3)  # deterministic bbox colors
    #video = set_saved_video(args.out_filename, (width, height))
    #while cap.isOpened():
    i=0
    t = threading.currentThread()
    while getattr(t, "do_run", True):

        if darknet_image_queue.empty():
            continue

        print("drawing")
        frame_resized = frame_queue.get()
        detections = detections_queue.get()
        fps = fps_queue.get()
        if frame_resized is not None:
            image = darknet.draw_boxes(detections, frame_resized, class_colors)
            resultImage = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
            text = str(detections[0][0]) + ': ' + str(total) + 'EA'
            if place == 'R-3':
                cv2.putText(resultImage, text, (50, 400), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2, cv2.LINE_AA)
            else:
                cv2.putText(resultImage, text, (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2, cv2.LINE_AA)
            resultFlag = True

            # if args.out_filename is not None:
            #    video.write(image)
            # if not args.dont_show:
            #     cv2.imshow('Inference', image)
            # if cv2.waitKey(fps) == 27:
            #     break
        i+=1
    #cap.release()
    #video.release()
    cv2.destroyAllWindows()





def realSense_capture(frame_queue, darknet_image_queue,depth_frame_queue):
    global pipeline, config, colorizer
    global clipping_distance_min, clipping_distance_max
    global SHELF_DEPTH, MAX_DIST, MIN_DIST
    global erosionColor, color_image,depth_image
    global resultImage, resultFlag, limitTime
    global place
    resultFlag = False
    SHELF_DEPTH = 0.45
    MIN_DIST = 0.80
    MAX_DIST = MIN_DIST + SHELF_DEPTH
    # depth값에서 hole발생시 사용하는 필터 설정
    hole_filling = rs.hole_filling_filter()
    threshold_filling = rs.threshold_filter(MIN_DIST, MAX_DIST)

    align_to = rs.stream.color
    align = rs.align(align_to)
    i = 0
    object_min_distance = 0.7
    object_depth=0.18
    max_distance = 1 + SHELF_DEPTH
    #add
    # darknet_image = darknet_image_queue.get()

    try:

        t = threading.currentThread()
        while getattr(t, "do_run", True):

            print("realSense_capture")

            # Wait for a coherent pair of frames: depth and color
            frames = pipeline.wait_for_frames()
            aligned_frames = align.process(frames)

            color_frame = frames.get_color_frame()
            aligned_depth_frame = aligned_frames.get_depth_frame()
            filtered = hole_filling.process(aligned_depth_frame)
            filtered = threshold_filling.process(filtered)
            # if not aligned_depth_frame or not color_frame:
            #     continue

            # Convert images to numpy arrays
            depth_image = np.asanyarray(filtered.get_data())
            color_image = np.asanyarray(color_frame.get_data())
            #print(color_image.shape)
            ########################
            # if not color_image:
            #    continue
            frame_rgb = cv2.cvtColor(color_image, cv2.COLOR_BGR2RGB)
            frame_resized = cv2.resize(frame_rgb, (width, height),
                                       interpolation=cv2.INTER_LINEAR)
            mask_img = np.zeros((width, height, 3), dtype=np.uint8)
            mask_img[:, :, :] = 0
            if place == 'R-1' or place == 'R-2':
                cv2.rectangle(mask_img, (110, 150), (480, 510), (255, 255, 255), -1)
            elif place == 'R-3':
                cv2.rectangle(mask_img, (110, 0), (480, 270), (255, 255, 255), -1)
            frame_resized = cv2.bitwise_and(frame_resized, mask_img)

            #print('resultFlag:{}'.format(resultFlag))

            if frame_queue.full():
                continue
            if depth_frame_queue.full():
                continue

            if darknet_image_queue.full():
                continue
            print("realSense_capture2")

            frame_queue.put(frame_resized)
            depth_frame_queue.put(aligned_depth_frame)
            img_for_detect = darknet.make_image(width, height, 3)
            darknet.copy_image_from_bytes(img_for_detect, frame_resized.tobytes())
            darknet_image_queue.put(img_for_detect)

            # print('distance:'+str(aligned_depth_frame.get_distance(10,50)))
            depth_colormap = np.asanyarray(colorizer.colorize(filtered).get_data())
            # cv2.imshow('depth',depth_colormap)

            # X = np.array(detections)

            # cv2.imshow('Test1', color_image)


            if resultFlag==True:
                # cv2.imshow('Test2', resultImage)
                #cv2.imshow('depthmap',depth_colormap)
                cv2.waitKey(30)
                # cv2.destroyAllWindows()
                # resultFlag = False

            print("realSense_capture3")
            ########################################

            # depth_image_norm = copy.deepcopy(depth_image)
            # cv2.normalize(depth_image, depth_image_norm, 0, 255, cv2.NORM_MINMAX)
            # depth_image_gray =  depth_image_norm.astype('uint8')
            #
            # tmp = np.dstack((depth_image_gray, depth_image_gray, depth_image_gray))  # depth image is 1 channel, color is 3 channels
            # # Erode 후 검색영역을 찾고 MASK를 생성한다.
            # erosionColor, newMask = SearchArea_MakeMask(depth_image_gray)
            # maskedImage = excuteMask(color_image, newMask)
            #
            # images = np.hstack((color_image, erosionColor, newMask, maskedImage))
            # cv2.namedWindow('Test1', cv2.WINDOW_NORMAL)
            # cv2.imshow('Test1', images)

            i+=1

    #  image, light
    finally:

        # Stop streaming
        pipeline.stop()
        # cv2.destroyWindow('Test2')
        #cv2.destroyWindow('depthmap')
    #    return count


def inference(darknet_image_queue, detections_queue, fps_queue,depth_frame_queue):
    global limitTime
    global total, detections
    #while cap.isOpened():
    #while True:
    i = 0
    object_min_distance = 0.7
    object_depth = 0.18
    max_distance = 1 + SHELF_DEPTH
    large_box_depth = 0.395
    hole_filling = rs.hole_filling_filter()
    threshold_filling = rs.threshold_filter(object_min_distance, max_distance)
    colorizer = rs.colorizer()
    colorizer.set_option(rs.option.visual_preset, 1)  # 0=Dynamic, 1=Fixed, 2=Near, 3=Far
    colorizer.set_option(rs.option.min_distance, object_min_distance)
    colorizer.set_option(rs.option.max_distance, max_distance)

    #t = threading.currentThread()

    t = threading.currentThread()
    while getattr(t, "do_run", True):

        if darknet_image_queue.empty():
            continue

        if depth_frame_queue.empty():
            continue

        darknet_image = darknet_image_queue.get()
        depth_frame=depth_frame_queue.get()

        prev_time = time.time()
        detections = darknet.detect_image(network, class_names, darknet_image, thresh=args.thresh)
        detections_queue.put(detections)
        print("inference")
        fps = int(1/(time.time() - prev_time))
        fps_queue.put(fps)
        # print("FPS: {}".format(fps))
        # darknet.print_detections(detections, args.ext_output)
        #centerx,centery,width,height
        # print(type(detections))
        #X=np.array(detections)
        # print(detections)
        # print('class:'+X[0,0])#class
        # print('Xcetner:' + str(int(X[0, 2][0])))
        global total
        total=0
        for i in range(0,len(detections)):
            Xcenter=detections[i][2][0]
            Ycenter=detections[i][2][1]
            X_convert = (DEPTH_WIDTH / width) * Xcenter
            Y_convert = (DEPTH_HEIGHT / height) * Ycenter
            dist_to_center = depth_frame.get_distance(int(X_convert), int(Y_convert))
            print('class:'+detections[i][0]+str(round(dist_to_center, 3)) + 'm '+' Xcenter:'+str(Xcenter)+' Ycenter:'+str(Ycenter)+' X_convert:'+str(X_convert)+' Y_convert'+str(Y_convert))
            if detections[i][0] == 'middle_box':
                if object_min_distance < dist_to_center < object_min_distance + object_depth + 0.1:
                    total += 2
                elif object_min_distance + object_depth < dist_to_center < max_distance:
                    total += 1
            if detections[i][0] == 'small_box':
                if object_min_distance < dist_to_center < object_min_distance + object_depth + 0.1:
                    total += 2
                elif object_min_distance + object_depth < dist_to_center < max_distance:
                    total += 1
            if detections[i][0] == 'large_box':
                if object_min_distance < dist_to_center < object_min_distance + large_box_depth + 0.1:
                    total += 1

        print('total:'+str(total)+'개')

        # X_convert=(DEPTH_WIDTH/width)*Xcenter
        # Y_convert=(DEPTH_HEIGHT/height)*Ycenter
        # dist_to_center = depth_frame.get_distance(int(X_convert), int(Y_convert))
        # print(str(round(dist_to_center,3))+'m')
        # print(X[0,1])#score ex)
        # print('Xcetner:'+str(int(X[0, 2][0])))#xcenter
        # print('Ycetner:'+str(int(X[0, 2][1])))#ycenter
        # for idx,value in
        # dist_to_center=depth_frame.get_distance(100,100)
        # print(dist_to_center)
        # if object_min_distance < dist_to_center < object_min_distance + object_depth + 0.1:
        #     total += 2
        # elif object_min_distance + object_depth < dist_to_center < max_distance:
        #     total += 1

        darknet.free_image(darknet_image)
        i+=1
    #cap.release()
    print("inference is Done!!!!")
    print("#######################################################")




DEBUG = True

SHELF_DEPTH = 0.45
MIN_DIST = 0.80
MAX_DIST = MIN_DIST + SHELF_DEPTH



def device_init():
    global pipeline, config, profile, colorizer
    global clipping_distance_min, clipping_distance_max
    global isOpened

    isOpened = False

    # Configure depth and color streams
    # : realSense 카메라는 pipeline을 통해 컴퓨터 비전처리 모듈과 디바이스의
    #   사용자 상호작용을 단순화 한다.
    pipeline = rs.pipeline()

    # 파이프라인을 설정하는 객체명을 config로 생성
    config = rs.config()

    # 연결된 realSense모델명을 읽어온다.
    pipeline_wrapper = rs.pipeline_wrapper(pipeline)

    pipeline_profile = config.resolve(pipeline_wrapper)
    device = pipeline_profile.get_device()
    device_product_line = str(device.get_info(rs.camera_info.product_line))

    # realSense모델이 D400인 경우
    if (device_product_line == 'D400'):
        print('RealSense {}제품이 인식되었습니다'.format(device_product_line))

        # depth센서의 해상도는 848x480 포맷은 16비트 depth값
        config.enable_stream(rs.stream.depth, 848, 480, rs.format.z16, 30)

        # 컬러 이미지 센서의 해상도는 848x480 24비트 RGB (각 8비트)
        config.enable_stream(rs.stream.color, 848, 480, rs.format.bgr8, 30)

        # pipeline을 활성화
        profile = pipeline.start(config)

        isOpen = True

        # depth센서의 객체를 생성하여 depth센서의 scale값을 확인한다.
        depth_sensor = profile.get_device().first_depth_sensor()
        depth_scale = depth_sensor.get_depth_scale()

        # 환경설정 파라메터(단위는 미터)
        # 선반의 depth, 최대측정거리, 최소측정거리를 realSense에 설정하기 위한 파라메터를 구한다.
        clipping_distance_in_meters_min = MIN_DIST
        clipping_distance_in_meters_max = MAX_DIST + SHELF_DEPTH  # unit : meter
        clipping_distance_min = clipping_distance_in_meters_min / depth_scale
        clipping_distance_max = clipping_distance_in_meters_max / depth_scale

        if DEBUG == True:
            print("*******************************************************")
            print("depth_scale={}".format(depth_scale))
            print('clipping_distance_min : {}'.format(clipping_distance_min))
            print('clipping_distance_max : {}'.format(clipping_distance_max))
            print("*******************************************************")

        # realSense카메라의 depth에 따른 colormap를 재설정하기 위한 설정
        colorizer = rs.colorizer()
        colorizer.set_option(rs.option.visual_preset, 1)  # 0=Dynamic, 1=Fixed, 2=Near, 3=Far
        colorizer.set_option(rs.option.min_distance, MIN_DIST)
        colorizer.set_option(rs.option.max_distance, MAX_DIST)

        return 0
    else:
        print('D400모델이 아닙니다.')
        return 1



def barcode_detect(floor):
    global text,place
    pipeline = rs.pipeline()
    config = rs.config()
    config.enable_stream(rs.stream.depth, 1920, 1080, rs.format.z16, 60)  # Depth stream info
    print('local floor:' + floor)
    # second floor
    if floor == 'R-2':
        x1 = 50
        y1 = 180
        x2 = 270
        y2 = 420
    # first floor
    elif floor == 'R-1':
        x1 = 50
        y1 = 320
        x2 = 270
        y2 = 570
    # third floor
    elif floor == 'R-3':
        x1 = 50
        y1 = 30
        x2 = 270
        y2 = 270
    # Start streaming
    pipeline.start()
    i = 0

    try:
        while i<10:

            # Wait for a coherent pair of frames: depth and color
            frames = pipeline.wait_for_frames()
            color_frame = frames.get_color_frame()

            if not color_frame:
                continue

            # Convert images to numpy arrays
            color_image = np.asanyarray(color_frame.get_data())

            # Convert images RGB to Gray Scale
            gray = cv2.cvtColor(color_image, cv2.COLOR_BGR2GRAY)  # 3ch -> 1ch
            _, gray = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)  # 2021_0204_add code
            # 270 420
            # images ROI
            roi = cv2.rectangle(color_image, (x1, y1), (x2, y2), (0, 255, 0), 2)
            # roi=color_image[x:2*x,y:2*y]
            # img2=roi.copy()

            decoded = pyzbar.decode(gray)

            for d in decoded:
                # 270 420
                x, y, w, h = d.rect  # 인식좌표 포인트
                if x >= x1 and y >= y1 and x + w <= x2 and y + h <= y2:  # 인식범위
                    barcode_data = d.data.decode("utf-8")
                    barcode_type = d.type

                    # cv2.rectangle(color_image, (x, y), (x + w, y + h), (0, 0, 255), 2)
                    cv2.rectangle(gray, (x, y), (x + w, y + h), (0, 0, 255), 2)  # 2021_0204 kjh add

                    text = '%s' % (barcode_data)

                    # print(text)
                    cv2.putText(color_image, text, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2, cv2.LINE_AA)
                    cv2.putText(gray, text, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 0), 2,
                                cv2.LINE_AA)  # 2021_0204 kjh add
            i += 1
            # Show images
            # cv2.imshow('gray',gray) #2021_0204 kjh add

            # cv2.namedWindow('RealSense', cv2.WINDOW_AUTOSIZE)
            #
            #
            # cv2.imshow('RealSense', color_image)
            print(i)
            # cv2.waitKey(1)
            # cv2.imshow('mask',mask_img)
            # bg_removed = cv2.bitwise_and(bg_removed, mask_img)


    #
    finally:
        # Stop streaming
        # text = 'sdfsdfd,R-1'
        # cv2.destroyWindow('RealSense')
        place = text.split((',')[0])
        place = place[1]

        print("pipeline stop")

        pipeline.stop()

def mask(color_image,depth_colormap): #input image
    #cv2.imshow('mask',mask)
    ########################################make mask Bitwise_and
    mask_img = np.zeros_like(color_image)
    mask_img = cv2.rectangle(mask_img, (170, 0), (750, 700), (255, 255, 255), -1)  # same height between object and camera
    # cv2.imshow('mask',mask_img)
    # bg_removed = cv2.bitwise_and(bg_removed, mask_img)
    color_image = cv2.bitwise_and(color_image, mask_img)
    depth_colormap = cv2.bitwise_and(depth_colormap, mask_img)
    return color_image,depth_colormap

class object():
    count=0
    width=0
    height=0
    depth=0
if __name__ == '__main__':

    video_stream=True
    class_box = ['middle_box', 'small_box', 'large_box']
    class_place = ['R-1', 'R-2', 'R-3']

    video_stream=True
    small_box=object()
    middle_box=object()
    large_box=object()

    small_box_bool = isinstance(small_box, object)
    middle_box_bool = isinstance(middle_box, object)
    large_box_bool = isinstance(large_box, object)

    queue_cnt = 3
    frame_queue = Queue(maxsize=queue_cnt)
    darknet_image_queue = Queue(maxsize=queue_cnt)
    detections_queue = Queue(maxsize=queue_cnt)
    depth_frame_queue = Queue(maxsize=queue_cnt)
    fps_queue = Queue(maxsize=queue_cnt)

    args = parser()
    check_arguments_errors(args)
    network, class_names, class_colors = darknet.load_network(
        args.config_file,
        args.data_file,
        args.weights,
        batch_size=1
    )

    os.system("ps -ef&")
    if DEBUG==True:
        print("middle_box_bool={}".format(middle_box_bool))

    class_box = ['small_box','middle_box', 'large_box']
    class_place = ['R-1', 'R-2', 'R-3']
    for list_place in  class_place:
        print('start-=====================================================')
        print('start-=====================================================')
        print('start-=====================================================')
        print('start-=====================================================')
        print('start-=====================================================')
        print('start-=====================================================')

        global place
        print(list_place)

        text=''
        barcode_detect(list_place)
        print("barcde scanning complete: "+place)


        frame_queue.queue.clear()
        darknet_image_queue.queue.clear()
        detections_queue.queue.clear()
        depth_frame_queue.queue.clear()
        fps_queue.queue.clear()

        width = darknet.network_width(network)
        height = darknet.network_height(network)
        input_path = str2int(args.input)

        print("width:{}, height:{}".format(width, height))

            # cap = cv2.VideoCapture(input_path)
            # Thread(target=video_capture, args=(frame_queue, darknet_image_queue)).start()

        setTime = 5
                # add
        ret = device_init()
        th1 = threading.Thread(target=realSense_capture, args=(frame_queue, darknet_image_queue, depth_frame_queue))
        th2 = threading.Thread(target=inference, args=(darknet_image_queue, detections_queue, fps_queue, depth_frame_queue))
        th3 = threading.Thread(target=drawing, args=(frame_queue, detections_queue, fps_queue))


        th1.start()
        th2.start()
        th3.start()
        print("Hi1")
        time.sleep(5)
        th1.do_run = False
        print("Hi2")
        th1.join()
        print("thread1 is Done")
        th2.do_run = False
        th3.do_run = False
        th2.join()
        print("thread2 is Done")
        th3.join()
        print("thread3 is Done")
        #cv2.destroyAllWindows()
        print("finish")
        print('movemovemovemove')
        if place=='R-1':
            time.sleep(32)
        elif place=='R-2':
            time.sleep(32)
