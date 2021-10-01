# Name: Gerallt Franke
# Student ID: 138793
# Program Name: ZAT113 AT3 - gcrypt hashing algorithm
# Python version: 3.9.5

import GCrypt           # For GCrypt Algrithm.
import tkinter as tk    # For GUI.
import sys              # For parsing program arguments.

from tkinter import filedialog as fd # For file hashing.

gCrypt = GCrypt.gCrypt()
#print(gCrypt.ghashString("The quick brown fox jumps over the lazy dog"))  # should be: 0xcd1ab45250a5b23932ef645d8ad5a0f0a7b23f97
#print(gCrypt.ghashString("The quick brown fox jumps over the lazy dog.")) # should be: 0xc4af78e4818afa6ca66bfeb10a55c8d1105564f8
#print(gCrypt.ghashString("the quick brown fox jumps over the lazy dog")) # should be: 0x1bc9559262d95592ff6dde9deb9122c30a74a3f7 
#print(gCrypt.ghashString("")) # should be: 0xa8e489e03d5dcad3fc0d4b662ae60b0ca90cf2fe

#gCrypt.bruteforceSimple() # Run a test bruteforce of the hashing algorithm.
#gCrypt.findCollisions()

#print(gCrypt.ghashFile("test.txt"))

#print("done.")


def openFile():
    # Use the open file dialog to select a file.
    fileName = fd.askopenfilename()

    # Hash the file and return the hash result.
    hashResult = gCrypt.ghashFile(fileName)
    
    # Display the hash result on the GUI.
    textEntry.set(hashResult)

def runBruteforce():
    gCrypt.bruteforceSimple()

def hashString():
    # Get a string input from the text box (Entry)
    stringInput = textString.get()

    # Hash the string inputted from the GUI and return the hash result.
    hashResult = gCrypt.ghashString(stringInput)

    # Display the hash result on the GUI.
    textEntry.set(hashResult)
    
def createGUI():
    global textEntry
    global textString
    
    myWindow = tk.Tk()
    myWindow.title("ZAT113 AT3 - GCrypt");
    myWindow.geometry("640x480")

    openFileButton = tk.Button(
        text="Open File",
        bg="black",
        fg="white",
        width=8,
        height=2,
        command = openFile
    )

    lblHashResult = tk.Label(text="Hash result:")
    textEntry = tk.StringVar()
    txtHashResult = tk.Entry(myWindow,
        textvariable = textEntry,
        bg="yellow",
        fg="blue",
        width=50
    )

    bruteforceButton = tk.Button(
        text="Run bruteforce",
        bg="black",
        fg="white",
        width=16,
        height=1,
        command = runBruteforce
    )

    lblEnterString = tk.Label(text="Enter a string to hash it:")
    textString = tk.StringVar()
    txtStringEntry = tk.Entry(myWindow,
        textvariable = textString,
        bg="green",
        fg="white",
        width=50
    )
    hashStringButton = tk.Button(
        text="Hash String",
        bg="black",
        fg="white",
        width=16,
        height=1,
        command = hashString
    )


    openFileButton.pack()
    lblHashResult.pack()
    txtHashResult.pack()

    bruteforceButton.pack()

    lblEnterString.pack()
    txtStringEntry.pack()
    hashStringButton.pack()
    
    myWindow.mainloop()

def main(argv):          
    if argv is None:
        argv = sys.argv
    #print("Program arguments:")
    #print(argv)
    if len(argv) > 1:
        if argv[1] == "-s":
            # Hash the string input parameter and return the result in the console.
            inputString = argv[2]

            # Hash the string inputted.
            hashResult = gCrypt.ghashString(inputString)

            # Print the hash result.
            print(hashResult)
        elif argv[1] == "-f":
            # Hash the file input and return the result in the console.
            inputFile = argv[2]
            
            # Hash the file and return the hash result.
            hashResult = gCrypt.ghashFile(inputFile)
            
            # Print the hash result.
            print(hashResult)
        else:
            print("Invalid parameter '{}'".format(argv[1]))
    else:
        # Since there are no other parameters load the GUI.
        createGUI()

if __name__ == "__main__":
    sys.exit(main(sys.argv))
