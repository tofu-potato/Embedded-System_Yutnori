# yutpan.py

from Yutplaypkg.gamecomponent import GameComponent
from Yutplaypkg.setmal import SetMal
from PIL import Image
import matplotlib.pyplot as plt


class Yutpan(GameComponent):
    def __init__(self, verbose=False):
        super().__init__(verbose)

        # ------------------------------------------------------------
        # (A) 원본 그대로: board_matrix, rows, cols, squares 정의
        # ------------------------------------------------------------
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

        # board_matrix에서 “□” 있는 칸 좌표(r,c)를 squares에 모으고, GOAL도 마지막에 추가
        self.squares = []
        for r in range(self.rows):
            for c in range(self.cols):
                if self.board_matrix[r][c] == "□":
                    self.squares.append((r, c))
                elif self.board_matrix[r][c] == "GOAL":
                    self.squares.append("GOAL")

        if self.verbose:
            print(f"[DEBUG] 총 squares 개수: {len(self.squares)}")
        # ------------------------------------------------------------


        # ------------------------------------------------------------
        # (B) 원본 그대로: routes 정의
        # ------------------------------------------------------------
        # 프로젝트 원본의 routes 배열을 그대로 복사해 넣으셔야 합니다.
        self.routes = {
            'default': [22, 18, 13, 9, 5, 4, 3, 2, 1, 0,
                        6, 10, 15, 19, 23, 24, 25, 26, 27, 28, 29],
            'from_5':  [5,  8,  12, 14, 16, 20, 23],
            'from_0':  [0,  7,  11, 14, 17, 21, 28, 29],
            'from_14': [14, 17, 21, 28, 29]
        }
        if self.verbose:
            print(f"[DEBUG] Route 정의: {self.routes}")
        # ------------------------------------------------------------


        # ------------------------------------------------------------
        # (C) 이미지 로드
        # ------------------------------------------------------------
        # 실제 사용하실 이미지 파일명(또는 경로)으로 바꾸세요.
        IMG_PATH = "C:/Users/Hoon/Desktop/다운로드.jfif"
        try:
            self.board_img = Image.open(IMG_PATH).convert("RGBA")
        except FileNotFoundError:
            raise FileNotFoundError(f"'{IMG_PATH}' 파일을 찾을 수 없습니다. 경로를 확인하세요.")

        # 이미지 폭·높이 픽셀 값을 읽어 둡니다.
        self.img_w, self.img_h = self.board_img.size
        if self.verbose:
            print(f"[DEBUG] 윷판 이미지 크기: {self.img_w}×{self.img_h} 픽셀")
        # ------------------------------------------------------------


        # ------------------------------------------------------------
        # (D) 한 번만 창(Figure) 생성 (비블로킹 모드)
        # ------------------------------------------------------------
        # 인터랙티브 모드를 켜서, 창을 열어둔 뒤에 매번 업데이트만 하도록 합니다.
        plt.ion()
        self.fig, self.ax = plt.subplots(figsize=(6, 6))
        # 창을 한 번 띄워놓고 배경 이미지를 먼저 그려둡니다.
        self.ax.imshow(self.board_img)
        self.ax.axis("off")
        # ------------------------------------------------------------


    # ------------------------------------------------------------
    # (E) 원본 그대로: is_all_goal 메서드
    # ------------------------------------------------------------
    def is_all_goal(self, mals):
        """
        mals: [SetMal 객체, SetMal 객체, …]
        모든 말의 is_finished 속성이 True인지 확인합니다.
        """
        return all(m.is_finished for m in mals)


    # ------------------------------------------------------------
    # (F) 원본 그대로: home_position 메서드
    # ------------------------------------------------------------
    def home_position(self):
        """
        말이 출발 지점에서 첫 이동할 때 올라갈 인덱스를 반환합니다.
        실제 로직이 다르다면, 원본 버전을 그대로 복사해 오세요.
        """
        return self.routes['default'][0]


    # ------------------------------------------------------------
    # (G) 원본 그대로: is_goal_position 메서드
    # ------------------------------------------------------------
    def is_goal_position(self, index):
        """
        index: 정수 인덱스
        해당 인덱스가 GOAL 칸인지 여부를 반환합니다.
        """
        if index < 0 or index >= len(self.squares):
            return False
        return (self.squares[index] == "GOAL")


    # ------------------------------------------------------------
    # (H) 원본 그대로: go_home 메서드
    # ------------------------------------------------------------
    def go_home(self, mal):
        """
        mal: SetMal 객체
        출발 전 상태로 되돌릴 때 호출됩니다.
        position = -1, stacked = False, is_stack_leader = False, is_finished = False
        """
        mal.position = -1
        mal.stacked = False
        mal.is_stack_leader = False
        mal.is_finished = False


    # ------------------------------------------------------------
    # (I) 변경: print_board메서드 (한 번 띄운 창 위에 갱신)
    # ------------------------------------------------------------
    def print_board(self, player_mals):
        """
        player_mals: {
            "player1": [SetMal 객체1, SetMal 객체2],
            "player2": [SetMal 객체3, SetMal 객체4],
            …
        }
        이 함수를 호출할 때마다 “한 번 띄운 창 위에” 말만 갱신해서 그립니다.
        """

        # (1) 화면을 깨끗하게 지우고, 배경 이미지를 다시 그립니다.
        self.ax.clear()
        self.ax.imshow(self.board_img)
        self.ax.axis("off")

        # (2) 말 그리기: 인덱스→(r,c)→픽셀 좌표 계산 후 Circle+Text
        cell_w = self.img_w / self.cols
        cell_h = self.img_h / self.rows

        for player_name, mals in player_mals.items():
            for m in mals:
                # 시작 전(position < 0) 또는 이미 골인된(is_finished) 말은 생략
                if m.position is None or m.position < 0 or m.is_finished:
                    continue

                idx = m.position
                # 인덱스가 범위 밖이거나 GOAL일 경우 건너뜀
                if idx < 0 or idx >= len(self.squares):
                    continue
                if self.squares[idx] == "GOAL":
                    continue

                # squares[idx]가 (r,c) 형태라면 좌표를 꺼냅니다.
                pos = self.squares[idx]
                if not isinstance(pos, tuple):
                    continue
                r, c = pos

                # “셀 중심 픽셀” 계산
                x_pix = c * cell_w + (cell_w / 2)
                y_pix = r * cell_h + (cell_h / 2)

                # 이미지 범위 내에 있는지 확인 (디버그 용)
                if not (0 <= x_pix < self.img_w and 0 <= y_pix < self.img_h):
                    if self.verbose:
                        print(f"[DEBUG] 경고: ({x_pix:.1f}, {y_pix:.1f}) 범위 벗어남")
                    continue

                # (2-1) 말 기호(symbol) 얻기
                symbol = m.print_symbol()
                if symbol is None:
                    continue

                # (2-2) 말 원 색상 선택 (예: 첫 플레이어 빨강, 나머지 파랑)
                first_player = list(player_mals.keys())[0]
                circle_color = "red" if (player_name == first_player) else "blue"

                # (2-3) 원(circle) 그리기
                radius = min(cell_w, cell_h) * 0.4  # 칸 크기의 40% 정도
                circle = plt.Circle(
                    (x_pix, y_pix),
                    radius=radius,
                    facecolor=circle_color,
                    edgecolor="black",
                    linewidth=1,
                    alpha=0.8
                )
                self.ax.add_patch(circle)

                # (2-4) 원 위에 기호(text) 그리기
                self.ax.text(
                    x_pix,
                    y_pix,
                    symbol,
                    fontsize=int(min(cell_w, cell_h) * 0.3),
                    fontweight="bold",
                    color="white",
                    ha="center",
                    va="center"
                )

        # (3) 한 번 열어둔 창(Figure)을 **업데이트**합니다.
        self.fig.canvas.draw()
        self.fig.canvas.flush_events()


    # ------------------------------------------------------------
    # (J) 필요 시: 원본 select_route 로직 그대로 복사/붙여넣기
    # ------------------------------------------------------------
    # def select_route(self, current_index, yut_result):
    #     """
    #     원본 코드를 여기에 붙여넣으셔야 갈림길 로직이 정상 작동합니다.
    #     """
    #     pass
