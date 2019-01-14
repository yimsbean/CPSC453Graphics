# CPSC453 Assignment 2 README

* Slightly modified README from 'final version'
* 19/20 (Mouse Translation Not Enough (-1)) + Bonus(?)

- SeungBin Yim
- Chenlei Bao

> Platform: Linux(Arch, Fedora)
>
> Compiler: Clang++6.0.1 (Build with Makefile)

Finished Parts:
- I, II, III, IV, V, Bonus

Comments :
> PartIV was finished by doing the Bonus part, so there are no fixed size kernels.
---------
Use the makefile to compile under linux environment
The program starts from loading "image1-mandrill.png" image

Keyboard Controls:
Page Up/Down - Switch images

W - Zoom in
S - Zoom out

A - Rotate left
D - Rotate right

G - Apply greyscale1 [Toggle]
H - Apply greyscale2 [Toggle]
J - Apply greyscale3 [Toggle]
K - Apply sepia tone [Toggle]

B - Apply the vertical edge [Toggle]
N - Apply the horizontal edge [Toggle]
M - Apply the unsharp mask [Toggle]

Arrow Left/Right/Up/Down - Pan

Home - Increase the amount of Gaussian Kernel Size
End - Decrease the amount of Gaussian Kernel Size

Left/Right Bracket - Switch background images

Num Key from 1~3 will enable some modes:
    [1]: Default
    [2]: Apply Part V (Alpha Blending)
    [3]: Gaussian Kernel Size increases/decreases 3 times than before, limited to 1000

Mouse Controls:
Move - Move
Left Button - Pan
Scroll up - Zoom in 
Scroll down - Zoom out

---------
For Part2, Question1: 'Which one looks the most "correct" to you?'
> 3rd formula fits the best
> > 1st formula is too dark
> > 2nd formula is similiar to 3rd one, but we think 3rd one is better.

For Part2, Question2: 'Why do you think different formulas exist?'
> Different formulas exist because everybody percieve lights in different ways.
> some might get comfortable with more green colours, others might not.
