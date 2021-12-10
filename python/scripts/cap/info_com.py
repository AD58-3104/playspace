# -*- Mode: Python; indent-tabs-mode: nil; py-indent-offset: 4; tab-width: 4 -*-
# vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4:

import time
import math
import infoshare_pb2
import joystick_controll_pb2 
import socket
import sys
from enum import IntEnum, auto
class Pos2D:
    x = 0.0
    y = 0.0
    th = 0.0
    def __init__(self,X,Y,TH):
        self.x = X
        self.y = Y
        self.th = TH
        
def convertPos2D(info_protobuf):
    tmp = info_protobuf.target_pos_vec[0]
    target_pos = Pos2D(float(tmp.pos_x),float(tmp.pos_y),float(tmp.pos_theta) / 100.0)
    tmp = info_protobuf.self_pos_cf
    self_pos = Pos2D(float(tmp.position.pos_x),float(tmp.position.pos_y),float(tmp.position.pos_theta) / 100.0)
    tmp = info_protobuf.ball_gl_cf
    ball_gl = Pos2D(float(tmp.position.pos_x),float(tmp.position.pos_y),float(tmp.position.pos_theta) / 100.0)
    return  self_pos , ball_gl, target_pos


def receive_data():
    MESSAGE_SIZE = 512
    PORT = 7120
    SERVER_ADDR = ('127.0.0.1', PORT)
    sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    sock.bind(SERVER_ADDR)
    message , recv_adr = sock.recvfrom(MESSAGE_SIZE)
    data = infoshare_pb2.SharingData()
    data.ParseFromString(message)
    print(data)
    return convertPos2D(data)


def sendWalkCommand(walk_num):
    command = joystick_controll_pb2.JoystickType()
    class ACTION(IntEnum):
        WALK_RIGHT = auto()
        WALK_LEFT = auto() 
        WALK_FRONT = auto() 
        WALK_BACK = auto() 
        CANCEL = auto() 
        KICK_RIGHT = auto() 
        KICK_LEFT = auto() 
        TURN_RIGHT = auto() 
        TURN_LEFT = auto() 

    if walk_num is ACTION.WALK_FRONT:
        command.walk_step = 1
        command.walk_angle = 0
        command.walk_period = 10
        command.walk_x_stride = 15
        command.walk_y_stride = 0
    elif walk_num is ACTION.WALK_BACK:
        command.walk_step = 1
        command.walk_angle = 0
        command.walk_period = 10
        command.walk_x_stride = 15
        command.walk_y_stride = 0
    elif walk_num is ACTION.WALK_RIGHT:
        command.walk_step = 1
        command.walk_angle = 0
        command.walk_period = 10
        command.walk_x_stride = 15
        command.walk_y_stride = 0
    elif walk_num is ACTION.WALK_LEFT:
        command.walk_step = 1
        command.walk_angle = 0
        command.walk_period = 10
        command.walk_x_stride = 15
        command.walk_y_stride = 0
    elif walk_num is ACTION.WALK_FRONT:
        command.walk_step = 1
        command.walk_angle = 0
        command.walk_period = 10
        command.walk_x_stride = 15
        command.walk_y_stride = 0
    elif walk_num is ACTION.CANCEL:
        command.cancel_motion = 1
    elif walk_num is ACTION.KICK_RIGHT:
        command.motion = 30
    elif walk_num is ACTION.KICK_LEFT:
        command.motion = 31
    sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    sock.sendto(command.SerializeToString(),("127.0.0.1",joystick_controll_pb2.JoystickType.RobotPort.port))




if __name__ == '__main__':
    while True:
        self , ball , target = receive_data()
        sendWalkCommand(100)