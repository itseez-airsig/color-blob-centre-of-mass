all: COLOR

COLOR: 
	g++ `pkg-config --cflags opencv` SOP_code.cpp -lopencv_highgui -lopencv_core -lopencv_imgproc `pkg-config --libs opencv` -o a 

clean:
	rm *o a -f
