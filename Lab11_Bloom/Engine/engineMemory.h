#pragma once

void *AlignedMalloc(size_t size, size_t align);
void AlignedFree(void *inMem);
bool AlignedMalloc_UnitTest();