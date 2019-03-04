#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <algorithm>
#define HEIGHT 9 // Fiddle with this until the message appears. 8 for test.txt, 9 for input.txt


class Point
{
    int x, y, vx, vy;
    
    public:
        Point(int a, int b, int c, int d)
        {
            x = a;
            y = b;
            vx = c;
            vy = d;
        }        
        void advance()
        {
            x += vx;
            y += vy;
        }
        void print(const char * str, int offsetx, int offsety)
        {
            COORD pos = {x-offsetx, y-offsety+1};
            HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleActiveScreenBuffer(hConsole_c);
            DWORD len = strlen(str);
            DWORD dwBytesWritten = 0;
            WriteConsoleOutputCharacter(hConsole_c, str, len, pos, &dwBytesWritten);
        }
        int getY()
        {
            return y;
        }
        int getX()
        {
            return y;
        }
};

bool xComparator(Point p, Point q) { return p.getX() < q.getX(); }

bool yComparator(Point p, Point q) { return p.getY() < q.getY(); }

int main(int argc, char const *argv[])
{
    int returnValue = 0;

    std::ifstream inputFile;
    inputFile.open(argv[1]);
    if (inputFile.is_open())
    {
        std::string line;
        std::vector<Point> points;

        while (getline(inputFile,line))
        {
            int x,y,vx,vy;
            sscanf(line.c_str(),"position=< %d,  %d> velocity=< %d,  %d>", &x,&y,&vx,&vy);
            Point point(x,y,vx,vy);
            points.push_back(point);
        }

        int maxy = 0;
        int miny = -(UINT16_MAX);
        int t = 0;
        while (maxy-miny > HEIGHT)
        {
            t++;

            for (std::vector<Point>::iterator point = points.begin(); point != points.end(); point++)
                point->advance();

            maxy = std::max_element(points.begin(),points.end(), yComparator)->getY();
            miny = std::min_element(points.begin(),points.end(), yComparator)->getY();
        }

        std::cout << "ANSWER TO PART TWO: " << t;
        int minx = std::min_element(points.begin(),points.end(), xComparator)->getX();
        miny = std::min_element(points.begin(),points.end(), yComparator)->getY();

        for (Point point : points)
            point.print("0",minx,miny);
    }
    else
    {
        std::cout << " Cannot open file: " << argv[1] << std::endl;
        returnValue = -1;
    }

    return returnValue;
}
