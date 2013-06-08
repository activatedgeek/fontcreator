GL := -lGLEW -lGL -lGLU -lglut

output: fontcreator.cpp
        g++ -o $@ $< $(GL)
