///============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:		Color3.h
//	Purpose: Color structure with RGB. Supports adding and blending colors.
//          Includes a method to clamp to [0,1] range.
//============================================================================

#ifndef __COLOR3_H__
#define __COLOR3_H__

#include <math.h>

struct Color4;

/**
 * Color structure. Colors are specified as red, green, and blue values
 * with range [0.0-1.0].
 * Note: methods no longer clamp to [0,1] range. It is the responsibility
 * of the app to call Clamp when needed. This is an efficiency consideration
 * to support ray-tracing.
 */
struct Color3
{
	float r, g, b;

	/**
    * Constructor.  Values default to 0,0,0.
    */
   Color3(void) : r(0.0f), g(0.0f), b(0.0f) { }

   /**
    * Constructor.  Set RGB to specified values. Clamps to range [0.0, 1.0]
    *
    * @param	ir		Red intensity
	 * @param	ig		Green intensity
	 * @param	ib		Blue intensity
    */
   Color3(const float ir, const float ig, const float ib)
   {
      r = ir;
      g = ig;
      b = ib;
   }

	/**
    * Copy constructor.
    * @param	c	Color assigned to member.
    */
   Color3(const Color3& c)
   {
      r = c.r;
      g = c.g;
      b = c.b;
   }

   /**
    * Copy constructor given an RGBA color (ignores alpha).
    * @param	c	Color assigned to member.
    */
   Color3(const Color4& c);

	/**
    * Assignment operator.
    * @param	c	Color to assign to the object.
    * @return	Returns the address of the member data.
    */
   Color3& operator = (const Color3& c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		return *this;
	}

	/**
    * Destructor
    */
	~Color3(void) { };

   /**
    *	Set the color to the specified RGB values.
	 * @param	ir		Red intensity
	 * @param	ig		Green intensity 
	 * @param	ib		Blue intensity
    */
	void Set(const float ir, const float ig, const float ib)
	{
		r = ir;
		g = ig;
		b = ib;
	}

   /**
    *	Get the color components.
	 * @param	ir		Red intensity (return)
	 * @param	ig		Green intensity (return)
	 * @param	ib		Blue intensity (return)
    */
	void Get(float& ir, float& ig, float& ib) const
   {
      ir = r;
      ig = g;
      ib = b;
   }

   /**
    * Get the red value in the range 0-255
    */
   unsigned char GetRed() const
   {
      return (unsigned char)(r * 255.0f);
   }

   /**
    * Get the green value in the range 0-255
    */
   unsigned char GetGreen() const
   {
      return (unsigned char)(g * 255.0f);
   }

   /**
    * Get the blue value in the range 0-255
    */
   unsigned char GetBlue() const
   {
      return (unsigned char)(b * 255.0f);
   }

   /**
    * Multiplication operator: Multiplies the color by another color
    */
   Color3 operator * (const Color3& color) const
   {
      return Color3(r * color.r, g * color.g, b * color.b);
   }

   /**
    * Multiplication operator: Multiplies the color by an RGBA color.
    * Ignores alpha.
    * @return  Returns RGB color
    */
   inline Color3 operator * (const Color4& color) const;

   /**
    * Scales the color by a scalar factor.
    */
   Color3 operator * (const float factor)
   {
      return Color3(r * factor, g * factor, b * factor);
   }

   /**
    * Adds another color to the current color. Clamps to the valid range.
    */
   Color3& operator += (const Color3& color)
   {
      r += color.r;
      g += color.g;
      b += color.b;
      return *this;
   }

   /**
    * Creates a new color that is the current color plus the
    * specified color.
    *
    * @param   c  Color to add to the current color.
    *
    * @return  Returns the resulting color.
    */
	Color3 operator + (const Color3& c) const
	{
		return Color3(r + c.r, g + c.g, b + c.b);
	}

  // Clamps a color to the range [0.0, 1.0]
   void Clamp(void)
   {
      if (r <= 0.0f)
         r = 0.0f;
      else if (r >= 1.0f)
         r = 1.0f;
      if (g <= 0.0f)
         g = 0.0f;
      else if (g >= 1.0f)
         g = 1.0f;      
      if (b <= 0.0f)
         b = 0.0f;
      else if (b >= 1.0f)
         b = 1.0f;
   }
};

#endif
