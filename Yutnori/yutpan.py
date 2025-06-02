# yutpan.py

from Yutplaypkg.gamecomponent import GameComponent
from Yutplaypkg.setmal import SetMal

class Yutpan(GameComponent):
    def __init__(self, verbose=False):
        super().__init__(verbose)
        self.board_matrix = [
            ["□", "□", "□", " ", "□", "□", "□", " "],
            ["□", "□", " ", " ", " ", "□", "□", " "],
            ["□", " ", "□", " ", "□", " ", "□", " "],
            [" ", " ", " ", "□", " ", " ", " ", " "],
            ["□", " ", "□", " ", "□", " ", "□", " "],
            ["□", "□", " ", " ", " ", "□", "□", " "],
            ["□", "□", "□", " ", "□", "□", "□", "GOAL"]
        ]
        self.rows = 7
        self.cols = 8

        self.squares = []
        for r in range(self.rows):
            for c in range(self.cols):
                if self.board_matrix[r][c] == "□":
                    self.squares.append((r, c))
                elif self.board_matrix[r][c] == "GOAL":
                    self.squares.append("GOAL")

        if self.verbose:
            print(f"[DEBUG] 총 squares 개수: {len(self.squares)}")

        self.routes = {
            'default': [22, 18, 13, 9, 5, 4, 3, 2, 1, 0, 6, 10, 15, 19, 23, 24, 25, 26, 27, 28, 29],
            'from_5': [5, 8, 12, 14, 16, 20, 23],
            'from_0': [0, 7, 11, 14, 17, 21, 28, 29],
            'from_14': [14, 17, 21, 28, 29]
        }
        if self.verbose:
            print(f"[DEBUG] Route 정의: {self.routes}")

    def print_board(self, player_mals):
        temp_board = [row[:] for row in self.board_matrix]

        for p, mals in player_mals.items():
            for m in mals:
                if m.position != -1 and not m.is_finished:
                    if m.position == 29:
                        continue
                    if isinstance(m.position, tuple):
                        r, c = m.position
                    else:
                        if m.position >= len(self.squares) or self.squares[m.position] == "GOAL":
                            continue
                        pos = self.squares[m.position]
                        if isinstance(pos, tuple):
                            r, c = pos
                        else:
                            continue
                    symbol = m.print_symbol()
                    if symbol is not None:
                        temp_board[r][c] = symbol.upper() if m.stacked and m.is_stack_leader else symbol

        print('\n현재 윷판 상태:')
        for row in temp_board:
            print(' '.join(row))
        print()

    def is_all_goal(self, mals):
        return all(m.is_finished for m in mals)
