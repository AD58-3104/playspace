# -*- Mode: Python; indent-tabs-mode: nil; py-indent-offset: 4; tab-width: 4 -*-
# vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4:

##########################################################################
 # Copyright (c) 2019 CIT Brains All Rights Reserved.                   #
 #                                                                      #
 # @file common_network.py                                              #
 # @brief collection of common compoound task                           #
 # @author Joshua Supratman                                             #
 # @date 2019-07-04                                                     #
##########################################################################

import actionbase.search
import actionbase.bodymotion
import kid.action.approach
from rcl import WorldState
import kid.role

class KickMode(object):
    def __init__(self, move_conf):
        self.task_type ='Compound'
        self.method_list = [KickMode.DribbleBall(move_conf), KickMode.KickBall(move_conf)]

    class KickBall(list):
        def __init__(self, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return []

        def subtask(self):
            return [kid.action.approach.AdjustToKickpos(self._move_conf),
                    actionbase.bodymotion.KickBallToTarget(self._move_conf)]

    class DribbleBall(list):
        def __init__(self, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return [(WorldState.K_TARGET_IN_SHOT_RANGE, False)]

        def subtask(self):
            return [kid.action.approach.DribbleToTarget(self._move_conf)]

class WaitMode(object):
    def __init__(self):
        self.task_type ='Compound'
        self.method_list = [WaitMode.Root(), WaitMode.IdleKick()]

    class IdleKick(list):
        def preconditions(self):
            return []

        def subtask(self):
            return [actionbase.bodymotion.IdleKickoff()]

    class Root(list):
        def preconditions(self):
            return [(WorldState.K_PERMITTED_INTRUSION_CIRCLE, True)]

        def subtask(self):
            return []

class SearchMode(object):
    def __init__(self, move_conf):
        self.task_type ='Compound'
        self.method_list = [SearchMode.Explore(), SearchMode.Root(move_conf)]

    class Root(list):
        def __init__(self, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return []

        def subtask(self):
            return [actionbase.search.TurnAndSearchCloseBall(self._move_conf)]

    class Explore(list):
        def preconditions(self):
            return [(kid.role.Neutral.K_EXPLORE_AREA, True)]

        def subtask(self):
            return [actionbase.search.SearchFarBall()]

class FreeKickMode(object):
    def __init__(self, home_pos_gl, move_conf):
        self.task_type ='Compound'
        self.method_list = [FreeKickMode.Freeze(), FreeKickMode.Ready(home_pos_gl, move_conf)]

    class Freeze(list):
        def preconditions(self):
            return [(WorldState.K_SECONDARY_STATE_FREEZE, True)]

        def subtask(self):
            return [actionbase.search.TrackBall()]

    class Ready(list):
        def __init__(self, home_pos_gl, move_conf):
            self._move_conf = move_conf
            self.home_pos_gl = home_pos_gl

        def preconditions(self):
            return []

        def subtask(self):
            return [actionbase.search.FindLandmarksToLocalize(),
                    kid.action.approach.ApproachFreeKickTargetPos(self.home_pos_gl, self._move_conf),
                    actionbase.search.TrackBall()]


