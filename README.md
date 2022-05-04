# hough_lines_and_circles_detection
A program to detect lines, circles, or both from a USB camera stream using Hough detection method.

# Requirments
1. OpenCV 4.1.1
2. Linux OS (tested with (JetPack 4.6)[https://developer.nvidia.com/jetpack-sdk-46] on Jetson Nano 2GB)
3. USB Camera

# Build and run
The program can be built using the included Makefile.
![image](https://user-images.githubusercontent.com/72912013/166826000-11230a28-b302-402d-91cc-b1e8fc1fc851.png)

# Example
1. Launching the executable and showing the camera stream as-is:
    1. Upon launching the program, you should see a window with the camera feed as-is (no filtering or processing).
    2. The program should also print the possible options and runtime controls to switch on-the-fly.
    ![image](https://user-images.githubusercontent.com/72912013/166826489-ab804f56-e2df-40d8-8b99-55ef78b99c2d.png)

2. Enabling Hough Line detection:
    1. As stated in the message displayed in the terminal, pressing ‘l’ (lower case L) key will enable the Hough Line detection on the current camera stream.
    2. Notice how the size lines (shown in gold of my two-wheeled bot, along with the break-out PCB (mounted on top of the blue plate) have been detected. However, the blue plate itself is not, that’s due to the wheel being in the way of the line.
    ![image](https://user-images.githubusercontent.com/72912013/166826620-8630f41a-6276-45b5-8a44-5abe738a2f48.png)


3. Enabling Hough Circle detection:
    1. Now, by pressing ‘c’ (lower case C) key, we can switch to the Hough Circle detection and disable any other detection methods that may be enabled.
    2. Notice how the wheel of my two-wheeled bot was detected as a circle.
    ![image](https://user-images.githubusercontent.com/72912013/166826726-6344ff00-73f0-4f90-b836-96db0412af56.png)


4. Enabling both Hough line and circle:
    1. Now, by pressing ‘b’ (lower case B) key, we can enable both the Hough Line and Circle detection at the same time on the same frame.
    2. Notice how both the lines and the wheel shown discussed in previous sections above are detected at the same time.
    ![image](https://user-images.githubusercontent.com/72912013/166826379-f781f6d3-a54a-4bd7-a782-3ed3fcd80c60.png)

