/**
 * Final project for CS 605.467 Computer Graphics Fall 2014
 *
 * author: Michael Hogue
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <vector>
#include <time.h>
#include <fcntl.h>
#include <io.h>

#include <GL/gl3w.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <IL/devil_internal_exports.h>

#include "geometry/geometry.h"
#include "ShaderSupport/GLSLShader.h"
#include "Scene/scene.h"

#include "LightingShaderNode.h"
#include "BallTransform.h"
#include "Fitting.h"

#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ILU.lib")
#pragma comment(lib, "ILUT.lib")

// While mouse button is down, the view will be updated
bool  Animate = false;
bool  Forward = true;
float Velocity = 1.0f;
int   MouseX;
int   MouseY;

// Bounding planes of the enclosure. Used for intersection testing.
std::vector<Plane> BoundingPlanes;

unsigned int currentNumBalls = 0;
const unsigned int MAX_NUMBER_OF_BALLS = 900;
// Ball transforms. We have this global so we can more easily do intersection testing
BallTransform* BallTransforms[MAX_NUMBER_OF_BALLS];
int numBallsToShoot = 1;
PresentationNode* ballColor;
float ballSpeed = 95.0f;

// Initial window dimensions
int RenderWidth = 640;
int RenderHeight = 480;

const float FrameRate = 72.0f;

// Scene root
SceneNode* SceneRoot;

// Global camera node. Use this as the root of the scene graph.
CameraNode* MyCamera;

// global transform to keep the shooter positioned as camera moves
TransformNode* shooterTransform;
Point3 shooterPosition;

// Keep the spotlight global so we can update its poisition
LightNode* Spotlight;

// Global scene state
SceneState MySceneState;

SphereSection* sphere;

// Simple logging function
void logmsg(const char *message, ...)
{
	// Open file if not already opened
	static FILE *lfile = NULL;
	if (lfile == NULL)
		lfile = fopen("PhongShading.log", "w");

	va_list arg;
	va_start(arg, message);
	vfprintf(lfile, message, arg);
	putc('\n', lfile);
	fflush(lfile);
	va_end(arg);
}

/**
* Convencience method to add a material, then a transform, then a geometry
* node as a child to a specified node.
*/
void AddSubTree(SceneNode* parent, SceneNode* material,
	SceneNode* transform, SceneNode* geometry)
{
	parent->AddChild(material);
	material->AddChild(transform);
	transform->AddChild(geometry);
}

/**
* Construct a unit box.
* @param  unitSquare  Geometry node to use
*/
SceneNode* ConstructUnitBox(UnitSquareSurface* unitSquare)
{
	// Contruct transform nodes for the sides of the box.
	// Perform rotations so the sides face outwards

	// Bottom is rotated 180 degrees so it faces outwards
	TransformNode* bottomTransform = new TransformNode;
	bottomTransform->Translate(0.0f, 0.0f, -0.5f);
	bottomTransform->Rotate(180.0f, 1.0f, 0.0f, 0.0f);

	// Back is rotated -90 degrees about x: (z -> y)
	TransformNode* backTransform = new TransformNode;
	backTransform->Translate(0.0f, 0.5f, 0.0f);
	backTransform->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);

	// Front wall is rotated 90 degrees about x: (y -> z)
	TransformNode* frontTransform = new TransformNode;
	frontTransform->Translate(0.0f, -0.5f, 0.0f);
	frontTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);

	// Left wall is rotated -90 about y: (z -> -x)
	TransformNode* leftTransform = new TransformNode;
	leftTransform->Translate(-0.5f, 0.0f, 00.0f);
	leftTransform->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);

	// Right wall is rotated 90 degrees about y: (z -> x)
	TransformNode* rightTransform = new TransformNode;
	rightTransform->Translate(0.5f, 0.0f, 0.0f);
	rightTransform->Rotate(90.0f, 0.0f, 1.0f, 0.0f);

	// Top 
	TransformNode* topTransform = new TransformNode;
	topTransform->Translate(0.0f, 0.0f, 0.50f);

	// Create a SceneNode and add the 6 sides of the box.
	SceneNode* box = new SceneNode;
	box->AddChild(backTransform);
	backTransform->AddChild(unitSquare);
	box->AddChild(leftTransform);
	leftTransform->AddChild(unitSquare);
	box->AddChild(rightTransform);
	rightTransform->AddChild(unitSquare);
	box->AddChild(frontTransform);
	frontTransform->AddChild(unitSquare);
	box->AddChild(bottomTransform);
	bottomTransform->AddChild(unitSquare);
	box->AddChild(topTransform);
	topTransform->AddChild(unitSquare);

	return box;
}

/**
* Construct room as a child of the specified node
* @param  parent      Parent node
* @param  unitSquare  Geometry node to use
*/
void ConstructRoom(SceneNode* parent, UnitSquareSurface* unitSquare)
{

	// Form bounding planes (for intersection testing)
	Plane p0(Point3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));
	BoundingPlanes.push_back(p0);
	Plane p1(Point3(0.0f, 0.0f, 80.0f), Vector3(0.0f, 0.0f, -1.0f));
	BoundingPlanes.push_back(p1);
	Plane p2(Point3(-100.0f, 0.0f, 50.0f), Vector3(1.0f, 0.0f, 0.0f));
	BoundingPlanes.push_back(p2);
	Plane p3(Point3(100.0f, 0.0f, 50.0f), Vector3(-1.0f, 0.0f, 0.0f));
	BoundingPlanes.push_back(p3);
	Plane p4(Point3(0.0f, 100.0f, 50.0f), Vector3(0.0f, -1.0f, 0.0f));
	BoundingPlanes.push_back(p4);
	Plane p5(Point3(0.0f, -100.0f, 50.0f), Vector3(0.0f, 1.0f, 0.0f));
	BoundingPlanes.push_back(p5);

	// Contruct transform nodes for the walls. Perform rotations so the 
	// walls face inwards
	TransformNode* floorTransform = new TransformNode;
	floorTransform->Scale(200.0f, 200.0f, 1.0f);

	// Back wall is rotated +90 degrees about x: (y -> z)
	TransformNode* backWallTransform = new TransformNode;
	backWallTransform->Translate(0.0f, 100.0f, 40.0f);
	backWallTransform->Rotate(90.0f, 1.0f, 0.0f, 0.0f);
	backWallTransform->Scale(200.0f, 80.0f, 1.0f);

	// Front wall is rotated -90 degrees about x: (z -> y)
	TransformNode* frontWallTransform = new TransformNode;
	frontWallTransform->Translate(0.0f, -100.0f, 40.0f);
	frontWallTransform->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	frontWallTransform->Scale(200.0f, 80.0f, 1.0f);

	// Left wall is rotated 90 degrees about y: (z -> x)
	TransformNode* leftWallTransform = new TransformNode;
	leftWallTransform->Translate(-100.0f, 0.0f, 40.0f);
	leftWallTransform->Rotate(90.0f, 0.0f, 1.0f, 0.0f);
	leftWallTransform->Scale(80.0f, 200.0f, 1.0f);

	// Right wall is rotated -90 about y: (z -> -x)
	TransformNode* rightWallTransform = new TransformNode;
	rightWallTransform->Translate(100.0f, 0.0f, 40.0f);
	rightWallTransform->Rotate(-90.0f, 0.0f, 1.0f, 0.0f);
	rightWallTransform->Scale(80.0f, 200.0f, 1.0f);

	// Ceiling is rotated 180 about x so it faces inwards
	TransformNode* ceilingTransform = new TransformNode;
	ceilingTransform->Translate(0.0f, 0.0f, 80.0f);
	ceilingTransform->Rotate(180.0f, 1.0f, 0.0f, 0.0f);
	ceilingTransform->Scale(200.0f, 200.0f, 1.0f);

	// Floor should be tan, mostly dull
	PresentationNode* floorMaterial = new PresentationNode;
	floorMaterial->SetTexture("../images/wooden-floor.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_TEXTURE0 + 4);
	floorMaterial->SetMaterialAmbientAndDiffuse(Color4(0.54f, 0.27f, 0.07f));
	floorMaterial->SetMaterialSpecular(Color4(0.1f, 0.1f, 0.1f));
	floorMaterial->SetMaterialShininess(2.0f);

	// Make the walls reddish, slightly shiny
	PresentationNode* wallMaterial = new PresentationNode;
	wallMaterial->SetTexture("../images/stone_wall1.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_TEXTURE0 + 5);
	wallMaterial->SetMaterialAmbientAndDiffuse(Color4(0.7f, 0.55f, 0.55f));
	wallMaterial->SetMaterialSpecular(Color4(0.5f, 0.5f, 0.5f));
	wallMaterial->SetMaterialShininess(16.0f);

	// Ceiling should be white, moderately shiny
	PresentationNode* ceilingMaterial = new PresentationNode;
	ceilingMaterial->SetMaterialAmbientAndDiffuse(Color4(1.0f, 1.0f, 1.0f));
	ceilingMaterial->SetMaterialSpecular(Color4(0.9f, 0.9f, 0.9f));
	ceilingMaterial->SetMaterialShininess(64.0f);

	// Add floor and ceiling to the parent. Use convenience method to add material,
	// then presentation, then geometry.
	AddSubTree(parent, floorMaterial, floorTransform, unitSquare);
	AddSubTree(parent, ceilingMaterial, ceilingTransform, unitSquare);

	// Walls. We can group these all under a single presentation node.
	parent->AddChild(wallMaterial);
	wallMaterial->AddChild(backWallTransform);
	backWallTransform->AddChild(unitSquare);
	wallMaterial->AddChild(leftWallTransform);
	leftWallTransform->AddChild(unitSquare);
	wallMaterial->AddChild(rightWallTransform);
	rightWallTransform->AddChild(unitSquare);
	wallMaterial->AddChild(frontWallTransform);
	frontWallTransform->AddChild(unitSquare);
}

/**
* Construct table
* @param  unitSquare  Geometry node to use for table top
* @param  legs        Geometry node to use for legs
* @return  Returns a scene node representing the table
*/
SceneNode* ConstructTable(SceneNode* box, ConicSurface* leg)
{
	// Table legs (relative to center of table)
	TransformNode* lfLegTransform = new TransformNode;
	lfLegTransform->Translate(-20.0f, -10.0f, 10.0f);
	lfLegTransform->Scale(3.0f, 3.0f, 20.0f);
	TransformNode* lrLegTransform = new TransformNode;
	lrLegTransform->Translate(-20.0f, 10.0f, 10.0f);
	lrLegTransform->Scale(3.0f, 3.0f, 20.0f);
	TransformNode* rfLegTransform = new TransformNode;
	rfLegTransform->Translate(20.0f, -10.0f, 10.0f);
	rfLegTransform->Scale(3.0f, 3.0f, 20.0f);
	TransformNode* rrLegTransform = new TransformNode;
	rrLegTransform->Translate(20.0f, 10.0f, 10.0f);
	rrLegTransform->Scale(3.0f, 3.0f, 20.0f);

	// Construct dimensions for the table top
	TransformNode* topTransform = new TransformNode;
	topTransform->Translate(0.0f, 0.0f, 23.0f);
	topTransform->Scale(60.0f, 30.0f, 6.0f);

	// Create the tree
	SceneNode* table = new SceneNode;
	table->AddChild(topTransform);
	topTransform->AddChild(box);
	table->AddChild(lfLegTransform);
	lfLegTransform->AddChild(leg);
	table->AddChild(rfLegTransform);
	rfLegTransform->AddChild(leg);
	table->AddChild(lrLegTransform);
	lrLegTransform->AddChild(leg);
	table->AddChild(rrLegTransform);
	rrLegTransform->AddChild(leg);

	return table;
}

/**
* Construct the scene
*/
void ConstructScene()
{
	// Construct the lighting shader node
	LightingShaderNode* lightingShader = new LightingShaderNode();
	if (!lightingShader->Create("phong.vert", "phong.frag") ||
		!lightingShader->GetLocations())
		exit(-1);

	int positionLoc = lightingShader->GetPositionLoc();
	int normalLoc = lightingShader->GetNormalLoc();
	int textureLoc = lightingShader->GetTextureCoordinateLoc();

	// -------------------- Geometry -------------------- //

	// Construct a unit square - use less subdivisions to see how
	// phong shading improves the lighting
	UnitSquareSurface* unitSquare = new UnitSquareSurface(2, positionLoc, normalLoc, textureLoc);

	// Construct a unit box
	SceneNode* box = ConstructUnitBox(unitSquare);

	// Construct a unit cylinder surface
	ConicSurface* cylinder = new ConicSurface(1.0f, 1.0f, 18, 4, positionLoc, normalLoc, textureLoc);

	// Construct a torus
	TorusSurface* torus = new TorusSurface(20.0f, 5.0f, 18, 18, positionLoc, normalLoc, textureLoc);

	// Construct a cylinder surface with two different radii
	ConicSurface* shooterCylinder = new ConicSurface(1.0f, 2.0f, 18, 4, positionLoc, normalLoc, textureLoc);

	// Teapot
	MeshTeapot* teapot = new MeshTeapot(3, positionLoc, normalLoc, textureLoc);

	// Sphere
	sphere = new SphereSection(-90.0f, 90.0f, 18,
		-180.0f, 180.0f, 36, 1.0f, positionLoc, normalLoc, textureLoc);

	// Construct a fitting
	Fitting* fitting = new Fitting(positionLoc, normalLoc, textureLoc);

	//-------------------- Materials ------------------------- //

	// Wood
	PresentationNode* wood = new PresentationNode;
	wood->SetTexture("../images/woodgrain.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_TEXTURE0 + 1);
	wood->SetMaterialAmbientAndDiffuse(Color4(0.55f, 0.45f, 0.15f));
	wood->SetMaterialSpecular(Color4(0.3f, 0.3f, 0.3f));
	wood->SetMaterialShininess(64.0f);

	// Silver
	PresentationNode* silver = new PresentationNode;
	silver->SetMaterialAmbient(Color4(0.19225f, 0.19225f, 0.19225f));
	silver->SetMaterialDiffuse(Color4(0.50754f, 0.50754f, 0.50754f));
	silver->SetMaterialSpecular(Color4(0.508273f, 0.508273f, 0.508273f));
	silver->SetMaterialShininess(51.2f);

	// Black, shiny
	PresentationNode* shinyBlack = new PresentationNode;
	shinyBlack->SetMaterialAmbient(Color4(0.0f, 0.0f, 0.0f));
	shinyBlack->SetMaterialDiffuse(Color4(0.01f, 0.01f, 0.01f));
	shinyBlack->SetMaterialSpecular(Color4(0.5f, 0.5f, 0.5f));
	shinyBlack->SetMaterialShininess(32.0f);

	// Shiny blue
	PresentationNode* shinyBlue = new PresentationNode;
	shinyBlue->SetMaterialAmbient(Color4(0.05f, 0.05f, 0.2f));
	shinyBlue->SetMaterialDiffuse(Color4(0.2f, 0.2f, 0.6f));
	shinyBlue->SetMaterialSpecular(Color4(0.75f, 0.75, 0.75f));
	shinyBlue->SetMaterialShininess(76.8f);


	// ------------------ Transformations ------------------- //

	// Position the table in the room
	TransformNode* tableTransform = new TransformNode;
	tableTransform->Translate(-50.0f, 50.0f, 0.0f);
	tableTransform->Rotate(30.0f, 0.0f, 0.0f, 1.0f);

	// Teapot transform
	TransformNode* teapotTransform = new TransformNode;
	teapotTransform->Translate(0.0f, 0.0f, 26.0f);
	teapotTransform->Scale(2.5f, 2.5f, 2.5f);

	// Torus
	TransformNode* torusTransform = new TransformNode;
	torusTransform->Translate(0.0f, 90.0f, 20.0f);
	torusTransform->Rotate(60.0f, 1.0f, 0.0f, 0.0f);

	// Sphere
	TransformNode* sphereTransform = new TransformNode;
	sphereTransform->Translate(80.0f, 20.0f, 10.0f);
	sphereTransform->Scale(10.0f, 10.0f, 10.0f);

	// Fitting
	TransformNode* fittingTransform = new TransformNode;
	fittingTransform->Translate(40.0f, 10.0f, 5.0f);
	fittingTransform->Rotate(180, 1.0f, 0.0f, 0.0f);
	fittingTransform->Scale(20.0f, 20.0f, 20.0f);


	// --------------------------- Camera ----------------------- //
	MyCamera = new CameraNode;
	MyCamera->SetPosition(Point3(0.0f, -100.0f, 20.0f));
	MyCamera->SetLookAtPt(Point3(0.0f, 0.0f, 20.0f));
	MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
	MyCamera->SetPerspective(50.0, 1.0, 1.0, 300.0);

	// shooter Cylinder
	shooterTransform = new TransformNode;
	Point3 pos = MyCamera->GetPosition();
	shooterPosition = Point3(pos.x + 5.0f, pos.y + 10.0f, pos.z - 5.0f);
	shooterTransform->Translate(shooterPosition.x, shooterPosition.y, shooterPosition.z);
	shooterTransform->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	shooterTransform->Scale(0.5f, 0.5f, 10.0f);

	// shooter ball color
	ballColor = new PresentationNode;
	ballColor->SetTexture("../images/golfball.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_TEXTURE0 + 2);
	ballColor->SetMaterialAmbientAndDiffuse(Color4(1.0f, 1.0f, 1.0f));
	ballColor->SetMaterialSpecular(Color4(0.808273f, 0.808273f, 0.808273f));
	ballColor->SetMaterialShininess(61.2f);

	PresentationNode* shooterMaterial = new PresentationNode;
	shooterMaterial->SetTexture("../images/shooter.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_TEXTURE0 + 3);
	shooterMaterial->SetMaterialAmbient(Color4(0.2f, 0.2f, 0.2f));
	shooterMaterial->SetMaterialDiffuse(Color4(0.01f, 0.01f, 0.01f));
	shooterMaterial->SetMaterialSpecular(Color4(0.5f, 0.5f, 0.5f));
	shooterMaterial->SetMaterialShininess(32.0f);

	// -------------------- Lighting --------------------------/

	// Set the global light ambient
	Color4 globalAmbient(0.4f, 0.4f, 0.4f, 1.0f);
	lightingShader->SetGlobalAmbient(globalAmbient);

	// Light 0 - point light source in back right corner
	LightNode* light0 = new LightNode(0);
	light0->SetDiffuse(Color4(0.5f, 0.5f, 0.5f, 1.0f));
	light0->SetSpecular(Color4(0.5f, 0.5f, 0.5f, 1.0f));
	light0->SetPosition(HPoint3(90.0f, 90.0f, 30.f, 1.0f));
	light0->Enable();

	// Light1 - directional light from the ceiling
	LightNode* light1 = new LightNode(1);
	light1->SetDiffuse(Color4(0.7f, 0.7f, 0.7f, 1.0f));
	light1->SetSpecular(Color4(0.7f, 0.7f, 0.7f, 1.0f));
	light1->SetPosition(HPoint3(0.0f, 0.0f, 1.0f, 0.0f));
	light1->Enable();

	// Spotlight - reddish spotlight - we will place at the camera location
	// shining along -VPN
	Spotlight = new LightNode(2);
	Spotlight->SetDiffuse(Color4(0.5f, 0.1f, 0.1f, 1.0f));
	Spotlight->SetSpecular(Color4(0.5f, 0.1f, 0.1f, 1.0f));
	Point3 camPos = MyCamera->GetPosition();
	Spotlight->SetPosition(HPoint3(camPos.x, camPos.y, camPos.z, 1.0f));
	Vector3 dir = MyCamera->GetViewPlaneNormal() * -1.0f;
	Spotlight->SetSpotlight(dir, 32.0f, 30.0f);
	Spotlight->Enable();

	// --------------------- Scene construction ----------------- //

	// Construct the scene root node
	SceneRoot = new SceneNode;
	SceneRoot->AddChild(lightingShader);
	lightingShader->AddChild(MyCamera);

	// Add the lights as the children of the camera
	MyCamera->AddChild(light0);
	light0->AddChild(light1);
	light1->AddChild(Spotlight);

	// Create a scene node to hold all scene objects (other than camera
	// and lights)
	SceneNode* myScene = new SceneNode;

	// Add the scene under the last light
	Spotlight->AddChild(myScene);

	// attach shooter under lighting
	myScene->AddChild(shooterMaterial);
	shooterMaterial->AddChild(shooterTransform);
	shooterTransform->AddChild(shooterCylinder);

	// attach ball color to scene root
	myScene->AddChild(ballColor);

	// Construct the room (walls, floor, ceiling)
	ConstructRoom(myScene, unitSquare);

	// Construct the table
	SceneNode* table = ConstructTable(box, cylinder);
	myScene->AddChild(wood);
	wood->AddChild(tableTransform);
	tableTransform->AddChild(table);

	// Place a teapot on the table
	tableTransform->AddChild(teapotTransform);
	teapotTransform->AddChild(silver);
	silver->AddChild(teapot);

	// Place a torus
	myScene->AddChild(shinyBlack);
	shinyBlack->AddChild(torusTransform);
	torusTransform->AddChild(torus);

	// Place a sphere
	PresentationNode* sphereTexture = new PresentationNode;
	sphereTexture->SetTexture("../images/dodgeball.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_TEXTURE0 + 6);
	sphereTexture->SetMaterialAmbient(Color4(0.4f, 0.1f, 0.05f));
	sphereTexture->SetMaterialDiffuse(Color4(0.8f, 0.1f, 0.1f));
	sphereTexture->SetMaterialSpecular(Color4(0.75f, 0.75, 0.75f));
	sphereTexture->SetMaterialShininess(76.8f);

	myScene->AddChild(sphereTexture);
	sphereTexture->AddChild(sphereTransform);
	sphereTransform->AddChild(sphere);

	// Fitting material
	PresentationNode* fittingMaterial = new PresentationNode;
	fittingMaterial->SetTexture("../images/brushed_copper.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR, GL_TEXTURE0 + 7);
	fittingMaterial->SetMaterialAmbient(Color4(0.19225f, 0.19225f, 0.19225f));
	fittingMaterial->SetMaterialDiffuse(Color4(0.70754f, 0.70754f, 0.70754f));
	fittingMaterial->SetMaterialSpecular(Color4(0.708273f, 0.708273f, 0.708273f));
	fittingMaterial->SetMaterialShininess(71.2f);

	// place fitting
	myScene->AddChild(fittingMaterial);
	fittingMaterial->AddChild(fittingTransform);
	fittingTransform->AddChild(fitting);

}

// update the shooter position based on camera position
void updateShooter() {
	Point3 pos = MyCamera->GetPosition();
	// recalculate translation
	shooterTransform->LoadIdentity();
	shooterPosition = Point3(pos.x + 5.0f, pos.y + 10.0f, pos.z - 5.0f);
	shooterTransform->Translate(shooterPosition.x, shooterPosition.y, shooterPosition.z);
	Vector3 dir = Vector3(Point3(), MyCamera->GetLookAtPt());

	//float angle = radiansToDegrees(Vector3(1.0f, 1.0f, 1.0f).AngleBetween(dir));
	//fprintf(stderr, "angle: %f\n", angle);
	//shooterTransform->Rotate(-angle, 1.0f, 1.0f, 1.0f);
	shooterTransform->Rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	shooterTransform->Scale(0.5f, 0.5f, 10.0f);
}

// Update the spotlight based on camera position change
void UpdateSpotlight()
{
	Point3 pos = MyCamera->GetPosition();
	Spotlight->SetPosition(HPoint3(pos.x, pos.y, pos.z, 1.0f));
	Vector3 dir = MyCamera->GetViewPlaneNormal() * -1.0f;
	Spotlight->SetSpotlightDirection(dir);
}

/**
* Use a timer method to try to do a consistent update rate or 72Hz.
* Without using a timer, the speed of movement will depend on how fast
* the program runs (fast movement on a fast PC and slow movement on a
* slower PC)
*/
void timerFunction(int value)
{
	// If mouse button is down, generate another view
	if (Animate)
	{
		// Find relative dx and dy relative to center of the window
		float dx = 4.0f * ((MouseX - ((float)RenderWidth * 0.5f)) / (float)RenderWidth);
		float dy = 4.0f * ((((float)RenderHeight * 0.5f) - MouseY) / (float)RenderHeight);
		float dz = (Forward) ? Velocity : -Velocity;
		MyCamera->MoveAndTurn(dx * Velocity, dy * Velocity, dz);

		MyCamera->Update(MySceneState);
		UpdateSpotlight();
		updateShooter();
	}

	// Initialize all balls to have no intersection
	for (unsigned int i = 0; i < currentNumBalls % MAX_NUMBER_OF_BALLS; i++)
		BallTransforms[i]->SetIntersectTime(0.0f);

	// Go through all balls and test for intersection with subsequent ball
	for (unsigned int i = 0; i < currentNumBalls % MAX_NUMBER_OF_BALLS; i++)
	{
		// If intersection with a prior ball is not found, test for intersection with successive balls
		if (BallTransforms[i]->GetIntersectTime() == 0.0f)
		{
			for (unsigned int j = i + 1; j < currentNumBalls % MAX_NUMBER_OF_BALLS; j++)
			{
				// If an intersection occurs, break out of loop. We will only worry about a ball intersecting
				// one other in a single frame and won't care much if it is the closest
				if (BallTransforms[i]->IntersectBall(BallTransforms[j]))
					break;
			}
		}
	}

	// Go through all ball and test for plane intersection on those that do not intersect with another ball
	for (unsigned int i = 0; i < currentNumBalls % MAX_NUMBER_OF_BALLS; i++)
	{
		// Check for collision with any planes
		if (BallTransforms[i]->GetIntersectTime() == 0.0f)
		{
			float t = 0.0f;
			float smallestT = 1.0f;
			std::vector<Plane>::iterator plane = BoundingPlanes.begin();
			Plane intersectPlane;
			for (; plane != BoundingPlanes.end(); plane++)
			{
				t = BallTransforms[i]->IntersectWithPlane(*plane);
				if (t < smallestT)
				{
					// Copy the nearest intersection and the plane of intersection
					smallestT = t;
					intersectPlane = *plane;
				}
			}
			if (smallestT != 1.0f)
			{
				BallTransforms[i]->SetIntersectTime(smallestT);
				BallTransforms[i]->SetIntersectPlane(intersectPlane);
			}
		}
	}

	// update all balls in the scene if we haven't already updated the scene due to being in animate mode
	if (!Animate)
		ballColor->Update(MySceneState);

	glutPostRedisplay();

	glutTimerFunc((int)(1000.0f / FrameRate), timerFunction, 0);
}

/**
* Display callback function
*/
void display(void)
{
	// Clear the framebuffer and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Initialize the scene state and draw the scene graph
	MySceneState.Init();
	SceneRoot->Draw(MySceneState);

	// Swap buffers
	glutSwapBuffers();
}

// method invoked when a ball is shot
void shootBalls(){

	for (int i = 0; i < numBallsToShoot; i++){
		BallTransform* newBall = new BallTransform(FrameRate);
		newBall->SetPosition(shooterPosition);
		newBall->SetSpeed(ballSpeed);
		Point3 lap = MyCamera->GetLookAtPt();
		// generate random numbers in the range [-20,20] to offset ball direction
		lap.x += (rand() % 40 - 20) * i;
		lap.y += (rand() % 40 - 20) * i;
		lap.z += (rand() % 40 - 20) * i;
		newBall->SetDirection(Vector3(shooterPosition, lap));
		newBall->SetRadius(1.5f);
		newBall->setTransform();

		// delete oldest balls when we exceed 30 balls alive
		if (currentNumBalls >= MAX_NUMBER_OF_BALLS) {
			delete BallTransforms[currentNumBalls % MAX_NUMBER_OF_BALLS];
		}
		BallTransforms[currentNumBalls % MAX_NUMBER_OF_BALLS] = newBall;
		currentNumBalls += 1;

		ballColor->AddChild(newBall);
		newBall->AddChild(sphere);
	}
}

/**
* Keyboard callback.
*/
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		// Escape key
	case 27:
		delete MyCamera;
		exit(0);
		break;

	case '1':
		numBallsToShoot = 1;
		break;

	case '2':
		numBallsToShoot = 2;
		break;

	case '3':
		numBallsToShoot = 3;
		break;

	case '4':
		numBallsToShoot = 4;
		break;

	case '5':
		numBallsToShoot = 5;
		break;

	case '6':
		numBallsToShoot = 6;
		break;

	case '7':
		numBallsToShoot = 7;
		break;

	case '8':
		numBallsToShoot = 8;
		break;

	case '9':
		numBallsToShoot = 9;
		break;

	case '0':
		numBallsToShoot = 0;
		break;

		// Reset the view
	case 'i':
		MyCamera->SetPosition(Point3(0.0f, -100.0f, 20.0f));
		MyCamera->SetLookAtPt(Point3(0.0f, 0.0f, 20.0f));
		MyCamera->SetViewUp(Vector3(0.0, 0.0, 1.0));
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// Roll the camera by 5 degrees
	case 'r':
		MyCamera->Roll(5);
		glutPostRedisplay();
		break;

		// Roll the camera by 5 degrees (clockwise)
	case 'R':
		MyCamera->Roll(-5);
		glutPostRedisplay();
		break;

		// Change the pitch of the camera by 5 degrees
	case 'p':
		MyCamera->Pitch(5);
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// Change the pitch of the camera by 5 degrees (clockwise)
	case 'P':
		MyCamera->Pitch(-5);
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// Change the heading of the camera by 5 degrees
	case 'h':
		MyCamera->Heading(5);
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// Change the heading of the camera by 5 degrees (clockwise)
	case 'H':
		MyCamera->Heading(-5);
		UpdateSpotlight();
		glutPostRedisplay();
		break;

		// slide camera right
	case 'X':
		MyCamera->Slide(5.0f, 0.0f, 0.0f);
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// slide camera left
	case 'x':

		MyCamera->Slide(-5.0f, 0.0f, 0.0f);
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// slide camera up
	case 'Y':
		MyCamera->Slide(0.0f, 5.0f, 0.0f);
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// slide camera down
	case 'y':

		MyCamera->Slide(0.0f, -5.0f, 0.0f);
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// move camera forward
	case 'F':
		MyCamera->Slide(0.0f, 0.0f, 5.0f);
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// move camera backward
	case 'f':
		MyCamera->Slide(0.0f, 0.0f, -5.0f);
		UpdateSpotlight();
		updateShooter();
		glutPostRedisplay();
		break;

		// Go faster
	case 'V':
		Velocity += 0.2f;
		break;

		// Go slower
	case 'v':
		Velocity -= 0.2f;
		if (Velocity < 0.2f)
			Velocity = 0.1f;
		break;

	case 's':
		shootBalls();
		glutPostRedisplay();
		break;

	default:
		break;
	}
}

/**
* Mouse callback (called when a mouse button state changes)
*/
void mouse(int button, int state, int x, int y)
{
	// On a left button up event MoveAndTurn the view with forward motion
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			MouseX = x;
			MouseY = y;
			Forward = true;
			Animate = true;
		}
		else
			Animate = false;  // Disable animation when the button is released
	}

	// On a right button up event MoveAndTurn the view with reverse motion
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			MouseX = x;
			MouseY = y;
			Forward = false;
			Animate = true;
		}
		else
			Animate = false;  // Disable animation when the button is released
	}
}

/**
* Mouse motion callback (called when mouse button is depressed)
*/
void mouseMotion(int x, int y)
{
	// Update position used for changing the view and force a new view
	MouseX = x;
	MouseY = y;
}

/**
* Reshape callback.  Reset the perpective projection so the field of
* view matches the window's aspect ratio.
*/
void reshape(int width, int height)
{
	RenderWidth = width;
	RenderHeight = height;

	// Reset the viewport
	glViewport(0, 0, width, height);

	// Reset the perspective projection to reflect the change of 
	// the aspect ratio 
	MyCamera->ChangeAspectRatio((float)width / (float)height);
}

/**
* Main	
*/
int main(int argc, char** argv)
{
	// Print the keyboard commands
	printf("i - Reset to initial view\n");
	printf("R - Roll    5 degrees clockwise   r - Counter-clockwise\n");
	printf("P - Pitch   5 degrees clockwise   p - Counter-clockwise\n");
	printf("H - Heading 5 degrees clockwise   h - Counter-clockwise\n");
	printf("X - Slide camera right            x - Slide camera left\n");
	printf("Y - Slide camera up               y - Slide camera down\n");
	printf("F - Move camera forward           f - Move camera backwards\n");
	printf("V - Faster mouse movement         v - Slower mouse movement\n");
	printf("s - Shoot Balls --- Use Number keys [0-9] to set the number of balls to shoot at one time.\n\n\n");

	// Initialize free GLUT
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);  // Using this causes LineWidth to error
	glutInitContextProfile(GLUT_CORE_PROFILE);

	// Set up depth buffer and double buffering
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);

	// Set callback methods
	glutCreateWindow("Final Project by Michael Hogue");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);

	/* initialize random seed: */
	srand(time(NULL));

	// Initialize Open 3.2 core profile
	if (gl3wInit()) {
		fprintf(stderr, "gl3wInit: failed to initialize OpenGL\n");
		return -1;
	}
	if (!gl3wIsSupported(3, 2)) {
		fprintf(stderr, "OpenGL 3.2 not supported\n");
		return -1;
	}
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Set the clear color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// initialize DevIL library
	ilInit();

	// Enable the depth buffer
	glEnable(GL_DEPTH_TEST);

	// set static update interval
	glutTimerFunc((int)(1000.0f / FrameRate), timerFunction, 0);

	// Enable back face polygon removal
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// Enable mutlisample anti-aliasing
	glEnable(GL_MULTISAMPLE);

	// Construct scene
	ConstructScene();

	glutMainLoop();
	return 0;
}
