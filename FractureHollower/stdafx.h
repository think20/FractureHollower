#pragma once

constexpr unsigned int SurfaceType = 0;
constexpr unsigned int FractureType = 1;

//Unit as Pixels
constexpr unsigned int XMax = 1280;
constexpr unsigned int YMax = 1280;
constexpr unsigned int ZMax = 1280;

constexpr unsigned int NMax = XMax * YMax;
constexpr unsigned int FracturesMax = 20000; // raised from 1280: tensile-scan blocks carry up to ~2700 fractures

constexpr double PixelperUm = 1 / 3.07;
constexpr double FiberDiameter = 51 * 2;//um, for F2 and F3
constexpr double LayerDistance = 100;//um
constexpr double ZOffsetUm = 50; //unified: Slice 0 at surface minimum //um, align Slice 0 with fracture surface reference plane

constexpr double XMean = double(XMax) / 2;
constexpr double YMean = double(YMax) / 2;

constexpr char CompositeSurfacePath[] = "C:\\Users\\12155\\Source\\repos\\think20\\FiberPostprocessor\\FiberPostprocessor\\Results\\F2Report\\";
constexpr char CompositeSurfaceFile[] = "StartLayerS.txt";
constexpr char FracturesPath[] = "C:\\Users\\12155\\Source\\repos\\think20\\HexaSimulator\\HexaSimulator\\";
constexpr char FracturesFile[] = "Fractures.csv";
// Runtime override (-1 size = use the constexpr defaults above)
extern char g_FracturesDir[512];
extern char g_FracturesFile[512];

constexpr char RemainingFracturesFile[] = "RemainingFractures.csv";

constexpr unsigned int RecordMax = 600000; // raised from 25600: the 42-test Fractures.csv has >25600 whitespace tokens
constexpr unsigned int RecordLenMax = 64;

unsigned int SearchRangeMax = 8;

unsigned int TestPieceMax = 64;