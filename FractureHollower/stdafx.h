#pragma once

constexpr unsigned int SurfaceType = 0;
constexpr unsigned int FractureType = 1;

//Unit as Pixels
constexpr unsigned int XMax = 1280;
constexpr unsigned int YMax = 1280;
constexpr unsigned int ZMax = 1280;

constexpr unsigned int NMax = XMax * YMax;
constexpr unsigned int FracturesMax = 1280;

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

constexpr char RemainingFracturesFile[] = "RemainingFractures.csv";

constexpr unsigned int RecordMax = 25600;
constexpr unsigned int RecordLenMax = 64;

unsigned int SearchRangeMax = 8;

unsigned int TestPieceMax = 64;