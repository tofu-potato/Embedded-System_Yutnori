# player.py

from Yutplaypkg.gamecomponent import GameComponent
from Yutplaypkg.mal import Mal

class Player(GameComponent):
    def __init__(self, name, mal_symbols, verbose=False):
        super().__init__(verbose)
        self.name = name
        self.mal_symbols = mal_symbols
        self.mals = []
        
    def get_active_mals(self):
        return [mal for mal in self.mals if not mal.is_finished]
