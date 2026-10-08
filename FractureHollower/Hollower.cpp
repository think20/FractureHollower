#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <string>

#include "stdafx.h"
#include "TestPiece.h"

void TestPieceManager_t::GetData()
{
	InputManager->GetData();
}
void TestPieceManager_t::CreateTestPieces()
{
	while (Line < RecordCount)
	{
		TestPieces[TestPieceCount] = new TestPiece_t(this);
		TestPieceCount++;
	}
}
void TestPieceManager_t::StartTests()
{
	for (auto t = 0; t < TestPieceCount; t++)
	{
		TestPieces[t]->Compare();
	}
}
void TestPieceManager_t::SaveResults()
{
	OutputManager->SaveData();
}
void TestPiece_t::Compare()
{

	auto& Fractures = this->Fractures->Fractures;
	auto& Count = this->Fractures->FracturesCount;

	for (auto f = 0; f < Count; f++)
	{
		auto& Fracture = *(Fractures[f]);
		unsigned int x = unsigned int(Fracture.x);
		unsigned int y = unsigned int(Fracture.y);
		unsigned int z = unsigned int(Fracture.z);

		auto zSurf = FindSurface(x, y);

		if (z < zSurf + 0)
		{
			Fracture.lost = true;
			printf("TestPiece: Fracture at (%d,%d) lost due to z %d < surface z %d\n", x, y, z, zSurf);
		}
		else Fracture.lost = false;
	}
}
unsigned int TestPiece_t::FindSurface(unsigned int x, unsigned int y)
{
	auto& Surface = this->Surface->CompositeSurfaceHeight->Element;
	unsigned int z = 0;
	bool found = false;

	for (auto XOffset = 0; XOffset <= SearchRangeMax; XOffset++)
	{
		for (auto YOffset = 0; YOffset <= XOffset; YOffset++)
		{
			//printf("TestPiece: x:%u y:%u dx:%d dy:%d\n", x, y, XOffset, YOffset);

			int x11 = x + XOffset;
			int x12 = x - XOffset;
			int x21 = x + YOffset;
			int x22 = x - YOffset;

			int y11 = y + YOffset;
			int y12 = y - YOffset;
			int y21 = y + XOffset;
			int y22 = y - XOffset;

			found = true;
			if (Surface[x11][y11] != 0) z = Surface[x11][y11];
			else if (Surface[x11][y12] != 0) z = Surface[x11][y12];
			else if (Surface[x12][y11] != 0) z = Surface[x12][y11];
			else if (Surface[x12][y12] != 0) z = Surface[x12][y12];
			else if (Surface[x21][y21] != 0) z = Surface[x21][y21];
			else if (Surface[x21][y22] != 0) z = Surface[x21][y22];
			else if (Surface[x22][y21] != 0) z = Surface[x22][y21];
			else if (Surface[x22][y22] != 0) z = Surface[x22][y22];
			else found = false;

			if (found) break;
		}
		if (found) break;
	}

	return z;
}
void TestPiece_t::FractureSurface_t::ReadyCompositeSurface()
{
	GetFractureSurface();
	Centering();
	Rotating();
	ContainerFilling();
}
void TestPiece_t::FractureSurface_t::GetFractureSurface()
{
	auto& r = TestPiece->TestPieceManager->SurfaceRecordCount;
	auto& buf = TestPiece->TestPieceManager->SurfaceBuf;

	auto& Pos = SurfaceOri;
	auto& Count = PointCount;

	for (auto Record = 0; Record < r; Record++)
	{
		unsigned int x = 0;
		unsigned int y = 0;
		unsigned int z = 0;

		sscanf(buf[Record], "%u,%u,%u", &x, &y, &z);

		Pos[Record] = new Pos_t(double(x), double(y), double(z));
	}
	Count = r;
}
void TestPiece_t::FractureSurface_t::Centering()
{
	double XTotal = 0;
	double YTotal = 0;

	for (auto p = 0; p < PointCount; p++)
	{
		auto& NodeNow = *(SurfaceOri[p]);
		XTotal += NodeNow.x;
		YTotal += NodeNow.y;
	}

	double XMean = XTotal / double(PointCount);
	double YMean = YTotal / double(PointCount);

	double XOffset = double(XMax) / 2 - XMean;
	double YOffset = double(YMax) / 2 - YMean;

	//printf("Centering: centroid=(%.0f,%.0f), offset=(%.0f,%.0f)\n", XMean, YMean, XOffset, YOffset);

	for (auto p = 0; p < PointCount; p++)
	{
		auto& NodeNow = *(SurfaceOri[p]);
		NodeNow.Shift(XOffset, YOffset);
	}
}
void TestPiece_t::FractureSurface_t::Rotating()
{
	if (g_SurfaceRotateDeg == 0.0) return;

	double dtheta = g_SurfaceRotateDeg * 3.14159265358979323846 / 180.0;
	printf("Rotating surface by %.1f degrees...\n", g_SurfaceRotateDeg);

	for (auto p = 0; p < PointCount; p++)
	{
		auto& NodeNow = *(SurfaceOri[p]);
		NodeNow.Rotate(dtheta);
	}
}
void TestPiece_t::FractureSurface_t::ContainerFilling()
{
	auto& Tar = CompositeSurfaceHeight->Element;
	for (auto p = 0; p < PointCount; p++)
	{
		auto& NodeNow = *(SurfaceOri[p]);
		if (NodeNow.x < 0.0 || NodeNow.y < 0.0 || NodeNow.x >= double(XMax) || NodeNow.y >= double(YMax)) continue; // bounds guard: rotation can move points outside the container
  unsigned int x = unsigned int(NodeNow.x);
		unsigned int y = unsigned int(NodeNow.y);
		unsigned int z = unsigned int(NodeNow.z);
		Tar[x][y] = z;
	}
}
void TestPiece_t::Fractures_t::ReadyFractures()
{
	GetFiberFractures();
	XYReScale();
	ZReScale();
}
void TestPiece_t::Fractures_t::GetFiberFractures()
{
	auto& Line = TestPiece->TestPieceManager->Line;
	Line++; //First Line is Title

	auto& buf = TestPiece->TestPieceManager->FractureBuf;

	auto& Pos = Fractures;
	auto& Count = FracturesCount;

	sscanf(buf[Line], "%u", &Count); //Second Line for Fracture Count;

	printf("InputManager:TestPiece has %u fractures\n", Count);
	Line++;

	for (auto Record = 0; Record < Count; Record++)//Not all records (r) for F2, only "Count" terms
	{
		double x = 0;
		double y = 0;
		double z = 0;
		double r = 0;
		double r2 = 0;
		double theta = 0;

		sscanf(buf[Line], "%lf,%lf,%lf,%lf,%lf,%lf", &x, &y, &z, &r, &r2, &theta);

		Pos[Record] = new Pos_t(x, y, z, r, r2, theta);//Record = Line - 2

		Line++;
	}
}
void TestPiece_t::Fractures_t::XYReScale()
{
	double Factor = FiberDiameter * PixelperUm;
	for (auto f = 0; f < FracturesCount; f++)
	{
		auto& Fracture = *(Fractures[f]);
		Fracture.XYScale(Factor);
	}
}
void TestPiece_t::Fractures_t::ZReScale()
{
	double Factor = LayerDistance * PixelperUm;
	double zoff = (g_ZOffsetUm >= 0) ? g_ZOffsetUm : ZOffsetUm;
	double OffsetPx = zoff * PixelperUm;
	for (auto f = 0; f < FracturesCount; f++)
	{
		auto& Fracture = *(Fractures[f]);
		Fracture.z = Fracture.z * Factor + OffsetPx;
	}
}
void Pos_t::Shift(double dx, double dy)
{
	x += dx;
	y += dy;
}
void Pos_t::Rotate(double dtheta)
{
	double c = cos(dtheta);
	double s = sin(dtheta);
	double xc = XMean;
	double yc = YMean;

	double dx = x - xc;
	double dy = y - yc;

	x = xc + dx * c - dy * s;
	y = yc + dx * s + dy * c;

	double dx0 = x0 - xc;
	double dy0 = y0 - yc;
	x0 = xc + dx0 * c - dy0 * s;
	y0 = yc + dx0 * s + dy0 * c;
}
void Pos_t::XYScale(double factor)
{
	xr *= factor;
	yr *= factor;

	x = xr + XMean;
	y = yr + YMean;
}
Pos_t::Pos_t(double x, double y, double z)
{
	this->x = x;
	this->y = y;

	this->xr = x;
	this->yr = y;

	this->x0 = x;
	this->y0 = y;

	this->z = z;
	this->r = 0;
	this->theta = 0;
	this->lost = false;
}
Pos_t::Pos_t(double x, double y, double z, double r,double r2, double theta)
{

	this->x = x;
	this->y = y;

	this->xr = x;
	this->yr = y;

	this->x0 = 0;
	this->y0 = 0;
			
	this->z = z;
	this->r = r;
	this->r2 = r2;
	this->theta = theta;
	this->lost = false;
}
void TestPieceManager_t::OutputManager_t::SaveData()
{
	auto& TCount = TestPieceManager->TestPieceCount;
	for (auto t = 0; t < TCount; t++)
	{
		fprintf(RemFractures,"TestPiece%u:,\n", t);
		auto& Count = TestPieceManager->TestPieces[t]->Fractures->FracturesCount;
		auto& Fractures = TestPieceManager->TestPieces[t]->Fractures->Fractures;

		for (unsigned int rf = 0; rf < Count; rf++)
		{
			auto& Fracture = *(Fractures[rf]);
			if (Fracture.lost) continue;
			fprintf(RemFractures, "%lf,%lf,%lf\n", Fracture.r, Fracture.r2, Fracture.theta);
		}
	}
}
void TestPieceManager_t::InputManager_t::GetData()
{
	GetFractureSurface();
	GetFiberFractures();
}
void TestPieceManager_t::InputManager_t::GetFractureSurface()
{
	auto& sbuf = TestPieceManager->SurfaceBuf;
	auto& r = TestPieceManager->SurfaceRecordCount;

	while (fscanf(SurfaceF, "%s", sbuf[r]) != EOF) r++;

}
void TestPieceManager_t::InputManager_t::GetFiberFractures()
{
	auto& buf = TestPieceManager->FractureBuf;
	auto& r = TestPieceManager->RecordCount;
	
	while (fscanf(FracturesF, "%s", buf[r]) != EOF) r++;

	printf("InputManager:Fracture file has %u Records\n", r);
}
TestPiece_t::TestPiece_t(TestPieceManager_t* Manager)
{
	this->TestPieceManager = Manager;
	this->Surface = new FractureSurface_t(this);
	this->Fractures = new Fractures_t(this);

	Surface->ReadyCompositeSurface();
	Fractures->ReadyFractures();
}
TestPiece_t::FractureSurface_t::FractureSurface_t(TestPiece_t* TestPiece)
{
	this->CompositeSurfaceHeight = new Container_t<unsigned int>(XMax, YMax);
	this->SurfaceOri = new Pos_t * [NMax];
	this->PointCount = 0;
	this->TestPiece = TestPiece;
}
TestPiece_t::Fractures_t::Fractures_t(TestPiece_t* TestPiece)
{
	this->Fractures = new Pos_t * [FracturesMax];
	this->FracturesCount = 0;
	this->TestPiece = TestPiece;
}
TestPieceManager_t::InputManager_t::InputManager_t(TestPieceManager_t* Manager)
{
	this->TestPieceManager = Manager;

	std::string SurfacePathS(g_SurfacePath[0] ? g_SurfacePath : CompositeSurfacePath);
	std::string SurfaceFileS(CompositeSurfaceFile);

	SurfaceF = fopen((SurfacePathS + SurfaceFileS).c_str(), "r");
	if (!SurfaceF) { printf("ERROR: Cannot open surface: %s%s\n", SurfacePathS.c_str(), SurfaceFileS.c_str()); }
	else printf("InputManager: Reading surface from %s\n", SurfaceFileS.c_str());

	std::string FracturesPathS(FracturesPath);
	std::string FracturesFileS(FracturesFile);

	FracturesF = fopen((FracturesPathS + FracturesFileS).c_str(), "r");
	if (!FracturesF) { printf("ERROR: Cannot open fractures: %s%s\n", FracturesPathS.c_str(), FracturesFileS.c_str()); }
	else printf("InputManager: Reading fractures from %s\n", FracturesFileS.c_str());

}
TestPieceManager_t::OutputManager_t::OutputManager_t(TestPieceManager_t* Manager)
{
	this->TestPieceManager = Manager;

	std::string FracturesPathS(FracturesPath);
	std::string RemFracturesFileS(RemainingFracturesFile);

	RemFractures = fopen((FracturesPathS + RemFracturesFileS).c_str(), "w");
}
TestPieceManager_t::InputManager_t::~InputManager_t()
{
	fclose(SurfaceF);
	fclose(FracturesF);
}
TestPieceManager_t::OutputManager_t::~OutputManager_t()
{
	fclose(RemFractures);
}
TestPieceManager_t::TestPieceManager_t()
{
	this->InputManager = new InputManager_t(this);
	this->OutputManager = new OutputManager_t(this);

	auto& sbuf = SurfaceBuf;
	sbuf = new char* [RecordMax];
	for (auto r = 0; r < RecordMax; r++)
	{
		sbuf[r] = new char[RecordLenMax];
		memset(sbuf[r], 0, RecordLenMax);
	}
	Line = 0;
	RecordCount = 0;

	auto& buf = FractureBuf;
	buf = new char* [RecordMax];
	for (auto r = 0; r < RecordMax; r++)
	{
		buf[r] = new char[RecordLenMax];
		memset(buf[r], 0, RecordLenMax);
	}
	SurfaceRecordCount = 0;

	this->TestPieces = new TestPiece_t * [TestPieceMax];
	this->TestPieceCount = 0;
}
TestPieceManager_t::~TestPieceManager_t()
{
	delete this->InputManager;
	delete this->OutputManager;

	auto& sbuf = SurfaceBuf;
	for (auto r = 0; r < RecordMax; r++)
	{
		delete sbuf[r];
	}
	delete[] sbuf;

	auto& buf = FractureBuf;
	for (auto r = 0; r < RecordMax; r++)
	{
		delete buf[r];
	}
	delete[] buf;
}