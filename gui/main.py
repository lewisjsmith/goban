import pygame

from pathlib import Path
import subprocess

# TODO: 
# settings data file 
# consistent use of "size" throughout range iterators
# subprocess and threads for non-blocking engine usage
# separation of functions into modules
# proper usage of global variables and passed parameters 

window_width = 1280
window_height = 720
size = 9

state = []
for i in range(0, 81):
    state.append(0)

engine_path = Path(__file__).parent.parent / "engine" / "build" / "prog"
proc = subprocess.Popen(
    [str(engine_path)],  
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    text=True,
    bufsize=1  
)

# Test
# state[10] = 1
# state[30] = 2
# state[8] = 1

def paintBoard(window, size):
    width, height = window.get_size()
    square_width = (1/(size - 1))*(width/2)
    canvas = pygame.Surface((width/2, width/2))
    border = 2

    for i in range(size - 1):
        for j in range(size - 1):
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

            pygame.draw.rect(canvas, (0, 0, 0), outer)
            pygame.draw.rect(canvas, (210, 180, 140), inner)                

        window.blit(canvas, (width/4, (height - width/2)/2))

def paintStones(window, state):
    width, height = window.get_size()
    square_width = (1/(size-1))*(width/2)

    board_x = width / 4
    board_y = (height - width / 2) / 2

    for i in range(size):
        for j in range(size):
            if state[i*9 + j] == 1:
                pygame.draw.circle(
                    window, 
                    (0, 0, 0), 
                    ((board_x + j * square_width), (board_y + i * square_width)), 
                    square_width/2
                )
            if state[i*9 + j] == 2:
                pygame.draw.circle(
                    window, 
                    (255, 255, 255), 
                    ((board_x + j * square_width), (board_y + i * square_width)), 
                    square_width/2
                )

def posToStone(window, state, pos) -> (int, int):
    width, height = window.get_size()
    square_width = (1/(size-1))*(width/2)

    zero = (width/4, (height - width/2)/2)
    zeroed_pos = (pos[0] - zero[0], pos[1] - zero[1])
    zeroes_pos_rounded = (square_width*round(zeroed_pos[0]/square_width), square_width*round(zeroed_pos[1]/square_width))

    if zeroed_pos[0] < 0 or zeroed_pos[0] > width/2 or zeroed_pos[1] < 0 or zeroed_pos[1] > width/2:
        return (None, None)
    else:
        return zeroes_pos_rounded

def paintHover(window, state):
    width, height = window.get_size()
    square_width = (1/(size-1))*(width/2)

    pos = posToStone(window, state, pygame.mouse.get_pos())
    if pos[0] == None or pos[1] == None: 
        return

    pygame.draw.circle(
                    window, 
                    (0, 0, 0), 
                    (pos[0] + width/4, pos[1] + (height - width/2)/2), 
                    square_width/2,
                    4
                )

def main():
    # pygame setup
    pygame.init()

    window = pygame.display.set_mode((window_width, window_height))
    clock = pygame.time.Clock()
    running = True
    
    while running:

        for event in pygame.event.get():
            if event.type == pygame.MOUSEBUTTONDOWN:
                pos = posToStone(window, state, pygame.mouse.get_pos())
                if pos[0] != None and pos[1] != None:
                    state[int(pos[0]/80 + pos[1]*9/80)] = 1

            if event.type == pygame.QUIT:
                running = False
        
        # Painting
        window.fill("white")
        paintBoard(window, size)
        paintStones(window, state)
        paintHover(window, state)


        pygame.display.flip()
        clock.tick(60)

    proc.stdin.write("quit\n")
    proc.stdin.flush()
    proc.wait()
    pygame.quit()


if __name__ == "__main__":
    main()