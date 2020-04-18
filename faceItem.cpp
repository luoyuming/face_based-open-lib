#include "faceItem.h"



CFaceItem::CFaceItem()
{
	faceFeat.featureSize = 0;
	faceFeat.feature = nullptr;
}


CFaceItem::~CFaceItem()
{
	if (faceFeat.feature)
		SafeFree(faceFeat.feature);
}
