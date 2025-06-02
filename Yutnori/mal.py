# mal.py

from Yutplaypkg.setmal import SetMal

class Mal(SetMal):
    def __init__(self, player, mal, verbose=False):
        super().__init__(player, mal, verbose)
        self.backdo_moves = {
            0: 1,
            1: 2,
            2: 3,
            3: 4,
            4: 5,
            5: 9,
            6: 0,
            7: 0,
            8: 5,
            9: 13,
            10: 6,
            11: 7,
            12: 8,
            13: 18,
            14: 11,
            15: 10,
            16: 14,
            17: 14,
            18: 22,
            19: 15,
            20: 16,
            21: 17,
            22: 28,
            23: 19,
            24: 23,
            25: 24,
            26: 25,
            27: 26,
            28: 21}

        self.initial_moves = {
            '도': 22,
            '개': 18,
            '걸': 13,
            '윷': 9,
            '모': 5}

    def mal_move_value(self, yut_result):
        if yut_result == '도':
            return 1
        elif yut_result == '개':
            return 2
        elif yut_result == '걸':
            return 3
        elif yut_result == '윷':
            return 4
        elif yut_result == '모':
            return 5
        elif yut_result == '빽도':
            return -1
        return 0

    def select_route(self, yutpan):
        default_route = yutpan.routes['default']
        from_5 = yutpan.routes['from_5']
        from_0 = yutpan.routes['from_0']
        from_14 = yutpan.routes['from_14']

        pos = self.position

        def in_route(p, r):
            return p in r

        if pos == -1:
            self.current_route = default_route
            self.route_name = 'default'
            return

        if self.route_name == 'from_5' and pos == 23:
            if 23 in default_route:
                self.route_name = 'default'
                self.current_route = default_route
            return

        if self.route_name == 'from_0' and pos >= 29:
            if 29 in default_route:
                self.route_name = 'default'
                self.current_route = default_route
            return

        if self.route_name == 'from_14' and pos >= 29:
            if 29 in default_route:
                self.route_name = 'default'
                self.current_route = default_route
            return

        if pos == 5:
            self.route_name = 'from_5'
            self.current_route = from_5
            return
        elif pos == 0:
            self.route_name = 'from_0'
            self.current_route = from_0
            return
        elif pos == 14:
            self.route_name = 'from_14'
            self.current_route = from_14
            return

        if self.route_name == 'default':
            route = default_route
        elif self.route_name == 'from_5':
            route = from_5
        elif self.route_name == 'from_0':
            route = from_0
        elif self.route_name == 'from_14':
            route = from_14
        else:
            route = default_route

        if not in_route(pos, route):
            if in_route(pos, from_5):
                self.route_name = 'from_5'
                route = from_5
            elif in_route(pos, from_0):
                self.route_name = 'from_0'
                route = from_0
            elif in_route(pos, from_14):
                self.route_name = 'from_14'
                route = from_14
            elif in_route(pos, default_route):
                self.route_name = 'default'
                route = default_route
            else:
                self.route_name = 'default'
                route = default_route

        self.current_route = route

    def move(self, steps, yutpan, game=None, yut_result=None):
        if self.is_finished:
            if self.verbose:
                print(f"[DEBUG] {self.player}님의 말 '{self.mal}'은 이미 골인했습니다.")
            return False

        if yut_result == '빽도':
            if self.position in self.backdo_moves:
                new_pos = self.backdo_moves[self.position]
                self.position = new_pos
                self.select_route(yutpan)
                if self.verbose:
                    print(f"[DEBUG] {self.player}님의 말 '{self.mal}'이 빽도에 의해 인덱스 {new_pos}로 이동했습니다.")
                return False
            else:
                if self.verbose:
                    print(f"[DEBUG] {self.player}님의 말 '{self.mal}'은 현재 위치에서 빽도를 사용할 수 없습니다.")
                return False

        if self.position == -1 and yut_result in self.initial_moves:
            self.position = self.initial_moves[yut_result]
            self.select_route(yutpan)
            if self.verbose:
                print(f"[DEBUG] {self.player}님의 말 '{self.mal}'이 출발했습니다. 인덱스 {self.position}")
            return False

        if self.position == -1 and steps > 0:
            self.position = yutpan.routes['default'][0]
            self.select_route(yutpan)
            if self.verbose:
                print(f"[DEBUG] {self.player}님의 말 '{self.mal}'이 출발했습니다. 인덱스 {self.position}")
            steps -= 1

        while steps > 0 and not self.is_finished:
            route = self.current_route
            try:
                current_idx = route.index(self.position)
            except ValueError:
                if self.verbose:
                    print(f"Error: Position {self.position} not found in the current route.")
                return False

            new_idx = current_idx + steps
            last_idx = len(route) - 1

            if self.verbose:
                print(f"[DEBUG] {self.player}님의 말 '{self.mal}'을 {steps}칸 이동합니다. 현재 인덱스: {current_idx}, 새 인덱스: {new_idx}")

            if new_idx > last_idx:
                if self.current_route == yutpan.routes['default']:
                    self.position = -1
                    self.is_finished = True
                    print(f"{self.player}님의 말 '{self.mal}'이 골인했습니다!")

                    if self.stacked and game is not None:
                        for m in game.players_mal_dict[self.player]:
                            if m.stack_mal.upper() == self.stack_mal.upper():
                                m.is_finished = True
                                if self.verbose:
                                    print(f"[DEBUG] {self.player}님의 말 '{m.mal}'도 골인 처리되었습니다.")
                    return True
                else:
                    steps_to_end = last_idx - current_idx
                    self.position = route[last_idx]
                    steps_remaining = steps - (steps_to_end + 1)
                    self.select_route(yutpan)
                    if self.verbose:
                        print(f"[DEBUG] {self.player}님의 말 '{self.mal}'이 경로 끝에 도달하여 default 경로로 전환됩니다.")
                    steps = steps_remaining
            elif new_idx == last_idx:
                if self.current_route == yutpan.routes['default']:
                    self.position = -1
                    self.is_finished = True
                    print(f"{self.player}님의 말 '{self.mal}'이 골인했습니다!")

                    if self.stacked and game is not None:
                        for m in game.players_mal_dict[self.player]:
                            if m.stack_mal.upper() == self.stack_mal.upper():
                                m.is_finished = True
                                if self.verbose:
                                    print(f"[DEBUG] {self.player}님의 말 '{m.mal}'도 골인 처리되었습니다.")
                    return True
                else:
                    self.position = route[new_idx]
                    self.select_route(yutpan)
                    if self.verbose:
                        print(f"[DEBUG] {self.player}님의 말 '{self.mal}'이 경로 끝에 도달하여 default 경로로 전환됩니다.")
                    steps = 0
            else:
                self.position = route[new_idx]
                self.select_route(yutpan)
                if self.verbose:
                    print(f"[DEBUG] {self.player}님의 말 '{self.mal}'이 인덱스 {new_idx}로 이동했습니다.")
                steps = 0

        if self.position >= 29:
            self.position = -1
            self.is_finished = True
            print(f"{self.player}님의 말 '{self.mal}'이 골인했습니다!")

            if self.stacked and game is not None:
                for m in game.players_mal_dict[self.player]:
                    if m.stack_mal.upper() == self.stack_mal.upper():
                        m.is_finished = True
                        if self.verbose:
                            print(f"[DEBUG] {self.player}님의 말 '{m.mal}'도 골인 처리되었습니다.")
            return True

        return False

    def mal_catch(self, other_mal_list, game):
        if self.is_finished:
            return False

        caught = False
        for om in other_mal_list:
            if om.is_finished:
                continue
            if om.position == self.position and om.player != self.player:
                if self.verbose:
                    print(f"[DEBUG] {self.player}님의 말 '{self.mal}'이 {om.player}님의 말 '{om.mal}'을 잡았습니다!")
                print('말을 잡았습니다!')
                caught = True
                if om.stacked:
                    self.unstack_captured(om, game)
                else:
                    om.position = -1
        return caught

    def mal_stack(self, own_mal_list):
        if self.is_finished:
            if self.verbose:
                print(f"[DEBUG] {self.player}님의 말 '{self.mal}'은 골인되어 스택할 수 없습니다.")
            return False

        for om in own_mal_list:
            if om != self and om.player == self.player and om.position == self.position and om.position != -1 and not om.is_finished:
                if self.verbose:
                    print(f"[DEBUG] {self.player}님의 말 '{self.mal}'과 '{om.mal}'이 스택됩니다.")
                self.stacked = True
                om.stacked = True
                self.stack_mal = self.mal.upper()
                om.stack_mal = self.stack_mal
                om.position = -1
                om.is_stack_leader = False
                return True
        return False

    def unstack_captured(self, om, game):
        player = om.player
        mals = game.players_mal_dict[player]
        for m in mals:
            if m.stack_mal.upper() == om.stack_mal.upper():
                m.position = -1
                m.stacked = False
                m.is_stack_leader = True
                m.is_finished = False
                if self.verbose:
                    print(f"[DEBUG] {player}님의 말 '{m.mal}'이 캡처되어 시작 전 상태로 돌아갔습니다.")
