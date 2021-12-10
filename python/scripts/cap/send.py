# -*- Mode: Python; indent-tabs-mode: nil; py-indent-offset: 4; tab-width: 4 -*-
# vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4:

import time
import math
import infoshare_pb2
import socket
import sys

if __name__ == '__main__':
        sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        info = infoshare_pb2.SharingData()
        info.id = b"1"
        info.team_color = b"4"
        info.self_pos_cf.position.pos_x = 1200
        info.self_pos_cf.position.pos_y = 1200
        info.self_pos_cf.position.pos_theta = 1200  
        info.self_pos_cf.confidence = b"23"

        info.ball_gl_cf.position.pos_x = 1200
        info.ball_gl_cf.position.pos_y = 1200
        info.ball_gl_cf.position.pos_theta = 1200 
        info.ball_gl_cf.confidence = b"23"

        pos2d = info.target_pos_vec.add()
        pos2d.pos_x = 1200
        pos2d.pos_y = 1200
        pos2d.pos_theta = 2300
        info.target_pos_vec.append(pos2d)
        info.target_pos_vec.append(pos2d)
        info.target_pos_vec.append(pos2d)
        sock.sendto(info.SerializeToString(),("127.0.0.1",7120))