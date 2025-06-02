# gamestart.py

from Yutplaypkg.gamecomponent import GameComponent
from Yutplaypkg.player import Player

class Gamestart(GameComponent):
    def __init__(self, verbose=False):
        super().__init__(verbose)
        self.player1 = self.get_unique_player_name(1)
        self.player2 = self.get_unique_player_name(2)
        self.players_info = {}
        self.players_mal_dict = {}
        self.current_player = self.player1

    def get_unique_player_name(self, player_number):
        while True:
            name = input(f'{player_number}번 플레이어의 이름을 입력하세요 : ').strip()
            if not name:
                print("이름을 입력해야 합니다. 다시 입력하세요.")
                continue
            if player_number == 2 and name == self.player1:
                print("플레이어1과 동일한 이름입니다. 다른 이름을 입력하세요.")
                continue
            return name

    def set_mal(self):
        while True:
            try:
                mal1 = input(f'{self.player1}님이 사용할 말을 소문자 영어 한 글자로 입력하세요 : ').strip()
                if len(mal1) != 1 or not mal1.isalpha() or not mal1.islower():
                    raise ValueError('말은 소문자 영어 한 글자로 입력해야합니다.')

                mal2 = input(f'{self.player2}님이 사용할 말을 소문자 영어 한 글자로 입력하세요 : ').strip()
                if len(mal2) != 1 or not mal2.isalpha() or not mal2.islower():
                    raise ValueError('말은 소문자 영어 한 글자로 입력해야합니다.')

                if mal2 == mal1:
                    raise ValueError('플레이어1과 플레이어2의 말 심볼이 동일합니다. 다른 심볼을 선택하세요.')

                self.players_info[self.player1] = [mal1, mal1]
                self.players_info[self.player2] = [mal2, mal2]
                break
            except ValueError as e:
                print(e)

        player1_obj = Player(self.player1, self.players_info[self.player1], self.verbose)
        player2_obj = Player(self.player2, self.players_info[self.player2], self.verbose)

        self.players_mal_dict[self.player1] = player1_obj.mals
        self.players_mal_dict[self.player2] = player2_obj.mals

        return self.players_info

    def switch_player(self):
        self.current_player = self.player1 if self.current_player == self.player2 else self.player2
