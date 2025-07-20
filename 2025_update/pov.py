from __future__ import print_function
import os

characters = {
    'A' : ['0xf6','0x11','0x11','0x11','0xf6'],
    'B' : ['0x76','0x91','0x91','0x91','0xf7'],
    'C' : ['0x42','0x81','0x81','0x81','0x76'],
    'D' : ['0x76','0x81','0x81','0x81','0xf7'],
    'E' : ['0x81','0x91','0x91','0x91','0xf7'],
    'F' : ['0x01','0x11','0x11','0x11','0xf7'],
    'G' : ['0xf2','0x51','0x91','0x81','0x76'],
    'H' : ['0xf7','0x10','0x10','0x10','0xf7'],
    'I' : ['0x00','0x81','0xf7','0x81','0x00'],
    'J' : ['0x00','0x01','0x77','0x81','0x60'],
    'K' : ['0x81','0x42','0x24','0x10','0xf7'],
    'L' : ['0x80','0x80','0x80','0x80','0xf7'],
    'M' : ['0xf7','0x02','0x04','0x02','0xf7'],
    'N' : ['0xf7','0x20','0x10','0x04','0xf7'],
    'O' : ['0x76','0x81','0x81','0x81','0x76'],
    'P' : ['0x06','0x11','0x11','0x11','0xf7'],
    'Q' : ['0xf6','0x61','0x41','0x41','0x36'],
    'R' : ['0x86','0x51','0x31','0x11','0xf7'],
    'S' : ['0x61','0x91','0x91','0x91','0x86'],
    'T' : ['0x01','0x01','0xf7','0x01','0x01'],
    'U' : ['0x77','0x80','0x80','0x80','0x77'],
    'V' : ['0x17','0x60','0x80','0x60','0x17'],
    'W' : ['0x77','0x80','0x70','0x80','0x77'],
    'X' : ['0x81','0x66','0x10','0x66','0x81'],
    'Y' : ['0x01','0x06','0xf0','0x06','0x01'],
    'Z' : ['0x83','0x85','0x91','0xa1','0xc1'],
    '0' : ['0x76','0x85','0x91','0xa1','0x76'],
    '1' : ['0x00','0x80','0xf7','0x82','0x00'],
    '2' : ['0x86','0x91','0xa1','0xc1','0x82'],
    '3' : ['0x61','0x93','0x85','0x81','0x41'],
    '4' : ['0x20','0xf7','0x22','0x24','0x30'],
    '5' : ['0x71','0x85','0x85','0x85','0x47'],
    '6' : ['0x60','0x91','0x91','0x92','0x74'],
    '7' : ['0x03','0x05','0x11','0xe1','0x01'],
    '8' : ['0x66','0x91','0x91','0x91','0x66'],
    '9' : ['0x36','0x51','0x91','0x91','0x06'],
    ':' : ['0x00','0x00','0x66','0x66','0x00'],
    '!' : ['0x00','0x00','0x97','0x00','0x00'],
    ',' : ['0x00','0x00','0x60','0xa0','0x00'],
    '-' : ['0x00','0x10','0x10','0x10','0x00'],
    '?' : ['0x04','0x12','0xa1','0x02','0x04'],
    '.' : ['0x00','0x00','0x60','0x60','0x00'],
    ' ' : ['0x00','0x00','0x00','0x00','0x00'],
}


class POV():
    """
    :param screens: Screen contents
    :type screens: list of strings
    :param name: Folder/File name for Arduino sketch
    :type name: string
    """
    def __init__(self, screens, name='pov_sketch'):
        self.screens = screens
        self.name = name
        self.check_inputs()

        self.address_counter = 0
        self.buffer = ''

        self.buffer += '// Number of screens\n' + str(hex(len(screens))) + ','
        self.address_counter += 1

        for screen in self.screens:
            self.write_screen(screen)
        print('\n\n// ****************** COPY FROM HERE ***************** ')
        print('byte eepromData[' + str(self.address_counter) + '] = {\n')
        print(self.buffer)
        print('};')


    def check_inputs(self):
        #Convert all characters to uppercase
        for idx,screen in enumerate(self.screens):
            self.screens[idx] = screen.upper()

        #Drop screens with invalid characters
        for screen in self.screens:
            try:
                for letter in screen:
                    characters[letter]
            except KeyError:
                print('Bad character: ' + letter)
                print('\tDropping screen: ' + screen)
                self.screens.remove(screen)

        #Drop screens with too many characters
        for screen in self.screens:
            if len(screen) > 20:
                print('Max number of letters is 20')
                print('\tDropping screen: ' + screen)
                self.screens.remove(screen)

        #Drop screens if too many provided
        if len(self.screens) > 6:
            print('Max number of screens is 6')
            while len(self.screens) > 6:
                print('\tDropping screen: ' + self.screens.pop())

        print('Valid Screens:')
        print(self.screens)


    def write_screen(self, screen):
        self.buffer += '\n// ' + '#'*20  + screen + '#'*20  +  '\n'
        self.buffer += '// lenght\n' + str(hex(len(screen)*5)) + ',\n'
        self.address_counter += 1
        self.buffer += '// Effect\n0x01,'
        self.address_counter += 1
        for letter in reversed(screen):
            self.buffer += '\n//' + '-'*10 + letter + '-'*10 +  '\n'
            for character in characters[letter]:
                self.buffer += character + ', '
                self.address_counter += 1
