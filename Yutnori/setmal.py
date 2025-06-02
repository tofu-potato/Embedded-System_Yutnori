# setmal.py

from Yutplaypkg.gamecomponent import GameComponent

class SetMal(GameComponent):
    def __init__(self, player, mal, verbose=False):
        super().__init__(verbose)
        self.player = player
        self.mal = mal
        self.position = -1
        self.is_finished = False
        self.stacked = False
        self.stack_mal = mal
        self.current_route = None
        self.is_stack_leader = True
        self.route_name = 'default'

    def print_symbol(self):
        if self.stacked and self.is_stack_leader:
            return self.stack_mal.upper()
        elif not self.stacked and not self.is_finished:
            return self.mal
        else:
            return None
