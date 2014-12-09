//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:		MeshTeapot.h
//	Purpose:	Construction of the Utah teapot using recursive subdivision.
//============================================================================


#ifndef __MESHTEAPOT_H__
#define __MESHTEAPOT_H__

typedef float point[3];
point TeapotVertexList[] = {
         {1.4f , 0.0f , 2.4f},
         {1.4f , -0.784f , 2.4f},
         {0.784f , -1.4f , 2.4f},
         {0.0f , -1.4f , 2.4f},
         {1.3375f , 0.0f , 2.53125f},
         {1.3375f , -0.749f , 2.53125f},
         {0.749f , -1.3375f , 2.53125f},
         {0.0f , -1.3375f , 2.53125f},
         {1.4375f , 0.0f , 2.53125f},
         {1.4375f , -0.805f , 2.53125f},
         {0.805f , -1.4375f , 2.53125f},
         {0.0f , -1.4375f , 2.53125f},
         {1.5f , 0.0f , 2.4f},
         {1.5f , -0.84f , 2.4f},
         {0.84f , -1.5f , 2.4f},
         {0.0f , -1.5f , 2.4f},
         {-0.784f , -1.4f , 2.4f},
         {-1.4f , -0.784f , 2.4f},
         {-1.4f , 0.0f , 2.4f},
         {-0.749f , -1.3375f , 2.53125f},
         {-1.3375f , -0.749f , 2.53125f},
         {-1.3375f , 0.0f , 2.53125f}, 
         {-0.805f , -1.4375f , 2.53125f},
         {-1.4375f , -0.805f , 2.53125f}, 
         {-1.4375f , 0.0f , 2.53125f},
         {-0.84f , -1.5f , 2.4f},
         {-1.5f , -0.84f , 2.4f},
         {-1.5f , 0.0f , 2.4f}, 
         {-1.4f , 0.784f , 2.4f},
         {-0.784f , 1.4f , 2.4f}, 
         {0.0f , 1.4f , 2.4f},
         {-1.3375f , 0.749f , 2.53125f},
         {-0.749f , 1.3375f , 2.53125f},
         {0.0f , 1.3375f , 2.53125f}, 
         {-1.4375f , 0.805f , 2.53125f},
         {-0.805f , 1.4375f , 2.53125f}, 
         {0.0f , 1.4375f , 2.53125f},
         {-1.5f , 0.84f , 2.4f},
         {-0.84f , 1.5f , 2.4f},
         {0.0f , 1.5f , 2.4f}, 
         {0.784f , 1.4f , 2.4f},
         {1.4f , 0.784f , 2.4f}, 
         {0.749f , 1.3375f , 2.53125f},
         {1.3375f , 0.749f , 2.53125f},
         {0.805f , 1.4375f , 2.53125f},
         {1.4375f , 0.805f , 2.53125f}, 
         {0.84f , 1.5f , 2.4f},
         {1.5f , 0.84f , 2.4f}, 
         {1.75f , 0.0f , 1.875},
         {1.75f , -0.98f , 1.875},
         {0.98f , -1.75f , 1.875},
         {0.0f , -1.75f , 1.875}, 
         {2.0f , 0.0f , 1.35f},
         {2.0f , -1.12f , 1.35f}, 
         {1.12f , -2.0f , 1.35f},
         {0.0f , -2.0f , 1.35f},
         {2.0f , 0.0f , 0.9f},
         {2.0f , -1.12f , 0.9f}, 
         {1.12f , -2.0f , 0.9f},
         {0.0f , -2.0f , 0.9f}, 
         {-0.98f , -1.75f , 1.875f},
         {-1.75f , -0.98f , 1.875f},
         {-1.75f , 0.0f , 1.875f},
         {-1.12f , -2.0f , 1.35f}, 
         {-2.0f , -1.12f , 1.35f},
         {-2.0f , 0.0f , 1.35f}, 
         {-1.12f , -2.0f , 0.9f},
         {-2.0f , -1.12f , 0.9f},
         {-2.0f , 0.0f , 0.9f},
         {-1.75f , 0.98f , 1.875f}, 
         {-0.98f , 1.75f , 1.875f},
         {0.0f , 1.75f , 1.875f}, 
         {-2.0f , 1.12f , 1.35f},
         {-1.12f , 2.0f , 1.35f},
         {0.0f , 2.0f , 1.35f},
         {-2.0f , 1.12f , 0.9f}, 
         {-1.12f , 2.0f , 0.9f},
         {0.0f , 2.0f , 0.9f}, 
         {0.98f , 1.75f , 1.875f},
         {1.75f , 0.98f , 1.875f},
         {1.12f , 2.0f , 1.35f},
         {2.0f , 1.12f , 1.35f}, 
         {1.12f , 2.0f , 0.9f},
         {2.0f , 1.12f , 0.9f}, 
         {2.0f , 0.0f , 0.45f},
         {2.0f , -1.12f , 0.45f},
         {1.12f , -2.0f , 0.45f},
         {0.0f , -2.0f , 0.45f}, 
         {1.5f , 0.0f , 0.225f},
         {1.5f , -0.84f , 0.225f}, 
         {0.84f , -1.5f , 0.225f},
         {0.0f , -1.5f , 0.225f},
         {1.5f , 0.0f , 0.15f},
         {1.5f , -0.84f , 0.15f}, 
         {0.84f , -1.5f , 0.15f},
         {0.0f , -1.5f , 0.15f}, 
         {-1.12f , -2.0f , 0.45f},
         {-2.0f , -1.12f , 0.45f},
         {-2.0f , 0.0f , 0.45f},
         {-0.84f , -1.5f , 0.225f}, 
         {-1.5f , -0.84f , 0.225f},
         {-1.5f , 0.0f , 0.225f}, 
         {-0.84f , -1.5f , 0.15f},
         {-1.5f , -0.84f , 0.15f},
         {-1.5f , 0.0f , 0.15f},
         {-2.0f , 1.12f , 0.45f}, 
         {-1.12f , 2.0f , 0.45f},
         {0.0f , 2.0f , 0.45f}, 
         {-1.5f , 0.84f , 0.225f},
         {-0.84f , 1.5f , 0.225f},
         {0.0f , 1.5f , 0.225f},
         {-1.5f , 0.84f , 0.15f}, 
         {-0.84f , 1.5f , 0.15f},
         {0.0f , 1.5f , 0.15f}, 
         {1.12f , 2.0f , 0.45f},
         {2.0f , 1.12f , 0.45f},
         {0.84f , 1.5f , 0.225f},
         {1.5f , 0.84f , 0.225f}, 
         {0.84f , 1.5f , 0.15f},
         {1.5f , 0.84f , 0.15f}, 
         {-1.6f , 0.0f , 2.025f},
         {-1.6f , -0.3f , 2.025f},
         {-1.5f , -0.3f , 2.25f},
         {-1.5f , 0.0f , 2.25f}, 
         {-2.3f , 0.0f , 2.025f},
         {-2.3f , -0.3f , 2.025f}, 
         {-2.5f , -0.3f , 2.25f},
         {-2.5f , 0.0f , 2.25f},
         {-2.7f , 0.0f , 2.025f},
         {-2.7f , -0.3f , 2.025f}, 
         {-3.0f , -0.3f , 2.25f},
         {-3.0f, 0.0f , 2.25f}, 
         {-2.7f , 0.0f , 1.8f},
         {-2.7f , -0.3f , 1.8f},
         {-3.0f , -0.3f , 1.8f},
         {-3.0f , 0.0f , 1.8f}, 
         {-1.5f , 0.3f , 2.25f},
         {-1.6f , 0.3f , 2.025f}, 
         {-2.5f , 0.3f , 2.25f},
         {-2.3f , 0.3f , 2.025f},
         {-3.0f , 0.3f , 2.25f},
         {-2.7f , 0.3f , 2.025f}, 
         {-3.0f , 0.3f , 1.8f},
         {-2.7f , 0.3f , 1.8f}, 
         {-2.7f , 0.0f , 1.575f},
         {-2.7f , -0.3f , 1.575f},
         {-3.0f , -0.3f , 1.35f},
         {-3.0f , 0.0f , 1.35f},
         {-2.5f , 0.0f , 1.125f},
         {-2.5f , -0.3f , 1.125f}, 
         {-2.65f , -0.3f , 0.9375f},
         {-2.65f , 0.0f , 0.9375f},
         {-2.0f , -0.3f , 0.9f},
         {-1.9f , -0.3f , 0.6f}, 
         {-1.9f , 0.0f , 0.6f},
         {-3.0f , 0.3f , 1.35f}, 
         {-2.7f , 0.3f , 1.575f},
         {-2.65f , 0.3f , 0.9375},
         {-2.5f , 0.3f , 1.125},
         {-1.9f , 0.3f , 0.6f}, 
         {-2.0f , 0.3f , 0.9f},
         {1.7f , 0.0f , 1.425f}, 
         {1.7f , -0.66f , 1.425f},
         {1.7f , -0.66f , 0.6f},
         {1.7f , 0.0f , 0.6f},
         {2.6f , 0.0f , 1.425f}, 
         {2.6f , -0.66f , 1.425f},
         {3.1f , -0.66f , 0.825f}, 
         {3.1f , 0.0f , 0.825f},
         {2.3f , 0.0f , 2.1f},
         {2.3f , -0.25f , 2.1f},
         {2.4f , -0.25f , 2.025f}, 
         {2.4f , 0.0f , 2.025f},
         {2.7f , 0.0f , 2.4f}, 
         {2.7f , -0.25f , 2.4f},
         {3.3f , -0.25f , 2.4f},
         {3.3f , 0.0f , 2.4f},
         {1.7f , 0.66f , 0.6f}, 
         {1.7f , 0.66f , 1.425f},
         {3.1f , 0.66f , 0.825f}, 
         {2.6f , 0.66f , 1.425f},
         {2.4f , 0.25f , 2.025f},
         {2.3f , 0.25f , 2.1f},
         {3.3f , 0.25f , 2.4f}, 
         {2.7f , 0.25f , 2.4f},
         {2.8f , 0.0f , 2.475f}, 
         {2.8f , -0.25f , 2.475f},
         {3.525f , -0.25f , 2.49375f},
         {3.525f , 0.0f , 2.49375f},
         {2.9f , 0.0f , 2.475f}, 
         {2.9f , -0.15f , 2.475f},
         {3.45f , -0.15f , 2.5125f}, 
         {3.45f , 0.0f , 2.5125f},
         {2.8f , 0.0f , 2.4f},
         {2.8f , -0.15f , 2.4f},
         {3.2f , -0.15f , 2.4f}, 
         {3.2f , 0.0f , 2.4f},
         {3.525f , 0.25f , 2.49375f}, 
         {2.8f , 0.25f , 2.475f},
         {3.45f , 0.15f , 2.5125f},
         {2.9f , 0.15f , 2.475f},
         {3.2f , 0.15f , 2.4f},
         {2.8f , 0.15f , 2.4f},
         {0.0f , 0.0f , 3.15f}, 
         {0.0f , -0.002f , 3.15f},
         {0.002f , 0.0f , 3.15f},
         {0.8f , 0.0f , 3.15f},
         {0.8f , -0.45f , 3.15f}, 
         {0.45f , -0.8f , 3.15f},
         {0.0f , -0.8f , 3.15f}, 
         {0.0f , 0.0f , 2.85f},
         {0.2f , 0.0f , 2.7f},
         {0.2f , -0.112f , 2.7f},
         {0.112f , -0.2f , 2.7f}, 
         {0.0f , -0.2f , 2.7f},
         {-0.002f , 0.0f , 3.15f}, 
         {-0.45f , -0.8f , 3.15f},
         {-0.8f , -0.45f , 3.15f},
         {-0.8f , 0.0f , 3.15f},
         {-0.112f , -0.2f , 2.7f}, 
         {-0.2f , -0.112f , 2.7f},
         {-0.2f , 0.0f , 2.7f}, 
         {0.0f , 0.002f , 3.15f},
         {-0.8f , 0.45f , 3.15f},
         {-0.45f , 0.8f , 3.15f},
         {0.0f , 0.8f , 3.15f}, 
         {-0.2f , 0.112f , 2.7f},
         {-0.112f , 0.2f , 2.7f}, 
         {0.0f , 0.2f , 2.7f},
         {0.45f , 0.8f , 3.15f},
         {0.8f , 0.45f , 3.15f},
         {0.112f , 0.2f , 2.7f}, 
         {0.2f , 0.112f , 2.7f},
         {0.4f , 0.0f , 2.55f}, 
         {0.4f , -0.224f , 2.55f},
         {0.224f , -0.4f , 2.55f},
         {0.0f , -0.4f , 2.55f},
         {1.3f , 0.0f , 2.55f}, 
         {1.3f , -0.728f , 2.55f},
         {0.728f , -1.3f , 2.55f}, 
         {0.0f , -1.3f , 2.55f},
         {1.3f , 0.0f , 2.4f},
         {1.3f , -0.728f , 2.4f},
         {0.728f , -1.3f , 2.4f}, 
         {0.0f , -1.3f , 2.4f},
         {-0.224f , -0.4f , 2.55f}, 
         {-0.4f , -0.224f , 2.55f},
         {-0.4f , 0.0f , 2.55f},
         {-0.728f , -1.3f , 2.55f},
         {-1.3f , -0.728f , 2.55f}, 
         {-1.3f , 0.0f , 2.55f},
         {-0.728f , -1.3f , 2.4f}, 
         {-1.3f , -0.728f , 2.4f},
         {-1.3f , 0.0f , 2.4f},
         {-0.4f , 0.224f , 2.55f},
         {-0.224f , 0.4f , 2.55f},
         {0.0f , 0.4f , 2.55f},
         {-1.3f , 0.728f , 2.55f}, 
         {-0.728f , 1.3f , 2.55f},
         {0.0f , 1.3f , 2.55f},
         {-1.3f , 0.728f , 2.4f},
         {-0.728f , 1.3f , 2.4f}, 
         {0.0f , 1.3f , 2.4f},
         {0.224f , 0.4f , 2.55f}, 
         {0.4f , 0.224f , 2.55f},
         {0.728f , 1.3f , 2.55f},
         {1.3f , 0.728f , 2.55f},
         {0.728f , 1.3f , 2.4f}, 
         {1.3f , 0.728f , 2.4f},
         {0.0f , 0.0f , 0.0f}, 
         {1.5f , 0.0f , 0.15f},
         {1.5f , 0.84f , 0.15f},
         {0.84f , 1.5f , 0.15f},
         {0.0f , 1.5f , 0.15f},
         {1.5f , 0.0f , 0.075f},
         {1.5f , 0.84f , 0.075f},
         {0.84f , 1.5f , 0.075f},
         {0.0f , 1.5f , 0.075f},
         {1.425f , 0.0f , 0.0f},
         {1.425f , 0.798f , 0.0f},
         {0.798f , 1.425f , 0.0f},
         {0.0f , 1.425f , 0.0f},
         {-0.84f , 1.5f , 0.15f},
         {-1.5f , 0.84f , 0.15f},
         {-1.5f , 0.0f , 0.15f},
         {-0.84f , 1.5f , 0.075f},
         {-1.5f , 0.84f , 0.075f},
         {-1.5f , 0.0f , 0.075f},
         {-0.798f , 1.425f , 0.0f},
         {-1.425f , 0.798f , 0.0f},
         {-1.425f , 0.0f , 0.0f},
         {-1.5f , -0.84f , 0.15f},
         {-0.84f , -1.5f , 0.15f},
         {0.0f , -1.5f , 0.15f},
         {-1.5f , -0.84f , 0.075f},
         {-0.84f , -1.5f , 0.075f},
         {0.0f , -1.5f , 0.075f},
         {-1.425f , -0.798f , 0.0f},
         {-0.798f , -1.425f , 0.0f},
         {0.0f , -1.425f , 0.0f},
         {0.84f , -1.5f , 0.15f},
         {1.5f , -0.84f , 0.15f},
         {0.84f , -1.5f , 0.075f},
         {1.5f , -0.84f , 0.075f},
         {0.798f , -1.425f , 0.0f},
         {1.425f , -0.798f , 0.0f}  };

// 32 patches are each defined by 16 vertices, arranged in a 4 x 4 array
// Numbering scheme for teapot has vertices labeled from 1 to 306
int PatchIndices[32][4][4]=  {
   { {1, 2, 3, 4}, { 5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}},
   {{4, 17, 18, 19}, {8, 20, 21, 22}, {12, 23, 24, 25}, {16, 26, 27, 28}},
   {{19, 29, 30, 31}, {22, 32, 33, 34}, {25, 35, 36, 37}, {28, 38, 39, 40}},
   {{31, 41, 42, 1}, {34, 43, 44, 5}, {37, 45, 46, 9}, {40, 47, 48, 13}},
   {{13, 14, 15, 16}, {49, 50, 51, 52}, {53, 54, 55, 56}, {57, 58, 59, 60}},
   {{16, 26, 27, 28}, {52, 61, 62, 63}, {56, 64, 65, 66}, {60, 67, 68, 69}},
   {{28, 38, 39, 40}, {63, 70, 71, 72}, {66, 73, 74, 75}, {69, 76, 77, 78}},
   {{40, 47, 48, 13}, {72, 79, 80, 49}, {75, 81, 82, 53}, {78, 83, 84, 57}},
   {{57, 58, 59, 60}, {85, 86, 87, 88}, {89, 90, 91, 92}, {93, 94, 95, 96}},
   {{60, 67, 68, 69}, {88, 97, 98, 99}, {92, 100, 101, 102}, {96, 103, 104, 105}},
   {{69, 76, 77, 78}, {99, 106, 107, 108}, {102, 109, 110, 111}, {105, 112, 113, 114}},
   {{78, 83, 84, 57}, {108, 115, 116, 85}, {111, 117, 118, 89}, {114, 119, 120, 93}},
   {{121, 122, 123, 124}, {125, 126, 127, 128}, {129, 130, 131, 132}, {133, 134, 135, 136}},
   {{124, 137, 138, 121}, {128, 139, 140, 125}, {132, 141, 142, 129}, {136, 143, 144, 133}},
   {{133, 134, 135, 136}, {145, 146, 147, 148}, {149, 150, 151, 152}, {69, 153, 154, 155}},
   {{136, 143, 144, 133}, {148, 156, 157, 145}, {152, 158, 159, 149}, {155, 160, 161, 69}},
   {{162, 163, 164, 165}, {166, 167, 168, 169}, {170, 171, 172, 173}, {174, 175, 176, 177}},
   {{165, 178, 179, 162}, {169, 180, 181, 166}, {173, 182, 183, 170}, {177, 184, 185, 174}},
   {{174, 175, 176, 177}, {186, 187, 188, 189}, {190, 191, 192, 193}, {194, 195, 196, 197}},
   {{177, 184, 185, 174}, {189, 198, 199, 186}, {193, 200, 201, 190}, {197, 202, 203, 194}},
   {{204, 204, 204, 204}, {207, 208, 209, 210}, {211, 211, 211, 211}, {212, 213, 214, 215}},
   {{204, 204, 204, 204}, {210, 217, 218, 219}, {211, 211, 211, 211}, {215, 220, 221, 222}},
   {{204, 204, 204, 204}, {219, 224, 225, 226}, {211, 211, 211, 211}, {222, 227, 228, 229}},
   {{204, 204, 204, 204}, {226, 230, 231, 207}, {211, 211, 211, 211}, {229, 232, 233, 212}},
   {{212, 213, 214, 215}, {234, 235, 236, 237}, {238, 239, 240, 241}, {242, 243, 244, 245}},
   {{215, 220, 221, 222}, {237, 246, 247, 248}, {241, 249, 250, 251}, {245, 252, 253, 254}},
   {{222, 227, 228, 229}, {248, 255, 256, 257}, {251, 258, 259, 260}, {254, 261, 262, 263}},
   {{229, 232, 233, 212}, {257, 264, 265, 234}, {260, 266, 267, 238}, {263, 268, 269, 242}},
   {{270, 270, 270, 270}, {279, 280, 281, 282}, {275, 276, 277, 278}, {271, 272, 273, 274}},
   {{270, 270, 270, 270}, {282, 289, 290, 291}, {278, 286, 287, 288}, {274, 283, 284, 285}},
   {{270, 270, 270, 270}, {291, 298, 299, 300}, {288, 295, 296, 297}, {285, 292, 293, 294}},
   {{270, 270, 270, 270}, {300, 305, 306, 279}, {297, 303, 304, 275}, {294, 301, 302, 271}}
};

/**
 *
 */
class MeshTeapot : public TriSurface
{
public:
   /**
    * Constructs the Utah teapot using uniform patch subdivision.  Reads in the 
    * patch vertices and indices into a Point3 array.  Recursively subdivides and store
    *	patches into a mesh surface.
    * @param      level    Number of levels to subdivide the patches
    */
   MeshTeapot(int level, const int positionLoc, const int normalLoc, const int textureLoc)
   {
      m_subdivisions = level;

      // Data is 32 patches, each with a 4x4 array of point[3].
      //	Convert array data into Point3 array.
      int m, patch;
      Point3 data[32][4][4];
      for (patch = 0; patch < 32; patch++)
      {
         for (int j = 0; j < 4; j++)
         {
            for (int k = 0; k < 4; k++)
            {
               // Put teapot data into single array for subdivision
               m = PatchIndices[patch][j][k];
               data[patch][j][k].x = TeapotVertexList[m-1][0];
               data[patch][j][k].y = TeapotVertexList[m-1][1];
               data[patch][j][k].z = TeapotVertexList[m-1][2];
            }
         }
      }

      // Subdivide all 32 patches
      for (patch = 0; patch < 32; patch++)
         dividePatch(data[patch], level);

      // End the mesh - construct vertex normals by averaging
      End(positionLoc, normalLoc, textureLoc);
   }

protected:
   unsigned int m_subdivisions;

   /**
    * Convenience function to transpose the u,v elements of the patch.  This
    * will allow the same subdivide curve code to be used when subdividing along
    * the v direction.
    *
    * @param   patch[4][4]    Patch (4x4 array of Point3) to transpose
    */
   void transpose(Point3 patch[4][4])
   {
      float tt;
      for (int i = 0; i < 4; i++)
      {
         for (int j = i; j < 4; j++)
         {
            tt = patch[i][j].x;
            patch[i][j].x = patch[j][i].x;
            patch[j][i].x = tt;

            tt = patch[i][j].y;
            patch[i][j].y = patch[j][i].y;
            patch[j][i].y = tt;

            tt = patch[i][j].z;
            patch[i][j].z = patch[j][i].z;
            patch[j][i].z = tt;
         }
      }
   }

   /**
    * Divides the curve c into a left and right subdivision curve.
    *
    * @param   c     Curve (4 control points) to subdivide
    * @param   r     Subdivided curve to the right
    * @param   l     Subdivided curve to the left
    */
   void divideCurve(Point3 c[4], Point3 r[4], Point3 l[4])
   {
      // Midpoint of original control points 1,2
      Point3 t = c[1].affineCombination(0.5f, 0.5f, c[2]);

      // Subdivide curve into left and right curves
      l[0] = c[0];
      r[3] = c[3];
      l[1] = c[0].affineCombination(0.5f, 0.5f, c[1]);
      r[2] = c[2].affineCombination(0.5f, 0.5f, c[3]);
      l[2] = t.affineCombination(0.5f, 0.5f, l[1]);
      r[1] = t.affineCombination(0.5f, 0.5f, r[2]);
      l[3] = l[2].affineCombination(0.5f, 0.5f, r[1]);
      r[0] = l[3];
   }


   /**
    * Divides a patch represented by a 4x4 set of Point3 vertices.  Each patch
    * is subdivided into 4 patches.  If the recursion level is 0, the patch is
    * stored in the mesh surface.
    *
    * @param   a     Patch (4x4 array of Point3) to transpose
    * @param   n     Current level of subdivision
    */
   void dividePatch(Point3 patch[4][4], int n)
   {
      // Store patch if recursion is done
      if (n == 0)
      {
         // Stores the patch as 2 triangles using the 4 corners of the patch.
         if (m_subdivisions % 2 == 1) 
         {
            // Odd number of subdivisions - make sure we store ccw
            Add(patch[0][0], patch[3][0], patch[3][3]);
            Add(patch[0][0], patch[3][3], patch[0][3]);
         }
         else 
         {
            // Even number of subdivisions - make sure we store ccw
            Add(patch[3][3], patch[3][0], patch[0][0]);
            Add(patch[0][3], patch[3][3], patch[0][0]);
         }
         return;
      }

      // Subdivide curves in u direction.
	   Point3 left[4][4];
      Point3 right[4][4];
      for (int row = 0; row < 4; row++)
          divideCurve(patch[row], left[row], right[row]);

      // Transpose the two resulting results, divide in u direction again
      // (equivalent to subdivision in v).  This will create 4 patches.
      transpose(left);
      transpose(right);
	   Point3 q[4][4];
      Point3 r[4][4];
      Point3 s[4][4];
      Point3 t[4][4];
      for (int row = 0; row < 4; row++)
      {
         divideCurve(left[row],  q[row], r[row]);
         divideCurve(right[row], s[row], t[row]);
      }

      // Recursive division of 4 resulting patches.  Decrement the level
      dividePatch(q, n-1);
      dividePatch(r, n-1);
      dividePatch(s, n-1);
      dividePatch(t, n-1);
   }
};

#endif