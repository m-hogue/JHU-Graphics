//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    Camera.h
//	Purpose: Provide support for setting and updating the view and projection.
//
//============================================================================

#ifndef __CAMERA_H
#define __CAMERA_H

enum ProjectionType 	{ PERSPECTIVE, ORTHOGRAPHIC };

/**
 * Camera node class.  
 */
class CameraNode : public SceneNode
{
public:
	/**
	 * Constructor. 
	 */
	CameraNode()
	{
		m_nodeType = SCENE_CAMERA;
		
		// Perspective settings
		m_fov    = 50.0f;
		m_aspect = 1.0f;
		m_near   = 1.0f;
		m_far    = 1000.0f;

		// Initial view settings
		m_lpt    = Point3(0.0f, 0.0f, 0.0f);
		m_vrp    = Point3(0.0f, 0.0f, 1.0f);
		m_v      = Vector3(0.0f, 1.0f, 0.0f);
	}

	/**
	 * Destructor.
	 */
	~CameraNode() { }
	
	/**
	 * Draw the scene node and its children. The base class just draws the
	 * children. Derived classes can use this (SceneNode::Draw()) to draw
	 * all children without having to duplicate this code.
	 */
	void Draw(SceneState& sceneState)
	{
		// Copy the current composite projection and viewing matrix to the scene state
		sceneState.m_pvMatrix = m_projection * m_view;

      // Set the shader PVM matrix - this will allow drawing children without a TransformNode
      glUniformMatrix4fv(sceneState.m_pvmLoc, 1, GL_FALSE, sceneState.m_pvMatrix.Get());

      // Set the camera position
      glUniform3fv(sceneState.m_cameraPositionLoc, 1, &m_vrp.x);

		// Draw children
		SceneNode::Draw(sceneState);
	}
	
	/**
	 * Sets the view reference point (camera position)
 	 *	@param	vp		View reference point.
	 */
	void SetPosition(const Point3& vp)
	{
		m_vrp = vp;
		lookAt();
	}

	/**
	 * Gets the view reference point (camera position)
	 *	@return		Returns the view reference point.
	 */
	Point3 GetPosition(void) const
	{
		return m_vrp;
	}

	/**
	 * Sets the lookat point.
	 * @param	lp		Lookat point.
	 */
	void SetLookAtPt(const Point3& lp)
	{
		m_lpt = lp;
		lookAt();
	}

	/**
	 * Gets the lookat point.
	 * @return	Returns the lookat point.
	 */
	Point3 GetLookAtPt(void) const
	{
		return m_lpt;
	}

	/**
	 * Sets the view up direction.
	 * @param	vup		View up direction.
	 */
	void SetViewUp(const Vector3& vup)
	{
		m_v = vup;
		lookAt();
	}

	/**
	 *	Gets the view plane normal (vz axis).
	 * @return	Returns the view plane normal (vz)
	 */
	Vector3 GetViewPlaneNormal() const
	{
		return m_n;
	}

	/**
	 *	Gets the view right axis (vx axis).
	 * @return	Returns the view right axis (vx)
	 */
	Vector3 GetViewRight() const
	{
		return m_u;
	}

	/**
	 *	Gets the view up axis (vy axis).
	 * @return	Returns the view up axis (vy)
	 */
	Vector3 GetViewUp() const
	{
		return m_v;
	}

	/**
	 * Roll the camera by the specified degrees about the VPN. 
	 * Rotation about n. Note that the lookat point does not change
    * @param  degrees   Rotation angle (degrees)
	 */
	void Roll(const float degrees)
	{
		// Method below is described in FS Hill and lecture notes. Just 
		// change the two axes and reset the view matrix
		float radians = degreesToRadians(degrees);
		float c = cosf(radians);
		float s = sinf(radians);

		Vector3 u = m_u;
		m_u.Set( c * u.x + s * m_v.x, 
				 c * u.y + s * m_v.y, 
				 c * u.z + s * m_v.z);
		m_v.Set(-s * u.x + c * m_v.x, 
				-s * u.y + c * m_v.y, 
				-s * u.z + c * m_v.z);
		setViewMatrix();
	}

	/**
	 * Change the pitch of the camera by the specified degrees.
	 * This is a rotation about u
    * @param  degrees   Rotation angle (degrees)
	 */
	void Pitch(const float degrees)
	{
		// Method below is described in FS Hill and lecture notes. Just 
		// change the two axes and reset the view matrix
		float radians = (float)degreesToRadians(degrees);
		float c = cosf(radians);
		float s = sinf(radians);
		Vector3 v = m_v;
		m_v.Set( c * v.x + s * m_n.x, 
				 c * v.y + s * m_n.y, 
				 c * v.z + s * m_n.z);
		m_n.Set(-s * v.x + c * m_n.x, 
				-s * v.y + c * m_n.y, 
				-s * v.z + c * m_n.z);

		setViewMatrix();

		// Reset the lookat (keep the same distance)
		Vector3 v1 = m_vrp - m_lpt;
		float d = v1.Norm();
		m_lpt = m_vrp - m_n * d;
	}

	/**
	 * Change the heading of the camera by the specified degrees.
	 * This is a rotation about v
    * @param  degrees   Rotation angle (degrees)
	 */
	void Heading(const float degrees)
	{
		// Method below is described in FS Hill and lecture notes. Just 
		// change the two axes and reset the view matrix
		float radians = (float)degreesToRadians(degrees);
		float c = cosf(radians);
		float s = sinf(radians);
		Vector3 u = m_u;
		m_u.Set(c * u.x - s * m_n.x, 
				c * u.y - s * m_n.y, 
				c * u.z - s * m_n.z);
		m_n.Set(s * u.x + c * m_n.x, 
				s * u.y + c * m_n.y, 
				s * u.z + c * m_n.z);

		setViewMatrix();

		// Reset the lookat (keep the same distance)
		Vector3 v1 = m_vrp - m_lpt;
		float d = v1.Norm();
		m_lpt = m_vrp - m_n * d;
	}

	/**
	 * Move and Turn.  Move the LookAt point by the specified vector.
	 * The VRP is then moved so that it is the same distance from the 
	 * LookAt point as previous.  This effectively moves and turns.
	 *
	 * NOTE: a different method would involve a rotation using pitch and 
	 * heading and then move forward or backwards along the new VPN. I figure
	 * that method is more common and wanted to present the method below
	 * as an alternate way to approach the problem
	 */
	void MoveAndTurn(const float right, const float up, const float forward)
	{
		// Construct a vector from current lookat to VRP
		Vector3 v1 = m_vrp - m_lpt;
		float d = v1.Norm();

		// Move the lookAt point by specified distances along the 3 view axes.
		Vector3 dir = m_u * right + m_v * up + m_n * -forward;
		m_lpt = m_lpt + dir;

		// Construct vector from new LookAt to current VRP
		Vector3 v2 = m_vrp - m_lpt;

		// Find the projection of v1 onto v2. Keep same distance as previous
		Vector3 v3 = v1.Projection(v2);
		v3.Normalize();
		v3 *=d;

		// New VRP is the new lookat point plus v3
		m_vrp = m_lpt + v3;
		lookAt();
	}

   /**
    * Slide the camera in the specified lengths along the view axes.
    * @param  x   Amount to move along u
    * @param  y   Amount to move along v
    * @param  z   Amount to move along n
    */
   void Slide(const float x, const float y, const float z)
   {
      Vector3 v;
      v += m_u * x;
      v += m_v * y;
      v += m_n * z;  
      m_vrp = m_vrp + v;
      m_lpt = m_lpt + v;
      lookAt();

   }

   /**
    * Gets the current matrix (used to store modeling transforms).
    *	@return	Returns the current modeling/viewing composite matrix.
    */
	Matrix4x4 GetViewMatrix() const
	{
      return m_view;
   }

   /**
    * Sets a symmetric perspective projection
    * @param  fov    Field of view angle y (degrees)
    * @param  aspect Aspect ratio (width / height)
    * @param  n      Near plane distance (must be positive)
    * @param  f      Far plane distance (must be positive)
    */
   void SetPerspective(const float fov, const float aspect, 
                       const float n, const float f)
   {
      m_fov    = fov;
      m_aspect = aspect;
      m_near   = n;
      m_far    = f;
      setPerspective();
   }

   /**
    * Change the field of view.
    * @param  fov  Field of view angle y (degrees)
    */
   void ChangeFieldOfView(const float fov)
   {
      m_fov = fov;
      setPerspective();
   }

   /**
    * Change the aspect ratio.
    * @param  aspect  Aspect ratio (width / height)
    */
   void ChangeAspectRatio(const float aspect)
   {
      m_aspect = aspect;
      setPerspective();
   }

   /**
    * Change the near and far clipping planes.
    * @param  n  Near plane distance (must be positive)
    * @param  f  Far plane distance (must be positive)
    */
   void ChangeClippingPlanes(const float n, const float f)
   {
      m_near = n;
      m_far  = f;
      setPerspective();
   }

private:
	// Perspective projection parameters
	float   m_fov;          // Field of view in degrees
	float   m_aspect;       // Aspect ratio (width / height)
	float   m_near;         // Near clipping plane distance
	float   m_far;          // Far clipping plane distance

	Point3  m_vrp;		      // View point (eye)
	Point3  m_lpt;		      // Lookat point

	// View axes
	Vector3  m_n;		      // View plane normal
	Vector3  m_u;		      // View right axis
	Vector3  m_v;		      // View up axis

	// Matrices
	Matrix4x4 m_view;	      // Viewing matrix
	Matrix4x4 m_projection; // Projection matrix

   // Sets the view axes
   void lookAt()
   {
  	   // Set the VPN, which is the vector vp - vc
	   m_n = m_vrp - m_lpt;
	   m_n.Normalize();

	   // Get the view right axis from UP cross VPN
	   m_u = m_v.Cross(m_n);
	   m_u.Normalize();

	   // Calculate the view up axis from VPN cross VRT. 
      // (No need to normalize here since VPN and VRT are orthogonal and unit length)
	   m_v = m_n.Cross(m_u);

      // Set the view matrix
      setViewMatrix();
	}

   // Sets the persective projection matrix
   void setPerspective()
   {
      // Get the dimensions at the near clipping plane
      float h = m_near * tanf(degreesToRadians(m_fov * 0.5f));
      float w = m_aspect * h;

      // Set the elements of the perspective projection matrix
      m_projection.m00() =  m_near / w;
      m_projection.m01() =  0.0f;
      m_projection.m02() =  0.0f;
      m_projection.m03() =  0.0f;
      m_projection.m10() =  0.0f;
      m_projection.m11() =  m_near / h;
      m_projection.m12() =  0.0f;
      m_projection.m13() =  0.0f;
      m_projection.m20() =  0.0f;
      m_projection.m21() =  0.0f;
      m_projection.m22() = -(m_far + m_near) / (m_far - m_near);
      m_projection.m23() = -(2.0f * m_far * m_near) / (m_far - m_near);
      m_projection.m30() =  0.0f;
      m_projection.m31() =  0.0f;
      m_projection.m32() = -1.0f;
      m_projection.m22() =  0.0f;
   }

	// Create viewing transformation matrix by composing the translation 
	// matrix with the rotation matrix given by the view coordinate axes
	void setViewMatrix()
	{
		// Set the view matrix using the view axes and the translation (simplified)
		float x = -m_vrp.x;
		float y = -m_vrp.y;
		float z = -m_vrp.z;
		m_view.m00() = m_u.x;
		m_view.m01() = m_u.y;
		m_view.m02() = m_u.z;
		m_view.m03() = m_u.x * x + m_u.y * y + m_u.z * z;
		m_view.m10() = m_v.x;
		m_view.m11() = m_v.y;
		m_view.m12() = m_v.z;
		m_view.m13() = m_v.x * x + m_v.y * y + m_v.z * z;
		m_view.m20() = m_n.x;
		m_view.m21() = m_n.y;
		m_view.m22() = m_n.z;
		m_view.m23() = m_n.x * x + m_n.y * y + m_n.z * z;
		m_view.m30() = 0.0f;
		m_view.m31() = 0.0f;
		m_view.m32() = 0.0f;
		m_view.m33() = 1.0f;
	}
};

#endif
