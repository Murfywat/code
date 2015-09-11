""" Implements the Array ADT along with memory visualization.

Loosely based on the Array class described in:
(c) 2011 John Wiley & Sons, Data Structures and Algorithms Using
Python, by Rance D. Necaise.
"""

import turtle
import time
import sys
import random

_BOX_WIDTH = 50
_BOX_HEIGHT = 40
_SCREEN_WIDTH = 800
_SCREEN_HEIGHT = 480


class Array(object):
    """ Array class that supports turtle graphics visualization.  

    There are two public, class-level variables that can be used
    to control the visualization: 

    Array.visible  - Set this to False to turn off graphics. 
    Array.speed -    Set this value to change the speed of the animation. 
                     This value indicates the length of the pauses (in seconds)
                     between updates. 
    """

    # Public static variables for controlling animation:
    speed = .5 # In seconds. 
    visible = True

    # Private static variables used to store information about the set
    # of arrays that exist at a given time.
    _which = 0
    _all_existing_arrays = []
    _screen = None


    def __init__( self, size):
        """Creates an array with a given capacity.
        """

        # Set up the new array... 
        self._size = size
        self._elements = [None] * size
        self._which = Array._which
        self._freed = False
        
        # Generate a random color:
        self._color = [1., 0., random.random()]
        random.shuffle(self._color)

        # Record the fact that a new array has been created...
        Array._which += 1
        Array._all_existing_arrays.append(self)

        if Array.visible:
            # Create a screen if this is the first array to be 
            # instantiated...
            if Array._screen == None:
                Array._screen = turtle.Screen()
                Array._screen.setup(width=_SCREEN_WIDTH, height=_SCREEN_HEIGHT, 
                                    startx=0, starty=0)
                Array._screen.setworldcoordinates(0, 0, 640, 480)
                Array._screen.tracer(0,0)

            # Create a turtle that will be used to draw the newly 
            # created array.  (Each array has its own turtle object.)
            self._my_turtle = turtle.RawTurtle(Array._screen)
            self._my_turtle.speed(0)
            self._my_turtle.hideturtle()
            self._draw()
            time.sleep(Array.speed)



    def __len__( self ):
        """ Returns the size of the array. """
        if self._freed:
            raise ReferenceError("Attempt to access freed Array.")
        return self._size

    def __getitem__( self, index ):
        """ Returns the contents of the indicated array entry. """
        if self._freed:
            raise ReferenceError("Attempt to access freed Array.")
        if (index < 0 or index >= len(self)):
            raise IndexError("Array index {0} out of range" \
                                 " (0, {1}).".format(index, len(self)-1))
        return self._elements[index]

    def __setitem__( self, index, value ):
        """ Stores the value at the indicated index. """
        if self._freed:
            raise ReferenceError("Attempt to access freed Array.")
        if (index < 0 or index >= len(self)):
            raise IndexError("Array index {0} out of range" \
                                 " (0, {1}).".format(index, len(self)-1))

        self._elements[index] = value
        if Array.visible:
            self._draw()
            time.sleep(Array.speed)

    def __iter__(self):
        """ Arrays do not support iteration. """
        raise NotImplementedError()

    def free(self):
        """ Frees this array.  

        The array will no longer appear in the visualization, and may
        be garbage collected as long as no external references exist.
        """
        if self._freed:
            raise ReferenceError("Attempt to free an already freed Array.")
        Array._all_existing_arrays.remove(self)
        self._size = 0
        self._elements = None
        self._freed = True
        self._draw_all()


    def _draw_cell(self, row, index, val):
        # Draw a single cell in an array.
            self._my_turtle.penup()
            self._my_turtle.goto((index + 1) * _BOX_WIDTH, _BOX_HEIGHT * row)
            self._my_turtle.setheading(0)
            self._my_turtle.pendown()
            self._my_turtle.forward(_BOX_WIDTH)
            self._my_turtle.left(90)
            self._my_turtle.forward(_BOX_HEIGHT - 4)
            self._my_turtle.left(90)
            self._my_turtle.forward(_BOX_WIDTH)
            self._my_turtle.left(90)
            self._my_turtle.forward(_BOX_HEIGHT - 4)

            self._my_turtle.penup()
            self._my_turtle.goto((index + 1) * _BOX_WIDTH + 5, 
                        _BOX_HEIGHT * row + _BOX_HEIGHT / 4)
            self._my_turtle.pendown()

            rep = str(val)
            if len(rep) > 7:
                rep = rep[:8] + "..."
            
            self._my_turtle.write(rep)
        

    def _draw(self):
        # Draw this array. 
        if Array.visible:
            self._my_turtle.clear()
            self._my_turtle.color(self._color)
            pos = Array._all_existing_arrays.index(self)
            self._my_turtle.penup()
            self._my_turtle.goto(0, _BOX_HEIGHT * pos + _BOX_HEIGHT / 4)
            self._my_turtle.write("size: " + str(len(self)))
            self._my_turtle.pendown()
            self._my_turtle.penup()
            self._my_turtle.goto(0, _BOX_HEIGHT * pos + _BOX_HEIGHT / 2)
            self._my_turtle.write("id: " + str(self._which))
            self._my_turtle.pendown()

            max_cell = _SCREEN_WIDTH / _BOX_WIDTH - 6
            for i in range(0, min(self._size, max_cell)):
                self._draw_cell(pos, i, self._elements[i])

            if max_cell < self._size:
                self._draw_cell(pos, max_cell, "...")

    def _draw_all(self):
        # Draw ALL arrays currently in existence.
        if Array.visible:
            Array._screen.clear()
            Array._screen.tracer(0,0)
            for array in Array._all_existing_arrays:
                array._draw()
            time.sleep(Array.speed)
