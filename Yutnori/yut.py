# yut.py

import numpy as np
from Yutplaypkg.gamecomponent import GameComponent

class Yut(GameComponent):
    def __init__(self, verbose=False):
        super().__init__(verbose)
        self.yut_myturn = []
        self.throw_result = None

        self.y_do = '''
┌---┐┌---┐┌---┐┌---┐
| x || x || x ||   |
|   ||   ||   ||   |
| x || x || x ||   |
|   ||   ||   ||   |
| x || x || x ||   |
└---┘└---┘└---┘└---┘
'''
        self.y_gae = '''
┌---┐┌---┐┌---┐┌---┐
| x || x ||   ||   |
|   ||   ||   ||   |
| x || x ||   ||   |
|   ||   ||   ||   |
| x || x ||   ||   |
└---┘└---┘└---┘└---┘
'''
        self.y_gul = '''
┌---┐┌---┐┌---┐┌---┐
| x ||   ||   ||   |
|   ||   ||   ||   |
| x ||   ||   ||   |
|   ||   ||   ||   |
| x ||   ||   ||   |
└---┘└---┘└---┘└---┘
'''
        self.y_yut = '''
┌---┐┌---┐┌---┐┌---┐
|   ||   ||   ||   |
|   ||   ||   ||   |
|   ||   ||   ||   |
|   ||   ||   ||   |
|   ||   ||   ||   |
└---┘└---┘└---┘└---┘
'''
        self.y_mo = '''
┌---┐┌---┐┌---┐┌---┐
| x || x || x || x |
|   ||   ||   ||   |
| x || x || x || x |
|   ||   ||   ||   |
| x || x || x || x |
└---┘└---┘└---┘└---┘
'''
        self.y_backdo = '''
┌---┐┌---┐┌---┐┌---┐
|   || x || x || x |
|   ||   ||   ||   |
| * || x || x || x |
|   ||   ||   ||   |
|   || x || x || x |
└---┘└---┘└---┘└---┘
'''

    def throw(self, allow_backdo=True):
        yut_choice = ['도', '개', '걸', '윷', '모', '빽도']
        yut_prob = [0.1875, 0.375, 0.25, 0.0625, 0.0625, 0.0625]

        if not allow_backdo:
            yut_choice = ['도', '개', '걸', '윷', '모']
            yut_prob = [0.1875, 0.375, 0.25, 0.0625, 0.0625]
            total_prob = sum(yut_prob)
            yut_prob = [p / total_prob for p in yut_prob]

        self.throw_result = np.random.choice(yut_choice, p=yut_prob)
        self.yut_myturn.append(self.throw_result)
        print(f'윷을 던져 {self.throw_result}가 나왔습니다.')

        if self.throw_result == '도':
            print(self.y_do)
        elif self.throw_result == '개':
            print(self.y_gae)
        elif self.throw_result == '걸':
            print(self.y_gul)
        elif self.throw_result == '윷':
            print(self.y_yut)
        elif self.throw_result == '모':
            print(self.y_mo)
        elif self.throw_result == '빽도':
            print(self.y_backdo)

        return self.throw_result

    def yut_list(self):
        return self.yut_myturn

    def reset_yut(self):
        self.yut_myturn = []
        self.throw_result = None
