
#pragma once

#include <algorithm>

namespace Math {

	/**
	 * Templated absolute value function
	 * @param a The value to get the absolute value from
	 * @return The absolute value
	 */
	template <typename T>
	inline T abs(T a) {
		return a < 0 ? -a : a;
	}

	/**
	 * Templated signum function
	 * @param a The value to get the signum of
	 * @return The signum: 1 for positive, -1 for negative, 0 for zero
	 */
	template <typename T>
	inline T sign(T a) {
		return (a > 0) ? static_cast<T>(1) : (a < 0) ? static_cast<T>(-1) : 0;
	}

	/**
	 * Checks for zero value
	 * @param a The value to check for zero
	 * @param t The maximum deviation from zero
	 * @return True if the value is zero, False otherwise
	 */
	template <typename T>
	inline bool isZero(T a, T t) {
		return (Math::abs(a) <= t) ? true : false;
	}

	/**
	 * Checks for equality / closeness
	 * @param a The first value to compare
	 * @param b The second value to compare
	 * @param t The maximum deviation to consider the values equal
	 * @return True if the values are equal, False otherwise
	 */
	template <typename T>
	inline bool equal(T a, T b, T t) {
		return Math::abs(a - b) <= t;
	}

	/**
	 * Linearly interpolates between a and b, using parameter t.
	 * @param a A value.
	 * @param b Another value.
	 * @param t A blending factor of a and b.
	 * @return Linear interpolation of a and b - a value between a and b if t is between 0 and 1.
	 */
	template <typename T, typename S>
	inline T lerp(T a, T b, S t) {
		return static_cast<T>((static_cast<S>(1) - t) * a + t * b);
	}

	/**
	* Gives the interpolation factor for value m between a and b.
	* @param a A value.
	* @param b Another value.
	* @param t A blending factor of a and b.
	* @return Inverse linear interpolation between a and b - a value between 0 and 1 if m is between a and b
	*/
	template <typename T>
	inline T lerpFactor(T a, T b, T m) {
		return (m - a) / (b - a);
	}

	/**
	* Interpolates using Barycentric factors between a, b and c, using parameters s and t.
	* @param a First value.
	* @param b Second value.
	* @param b Third value.
	* @param s Weight of the second value.
	* @param t Weight of the third value.
	* @return Barycentric interpolation between a, b and c - a value in the range of a, b and c if s, t and s + t is between 0 and 1.
	*/
	//! Barycentric interpolation.
	template <typename T, typename S>
	inline T bary(T a, T b, T c, S s, S t) {
		return (static_cast<S>(1) - s - t) * a + s * b + t * c;
	}

	/** Clamps a double between two bounds.
	 * @param a The value to clamp.
	 * @param l The lower bound.
	 * @param h The upper bound.
	 * @return The clamped value of a.
     * <b>Note:</b> This function has been specially crafted to prevent NaNs from propagating.
	*/
	template <typename T>
	inline T clamp(T a, T l, T h) {
		return (a >= l) ? ((a <= h) ? a : h) : l;
	}

	/**
	 * Returns a modulus b.
	 * @param a The value to compute the modulus of.
	 * @param b The modulus argument.
	 * @return a modulus b.
	 * @see % operator
	 * <b>Note:</b> Contrary to the % operator this always returns non-negative values.
	 */
	template <typename T>
	inline T mod(T a, T b) {
		int n = static_cast<int>(a/b);
		a -= n*b;
		if (a < 0)
			a += b;
		return a;
	}

	/**
	* Returns the fractional part of a.
	* This is also the value modulo 1.
	* @param a The value to get the fractional of.
	* @return Fractional part of a.
	* @see % operator, Math::mod
	* <b>Note:</b> Contrary to the mod function this does return negative values.
	*/
	template <typename T>
	inline T mod1(T a) {
		return a - static_cast<int>(a);
	}

	/**
	 * Computes the square of x
	 * @param x The value to square
	 * @return The square of x
	 */
	template <typename T> inline T pow2 (T x) { return x*x; }
	/**
	* Computes x to the power of 3
	* @param x The value to multiply
	* @return The power of 3 of x
	*/
	template <typename T> inline T pow3 (T x) { return x*x*x; }
	/**
	* Computes x to the power of 4
	* @param x The value to multiply
	* @return The power of 4 of x
	*/
	template <typename T> inline T pow4 (T x) { T x2 = x*x; return x2*x2; }
	/**
	* Computes x to the power of 5
	* @param x The value to multiply
	* @return The power of 5 of x
	*/
	template <typename T> inline T pow5 (T x) { T x2 = x*x; return x2*x2*x; }

} // namespace Math

/**
 * Momentarely unused but might come in handy at some point.
 * <b>Note:</b> Was earlier in namespace std. But as this is not allowed it was changed to namespace Math.
 */
namespace Math {
	/**
	 * Proxy for std::min with three arguments
	 * @param a First argument to compare
	 * @param b Second argument to compare
	 * @param c Third argument to compare
	 * @see std::min
	 * @return The minimum of all arguments
	 */
	template <typename T>
	inline const T& min(const T& a, const T& b, const T& c) {
		return std::min(std::min(a, b), c);
	}

	/**
	* Proxy for std::min with four arguments
	* @param a First argument to compare
	* @param b Second argument to compare
	* @param c Third argument to compare
	* @param d Fourth argument to compare
	* @see std::min
	* @return The minimum of all arguments
	*/
	template <typename T>
	inline const T& min(const T& a, const T& b, const T& c, const T& d) {
		return std::min(std::min(a, b), std::min(c, d));
	}

	/**
	* Proxy for std::min with five arguments
	* @param a First argument to compare
	* @param b Second argument to compare
	* @param c Third argument to compare
	* @param d Fourth argument to compare
	* @param e Fifth argument to compare
	* @see std::min
	* @return The minimum of all arguments
	*/
	template <typename T>
	inline const T& min(const T& a, const T& b, const T& c, const T& d, const T& e) {
		return std::min(std::min(a, b, c, d), e);
	}

	/**
	* Proxy for std::max with three arguments
	* @param a First argument to compare
	* @param b Second argument to compare
	* @param c Third argument to compare
	* @see std::max
	* @return The maximum of all arguments
	*/
	template <typename T>
	inline const T& max(const T& a, const T& b, const T& c) {
		return std::max(std::max(a, b), c);
	}

	/**
	* Proxy for std::max with four arguments
	* @param a First argument to compare
	* @param b Second argument to compare
	* @param c Third argument to compare
	* @param d Fourth argument to compare
	* @see std::max
	* @return The maximum of all arguments
	*/
	template <typename T>
	inline const T& max(const T& a, const T& b, const T& c, const T& d) {
		return std::max(std::max(a, b), std::max(c, d));
	}

	/**
	* Proxy for std::max with five arguments
	* @param a First argument to compare
	* @param b Second argument to compare
	* @param c Third argument to compare
	* @param d Fourth argument to compare
	* @param e Fifth argument to compare
	* @see std::max
	* @return The maximum of all arguments
	*/
	template <typename T>
	inline const T& max(const T& a, const T& b, const T& c, const T& d, const T& e) {
		return std::max(std::max(a, b, c, d), e);
	}
} // namespace std