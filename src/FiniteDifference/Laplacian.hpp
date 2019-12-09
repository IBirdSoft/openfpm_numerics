/*
 * Laplacian.hpp
 *
 *  Created on: Oct 5, 2015
 *      Author: i-bird
 *  Modified on: Dec 09, 2019
 *      Author: amfoggia
 */

#ifndef OPENFPM_NUMERICS_SRC_FINITEDIFFERENCE_LAPLACIAN_HPP_
#define OPENFPM_NUMERICS_SRC_FINITEDIFFERENCE_LAPLACIAN_HPP_

#include "FD_util_include.hpp"
#include "util/util_num.hpp"
#include "FiniteDifference/eq.hpp"

/*! \brief Laplacian second order on h (spacing)
 *
 * \tparam Field which field derive
 * \tparam impl which implementation
 *
 */
template<typename expr_type, unsigned int impl=CENTRAL>
class Lap
{

  typedef typename expr_type::sys_eqs_type Sys_eqs; /**< System of equations. */
  typedef Sys_eqs sys_eqs_type;

  expr_type expr;

  Lap() {}

  Lap(expr_type expr_) : expr{expr_} {}

  /*! \brief Calculate which colums of the Matrix are non zero
   *
   * stub_or_real it is just for change the argument type when testing, in normal
   * conditions it is a distributed map
   *
   * \param g_map map grid
   * \param kmap position in the grid
   * \param spacing of the grid
   * \param gs Grid info
   * \param cols non-zero colums calculated by the function
   * \param coeff coefficent (constant in front of the derivative)
   *
   * ### Example
   *
   * \snippet FDScheme_unit_tests.hpp Laplacian usage
   *
   */
  inline void value(const typename stub_or_real<Sys_eqs,Sys_eqs::dims,typename Sys_eqs::stype,typename Sys_eqs::b_grid::decomposition::extended_type>::type & g_map,
		    grid_dist_key_dx<Sys_eqs::dims> & kmap ,
		    const grid_sm<Sys_eqs::dims,void> & gs,
		    std::unordered_map<long int,typename Sys_eqs::stype > & cols,
		    typename Sys_eqs::stype coeff) const
  {
    std::cerr << "Error " << __FILE__ << ":" << __LINE__ << " only CENTRAL, FORWARD, BACKWARD derivative are defined";
  }
};

/*! \brief Laplacian second order approximation CENTRAL Scheme
 *
 * \verbatim

 1.0
 *
 | -4.0
 1.0 *---+---* 1.0
 |
 *
 1.0

 * \endverbatim
 *
 *
 */
template<typename expr_type>
class Lap<expr_type,CENTRAL>
{
public:
  
  typedef typename expr_type::sys_eqs_type Sys_eqs; /**< System of equations. */
  typedef Sys_eqs sys_eqs_type;

  expr_type expr;

  Lap() {}

  Lap(expr_type expr_) : expr{expr_} {}

  /*! \brief Calculate which colums of the Matrix are non zero
   *
   * stub_or_real it is just for change the argument type when testing, in normal
   * conditions it is a distributed map
   *
   * \param g_map map grid
   * \param kmap position in the grid
   * \param spacing of the grid
   * \param gs Grid info
   * \param cols non-zero colums calculated by the function
   * \param coeff coefficent (constant in front of the derivative)
   *
   * ### Example
   *
   * \snippet FDScheme_unit_tests.hpp Laplacian usage
   *
   */
  inline void value(const typename stub_or_real<Sys_eqs,Sys_eqs::dims,typename Sys_eqs::stype,typename Sys_eqs::b_grid::decomposition::extended_type>::type & g_map,
		    grid_dist_key_dx<Sys_eqs::dims> & kmap ,
		    const grid_sm<Sys_eqs::dims,void> & gs,
		    typename Sys_eqs::stype (& spacing )[Sys_eqs::dims] ,
		    std::unordered_map<long int,typename Sys_eqs::stype > & cols,
		    typename Sys_eqs::stype coeff) const
  {
    // for each dimension
    for (size_t i = 0 ; i < Sys_eqs::dims ; i++)
      {
	long int old_val = kmap.getKeyRef().get(i);
	kmap.getKeyRef().set_d(i, kmap.getKeyRef().get(i) + 1);
	expr.value(g_map,kmap,gs,spacing,cols,coeff/spacing[i]/spacing[i]);
	kmap.getKeyRef().set_d(i,old_val);

	old_val = kmap.getKeyRef().get(i);
	kmap.getKeyRef().set_d(i, kmap.getKeyRef().get(i) - 1);
	expr.value(g_map,kmap,gs,spacing,cols,coeff/spacing[i]/spacing[i]);
	kmap.getKeyRef().set_d(i,old_val);

	expr.value(g_map,kmap,gs,spacing,cols, - 2.0 * coeff/spacing[i]/spacing[i]);
      }
  }
};


/*! \brief Laplacian second order approximation CENTRAL Scheme (with central derivative in the single)
 *
 * \verbatim

 1.0
 *
 | -4.0
 1.0 *---+---* 1.0
 |
 *
 1.0

 * \endverbatim
 *
 *
 */
template<typename expr_type>
class Lap<expr_type,CENTRAL_SYM>
{
public:

  typedef typename expr_type::sys_eqs_type Sys_eqs; /**< System of equations. */
  typedef Sys_eqs sys_eqs_type;

  expr_type expr;

  Lap() {}

  Lap(expr_type expr_) : expr{expr_} {}

  /*! \brief Calculate which colums of the Matrix are non zero
   *
   * stub_or_real it is just for change the argument type when testing, in normal
   * conditions it is a distributed map
   *
   * \param g_map map grid
   * \param kmap position in the grid
   * \param spacing of the grid
   * \param gs Grid info
   * \param cols non-zero colums calculated by the function
   * \param coeff coefficent (constant in front of the derivative)
   *
   * ### Example
   *
   * \snippet FDScheme_unit_tests.hpp Laplacian usage
   *
   */
  inline void value(const typename stub_or_real<Sys_eqs,Sys_eqs::dims,typename Sys_eqs::stype,typename Sys_eqs::b_grid::decomposition::extended_type>::type & g_map,
		    grid_dist_key_dx<Sys_eqs::dims> & kmap,
		    const grid_sm<Sys_eqs::dims,void> & gs,
		    typename Sys_eqs::stype (& spacing )[Sys_eqs::dims],
		    std::unordered_map<long int,typename Sys_eqs::stype > & cols,
		    typename Sys_eqs::stype coeff) const
  {
    // for each dimension
    for (size_t i = 0 ; i < Sys_eqs::dims ; i++)
      {
	long int old_val = kmap.getKeyRef().get(i);
	kmap.getKeyRef().set_d(i, kmap.getKeyRef().get(i) + 2);
	expr.value(g_map,kmap,gs,spacing,cols,coeff/spacing[i]/spacing[i]/4.0);
	kmap.getKeyRef().set_d(i,old_val);

	old_val = kmap.getKeyRef().get(i);
	kmap.getKeyRef().set_d(i, kmap.getKeyRef().get(i) - 2);
	expr.value(g_map,kmap,gs,spacing,cols,coeff/spacing[i]/spacing[i]/4.0);
	kmap.getKeyRef().set_d(i,old_val);

	expr.value(g_map,kmap,gs,spacing,cols, - 2.0 * coeff/spacing[i]/spacing[i]/4.0);
      }
  }
};

#endif /* OPENFPM_NUMERICS_SRC_FINITEDIFFERENCE_LAPLACIAN_HPP_ */
