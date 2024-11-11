#pragma once

#include "datatypes/simd.hpp"
#include "enumerations/dimension.hpp"
#include "enumerations/medium.hpp"
#include <Kokkos_Core.hpp>

namespace specfem {
namespace point {
/**
 * @brief Store frechet kernels for a quadrature point
 *
 * @tparam DimensionType Dimension of the element where the quadrature point is
 * located
 * @tparam MediumTag Medium of the element where the quadrature point is located
 * @tparam PropertyTag  Property of the element where the quadrature point is
 * located
 * @tparam UseSIMD  Use SIMD instructions
 */
template <specfem::dimension::type DimensionType,
          specfem::element::medium_tag MediumTag,
          specfem::element::property_tag PropertyTag, bool UseSIMD>
struct kernels;

/**
 * @brief Template specialization for the kernels struct for 2D elastic
 * isotropic elements
 *
 * @tparam UseSIMD  Use SIMD instructions
 */
template <bool UseSIMD>
struct kernels<specfem::dimension::type::dim2,
               specfem::element::medium_tag::elastic,
               specfem::element::property_tag::isotropic, UseSIMD> {
public:
  /**
   * @name Typedefs
   *
   */
  ///@{
  using simd =
      typename specfem::datatype::simd<type_real, UseSIMD>; ///< SIMD type
  using value_type =
      typename simd::datatype; ///< Underlying data type to store the kernels
  ///@}

public:
  /**
   * @name Compile time constants
   *
   */
  ///@{
  constexpr static auto medium_tag = specfem::element::medium_tag::elastic;
  constexpr static auto property_tag =
      specfem::element::property_tag::isotropic;
  constexpr static auto dimension = specfem::dimension::type::dim2;
  ///@}

  /**
   * @name Misfit Kernels
   *
   */
  ///@{
  value_type rho;   ///< \f$ K_{\rho} \f$
  value_type mu;    ///< \f$ K_{\mu} \f$
  value_type kappa; ///< \f$ K_{\kappa} \f$
  value_type alpha; ///< \f$ K_{\alpha} \f$
  value_type beta;  ///< \f$ K_{\beta} \f$
  value_type rhop;  ///< \f$ K_{\rho'} \f$
  ///@}

  /**
   * @name Constructors
   *
   */
  ///@{
  /**
   * @brief Default constructor
   *
   */
  KOKKOS_FUNCTION
  kernels() = default;

  /**
   * @brief single value constructor
   *
   */
  KOKKOS_FUNCTION
  kernels(const value_type value)
      : rho(value), mu(value), kappa(value), alpha(value), beta(value),
        rhop(value) {}

  /**
   * @brief Constructor
   *
   * @param rho \f$ K_{\rho} \f$
   * @param mu \f$ K_{\mu} \f$
   * @param kappa \f$ K_{\kappa} \f$
   * @param rhop \f$ K_{\rho'} \f$
   * @param alpha \f$ K_{\alpha} \f$
   * @param beta \f$ K_{\beta} \f$
   */
  KOKKOS_FUNCTION
  kernels(const value_type rho, const value_type mu, const value_type kappa,
          const value_type rhop, const value_type alpha, const value_type beta)
      : rho(rho), mu(mu), kappa(kappa), rhop(rhop), alpha(alpha), beta(beta) {}

  ///@}

  /**
   * @brief Equality operator
   *
   */
  KOKKOS_FUNCTION
  bool operator==(const kernels &rhs) const {
    return rho == rhs.rho && mu == rhs.mu && kappa == rhs.kappa &&
           rhop == rhs.rhop && alpha == rhs.alpha && beta == rhs.beta;
  }

  /**
   * @brief Inequality operator
   *
   */
  KOKKOS_FUNCTION
  bool operator!=(const kernels &rhs) const { return !(*this == rhs); }

  KOKKOS_FUNCTION
  bool operator==(const value_type value) {
    return rho == value && mu == value && kappa == value && rhop == value &&
           alpha == value && beta == value;
  }

  KOKKOS_FUNCTION
  bool operator!=(const value_type value) { return !(*this == value); }
};

/**
 * @brief Template specialization for the kernels struct for 2D acoustic
 * isotropic elements
 *
 * @tparam UseSIMD  Use SIMD instructions
 */
template <bool UseSIMD>
struct kernels<specfem::dimension::type::dim2,
               specfem::element::medium_tag::acoustic,
               specfem::element::property_tag::isotropic, UseSIMD> {
public:
  /**
   * @name Typedefs
   *
   */
  ///@{
  using simd = typename specfem::datatype::simd<type_real, UseSIMD>; ///< SIMD
                                                                     ///< type
  using value_type = typename simd::datatype; ///< Underlying data type to store
                                              ///< the kernels
  ///@}

public:
  /**
   * @name Compile time constants
   *
   */
  ///@{
  constexpr static auto medium_tag = specfem::element::medium_tag::acoustic;
  constexpr static auto property_tag =
      specfem::element::property_tag::isotropic;
  constexpr static auto dimension = specfem::dimension::type::dim2;
  ///@}

public:
  /**
   * @name Misfit Kernels
   *
   */
  ///@{
  value_type rho;   ///< \f$ K_{\rho} \f$
  value_type kappa; ///< \f$ K_{\kappa} \f$
  value_type rhop;  ///< \f$ K_{\rho'} \f$
  value_type alpha; ///< \f$ K_{\alpha} \f$
  ///@}

  /**
   * @name Constructors
   *
   */
  ///@{
  /**
   * @brief Default constructor
   *
   */
  KOKKOS_FUNCTION
  kernels() = default;

  /**
   * @brief single value constructor
   *
   */
  KOKKOS_FUNCTION
  kernels(const value_type value)
      : rho(value), kappa(value), rhop(value), alpha(value) {}

  /**
   * @brief Constructor
   *
   * @param rho \f$ K_{\rho} \f$
   * @param kappa \f$ K_{\kappa} \f$
   */
  KOKKOS_FUNCTION
  kernels(const value_type rho, const value_type kappa)
      : rho(rho), kappa(kappa) {
    rhop = rho * kappa;
    alpha = static_cast<type_real>(2.0) * kappa;
  }
  ///@}

  /**
   * @brief Equality operator
   *
   */
  KOKKOS_FUNCTION
  bool operator==(const kernels &rhs) const {
    return rho == rhs.rho && kappa == rhs.kappa && rhop == rhs.rhop &&
           alpha == rhs.alpha;
  }

  /**
   * @brief Inequality operator
   *
   */
  KOKKOS_FUNCTION
  bool operator!=(const kernels &rhs) const { return !(*this == rhs); }

  KOKKOS_FUNCTION
  bool operator==(const value_type value) {
    return rho == value && kappa == value && rhop == value && alpha == value;
  }

  KOKKOS_FUNCTION
  bool operator!=(const value_type value) { return !(*this == value); }
};

} // namespace point
} // namespace specfem