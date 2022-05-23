from tkinter import * 

def test(canvas):
    img = PhotoImage(file='./knight_b.png')
    canvas.create_image(250, 250, image=img)

root = Tk()
canvas = Canvas(root, width=500, height=500)
canvas.pack()

SQUARE_SIZE = 80
TOTAL_SIZE = 8 * SQUARE_SIZE


WHITE_THEME = "#C0F7AD"
BLACK_THEME = "#42752F"



canvas.create_rectangle(0, 0,
                            TOTAL_SIZE, TOTAL_SIZE,
                            outline="#000000",
                            fill=BLACK_THEME)

for i in range(0, 8):
            for j in range(0, 8):
                if(((i + j) % 2) == 0):
                    canvas.create_rectangle(i*SQUARE_SIZE, j*SQUARE_SIZE,
                                            i*SQUARE_SIZE + SQUARE_SIZE, j*SQUARE_SIZE + SQUARE_SIZE,
                                            outline="#000000",
                                            fill=WHITE_THEME)

test(canvas)
root.mainloop()