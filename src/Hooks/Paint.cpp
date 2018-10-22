#include "hooks.h"

inline StartDrawingFn StartDrawing;
inline FinishDrawingFn FinishDrawing;

void __fastcall Hooks::Paint(void* thisptr, int, PaintMode_t mode)
{
	engineVGuiVMT->GetOriginalMethod<PaintFn>(41)(thisptr, mode);

	if (mode & PAINT_UIPANELS)
	{
		//StartDrawing(surface);

			// SURFACE DRAWING HERE

		//FinishDrawing(surface);
	}
}
