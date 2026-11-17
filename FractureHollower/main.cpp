#include "TestPiece.h"
#include <cstring>

// Global overrides set from command line
char g_SurfacePath[512] = "";
double g_ZOffsetUm = -1;   // -1 = use constexpr default
double g_SurfaceRotateDeg = 0; // surface rotation angle in degrees
char g_FracturesDir[512] = "";
char g_FracturesFile[512] = "";

int main(int argc, char* argv[])
{
	// Usage: FractureHollower.exe [F2Report|F3Report] [ZOffsetUm] [RotateDeg] [FracturesDir] [FracturesFile]
	if (argc >= 2) {
		// argv[1] may be either a report name (F2Report) or a full/partial path
		if (strchr(argv[1], '\\') || strchr(argv[1], '/')) {
			snprintf(g_SurfacePath, sizeof(g_SurfacePath), "%s", argv[1]);
		} else {
			snprintf(g_SurfacePath, sizeof(g_SurfacePath), "data\\surfaces\\%s\\", argv[1]);
		}
		printf("Using surface: %s\n", g_SurfacePath);
	}
	if (argc >= 3) {
		g_ZOffsetUm = atof(argv[2]);
		printf("ZOffsetUm override: %.0f um\n", g_ZOffsetUm);
	}
	if (argc >= 4) {
		g_SurfaceRotateDeg = atof(argv[3]);
		printf("Surface rotation: %.1f deg\n", g_SurfaceRotateDeg);
	}
	if (argc >= 5) {
		snprintf(g_FracturesDir, sizeof(g_FracturesDir), "%s", argv[4]);
		printf("Fractures directory: %s\n", g_FracturesDir);
	}
	if (argc >= 6) {
		snprintf(g_FracturesFile, sizeof(g_FracturesFile), "%s", argv[5]);
		printf("Fractures file: %s\n", g_FracturesFile);
	}

	auto Manager = new TestPieceManager_t;
	Manager->GetData();
	Manager->CreateTestPieces();
	Manager->StartTests();
	Manager->SaveResults();
	delete Manager;
}
