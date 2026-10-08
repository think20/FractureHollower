#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <cstring>
#include <cstdio>
#include <cmath>

// Command-line overrides (set from main.cpp)
extern char g_SurfacePath[512];
extern double g_ZOffsetUm;
extern double g_SurfaceRotateDeg;

template <class Ty>
class Container_t
{
public:
	Container_t(unsigned int, unsigned int);
	Ty** Element;
};

class Pos_t
{
public:
	Pos_t(double, double, double);
	Pos_t(double, double, double, double, double, double);
	void Shift(double, double);
	void Rotate(double);
	void XYScale(double);
	double x, y, z, xr, yr, x0, y0, r, r2, theta;
	bool lost;
};
class TestPiece_t;
class TestPieceManager_t;

class TestPiece_t
{
	friend class TestPieceManager_t;
public:
	TestPiece_t(TestPieceManager_t*);
	void Compare();
private:
	unsigned int FindSurface(unsigned int, unsigned int);

	class FractureSurface_t
	{
		friend class TestPiece_t;
	public:
		FractureSurface_t(TestPiece_t*);
		void ReadyCompositeSurface();
	private:
		void GetFractureSurface();
		void Centering();
		void Rotating();
		void ContainerFilling();
		Container_t<unsigned int>* CompositeSurfaceHeight;
		Pos_t** SurfaceOri;
		unsigned int PointCount;
		TestPiece_t* TestPiece;
	};
	FractureSurface_t* Surface;
	class Fractures_t
	{
		friend class TestPiece_t;
		friend class TestPieceManager_t;
	public:
		Fractures_t(TestPiece_t*);
		void ReadyFractures();
	private:
		void GetFiberFractures();
		void XYReScale();
		void ZReScale();
		Pos_t** Fractures;
		unsigned int FracturesCount;
		TestPiece_t* TestPiece;
	};
	Fractures_t* Fractures;
	TestPieceManager_t* TestPieceManager;
};

class TestPieceManager_t
{
	friend class TestPiece_t;
	friend class OutputManager_t;
public:
	TestPieceManager_t();
	~TestPieceManager_t();
	void GetData();
	void CreateTestPieces();
	void StartTests();
	void SaveResults();
private:
	class InputManager_t
	{
	public:
		InputManager_t(TestPieceManager_t*);
		~InputManager_t();
		void GetData();
	private:
		void GetFractureSurface();
		void GetFiberFractures();
		TestPieceManager_t* TestPieceManager;
		FILE* SurfaceF;
		FILE* FracturesF;
	};
	InputManager_t* InputManager;
	class OutputManager_t
	{
	public:
		OutputManager_t(TestPieceManager_t*);
		~OutputManager_t();
		void SaveData();
	private:
		TestPieceManager_t* TestPieceManager;
		FILE* RemFractures;
	};
	OutputManager_t* OutputManager;
	char** FractureBuf;
	unsigned int Line;
	unsigned int RecordCount;

	char** SurfaceBuf;
	unsigned int SurfaceRecordCount;

	TestPiece_t** TestPieces;
	unsigned int TestPieceCount;
};

template <class Ty>
Container_t<Ty>::Container_t(unsigned int Size1, unsigned int Size2)
{
	Element = new Ty * [Size1];
	for (auto x = 0; x < Size1; x++)
	{
		Element[x] = new Ty[Size2];
		for (auto y = 0; y < Size2; y++)
			Element[x][y] = 0;
	}
}

