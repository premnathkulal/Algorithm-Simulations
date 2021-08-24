## Simulation of Huffman Algorithm

This is a **C-code** for **Simulation of Huffman Algorithm** using **OpenGL** library.

### Steps to run the code.

You need to install OpenGL library.

```
sudo apt-get update.
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

Compiling the code.

```
gcc -o out  main.cpp  -lGL -lglut -lGLU  -lm -std=c99
```

Running the Code.

```
./out

Enter n: 5
Enter Frequency values: 5 6 0 2 4
```

### Sample Output

```
List contents:
Node : A  frequency:5
Node : B  frequency:6
Node : C  frequency:0
Node : D  frequency:2
Node : E  frequency:4

Huffman code :

NODE : C  Freq : 0  code: 000
NODE : D  Freq : 2  code: 001
NODE : E  Freq : 4  code: 01
NODE : A  Freq : 5  code: 10
```

<img align="left" alt="output" width="60%" src="./assets/output.png" />
