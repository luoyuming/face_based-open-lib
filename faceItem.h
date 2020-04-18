#pragma once
#include "common.h"
#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"


#define SafeFree(p) { if ((p)) free(p); (p) = NULL; }
#define SafeArrayDelete(p) { if ((p)) delete [] (p); (p) = NULL; } 
#define SafeDelete(p) { if ((p)) delete (p); (p) = NULL; } 

static const float FACE_THRESOLD_VALUE = 0.6f;


class CFaceItem
{
public:
	CFaceItem();
	~CFaceItem();

	ASF_SingleFaceInfo faceInfo;
	ASF_FaceFeature    faceFeat;
};

