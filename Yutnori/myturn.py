# myturn.py

from Yutplaypkg.gamecomponent import GameComponent
from Yutplaypkg.yut import Yut

class Myturn(GameComponent):
    def __init__(self, game, yutpan, verbose=False):
        super().__init__(verbose)
        self.game = game
        self.yutpan = yutpan
        self.yut = Yut(verbose)

    def prompt_throw(self):
        tries = 0
        while True:
            user_input = input('Enter 키로 윷을 던지세요! : ')
            try:
                if user_input != '':
                    raise ValueError('Enter 키가 아닌 다른 키를 입력했습니다!')
                return True
            except ValueError as e:
                print(e)
                tries += 1
                if tries == 1:
                    print('한 번 더 잘못 던지면 낙입니다!')
                else:
                    print('낙입니다. 다음 차례로 넘어갑니다.')
                    return False

    def playmyturn(self):
        self.yut.reset_yut()

        current_player = self.game.current_player
        current_mals = self.game.players_mal_dict[current_player]
        allow_backdo = any(m.position != -1 and not m.is_finished for m in current_mals)

        if not self.prompt_throw():
            return None
        result = self.yut.throw(allow_backdo=allow_backdo)
        if result is None:
            return None

        while result in ['윷', '모']:
            print("윷/모 나왔으니 한 번 더 던지세요!")
            if not self.prompt_throw():
                return self.yut.yut_list()
            result = self.yut.throw(allow_backdo=allow_backdo)
            if result is None:
                return None

        return self.yut.yut_list()

    def extra_throw(self):
        self.yut.reset_yut()
        current_player = self.game.current_player
        current_mals = self.game.players_mal_dict[current_player]
        allow_backdo = any(m.position != -1 and not m.is_finished for m in current_mals)

        if not self.prompt_throw():
            return None
        result = self.yut.throw(allow_backdo=allow_backdo)
        if result is None:
            return None
        while result in ['윷', '모']:
            print("윷/모 나왔으니 한 번 더 던지세요!")
            if not self.prompt_throw():
                return self.yut.yut_list()
            result = self.yut.throw(allow_backdo=allow_backdo)
            if result is None:
                return None
        return self.yut.yut_list()

    def choose_result(self, results, allow_backdo):
        available_results = results.copy()
        if not allow_backdo and '빽도' in available_results:
            available_results.remove('빽도')

        while True:
            print("현재 사용 가능한 윷 결과:", available_results)
            choice = input("사용할 윷 결과를 입력하세요(도/개/걸/윷/모/빽도): ").strip()
            if choice in available_results:
                available_results.remove(choice)
                results.remove(choice)
                return choice
            else:
                print("해당 윷 결과는 사용 가능한 윷 결과 목록에 없습니다. 다시 입력하세요.")

    def turn_process(self):
        initial_results = self.playmyturn()
        if initial_results is None or len(initial_results) == 0:
            return

        results = initial_results[:]
        current_player = self.game.current_player
        current_mals = self.game.players_mal_dict[current_player]
        other_player = self.game.player1 if current_player == self.game.player2 else self.game.player2
        other_mals = self.game.players_mal_dict[other_player]

        allow_backdo = any(m.position != -1 and not m.is_finished for m in current_mals)

        while len(results) > 0:
            selected_result = self.choose_result(results, allow_backdo)

            stack_leader = [m for m in current_mals if m.position != -1 and m.stacked and m.is_stack_leader and not m.is_finished]
            alive_mals = [m for m in current_mals if (m.position != -1 and not m.is_finished) or (m.position == -1 and not m.is_finished and not m.stacked and m.is_stack_leader)]

            if stack_leader:
                selected_mal = stack_leader[0]
            else:
                if len(alive_mals) == 1:
                    selected_mal = alive_mals[0]
                else:
                    if selected_result == '빽도':
                        selectable = [m for m in current_mals if m.position != -1 and not m.is_finished]
                        if len(selectable) == 0:
                            print('이동 가능한 말이 없습니다. 다음 차례로 넘어갑니다.')
                            return
                        print("현재 플레이어 말 상태(빽도 - 보드 위 말만 선택 가능):")
                        for idx, mm in enumerate(current_mals):
                            if mm in selectable:
                                pos_str = f"인덱스 {mm.position}"
                                print(f"{idx+1}. {mm.mal} ({pos_str})")
                        while True:
                            try:
                                choice = input("이동시킬 말 번호를 입력하세요(빽도): ")
                                if not choice.isdigit():
                                    raise ValueError("숫자를 입력하세요.")
                                c_idx = int(choice) - 1
                                if c_idx < 0 or c_idx >= len(current_mals):
                                    raise ValueError("잘못된 말 번호.")
                                selected_mal = current_mals[c_idx]
                                if selected_mal not in selectable:
                                    raise ValueError("빽도는 보드 위에 있는 말만 선택할 수 있습니다.")
                                break
                            except ValueError as e:
                                print(e)
                    else:
                        selectable = [m for m in current_mals if m.position != -1 and not m.is_finished]
                        not_started = [m for m in current_mals if m.position == -1 and not m.is_finished]

                        if len(selectable) == 0 and len(not_started) == 1:
                            selected_mal = not_started[0]
                        else:
                            print("현재 플레이어 말 상태:")
                            for idx, mm in enumerate(current_mals):
                                if mm.is_finished:
                                    continue
                                pos_str = "시작 전" if mm.position == -1 else f"인덱스 {mm.position}"
                                print(f"{idx+1}. {mm.mal} ({pos_str})")
                            while True:
                                try:
                                    choice = input("이동시킬 말 번호를 입력하세요: ")
                                    if not choice.isdigit():
                                        raise ValueError("숫자를 입력하세요.")
                                    c_idx = int(choice) - 1
                                    if c_idx < 0 or c_idx >= len(current_mals):
                                        raise ValueError("잘못된 말 번호.")
                                    selected_mal = current_mals[c_idx]
                                    if selected_mal.is_finished:
                                        raise ValueError("이미 골인한 말은 선택할 수 없습니다.")
                                    break
                                except ValueError as e:
                                    print(e)

            if selected_mal.is_finished:
                print(f"선택한 말 '{selected_mal.mal}'은 이미 골인했습니다. 다른 말을 선택하세요.")
                continue

            steps = selected_mal.mal_move_value(selected_result)

            if selected_result == '빽도' and all(m.position == -1 or m.is_finished for m in current_mals):
                print('이동 가능한 말이 없습니다. 다음 차례로 넘어갑니다.')
                return

            is_goal = selected_mal.move(steps, self.yutpan, game=self.game, yut_result=selected_result)

            caught = selected_mal.mal_catch(other_mal_list=other_mals, game=self.game)
            if caught:
                extra_res = self.extra_throw()
                if extra_res is None:
                    return
                else:
                    results.extend(extra_res)

            selected_mal.mal_stack(own_mal_list=current_mals)
            self.yutpan.print_board(self.game.players_mal_dict)

            if self.yutpan.is_all_goal(current_mals):
                print(f"{current_player}님이 모든 말을 골인시켰습니다! 승리!")
                return "WIN"

        return "CONTINUE"
