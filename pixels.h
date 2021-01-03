

extern const int ledsPerStrip;


void initLeds();
int getFirstPixel(int port);
int lastPixel(int port);
void setPixel(int num, int value);
void updateAllPixels();
