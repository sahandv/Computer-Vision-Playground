# ROADSIDE PANEL DETECTION

This is an old code which I am trying to save. 

Here I tried to detect roadside panels and billboards of ad and other stuff for advertisement tax system. The system worked partially, but not perfectly.
It's pretty simple actually. The system works like this: 

1- Get images of same resolution and do segmentation by MSER, using empirical parameters 

2- Try to scan each segment for a text

3- If a text is found, it is a panel. If not, delete it.

4- Add texts to a file and number the crops of image which are potntially panels.

The software uses QT5 and 4, OpenCV, tesseract and h-spell libs.

Have fun and don't run this code, unless you know what you are doing! But you may use some snippets for:

1- Parsing a text

2- Using tesseract for OCR (Optical Character Recognition)

3- Using Hunspell for spell checking 

4- MSER segmentation in OpenCV

5- Writing to file. 

6- Crop images and save them to disk 



