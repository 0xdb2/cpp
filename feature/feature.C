#include <stdio.h>
#include <iostream>

class Feature
{
public:
    enum FeatureType {eUnknown, eCircle, eTriangle, eSquare};

    Feature() : type(eUnknown), points(0) { }

    ~Feature()
    {
        if (points)
        {
            delete [] points;
        }
    }

    bool isValid()
    {
        return (type != eUnknown && points);
    }

    bool read(FILE* file)
    {
        int readItems = fread(&type, sizeof(FeatureType), 1, file);
        if (readItems != 1) return false;

        short n = 0;
        switch (type)
        {
            case eCircle: n = 3; break;
            case eTriangle: n = 6; break;
            case eSquare: n = 8; break;
            default: type = eUnknown; return false;
        }

        points = new double[n];
        if (!points) return false;

        readItems = fread(points, sizeof(double), n, file);
        return (readItems == n);
    }

    void draw()
    {
        switch (type)
        {
            case eCircle: drawCircle(points[0], points[1], points[2]); break;
            case eTriangle: drawPolygon(points, 6); break;
            case eSquare: drawPolygon(points, 8); break;
        }
    }

private:

    double* points;
    FeatureType type;

    void drawCircle(double centerX, double centerY, double radius)
    {
        std::cout << "Feature::drawCircle points=" << centerX << "/" << centerY << "/" <<  radius << std::endl;
    }

    void drawPolygon(double* points, int size)
    {
        std::cout << "Feature::drawPolygon points=";
        for (int i=0; i<size; i++) std::cout << points[i] << (i != size-1?"/":"");
        std::cout << std::endl;
    }

};

bool writeTestData(Feature::FeatureType ft)
{
        FILE* file = fopen("features.dat", "w");
        if (! file) return false;

        double points[8] = { 3.0, 4.0, 5.0, 6.0, 8.0, 1.0, 9.0, 7.0 };
        int wroteItems;

        wroteItems = fwrite(&ft, sizeof(Feature::FeatureType), 1, file);
        wroteItems =+ fwrite(&points, sizeof(points), 1, file);

        fclose(file);
        return (wroteItems == 2);
}

int main(int argc, char* argv[])
{
    std::cout << "::main start" << std::endl;
    Feature feature;

    writeTestData(Feature::eSquare);

    FILE* file = fopen("features.dat", "r");
    if (! file) return 2;

    feature.read(file);
    if (!feature.isValid()) return 1;

    feature.draw();
    std::cout << "::main end" << std::endl;

    return 0;
}

