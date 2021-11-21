# -*- Mode: Python; indent-tabs-mode: nil; py-indent-offset: 4; tab-width: 4 -*-
# vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4:

##########################################################################
 # Copyright (c) 2019 CIT Brains All Rights Reserved.                   #
 #                                                                      #
 # @file root_network.py                                                #
 # @brief role network for soccer agent                                 #
 # @author Joshua Supratman                                             #
 # @date 2019-07-04                                                     #
##########################################################################

import actionbase.search
import actionbase.bodymotion
import kid.action.approach
import kid.action.neutralaction
import kid.action.defenderaction
from rcl import WorldState
import kid.role
from kid.common_network import WaitMode, SearchMode, FreeKickMode

class Attacker(object):
    def __init__(self, home_pos_gl, move_conf):
        self.task_type ='Compound'
        self.method_list = [Attacker.Root(move_conf),
                            Attacker.FreeKickReady(move_conf),
                            Attacker.FreeKick(home_pos_gl, move_conf)]

    class Root(list):
        def __init__(self, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return [(WorldState.K_SECONDARY_STATE_DIRECT_FREEKICK, False)]

        def subtask(self):
            return [WaitMode(),
                    actionbase.search.FindLandmarksToLocalize(),
                    actionbase.search.SearchCloseBall(),
                    kid.action.approach.ApproachBallForGankenKun(self._move_conf),
                    kid.action.approach.TurnAroundBallToTarget(self._move_conf),
                    kid.action.approach.DribbleToTarget(self._move_conf),
                    kid.action.approach.AdjustToKickPosForGankenKun(self._move_conf),
                    actionbase.bodymotion.KickBallToTarget(self._move_conf)]

    class FreeKick(list):
        def __init__(self, home_pos_gl, move_conf):
            self._move_conf = move_conf
            self.home_pos_gl = home_pos_gl

        def preconditions(self):
            return []

        def subtask(self):
            return [FreeKickMode(self.home_pos_gl, self._move_conf)]

    class FreeKickReady(list):
        def __init__(self, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return [(WorldState.K_SECONDARY_STATE_FREEZE, False),
                    (WorldState.K_SECONDARY_STATE_KICK_TEAM_OWN, True)]

        def subtask(self):
            return [actionbase.search.FindLandmarksToLocalize(),
                    actionbase.search.SearchCloseBall(),
                    kid.action.approach.ApproachBall(self._move_conf),
                    #kid.action.approach.TurnAroundBallToTarget(self._move_conf),
                    actionbase.search.TrackBall()]

class Neutral(object):
    def __init__(self, home_pos_gl, move_conf):
        self.task_type ='Compound'
        self.method_list = [Neutral.Paused(),
                            Neutral.ReadyToEnter(),
                            Neutral.Set(),
                            Neutral.FreeKick(home_pos_gl, move_conf),
                            Neutral.SideEntry(move_conf),
                            Neutral.Ready(home_pos_gl, move_conf),
                            Neutral.Root(move_conf)]

    class Set(list):
        def preconditions(self):
            return [(kid.role.Neutral.K_GAME_STATE_SET, True)]

        def subtask(self):
            return [actionbase.search.TrackBall()]

    class Ready(list):
        def __init__(self, home_pos_gl, move_conf):
            self._move_conf = move_conf
            self.home_pos_gl = home_pos_gl

        def preconditions(self):
            return [(kid.role.Neutral.K_GAME_STATE_READY, True)]

        def subtask(self):
            return [actionbase.search.FindLandmarksToLocalize(),
                    kid.action.approach.ApproachTargetPosForGankenKun(self.home_pos_gl, self._move_conf, 200),
                    actionbase.search.TrackBall()]

    class SideEntry(list):
        def __init__(self, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return [(WorldState.K_SIDE_ENTRY_STATE, True)]

        def subtask(self):
            return [kid.action.neutralaction.WalkStraight(self._move_conf)]

    class Paused(list):
        def preconditions(self):
            return [(kid.role.Neutral.K_GAME_STATE_PAUSED, True)]

        def subtask(self):
            return [actionbase.bodymotion.Idling()]

    class ReadyToEnter(list):
        def preconditions(self):
            return [(kid.role.Neutral.K_GAME_STATE_READY_TO_ENTER, True)]

        def subtask(self):
            return [actionbase.search.FindLandmarksToLocalize(),
                    actionbase.bodymotion.Idling()]

    class Root(list):
        def __init__(self, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return []

        def subtask(self):
            return [actionbase.search.FindLandmarksToLocalize(),
                    SearchMode(self._move_conf)]

    class FreeKick(list):
        def __init__(self, home_pos_gl, move_conf):
            self._move_conf = move_conf
            self.home_pos_gl = home_pos_gl

        def preconditions(self):
            return [(WorldState.K_SECONDARY_STATE_DIRECT_FREEKICK, True)]

        def subtask(self):
            return [FreeKickMode(self.home_pos_gl, self._move_conf)]

class Defender(object):
    def __init__(self, home_pos_gl, move_conf):
        self.task_type ='Compound'
        self.method_list = [Defender.FreeKick(home_pos_gl, move_conf),
                            Defender.DynamicApproach(move_conf),
                            Defender.StaticApproach(home_pos_gl, move_conf)]

    class DynamicApproach(list):
        def __init__(self, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return [(kid.role.Defender.K_OBSERVE_BALL, True), (WorldState.K_ON_OUR_FIELD, True)]

        def subtask(self):
            return [actionbase.search.FindLandmarksToLocalize(),
                    kid.action.defenderaction.MoveToDefensePos(self._move_conf),
                    kid.action.defenderaction.TrackBallForDefender()]

    class StaticApproach(list):
        def __init__(self, home_pos_gl, move_conf):
            self._move_conf = move_conf
            self.home_pos_gl = home_pos_gl

        def preconditions(self):
            return []

        def subtask(self):
            return [actionbase.search.FindLandmarksToLocalize(),
                    kid.action.defenderaction.ApproachTargetPos(self.home_pos_gl, self._move_conf),
                    actionbase.search.SearchCloseBall()]

    class FreeKick(list):
        def __init__(self, home_pos_gl, move_conf):
            self._move_conf = move_conf
            self.home_pos_gl = home_pos_gl

        def preconditions(self):
            return [(WorldState.K_SECONDARY_STATE_DIRECT_FREEKICK, True)]

        def subtask(self):
            return [FreeKickMode(self.home_pos_gl, self._move_conf)]

class FreeKick(object):
    def __init__(self, home_pos_gl, move_conf):
        self.task_type ='Compound'
        self.method_list = [FreeKick.Root(home_pos_gl, move_conf)]

    class Root(list):
        def __init__(self, home_pos_gl, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return []

        def subtask(self):
            return [actionbase.search.FindLandmarksToLocalize(),
                    kid.action.approach.ApproachTargetPos(self.home_pos_gl, self._move_conf),
                    actionbase.search.TrackBall()]

class Keeper(object):
    def __init__(self, move_conf, keeper_conf, home_pos_gl):
        self.task_type ='Compound'
        self.method_list = [Keeper.Root(home_pos_gl, move_conf)]

    class Root(list):
        def __init__(self, home_pos_gl, move_conf):
            self._move_conf = move_conf

        def preconditions(self):
            return []

        def subtask(self):
            return [actionbase.search.FindLandmarksToLocalize(),
                    kid.action.approach.ApproachTargetPos(self.home_pos_gl, self._move_conf),
                    actionbase.search.TrackBall()]


