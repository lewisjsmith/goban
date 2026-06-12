import pygame
from pathlib import Path
import subprocess
import threading
import queue

# TODO: 
# settings data file 
# consistent use of "size" throughout range iterators
# subprocess and threads for non-blocking engine usage
# separation of functions into modules
# proper usage of global variables and passed parameters 
# currently fails for killing two groups
# clear engine command needed
# set engine board size
# add invalid status for out of bounds

class EngineWrapper:
    def __init__(self):
        self.engine_path = Path(__file__).parent.parent / "engine" / "build" / "prog"
        self.proc = subprocess.Popen(
            [str(self.engine_path)],  
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            text=True,
            bufsize=1
        )
        self.engine_messages = queue.Queue()
        threading.Thread(target=self.reader, daemon=True).start()
    
    def reader(self):
        while True:
            line = self.proc.stdout.readline()
            if not line:
                break
            self.engine_messages.put(line.strip())

    def send_command(self, command: str):
        self.proc.stdin.write(f"{command}\n")
        self.proc.stdin.flush()

    def terminate(self):
        self.send_command("quit")
        self.proc.wait()

class Context:
    def __init__(self, boardSize=9, width=1280, height=720):
        self.window = pygame.display.set_mode((width, height))
        self.boardSize = boardSize
        self.state = [0] * (boardSize ** 2)
        self.blackStoneTurn = True
        self.canvas = self._generateBoardSurface(width)
                
    def _generateBoardSurface(self, width):
        surface = pygame.Surface((width/2, width/2))

        square_width = (1/(self.boardSize - 1))*(width/2)
        border = 1

        for i in range(self.boardSize - 1):
            for j in range(self.boardSize - 1):
                outer = pygame.Rect(
                    j * square_width,
                    i * square_width,
                    square_width,
                    square_width
                )

                inner = pygame.Rect(
                    j * square_width + border,
                    i * square_width + border,
                    square_width - 2 * border,
                    square_width - 2 * border
                )

                pygame.draw.rect(surface, (0, 0, 0), outer)
                pygame.draw.rect(surface, (210, 180, 140), inner)  

        return surface

    def reset(self):
        self.state = [0] * (self.boardSize ** 2)
        self.blackStoneTurn = True
        
def paintBoard(context: Context):
    width, height = context.window.get_size()          
    context.window.blit(context.canvas, (width/4, (height - width/2)/2))

def paintStones(context: Context):
    width, height = context.window.get_size()
    boardSize = context.boardSize

    square_width = (1/(boardSize-1))*(width/2)

    board_x = width / 4
    board_y = (height - width / 2) / 2

    boarder_thickness = 2

    for i in range(boardSize):
        for j in range(boardSize):
            if context.state[i*boardSize + j] == 1:
                pygame.draw.circle(
                    context.window, 
                    (0, 0, 0), 
                    ((board_x + j * square_width), (board_y + i * square_width)), 
                    square_width/2
                )
            if context.state[i*boardSize + j] == 2:
                pygame.draw.circle(
                    context.window, 
                    (72, 72, 72), 
                    ((board_x + j * square_width), (board_y + i * square_width)), 
                    square_width/2
                )
                pygame.draw.circle(
                    context.window, 
                    (255, 255, 255), 
                    ((board_x + j * square_width), (board_y + i * square_width)), 
                    square_width/2 - boarder_thickness
                )

def posToStone(context: Context, pos: (int, int)) -> (int, int):
    width, height = context.window.get_size()
    boardSize = context.boardSize
    square_width = (1/(boardSize-1))*(width/2)

    zero = (width/4, (height - width/2)/2)
    zeroed_pos = (pos[0] - zero[0], pos[1] - zero[1])
    zeroes_pos_rounded = (square_width*round(zeroed_pos[0]/square_width), square_width*round(zeroed_pos[1]/square_width))

    if zeroed_pos[0] < 0 or zeroed_pos[0] > width/2 or zeroed_pos[1] < 0 or zeroed_pos[1] > width/2:
        return (None, None)
    else:
        return zeroes_pos_rounded

def paintHover(context: Context):
    width, height = context.window.get_size()
    boardSize = context.boardSize
    square_width = (1/(boardSize-1))*(width/2)

    pos = posToStone(context, pygame.mouse.get_pos())
    if pos[0] == None or pos[1] == None: 
        return

    colour = (0, 0, 0) if context.blackStoneTurn else (255, 255, 255)

    pygame.draw.circle(
                    context.window, 
                    colour, 
                    (pos[0] + width/4, pos[1] + (height - width/2)/2), 
                    square_width/2,
                    4
                )    

def eventHandler(context: Context, engine: EngineWrapper):
    for event in pygame.event.get():
                if event.type == pygame.MOUSEBUTTONDOWN:
                    pos = posToStone(context, pygame.mouse.get_pos())
                    if pos[0] is not None:

                        square_width = (1/(context.boardSize - 1))*(context.window.get_width()/2)

                        col = round(pos[0] / square_width)
                        row = round(pos[1] / square_width)

                        index = row * context.boardSize + col

                        if context.blackStoneTurn:
                            engine.send_command(f"play {index} 1")
                            
                        else:
                            engine.send_command(f"play {index} 2")

                if event.type == pygame.KEYDOWN:

                    if event.key == pygame.K_r:
                        engine.send_command(f"reset")
                        context.reset()

                    if event.key == pygame.K_q:
                        return False


                if event.type == pygame.QUIT:
                    return False
     
    return True

def engineHandler(context: Context, engine: EngineWrapper):
    while not engine.engine_messages.empty():
        line = engine.engine_messages.get()
        words = line.split()
        if words:
            print(words)
            if words[0] == "ok":
                context.state[int(words[1])] = int(words[2])
                context.blackStoneTurn = not context.blackStoneTurn

                if len(words) > 3 and words[3] == "dead":
                    for i in range(4, len(words)):
                        context.state[int(words[i])] = 0

            if words[0] == "invalid":
                print("ERROR: position already filled.")

def updateDisplay(context: Context):
    context.window.fill("white")
    paintBoard(context)
    paintStones(context)
    paintHover(context)
    pygame.display.flip()

def cleanUp(engine: EngineWrapper):
    engine.terminate()
    pygame.quit()

def main():
    # pygame setup
    pygame.init()

    # parallel exe thread
    engine = EngineWrapper()

    # game context
    context = Context(9)

    # fps
    clock = pygame.time.Clock()

    running = True    
    while running:
        
        # Events
        running = eventHandler(context, engine)

        # Engine -> Gui communication
        engineHandler(context, engine)
        
        # Refresh screen
        updateDisplay(context)
        
        # tick
        clock.tick(60)

    # close process
    cleanUp(engine)

if __name__ == "__main__":
    main()