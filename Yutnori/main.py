# main.py

from Yutplaypkg import Gamestart, Yutpan, Myturn

def main():
    verbose = False  # 디버그 출력을 원하지 않으면 False로 설정
    game = Gamestart(verbose)
    game.set_mal()
    yutpan = Yutpan(verbose)

    print('윷놀이를 시작합니다!')
    print('(참고 : 윷판에 해당하는 인덱스는 좌측 상단부터 우측 하단 순서로 실행됩니다!)')

    while True:
        print(f"\n{game.current_player}님의 차례입니다.")
        yutpan.print_board(game.players_mal_dict)

        turn = Myturn(game, yutpan, verbose)
        result = turn.turn_process()

        if result == "WIN":
            print(f"축하합니다 {game.current_player}님! 게임에서 승리하셨습니다!")
            break
        elif result in [None, "CONTINUE"]:
            game.switch_player()

if __name__ == "__main__":
    main()
